/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoRayPickAction SoRayPickAction.h Inventor/actions/SoRayPickAction.h
  \brief The SoRayPickAction class does ray intersection with scene graphs.
  \ingroup actions

  For interaction with the scene graph geometry, it is necessary to be
  able to do intersection testing for rays. This functionality is
  provided by the SoRayPickAction class.

  SoRayPickAction can be used to pass arbitrary rays through the scene
  for intersection detections, by using the setRay() method.

  Because a very common operation is to check for intersections along
  the ray from the mousecursor upon mouseclicks, it also contains
  convenience methods for setting up a ray from the near plane to the
  far plane from the 2D mousecursor coordinates. See the setPoint()
  and setNormalizedPoint() methods. A simple usage example for this
  case is presented below.


  Note that one common mistake when using a raypick action to
  intersect from a point under the mousecursor after a mouseclick is
  that one tries to apply it to a scenegraph that does not contain a
  camera \e explicitly set up by the application programmer. Without a
  camera as part of the traversal, the raypick action does not know
  which view volume to send the ray through.

  In this regard, be aware that the getSceneGraph() call in the
  So*-libraries' viewer classes will return the root of the
  user-supplied scenegraph, not the "real" internal scenegraph root
  used by the viewer (which should always contain a camera node). So
  raypicks done from the application code will fail when doing this:

  \code
  // initializing viewer scenegraph
  SoSeparator * root = new SoSeparator;
  root->ref();

  SoEventCallback * ecb = new SoEventCallback;
  ecb->addEventCallback(SoMouseButtonEvent::getClassTypeId(), event_cb, viewer);
  root->addChild(ecb);

  root->addChild(new SoCone);
  
  viewer->setSceneGraph( root );
  // -- [snip] -------------------------

  // attempting raypick in the event_cb() callback method
  SoRayPickAction rp( viewer->getViewportRegion() );
  rp.setPoint(mouseevent->getPosition());
  rp.apply(viewer->getSceneGraph());
  // BUG: results will not be what you expected, as no camera was
  // part of the "user's scenegraph"
  \endcode

  While this is the correct way to do it:

  \code
  // initializing viewer scenegraph
  SoSeparator * root = new SoSeparator;
  root->ref();

  // Need to set up our own camera in the "user scenegraph", or else
  // the raypick action will fail because the camera is hidden in the
  // viewer-specific root of the scenegraph.
  SoPerspectiveCamera * pcam = new SoPerspectiveCamera;
  root->addChild(pcam);

  SoEventCallback * ecb = new SoEventCallback;
  ecb->addEventCallback(SoMouseButtonEvent::getClassTypeId(), event_cb, viewer);
  root->addChild(ecb);

  root->addChild(new SoCone);
  
  viewer->setSceneGraph( root );
  pcam->viewAll( root, viewer->getViewportRegion() );
  // -- [snip] -------------------------

  // attempting raypick in the event_cb() callback method
  SoRayPickAction rp( viewer->getViewportRegion() );
  rp.setPoint(mouseevent->getPosition());
  rp.apply(viewer->getSceneGraph());
  \endcode

  Or if you do want the convenience of having the viewer set up a
  camera for you implicitly, you can get hold of the root-node of the
  "complete" scenegraph by simply calling:

  \code
  SoNode * realroot = viewer->getSceneManager()->getSceneGraph();
  \endcode
*/
// FIXME: in the class doc, also mention how one can use
// SoRayPickAction from within an SoHandleEventAction callback with
// the getNodeAppliedTo() method etc.  Include a usage example code
// snippet. 20010920 mortene.

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSubActionP.h>

#include <Inventor/SbLine.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/elements/SoClipPlaneElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoPickRayElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoLOD.h>
#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <float.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// *************************************************************************

// The private data for the SoRayPickAction.

class SoRayPickActionP {
public:
  SoRayPickActionP(SoRayPickAction * o) {
    this->owner = o;
  }

  // Hidden private methods.


  SbBool isBetweenPlanesWS(const SbVec3f & intersection,
                           const SoClipPlaneElement * planes) const;
  void cleanupPickedPoints(void);
  void setFlag(const unsigned int flag);
  void clearFlag(const unsigned int flag);
  SbBool isFlagSet(const unsigned int flag) const;
  void calcObjectSpaceData(SoState * ownerstate);
  void calcMatrices(SoState * ownerstate);

