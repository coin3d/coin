/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoRayPickAction Inventor/include/SoRayPickAction.h
  \brief The SoRayPickAction class is for picking objects in a scene through
  sending rays into it.

  FIXME: write full doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoPickRayElement.h>
#include <Inventor/elements/SoClipPlaneElement.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SbRotation.h>
#include <Inventor/misc/SoState.h>
#include <float.h>
#include <math.h>

#if !defined(COIN_EXCLUDE_SOSHAPE)
#include <Inventor/nodes/SoShape.h>
#endif // !COIN_EXCLUDE_SOSHAPE
#if !defined(COIN_EXCLUDE_SOCAMERA)
#include <Inventor/nodes/SoCamera.h>
#endif // !COIN_EXCLUDE_SOCAMERA
#if !defined(COIN_EXCLUDE_SOSEPARATOR)
#include <Inventor/nodes/SoSeparator.h>
#endif // !COIN_EXCLUDE_SOSEPARATOR
#if !defined(COIN_EXCLUDE_SOLOD)
#include <Inventor/nodes/SoLOD.h>
#endif // !COIN_EXCLUDE_SOLOD
#if !defined(COIN_EXCLUDE_SOLEVELOFDETAIL)
#include <Inventor/nodes/SoLevelOfDetail.h>
#endif // !COIN_EXCLUDE_SOLEVELOFDETAIL
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT

/*
 * Use bit-flags to hide implementation details.
 */

#define FLAG_WS_RAY_SET       0x0001  // ray set by ::setRay
#define FLAG_WS_RAY_COMPUTED  0x0002  // ray computed in ::computeWorldSpaceRay
#define FLAG_PICK_ALL         0x0004  // return all picked objects, or just closest
#define FLAG_NORM_POINT       0x0008  // is normalized vpPoint calculated
#define FLAG_CLIP_NEAR        0x0010  // clip ray at near plane?
#define FLAG_CLIP_FAR         0x0020  // clip ray at far plane?
#define FLAG_EXTRA_MATRIX     0x0040  // is extra matrix supplied in ::setObjectSpace


// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoRayPickAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoRayPickAction )

SoType SoRayPickAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoRayPickAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoRayPickAction::getTypeId(void) const
{
  return classTypeId;
}
//$ END TEMPLATE ActionClassTypeSource

#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// static variables
SoEnabledElementsList * SoRayPickAction::enabledElements;
SoActionMethodList * SoRayPickAction::methods;

/*!
  \fn SoRayPickAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoRayPickAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoRayPickAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoRayPickAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoRayPickAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method is for initializing static data for the
  SoRayPickAction class.
*/

void
SoRayPickAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource( SoRayPickAction )
  assert(SoRayPickAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoRayPickAction::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         "SoRayPickAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  ENABLE_ELEMENT(SoPickRayElement);
  ENABLE_ELEMENT(SoViewportRegionElement);
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
  ENABLE_ELEMENT(SoOverrideElement);
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT
}

// *************************************************************************

/*!
  A constructor.
*/

SoRayPickAction::SoRayPickAction(const SbViewportRegion & viewportRegion)
  : inherited(viewportRegion),
    radiusInPixels(5.0f),
    flags(0)
{
  SO_ACTION_CONSTRUCTOR(SoRayPickAction);

  static int first = 1;
  if (first) {
    first = 0;
    // most methods are inherited from SoPickAction
#if !defined(COIN_EXCLUDE_SOCAMERA)
    SO_ACTION_ADD_METHOD(SoCamera, SoNode::rayPickS);
#endif //!COIN_EXCLUDE_SOCAMERA
#if !defined(COIN_EXCLUDE_SOSEPARATOR)
    SO_ACTION_ADD_METHOD(SoSeparator, SoNode::rayPickS);
#endif //!COIN_EXCLUDE_SOSEPARATOR
#if !defined(COIN_EXCLUDE_SOLOD)
    SO_ACTION_ADD_METHOD(SoLOD, SoNode::rayPickS);
#endif //!COIN_EXCLUDE_SOLOD
#if !defined(COIN_EXCLUDE_SOLEVELOFDETAIL)
    SO_ACTION_ADD_METHOD(SoLevelOfDetail, SoNode::rayPickS);
#endif //!COIN_EXCLUDE_SOLEVELOFDETAIL
#if !defined(COIN_EXCLUDE_SOSHAPE)
    SO_ACTION_ADD_METHOD(SoShape, SoNode::rayPickS);
#endif //!COIN_EXCLUDE_SOSHAPE
  }

  methods->setUp(); // FIXME: not sure if this should be called here...
}

