/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoRayPickAction SoRayPickAction.h Inventor/actions/SoRayPickAction.h
  \brief The SoRayPickAction class does ray intersection with scene graphs.
  \ingroup actions

  For interaction with the scene graph geometry, it is necessary to be
  able to do intersection testing for rays. This functionality is
  provided by the SoRayPickAction class.
*/

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSubActionP.h>

#include <Inventor/SoPickedPoint.h>
#include <Inventor/elements/SoClipPlaneElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoPickRayElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoLOD.h>
#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <coindefs.h> // COIN_STUB()
#include <float.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_ACTION_SOURCE(SoRayPickAction);


// Override from parent class.
void
SoRayPickAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoRayPickAction, SoPickAction);

  SO_ENABLE(SoRayPickAction, SoPickRayElement);
  SO_ENABLE(SoRayPickAction, SoViewportRegionElement);
  SO_ENABLE(SoRayPickAction, SoOverrideElement);
}


/*!
  Constructor.

  Some node types need a \a viewportregion to know exactly how they
  are positioned within the scene.
*/
SoRayPickAction::SoRayPickAction(const SbViewportRegion & viewportregion)
  : inherited(viewportregion), radiusinpixels(5.0f), flags(0)
{
  SO_ACTION_CONSTRUCTOR(SoRayPickAction);

  // most methods are inherited from SoPickAction
  SO_ACTION_ADD_METHOD_INTERNAL(SoCamera, SoNode::rayPickS);
  SO_ACTION_ADD_METHOD_INTERNAL(SoSeparator, SoNode::rayPickS);
  SO_ACTION_ADD_METHOD_INTERNAL(SoLOD, SoNode::rayPickS);
  SO_ACTION_ADD_METHOD_INTERNAL(SoLevelOfDetail, SoNode::rayPickS);
  SO_ACTION_ADD_METHOD_INTERNAL(SoShape, SoNode::rayPickS);

  methods->setUp();
}

/*!
  Destructor, free temporary resources used by action.
*/
SoRayPickAction::~SoRayPickAction(void)
{
  this->cleanupPickedPoints();
}

/*!
  Sets the viewport-space point. This point is calculated into a line
  from the near clipping plane to the far clipping plane, and the
  intersection ray follows the line.

  This is a convenient way to detect object intersection below the
  cursor.
*/
void
SoRayPickAction::setPoint(const SbVec2s & viewportpoint)
{
  this->vppoint = viewportpoint;
  this->clearFlag(SoRayPickAction::NORM_POINT|SoRayPickAction::WS_RAY_SET|SoRayPickAction::WS_RAY_COMPUTED);
  this->setFlag(SoRayPickAction::CLIP_NEAR|SoRayPickAction::CLIP_FAR);
}

/*!
  Sets the viewport-space point which the ray is sent through.
  The coordinate is normalized, ranging from (0, 0) to (1, 1).

  \sa setPoint()
*/
void
SoRayPickAction::setNormalizedPoint(const SbVec2f & normpoint)
{
  this->normvppoint = normpoint;
  this->clearFlag(SoRayPickAction::WS_RAY_SET|SoRayPickAction::WS_RAY_COMPUTED);
  this->setFlag(SoRayPickAction::NORM_POINT|SoRayPickAction::CLIP_NEAR|SoRayPickAction::CLIP_FAR);
}

/*!
  Sets the radius of the picking ray, in screen pixels.
*/
void
SoRayPickAction::setRadius(const float radiusinpixels)
{
  this->radiusinpixels = radiusinpixels;
}

/*!
  Sets the ray in world-space coordinates.
*/
void
SoRayPickAction::setRay(const SbVec3f & start, const SbVec3f & direction,
                        float neardistance, float fardistance)
{
  if (neardistance >= 0.0f) this->setFlag(SoRayPickAction::CLIP_NEAR);
  else {
    this->clearFlag(SoRayPickAction::CLIP_NEAR);
    neardistance = 0.0f;
  }

  if (fardistance >= 0.0f) this->setFlag(SoRayPickAction::CLIP_FAR);
  else {
    this->clearFlag(SoRayPickAction::CLIP_FAR);
    fardistance = neardistance + 1.0f;
  }

  // FIXME: when do I calculate these?
  this->rayradiusstart = 0.01f;
  this->rayradiusdelta = 0.0f;

  this->raystart = start;
  this->raydirection = direction;
  this->raydirection.normalize();
  this->raynear = neardistance;
  this->rayfar = fardistance;
  this->wsline = SbLine(start, start + direction);

  // D = shortest distance from origin to plane
  const float D = this->raydirection.dot(this->raystart);
  this->nearplane = SbPlane(this->raydirection, D + this->raynear);

  this->setFlag(SoRayPickAction::WS_RAY_SET);
}