  // Hidden private variables.

  SbViewVolume osvolume;
  SbViewVolume wsvolume;
  SbLine osline;
  SbPlane nearplane;
  SbVec2s vppoint;
  SbVec2f normvppoint;
  SbVec3f raystart;
  SbVec3f raydirection;
  double rayradiusstart;
  double rayradiusdelta;
  double raynear;
  double rayfar;
  float radiusinpixels;

  SbLine wsline;
  SbMatrix obj2world;
  SbMatrix world2obj;
  SbMatrix extramatrix;

  SoPickedPointList pickedpointlist;
  SbList <float> ppdistance;

  unsigned int flags;
  SbBool objectspacevalid;

  enum {
    WS_RAY_SET =        0x0001, // ray set by setRay()
    WS_RAY_COMPUTED =   0x0002, // ray computed in computeWorldSpaceRay()
    PICK_ALL =          0x0004, // return all picked objects, or just closest
    NORM_POINT =        0x0008, // is normalized vppoint calculated
    CLIP_NEAR =         0x0010, // clip ray at near plane?
    CLIP_FAR =          0x0020, // clip ray at far plane?
    EXTRA_MATRIX =      0x0040, // is extra matrix supplied in setObjectSpace()
    PPLIST_IS_SORTED =  0x0080, // did we sort pickedpointslist ?
    OSVOLUME_DIRTY =    0x0100  // did we calculate osvolume?
  };

private:
  SoRayPickAction * owner;
};

#define THIS (this->pimpl)

// *************************************************************************

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
  are positioned within the scene. For an in-depth explanation of why
  the \a viewportregion argument is needed, see the documentation of
  SoGetBoundingBox::SoGetBoundingBox(const SbViewportRegion &).
*/
SoRayPickAction::SoRayPickAction(const SbViewportRegion & viewportregion)
  : inherited(viewportregion)
{
  THIS = new SoRayPickActionP(this);
  THIS->radiusinpixels = 5.0f;
  THIS->flags = 0;
  THIS->objectspacevalid = TRUE;

  SO_ACTION_CONSTRUCTOR(SoRayPickAction);
}

/*!
  Destructor, free temporary resources used by action.
*/
SoRayPickAction::~SoRayPickAction(void)
{
  THIS->cleanupPickedPoints();
  delete THIS;
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
  THIS->vppoint = viewportpoint;
  THIS->clearFlag(SoRayPickActionP::NORM_POINT |
                  SoRayPickActionP::WS_RAY_SET |
                  SoRayPickActionP::WS_RAY_COMPUTED);
  THIS->setFlag(SoRayPickActionP::CLIP_NEAR |
                SoRayPickActionP::CLIP_FAR);
}

/*!
  Sets the viewport-space point which the ray is sent through.
  The coordinate is normalized, ranging from (0, 0) to (1, 1).

  \sa setPoint()
*/
void
SoRayPickAction::setNormalizedPoint(const SbVec2f & normpoint)
{
  THIS->normvppoint = normpoint;
  THIS->clearFlag(SoRayPickActionP::WS_RAY_SET |
                  SoRayPickActionP::WS_RAY_COMPUTED);
  THIS->setFlag(SoRayPickActionP::NORM_POINT |
                SoRayPickActionP::CLIP_NEAR |
                SoRayPickActionP::CLIP_FAR);
}

/*!
  Sets the radius of the picking ray, in screen pixels.  Default value
  is 5.0.
*/
void
SoRayPickAction::setRadius(const float radiusinpixels)
{
  THIS->radiusinpixels = radiusinpixels;
}