/*!
  The destructor.
*/

SoRayPickAction::~SoRayPickAction(void)
{
}

/*!
  This method sets the viewport-space point which the ray is sent through.
*/

void
SoRayPickAction::setPoint(const SbVec2s &viewportPoint)
{
  this->vpPoint = viewportPoint;
  this->clearFlag(FLAG_NORM_POINT|FLAG_WS_RAY_SET|FLAG_WS_RAY_COMPUTED);
  this->setFlag(FLAG_CLIP_NEAR|FLAG_CLIP_FAR);
}

/*!
  This method sets the viewport-space point which the ray is sent through.
  The coordinate is normalized, ranging from (0,0) to (1,1).
*/

void
SoRayPickAction::setNormalizedPoint(const SbVec2f &normPoint)
{
  this->normvpPoint = normPoint;
  this->clearFlag(FLAG_WS_RAY_SET|FLAG_WS_RAY_COMPUTED);
  this->setFlag(FLAG_NORM_POINT|FLAG_CLIP_NEAR|FLAG_CLIP_FAR);
}

/*!
  This method sets the radius of the picking ray.
*/

void
SoRayPickAction::setRadius(const float radiusInPixels)
{
  this->radiusInPixels = radiusInPixels;
}

/*!
  This method sets the ray in world-space coordinates.
*/

void
SoRayPickAction::setRay(const SbVec3f &start,
                        const SbVec3f &direction,
                        float nearDistance,
                        float farDistance)
{
  if (nearDistance >= 0.0f) this->setFlag(FLAG_CLIP_NEAR);
  else {
    this->clearFlag(FLAG_CLIP_NEAR);
    nearDistance = 0.0f;
  }

  if (farDistance >= 0.0f) this->setFlag(FLAG_CLIP_FAR);
  else {
    this->clearFlag(FLAG_CLIP_FAR);
    farDistance = nearDistance + 1.0f;
  }

  // FIXME: when do I calculate these?
  this->rayRadiusStart = 0.01f;
  this->rayRadiusDelta = 0.0f;

  this->rayStart = start;
  this->rayDirection = direction;
  this->rayDirection.normalize();
  this->rayNear = nearDistance;
  this->rayFar = farDistance;
  this->wsLine = SbLine(start, start + direction);

  // D = shortest distance from origin to plane
  const float D = this->rayDirection.dot(this->rayStart);
  this->nearPlane = SbPlane(rayDirection, D + this->rayNear);

  this->setFlag(FLAG_WS_RAY_SET);
}

/*!
  This method lets you decide wether only the closest object or all the objects
  the ray intersects with should be picked.
*/

void
SoRayPickAction::setPickAll(const SbBool flag)
{
  if (flag) this->setFlag(FLAG_PICK_ALL);
  else this->clearFlag(FLAG_PICK_ALL);
}

/*!
  This method returns wether only the closest object or all the objects the
  ray intersects with is picked.
*/

SbBool
SoRayPickAction::isPickAll(void) const
{
  return this->isFlagSet(FLAG_PICK_ALL);
}

/*!
  This method returns a list of the picked points.
*/

const SoPickedPointList &
SoRayPickAction::getPickedPointList(void) const
{
  return this->pickedPointList;
}

/*!
  This method returns the picked point with index \a index in the list of
  picked points.  \a index starts at index 0.
*/

SoPickedPoint *
SoRayPickAction::getPickedPoint(const int index) const
{
  assert(index >= 0);
  if (index < this->pickedPointList.getLength())
    return this->pickedPointList[index];
  return NULL;
}