/*!
  Lets you decide whether only the closest object or all the objects
  the ray intersects with should be picked.
*/
void
SoRayPickAction::setPickAll(const SbBool flag)
{
  if (flag) this->setFlag(SoRayPickAction::PICK_ALL);
  else this->clearFlag(SoRayPickAction::PICK_ALL);
}

/*!
  Returns whether only the closest object or all the objects the ray
  intersects with is picked.
*/
SbBool
SoRayPickAction::isPickAll(void) const
{
  return this->isFlagSet(SoRayPickAction::PICK_ALL);
}

/*!
  Returns a list of the picked points.
*/
const SoPickedPointList &
SoRayPickAction::getPickedPointList(void) const
{
  return this->pickedpointlist;
}

/*!
  Returns the picked point with \a index in the list of picked
  points.
*/
SoPickedPoint *
SoRayPickAction::getPickedPoint(const int index) const
{
  assert(index >= 0);
  if (index < this->pickedpointlist.getLength())
    return this->pickedpointlist[index];
  return NULL;
}

/*!
  \internal
 */
void
SoRayPickAction::computeWorldSpaceRay(void)
{
  if (!this->isFlagSet(SoRayPickAction::WS_RAY_SET)) {
    const SbViewVolume & vv = SoViewVolumeElement::get(this->state);

    if (!this->isFlagSet(SoRayPickAction::NORM_POINT)) {

      SbVec2s pt = this->vppoint - this->vpRegion.getViewportOriginPixels();
      SbVec2s size = this->vpRegion.getViewportSizePixels();
      this->normvppoint.setValue(float(pt[0]) / float(size[0]),
                                 float(pt[1]) / float(size[1]));
      this->setFlag(SoRayPickAction::NORM_POINT);

    }

#if COIN_DEBUG
    if (vv.getDepth() == 0.0f || vv.getWidth() == 0.0f || vv.getHeight() == 0.0f) {
      SoDebugError::postWarning("SoRayPickAction::computeWorldSpaceRay",
                                "invalid frustum: <%f, %f, %f>",
                                vv.getWidth(), vv.getHeight(), vv.getDepth());
      return;
    }
#endif // COIN_DEBUG

    SbLine templine;
    vv.projectPointToLine(this->normvppoint, templine);
    this->raystart = templine.getPosition();
    this->raydirection = templine.getDirection();

    this->raynear = 0.0f;
    this->rayfar = vv.getDepth();

    // radius of ray for start and far positions
    this->rayradiusstart =
      vv.getWorldToScreenScale(this->raystart,
                               this->calcRayRadius(this->radiusinpixels));
    this->rayradiusdelta =
      vv.getWorldToScreenScale(this->raystart + this->raydirection,
                               this->calcRayRadius(this->radiusinpixels));
    this->rayradiusdelta -= this->rayradiusstart;
    this->wsline = SbLine(this->raystart,
                          this->raystart + this->raydirection);

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoRayPickAction::computeWorldSpaceRay",
                           "%f %f %f",
                           this->radiusinpixels,
                           this->rayradiusstart,
                           this->rayradiusdelta);
#endif // debug


    // shortest distance from origin to plane
    const float D = this->raydirection.dot(this->raystart);
    this->nearplane = SbPlane(this->raydirection, D);

    this->setFlag(SoRayPickAction::WS_RAY_COMPUTED);
  }
}

/*!
  \internal
 */
SbBool
SoRayPickAction::hasWorldSpaceRay(void) const
{
  return this->isFlagSet(SoRayPickAction::WS_RAY_SET|SoRayPickAction::WS_RAY_COMPUTED);
}