/*!
  Sets the intersection ray in world-space coordinates.

  Use this method if you want to send any ray through the scene to
  detect intersections, independently of mouseposition and
  mouseclicks.
*/
void
SoRayPickAction::setRay(const SbVec3f & start, const SbVec3f & direction,
                        float neardistance, float fardistance)
{
  if (neardistance >= 0.0f) THIS->setFlag(SoRayPickActionP::CLIP_NEAR);
  else {
    THIS->clearFlag(SoRayPickActionP::CLIP_NEAR);
    neardistance = 1.0f;
    // make sure neardistance is smaller than fardistance
    if (fardistance > 0.0f && neardistance >= fardistance) {
      neardistance = fardistance * 0.01f;
    }
  }

  if (fardistance >= 0.0f) THIS->setFlag(SoRayPickActionP::CLIP_FAR);
  else {
    THIS->clearFlag(SoRayPickActionP::CLIP_FAR);
    // just set to some value bigger than neardistance.
    fardistance = neardistance + 10.0f;
  }

  // set these to some values. They will be set to better values
  // in computeWorldSpaceRay() (when we know the view volume).
  THIS->rayradiusstart = 0.01f;
  THIS->rayradiusdelta = 0.0f;

  THIS->raystart = start;
  THIS->raydirection = direction;
  THIS->raydirection.normalize();
  THIS->raynear = neardistance;
  THIS->rayfar = fardistance;
  THIS->wsline = SbLine(start, start + direction);

  // D = shortest distance from origin to plane
  const float D = THIS->raydirection.dot(THIS->raystart);
  THIS->nearplane = SbPlane(THIS->raydirection, D + THIS->raynear);

  THIS->setFlag(SoRayPickActionP::WS_RAY_SET);

  // We use a real cone for picking, but keep pick view volume in sync to be
  // compatible with OIV
  THIS->wsvolume.perspective(0.0, 1.0, neardistance, fardistance);
  THIS->wsvolume.translateCamera(start);
  THIS->wsvolume.rotateCamera(SbRotation(SbVec3f(0.0, 0.0, -1.0), direction));
  THIS->setFlag(SoRayPickActionP::OSVOLUME_DIRTY);
}

/*!
  Lets you decide whether only the closest object or all the objects
  the ray intersects with should be picked.

  Default value of the "pick all" flag is \c FALSE.
*/
void
SoRayPickAction::setPickAll(const SbBool flag)
{
  if (flag) THIS->setFlag(SoRayPickActionP::PICK_ALL);
  else THIS->clearFlag(SoRayPickActionP::PICK_ALL);
}

/*!
  Returns whether only the closest object or all the objects the ray
  intersects with is picked.
*/
SbBool
SoRayPickAction::isPickAll(void) const
{
  return THIS->isFlagSet(SoRayPickActionP::PICK_ALL);
}

/*!
  Returns a list of the picked points.
*/
const SoPickedPointList &
SoRayPickAction::getPickedPointList(void) const
{
  int n = THIS->pickedpointlist.getLength();
  if (!THIS->isFlagSet(SoRayPickActionP::PPLIST_IS_SORTED) && n > 1) {
    SoPickedPoint ** pparray = (SoPickedPoint**) THIS->pickedpointlist.getArrayPtr();
    float * darray = (float*) THIS->ppdistance.getArrayPtr();

    int i, j, distance;
    SoPickedPoint * pptmp;
    float dtmp;

    // shell sort algorithm (O(nlog(n))
    for (distance = 1; distance <= n/9; distance = 3*distance + 1);
    for (; distance > 0; distance /= 3) {
      for (i = distance; i < n; i++) {
        dtmp = darray[i];
        pptmp = pparray[i];
        j = i;
        while (j >= distance && darray[j-distance] > dtmp) {
          darray[j] = darray[j-distance];
          pparray[j] = pparray[j-distance];
          j -= distance;
        }
        darray[j] = dtmp;
        pparray[j] = pptmp;
      }
    }
    THIS->setFlag(SoRayPickActionP::PPLIST_IS_SORTED);
  }

  return THIS->pickedpointlist;
}

/*!
  Returns the picked point with \a index in the list of picked points.

  Returns \c NULL if less than \a index + 1 points where picked during
  the last raypick action.
*/
SoPickedPoint *
SoRayPickAction::getPickedPoint(const int index) const
{
  assert(index >= 0);
  if (index < THIS->pickedpointlist.getLength()) {
    return this->getPickedPointList()[index];
  }
  return NULL;
}

/*!
  \internal
 */