void
SoRayPickAction::computeWorldSpaceRay()
{
  if (!this->isFlagSet(FLAG_WS_RAY_SET)) {
    const SbViewVolume &vv = SoViewVolumeElement::get(this->state);

    if (!this->isFlagSet(FLAG_NORM_POINT)) {

      SbVec2s pt = this->vpPoint - this->vpRegion.getViewportOriginPixels();
      SbVec2s size = this->vpRegion.getViewportSizePixels();
      this->normvpPoint.setValue(float(pt[0]) / float(size[0]),
                                 float(pt[1]) / float(size[1]));
      this->setFlag(FLAG_NORM_POINT);

    }
#if 1
    vv.getNearFarRay(normvpPoint, rayStart, rayDirection);
#else
    SbLine line;
    vv.projectPointToLine(this->normvpPoint, line);
    this->rayStart = line.getPosition();
    this->rayDirection = line.getDirection();
#endif

    this->rayDirection.normalize();
    this->rayNear = vv.getNearDist();
    this->rayFar = vv.getNearDist() + vv.getDepth();

    // radius of ray for start and far positions
    this->rayRadiusStart =
      vv.getWorldToScreenScale(rayStart,
                               this->calcRayRadius(this->radiusInPixels));
    this->rayRadiusDelta =
      vv.getWorldToScreenScale(this->rayStart + this->rayDirection,
                               this->calcRayRadius(this->radiusInPixels));
    this->rayRadiusDelta -= rayRadiusStart;
    this->wsLine = SbLine(this->rayStart,
                          this->rayStart + this->rayDirection);

#if COIN_DEBUG
    SoDebugError::postInfo("SoRayPickAction::computeWorldSpaceRay",
                           "%f %f %f",
                           radiusInPixels,
                           rayRadiusStart,
                           rayRadiusDelta);
#endif // COIN_DEBUG


    // shortest distance from origin to plane
    const float D = this->rayDirection.dot(this->rayStart);
    this->nearPlane = SbPlane(rayDirection, D);

    this->setFlag(FLAG_WS_RAY_COMPUTED);
  }
}

SbBool
SoRayPickAction::hasWorldSpaceRay() const
{
  return this->isFlagSet(FLAG_WS_RAY_SET|FLAG_WS_RAY_COMPUTED);
}

void
SoRayPickAction::setObjectSpace()
{
  this->clearFlag(FLAG_EXTRA_MATRIX);
  this->calcObjectSpaceData();
}

void
SoRayPickAction::setObjectSpace(const SbMatrix &matrix)
{
  this->setFlag(FLAG_EXTRA_MATRIX);
  this->extraMatrix = matrix;
  this->calcObjectSpaceData();
}

SbBool
SoRayPickAction::intersect(const SbVec3f &v0,
                           const SbVec3f &v1,
                           const SbVec3f &v2,
                           SbVec3f &intersection, SbVec3f &barycentric,
                           SbBool &front) const
{
  const SbVec3f &orig = osLine.getPosition();
  const SbVec3f &dir = osLine.getDirection();

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
  float &u = barycentric[0];
  float &v = barycentric[1];
  float &w = barycentric[2];

  float inv_det = 1.0 / det;

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
  intersection = orig + t*dir;

  return TRUE;
}

SbBool
SoRayPickAction::intersect(const SbVec3f &v0, const SbVec3f &v1,
                           SbVec3f &intersection) const
{
  SbLine line(v0,v1);
  SbVec3f op0, op1; // object space
  SbVec3f p0,p1; // world space

  this->osLine.getClosestPoints(line, op0, op1);

  // clamp op1 between v0 and v1
  if ((op1-v0).dot(line.getDirection()) < 0.0f) op1 = v0;
  else if ((v1-op1).dot(line.getDirection()) < 0.0f) op1 = v1;

  // FIXME: clamp op0 to rayStart, rayFar ???

  this->obj2World.multVecMatrix(op0, p0);
  this->obj2World.multVecMatrix(op1, p1);

  // distance between points
  float distance = (p1-p0).length();

  float raypos = (p0 - this->rayStart).length();

  float radius = this->rayRadiusStart +
    this->rayRadiusDelta*raypos/this->rayFar;

  if (radius >= distance) {
    intersection = op1;
    return TRUE;
  }
  return FALSE;
}