/*!
  \internal
 */
void
SoRayPickAction::setObjectSpace(void)
{
  this->clearFlag(SoRayPickAction::EXTRA_MATRIX);
  this->calcObjectSpaceData();
}

/*!
  \internal
 */
void
SoRayPickAction::setObjectSpace(const SbMatrix & matrix)
{
  this->setFlag(SoRayPickAction::EXTRA_MATRIX);
  this->extramatrix = matrix;
  this->calcObjectSpaceData();
}

/*!
  \internal
 */
SbBool
SoRayPickAction::intersect(const SbVec3f & v0,
                           const SbVec3f & v1,
                           const SbVec3f & v2,
                           SbVec3f & intersection, SbVec3f & barycentric,
                           SbBool & front) const
{
  const SbVec3f & orig = this->osline.getPosition();
  const SbVec3f & dir = this->osline.getDirection();

  SbVec3f edge1 = v1 - v0;
  SbVec3f edge2 = v2 - v0;

  SbVec3f pvec = dir.cross(edge2);

  // if determinant is near zero, ray lies in plane of triangle
  float det = edge1.dot(pvec);
  if (fabs(det) < FLT_EPSILON) return FALSE;

  // does ray hit front or back of triangle
  if (det > 0.0f) front = TRUE;
  else front = FALSE;

  // create some more intuitive barycentric coordinate names
  float & u = barycentric[0];
  float & v = barycentric[1];
  float & w = barycentric[2];

  float inv_det = 1.0f / det;

  // calculate distance from v0 to ray origin
  SbVec3f tvec = orig - v0;

  // calculate U parameter and test bounds
  u = tvec.dot(pvec) * inv_det;
  if (u < 0.0f || u > 1.0f)
    return FALSE;

  // prepare to test V parameter
  SbVec3f qvec = tvec.cross(edge1);

  // calculate V parameter and test bounds
  v = dir.dot(qvec) * inv_det;
  if (v < 0.0f || u + v > 1.0f)
    return FALSE;

  // third barycentric coordinate
  w = 1.0f - u - v;

  // calculate t and intersection point
  float t = edge2.dot(qvec) * inv_det;
  intersection = orig + t * dir;

  return TRUE;
}

/*!
  \internal
 */
SbBool
SoRayPickAction::intersect(const SbVec3f & v0, const SbVec3f & v1,
                           SbVec3f & intersection) const
{
  SbLine line(v0, v1);
  SbVec3f op0, op1; // object space
  SbVec3f p0, p1; // world space

  this->osline.getClosestPoints(line, op0, op1);

  // clamp op1 between v0 and v1
  if ((op1-v0).dot(line.getDirection()) < 0.0f) op1 = v0;
  else if ((v1-op1).dot(line.getDirection()) < 0.0f) op1 = v1;

  // FIXME: clamp op0 to raystart, rayfar ???

  this->obj2world.multVecMatrix(op0, p0);
  this->obj2world.multVecMatrix(op1, p1);

  // distance between points
  float distance = (p1-p0).length();

  float raypos = (p0 - this->raystart).length();

  float radius = this->rayradiusstart +
    this->rayradiusdelta * raypos/this->rayfar;

  if (radius >= distance) {
    intersection = op1;
    return TRUE;
  }
  return FALSE;
}

/*!
  \internal
 */
SbBool
SoRayPickAction::intersect(const SbVec3f & point) const
{
  SbVec3f wpoint;
  this->obj2world.multVecMatrix(point, wpoint);
  SbVec3f ptonline = this->wsline.getClosestPoint(wpoint);

  // distance between points
  float distance = (wpoint-ptonline).length();

  float raypos = (ptonline - this->raystart).length();

  float radius = this->rayradiusstart +
    this->rayradiusdelta * raypos/this->rayfar;

  return (radius >= distance);
}

/*!
  \internal
 */
SbBool
SoRayPickAction::intersect(const SbBox3f & box, const SbBool usefullviewvolume)
{
  COIN_STUB(); // FIXME
  return FALSE;
}

/*!
  \internal
 */
const SbViewVolume &
SoRayPickAction::getViewVolume(void)
{
  COIN_STUB();
  return this->osvolume;
}