void
SoRayPickAction::computeWorldSpaceRay(void)
{
  if (THIS->isFlagSet(SoRayPickActionP::WS_RAY_SET)) {
    // set the ray radius to some very small value, since
    // the user set the ray manually using setRay().
    //
    // FIXME: Wouldn't it be a nice new feature to be able to
    // set the radius of the ray in setRay()? pederb, 2001-01-05
    const SbViewVolume & vv = SoViewVolumeElement::get(this->state);
    THIS->rayradiusstart = SbMin(vv.getWidth(), vv.getHeight()) * FLT_EPSILON;
    THIS->rayradiusdelta = 0.0f;
  }
  else {
    const SbViewVolume & vv = SoViewVolumeElement::get(this->state);
    const SbViewportRegion & vp = SoViewportRegionElement::get(this->state);

    if (!THIS->isFlagSet(SoRayPickActionP::NORM_POINT)) {
      SbVec2s pt = THIS->vppoint - vp.getViewportOriginPixels();
      SbVec2s size = vp.getViewportSizePixels();
      THIS->normvppoint.setValue(float(pt[0]) / float(size[0]),
                                 float(pt[1]) / float(size[1]));
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
    vv.projectPointToLine(THIS->normvppoint, templine);
    THIS->raystart = templine.getPosition();
    THIS->raydirection = templine.getDirection();

    THIS->raynear = 0.0;
    THIS->rayfar = vv.getDepth();

    SbVec2s vpsize = vp.getViewportSizePixels();
    THIS->rayradiusstart = (double(vv.getHeight()) / double(vpsize[1]))*
      double(THIS->radiusinpixels);
    THIS->rayradiusdelta = 0.0;
    if (vv.getProjectionType() == SbViewVolume::PERSPECTIVE) {
      SbVec3f dir(0.0f, vv.getHeight()*0.5f, vv.getNearDist());
      dir.normalize();
      SbVec3f upperfar = dir * (vv.getNearDist()+vv.getDepth()) /
        dir.dot(SbVec3f(0.0f, 0.0f, 1.0f));

      double farheight = double(upperfar[1])*2.0;
      double farsize = (farheight / double(vpsize[1])) * double(THIS->radiusinpixels);
      THIS->rayradiusdelta = (farsize - THIS->rayradiusstart) / double(vv.getDepth());
    }
    THIS->wsline = SbLine(THIS->raystart,
                          THIS->raystart + THIS->raydirection);

    THIS->nearplane = SbPlane(vv.getProjectionDirection(), THIS->raystart);
    THIS->setFlag(SoRayPickActionP::WS_RAY_COMPUTED);

    // we pick on a real cone, but keep pick view volume in sync to be
    // compatible with OIV.
    float normradius = float(THIS->radiusinpixels) /
      float(SbMin(vp.getViewportSizePixels()[0], vp.getViewportSizePixels()[1]));

    THIS->wsvolume = vv.narrow(THIS->normvppoint[0] - normradius,
                               THIS->normvppoint[1] - normradius,
                               THIS->normvppoint[0] + normradius,
                               THIS->normvppoint[1] + normradius);
    SoPickRayElement::set(state, THIS->wsvolume);
    THIS->setFlag(SoRayPickActionP::OSVOLUME_DIRTY);
  }
}

/*!
  \internal
 */
SbBool
SoRayPickAction::hasWorldSpaceRay(void) const
{
  return THIS->isFlagSet(SoRayPickActionP::WS_RAY_SET|SoRayPickActionP::WS_RAY_COMPUTED);
}

/*!
  \internal
 */
void
SoRayPickAction::setObjectSpace(void)
{
  THIS->clearFlag(SoRayPickActionP::EXTRA_MATRIX);
  THIS->calcObjectSpaceData(this->state);
}

/*!
  \internal
 */
void
SoRayPickAction::setObjectSpace(const SbMatrix & matrix)
{
  THIS->setFlag(SoRayPickActionP::EXTRA_MATRIX);
  THIS->extramatrix = matrix;
  THIS->calcObjectSpaceData(this->state);
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
  // Calculating intersections when we have a degenerate transform
  // makes no sense. We could do the intersection calculations in
  // world space, but it is impossible to calculate the object space
  // intersection point, so we just return FALSE.
  if (!THIS->objectspacevalid) return FALSE;

  const SbVec3f & orig = THIS->osline.getPosition();
  const SbVec3f & dir = THIS->osline.getDirection();

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
  float & u = barycentric[1];
  float & v = barycentric[2];
  float & w = barycentric[0];

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
  // Calculating intersections when we have a degenerate transform
  // makes no sense. We could do the intersection calculations in
  // world space, but it is impossible to calculate the object space
  // intersection point, so we just return FALSE.
  if (!THIS->objectspacevalid) return FALSE;
  
  // test if we have a valid line, and do point intersection testing
  // if we don't
  if (v0 == v1) {
    intersection = v0;
    // this might return TRUE or FALSE. We already set the
    // intersection point.
    return this->intersect(v0);
  }

  SbLine line(v0, v1);
  SbVec3f op0, op1; // object space
  SbVec3f p0, p1; // world space

  THIS->osline.getClosestPoints(line, op0, op1);

  // clamp op1 between v0 and v1
  if ((op1-v0).dot(line.getDirection()) < 0.0f) op1 = v0;
  else if ((v1-op1).dot(line.getDirection()) < 0.0f) op1 = v1;

  THIS->obj2world.multVecMatrix(op0, p0);
  THIS->obj2world.multVecMatrix(op1, p1);

  // distance between points
  float distance = (p1-p0).length();

  float raypos = THIS->nearplane.getDistance(p0);

  float radius = THIS->rayradiusstart +
    THIS->rayradiusdelta * raypos;

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
  // Calculating intersections when we have a degenerate transform
  // makes no sense. We could do the intersection calculations in
  // world space, but it is impossible to calculate the object space
  // intersection point, so we just return FALSE.
  if (!THIS->objectspacevalid) return FALSE;

  SbVec3f wpoint;
  THIS->obj2world.multVecMatrix(point, wpoint);
  SbVec3f ptonline = THIS->wsline.getClosestPoint(wpoint);

  // distance between points
  float distance = (wpoint-ptonline).length();

  float raypos = THIS->nearplane.getDistance(ptonline);

  float radius = THIS->rayradiusstart +
    THIS->rayradiusdelta * raypos;

  return (radius >= distance);
}

// calculates the square distance (smallest possible) from a 2D point
// to a 2D rectangle
static float
dist_to_quad(const float xmin, const float ymin,
             const float xmax, const float ymax,
             const float x, const float y,
             float & cx, float & cy)
{
  if (x < xmin) {
    if (y < ymin) {
      cx = xmin;
      cy = ymin;
      return (x-xmin)*(x-xmin) + (y-ymin)*(y-ymin);
    }
    else if (y > ymax) {
      cx = xmin;
      cy = xmax;
      return (x-xmin)*(x-xmin) + (y-ymax)*(y-ymax);
    }
    else {
      cx = xmin;
      cy = y;
      return (x-xmin)*(x-xmin);
    }
  }
  else if (x > xmax) {
    if (y < ymin) {
      cx = xmax;
      cy = ymin;
      return (x-xmax)*(x-xmax) + (y-ymin) * (y-ymin);
    }
    else if (y > ymax) {
      cx = xmax;
      cy = ymax;
      return (x-xmax)*(x-xmax) + (y-ymax)*(y-ymax);
    }
    else {
      cx = xmax;
      cy = y;
      return (x-xmax)*(x-xmax);
    }
  }
  else {
    if (y < ymin) {
      cx = x;
      cy = ymin;
      return (y-ymin)*(y-ymin);
    }
    else if (y > ymax) {
      cx = x;
      cy = ymax;
      return (y-ymax)*(y-ymax);
    }
    else {
      // inside rectangle
      cx = x;
      cy = y;
      return -1.0f;
    }
  }
}

/*!
  \internal
*/
SbBool
SoRayPickAction::intersect(const SbBox3f & box, SbVec3f & intersection,
                           const SbBool usefullviewvolume)
{
  // Calculating intersections when we have a degenerate transform
  // makes no sense. We could do the intersection calculations in
  // world space, but it is impossible to calculate the object space
  // intersection point, so we just return FALSE.
  if (!THIS->objectspacevalid) return FALSE;

  const SbLine & line = THIS->osline;
  SbVec3f bounds[2];
  bounds[0] = box.getMin();
  bounds[1] = box.getMax();

  SbVec3f ptonray, ptonbox;
  float sqrmindist = FLT_MAX;

  SbBool conepick = usefullviewvolume && !THIS->isFlagSet(SoRayPickActionP::WS_RAY_SET);

  int i;

  if (THIS->isFlagSet(SoRayPickActionP::CLIP_NEAR|SoRayPickActionP::CLIP_FAR)) {
    // check if all points are in front of the near or behind the far
    // clipping plane
    int numnear = 0;
    int numfar = 0;

    for (i = 0; i < 8; i++) {
      SbVec3f bp(i&1 ? bounds[0][0] : bounds[1][0],
                 i&2 ? bounds[0][1] : bounds[1][1],
                 i&4 ? bounds[0][2] : bounds[1][2]);
      THIS->obj2world.multVecMatrix(bp, bp);
      float dist = THIS->nearplane.getDistance(bp);
      if (THIS->isFlagSet(SoRayPickActionP::CLIP_NEAR)) {
        if (dist < 0.0f) numnear++;
      }
      if (THIS->isFlagSet(SoRayPickActionP::CLIP_FAR)) {
        if (dist > (THIS->rayfar - THIS->raynear)) numfar++;
      }
      if ((numnear < i) && (numfar < i)) break;
    }
    if (numnear == 8 || numfar == 8) return FALSE;
  }

  for (int j = 0; j < 2; j++) {
    for (i = 0; i < 3; i++) {
      SbVec3f norm(0.0f, 0.0f, 0.0f);
      norm[i] = 1.0f;
      SbVec3f isect;

      SbPlane plane(norm, bounds[j][i]);
      if (plane.intersect(line, isect)) {
        int i1 = (i+1) % 3;
        int i2 = (i+2) % 3;
        float x, y;

        float d = dist_to_quad(bounds[0][i1], bounds[0][i2],
                               bounds[1][i1], bounds[1][i2],
                               isect[i1], isect[i2],
                               x, y);
        if (d <= 0.0f) {
          // center of ray hit box directly
          intersection = isect;
          return TRUE;
        }
        else if (d < sqrmindist) {
          sqrmindist = d;
          ptonray = ptonbox = isect;
          ptonbox[i1] = x;
          ptonbox[i2] = y;
        }
      }
    }
  }
  if (sqrmindist != FLT_MAX && conepick) {
    // transform ptonray and ptonbox to world space to test on ray cone
    SbVec3f wptonray, wptonbox;
    THIS->obj2world.multVecMatrix(ptonbox, wptonbox);
    THIS->obj2world.multVecMatrix(ptonray, wptonray);

    float raypos = THIS->nearplane.getDistance(wptonray);
    float distance = (wptonray-wptonbox).length();

    // find ray radius at wptonray
    float radius = THIS->rayradiusstart +
      THIS->rayradiusdelta * raypos;

    // test for cone intersection
    if (radius >= distance) {
      intersection = ptonbox; // set intersection to the point on box closest to ray
      return TRUE;
    }
  }
  return FALSE;
}


/*!
  \internal
 */
SbBool
SoRayPickAction::intersect(const SbBox3f & box, const SbBool usefullviewvolume)
{
  SbVec3f dummy;
  return this->intersect(box, dummy, usefullviewvolume);
}

/*!
  \internal
 */
const SbViewVolume &
SoRayPickAction::getViewVolume(void)
{
  if (THIS->objectspacevalid &&
      THIS->isFlagSet(SoRayPickActionP::OSVOLUME_DIRTY)) {
    // we pick on a real cone, but calculate pick view volume
    // to be compatible with OIV.
    THIS->osvolume = SoPickRayElement::get(this->getState());
    if (THIS->isFlagSet(SoRayPickActionP::EXTRA_MATRIX)) {
      SbMatrix m = THIS->world2obj * THIS->extramatrix;
      THIS->osvolume.transform(m);
    }
    else {
      THIS->osvolume.transform(THIS->world2obj);
    }
    THIS->clearFlag(SoRayPickActionP::OSVOLUME_DIRTY);
  }
  return THIS->osvolume;
}

/*!
  \internal
 */
const SbLine &
SoRayPickAction::getLine(void)
{
  return THIS->osline;
}

/*!
  \internal
 */
SbBool
SoRayPickAction::isBetweenPlanes(const SbVec3f & intersection) const
{
  SbVec3f worldpoint;
  THIS->obj2world.multVecMatrix(intersection, worldpoint);
  return THIS->isBetweenPlanesWS(worldpoint,
                                 SoClipPlaneElement::getInstance(this->state));
}

/*!
  \internal
 */
SoPickedPoint *
SoRayPickAction::addIntersection(const SbVec3f & objectspacepoint)
{
  SbVec3f worldpoint;
  THIS->obj2world.multVecMatrix(objectspacepoint, worldpoint);
  float dist = THIS->nearplane.getDistance(worldpoint);

  if (!THIS->isFlagSet(SoRayPickActionP::PICK_ALL) && THIS->pickedpointlist.getLength()) {
    // got to test if new candidate is closer than old one
    if (dist >= THIS->ppdistance[0]) return NULL; // farther
    // remove old point
    delete THIS->pickedpointlist[0];
    THIS->pickedpointlist.truncate(0);
    THIS->ppdistance.truncate(0);
  }

  // create the new picked point
  SoPickedPoint * pp = new SoPickedPoint(this->getCurPath(),
                                         this->state, objectspacepoint);
  THIS->pickedpointlist.append(pp);
  THIS->ppdistance.append(dist);
  THIS->clearFlag(SoRayPickActionP::PPLIST_IS_SORTED);
  return pp;
}

// Documented in superclass.
void
SoRayPickAction::beginTraversal(SoNode * node)
{
  THIS->cleanupPickedPoints();
  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), this->vpRegion);

  if (THIS->isFlagSet(SoRayPickActionP::WS_RAY_SET)) {
    SoPickRayElement::set(state, THIS->wsvolume);
  }
  inherited::beginTraversal(node);
  this->getState()->pop();
}