SbBool
SoRayPickAction::intersect(const SbVec3f &point) const
{
  SbVec3f wPoint;
  this->obj2World.multVecMatrix(point, wPoint);
  SbVec3f ptOnLine = this->wsLine.getClosestPoint(wPoint);

  // distance between points
  float distance = (wPoint-ptOnLine).length();

  float raypos = (ptOnLine - this->rayStart).length();

  float radius = this->rayRadiusStart +
    this->rayRadiusDelta*raypos/this->rayFar;

  return (radius >= distance);
}

SbBool
SoRayPickAction::intersect(const SbBox3f &/*box*/,
                           const SbBool /*useFullViewVolume*/)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

const SbViewVolume &
SoRayPickAction::getViewVolume()
{
  assert(0 && "FIXME: not implemented");
  return this->osVolume;
}

const SbLine &
SoRayPickAction::getLine()
{
  return this->osLine;
}

SbBool
SoRayPickAction::isBetweenPlanes(const SbVec3f &intersection) const
{
  SbVec3f worldPt;
  this->obj2World.multVecMatrix(intersection, worldPt);
  float dist = this->nearPlane.getDistance(worldPt);
  if (this->isFlagSet(FLAG_CLIP_NEAR)) {
    if (dist < 0) return FALSE;
  }
  if (this->isFlagSet(FLAG_CLIP_FAR)) {
    if (dist > (this->rayFar - this->rayNear)) return FALSE;
  }
  const SoClipPlaneElement *planes =
    SoClipPlaneElement::getInstance(this->state);
  int n =  planes->getNum();
  for (int i = 0; i < n; i++) {
    if (!planes->get(i).isInHalfSpace(worldPt)) return FALSE;
  }
  return TRUE;
}

SoPickedPoint *
SoRayPickAction::addIntersection(const SbVec3f &objectSpacePoint)
{
#if COIN_DEBUG // debug
  SoDebugError::postInfo("SoRayPickAction::addIntersection",
                         "%g %g %g",
                         objectSpacePoint[0],
                         objectSpacePoint[1],
                         objectSpacePoint[2]);
#endif // debug

  SbVec3f worldPt;
  this->obj2World.multVecMatrix(objectSpacePoint, worldPt);

  if (this->pickedPointList.getLength() && !this->isFlagSet(FLAG_PICK_ALL)) {
    // got to test if new candidate is closer than old one
    float dist = this->nearPlane.getDistance(worldPt);
    if (dist >= this->currentPickDistance) return NULL; // farther

    // remove old point
    delete this->pickedPointList[0];
    this->pickedPointList.truncate(0);
  }

  if (!this->isFlagSet(FLAG_PICK_ALL)) {
    this->currentPickDistance = this->nearPlane.getDistance(worldPt);
  }
  // create the new picked point
  SoPickedPoint * pp = new SoPickedPoint(this->getCurPath(),
                                         this->state, objectSpacePoint);
  this->pickedPointList.append(pp);
  return pp;
}

void
SoRayPickAction::beginTraversal(SoNode *node)
{
  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), this->vpRegion);
  inherited::beginTraversal(node);
  this->getState()->pop();
}

void
SoRayPickAction::setFlag(const unsigned int flag)
{
  this->flags |= flag;
}

void
SoRayPickAction::clearFlag(const unsigned int flag)
{
  this->flags &= ~flag;
}

SbBool
SoRayPickAction::isFlagSet(const unsigned int flag) const
{
  return (this->flags & flag) != 0;
}

void
SoRayPickAction::calcObjectSpaceData()
{
  this->calcMatrices();

  SbVec3f start, dir;

  this->world2Obj.multVecMatrix(this->rayStart, start);
  this->world2Obj.multDirMatrix(this->rayDirection, dir);
  this->osLine = SbLine(start, start + dir);

  // FIXME: calc osVolume
}

void
SoRayPickAction::calcMatrices()
{
  this->obj2World = SoModelMatrixElement::get(this->state);
  if (this->isFlagSet(FLAG_EXTRA_MATRIX)) {
    this->obj2World.multLeft(this->extraMatrix);
  }
  this->world2Obj = this->obj2World.inverse();
}

float
SoRayPickAction::calcRayRadius(const float radiusInPixels)
{
  const SbVec2s &size = this->vpRegion.getViewportSizePixels();

  float xsize = float(size[0]);
  float ysize = float(size[1]);

  return radiusInPixels / sqrt(xsize*xsize + ysize*ysize);
}