/*!
  \internal
 */
const SbLine &
SoRayPickAction::getLine(void)
{
  return this->osline;
}

/*!
  \internal
 */
SbBool
SoRayPickAction::isBetweenPlanes(const SbVec3f & intersection) const
{
  SbVec3f worldpoint;
  this->obj2world.multVecMatrix(intersection, worldpoint);
  float dist = this->nearplane.getDistance(worldpoint);
  if (this->isFlagSet(SoRayPickAction::CLIP_NEAR)) {
    if (dist < 0) return FALSE;
  }
  if (this->isFlagSet(SoRayPickAction::CLIP_FAR)) {
    if (dist > (this->rayfar - this->raynear)) return FALSE;
  }
  const SoClipPlaneElement * planes =
    SoClipPlaneElement::getInstance(this->state);
  int n =  planes->getNum();
  for (int i = 0; i < n; i++) {
    if (!planes->get(i).isInHalfSpace(worldpoint)) return FALSE;
  }
  return TRUE;
}

/*!
  \internal
 */
SoPickedPoint *
SoRayPickAction::addIntersection(const SbVec3f & objectspacepoint)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoRayPickAction::addIntersection",
                         "%g %g %g",
                         objectspacepoint[0],
                         objectspacepoint[1],
                         objectspacepoint[2]);
#endif // debug

  SbVec3f worldpoint;
  this->obj2world.multVecMatrix(objectspacepoint, worldpoint);

  if (this->pickedpointlist.getLength() && !this->isFlagSet(SoRayPickAction::PICK_ALL)) {
    // got to test if new candidate is closer than old one
    float dist = this->nearplane.getDistance(worldpoint);
    if (dist >= this->currentPickDistance) return NULL; // farther

    // remove old point
    delete this->pickedpointlist[0];
    this->pickedpointlist.truncate(0);
  }

  if (!this->isFlagSet(SoRayPickAction::PICK_ALL)) {
    this->currentPickDistance = this->nearplane.getDistance(worldpoint);
  }
  // create the new picked point
  SoPickedPoint * pp = new SoPickedPoint(this->getCurPath(),
                                         this->state, objectspacepoint);
  this->pickedpointlist.append(pp);
  return pp;
}

/*!
  Overloaded to set up internal data.
 */
void
SoRayPickAction::beginTraversal(SoNode * node)
{
  this->cleanupPickedPoints();
  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), this->vpRegion);
  inherited::beginTraversal(node);
  this->getState()->pop();
}

// Private.
void
SoRayPickAction::setFlag(const unsigned int flag)
{
  this->flags |= flag;
}

// Private.
void
SoRayPickAction::clearFlag(const unsigned int flag)
{
  this->flags &= ~flag;
}

// Private.
SbBool
SoRayPickAction::isFlagSet(const unsigned int flag) const
{
  return (this->flags & flag) != 0;
}

// Private.
void
SoRayPickAction::calcObjectSpaceData(void)
{
  this->calcMatrices();

  SbVec3f start, dir;

  this->world2obj.multVecMatrix(this->raystart, start);
  this->world2obj.multDirMatrix(this->raydirection, dir);
  this->osline = SbLine(start, start + dir);

  // FIXME: calc this->osvolume
}

// Private.
void
SoRayPickAction::calcMatrices(void)
{
  this->obj2world = SoModelMatrixElement::get(this->state);
  if (this->isFlagSet(SoRayPickAction::EXTRA_MATRIX)) {
    this->obj2world.multLeft(this->extramatrix);
  }
  this->world2obj = this->obj2world.inverse();
}

// Private.
float
SoRayPickAction::calcRayRadius(const float radiusinpixels)
{
  const SbVec2s &size = this->vpRegion.getViewportSizePixels();

  float xsize = float(size[0]);
  float ysize = float(size[1]);

  return float(radiusinpixels / sqrt(xsize * xsize + ysize * ysize));
}

// Private.
void
SoRayPickAction::cleanupPickedPoints(void)
{
  int n = this->pickedpointlist.getLength();

  for (int i = 0; i < n; i++) {
    delete this->pickedpointlist[i];
  }
  this->pickedpointlist.truncate(0);
}