//////// Hidden private methods for //////////////////////////////////////
//////// SoRayPickActionP (pimpl) ////////////////////////////////////////

SbBool
SoRayPickActionP::isBetweenPlanesWS(const SbVec3f & intersection,
                                    const SoClipPlaneElement * planes) const
{
  float dist = this->nearplane.getDistance(intersection);
  if (this->isFlagSet(CLIP_NEAR)) {
    if (dist < 0) return FALSE;
  }
  if (this->isFlagSet(CLIP_FAR)) {
    if (dist > (this->rayfar - this->raynear)) return FALSE;
  }
  int n =  planes->getNum();
  for (int i = 0; i < n; i++) {
    if (!planes->get(i).isInHalfSpace(intersection)) return FALSE;
  }
  return TRUE;
}

void
SoRayPickActionP::cleanupPickedPoints(void)
{
  int n = this->pickedpointlist.getLength();

  for (int i = 0; i < n; i++) {
    delete this->pickedpointlist[i];
  }
  this->pickedpointlist.truncate(0);
  this->ppdistance.truncate(0);
  this->clearFlag(PPLIST_IS_SORTED);
}

void
SoRayPickActionP::setFlag(const unsigned int flag)
{
  this->flags |= flag;
}

void
SoRayPickActionP::clearFlag(const unsigned int flag)
{
  this->flags &= ~flag;
}

SbBool
SoRayPickActionP::isFlagSet(const unsigned int flag) const
{
  return (this->flags & flag) != 0;
}

void
SoRayPickActionP::calcObjectSpaceData(SoState * ownerstate)
{
  this->calcMatrices(ownerstate);

  SbVec3f start, dir;

  if (this->objectspacevalid) {
    this->world2obj.multVecMatrix(this->raystart, start);
    this->world2obj.multDirMatrix(this->raydirection, dir);
    this->osline = SbLine(start, start + dir);
  }
}

void
SoRayPickActionP::calcMatrices(SoState * state)
{
  const float VALID_LIMIT = 1.0e-12f;  // FIXME: why not FLT_EPSILON? 20010916 mortene.
  this->obj2world = SoModelMatrixElement::get(state);
  if (this->isFlagSet(EXTRA_MATRIX)) {
    this->obj2world.multLeft(this->extramatrix);
  }
  this->objectspacevalid = FALSE;
  float det = this->obj2world.det4();
  if (SbAbs(det) > VALID_LIMIT) {
    this->world2obj = this->obj2world.inverse();
    this->objectspacevalid = TRUE;
  }
}
