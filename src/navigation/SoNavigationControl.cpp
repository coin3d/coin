/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <assert.h>
#include <stdio.h>

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoType.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <Inventor/navigation/SoNavigationControl.h>

// *************************************************************************

/*!
  \class SoNavigationInfo Inventor/navigation/SoNavigationInfo.h
  \brief Helper class for classes implementing navigation modes.

  This class contains helper functions for implementing navigation modes.

  Consider this class internal unless you are implementing your own
  navigation modes.

  Certain custom camera node types are also supported in this class as
  transparently as possible.

  \ingroup navigation
*/

// *************************************************************************

class SoNavigationControlP {
public:
  SoNavigationControlP(SoNavigationControl * api);
  ~SoNavigationControlP(void);

  SbViewportRegion viewport;
  SoCamera * initcamera;
  SoCamera * cameraptr;
  SbVec3f upvec;

  SoNode * scenegraph;

  SoRayPickAction * rpaction;

  // custom cameras
  SoType utmcamtype;
  SoType utmpostype;
  SoType frustumcamtype;
}; // SoNavigationControlP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/

SoNavigationControl::SoNavigationControl(void)
{
  PRIVATE(this) = new SoNavigationControlP(this);
}

/*!
  Destructor.
*/

SoNavigationControl::~SoNavigationControl(void)
{
  this->setSceneGraph(NULL);
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Sets the scene graph.  Needed for certain navigation modes that for
  instance perform picking.

  \sa getSceneGraph
*/

void
SoNavigationControl::setSceneGraph(SoNode * scenegraph)
{
  if (PRIVATE(this)->scenegraph) {
    PRIVATE(this)->scenegraph->unref();
    PRIVATE(this)->scenegraph = NULL;
  }
  if (scenegraph) {
    PRIVATE(this)->scenegraph = scenegraph;
    PRIVATE(this)->scenegraph->ref();
  }
}

/*!
  Returns the set scene graph.

  \sa setSceneGraph
*/

SoNode *
SoNavigationControl::getSceneGraph(void) const
{
  return PRIVATE(this)->scenegraph;
}

/*!
  Sets the pointer to the camera that is to be manipulated.

  \sa getCamera
*/

void
SoNavigationControl::setCamera(SoCamera * camera)
{
  if (PRIVATE(this)->initcamera) {
    PRIVATE(this)->initcamera->unref();
    PRIVATE(this)->initcamera = NULL;
  }
  if (PRIVATE(this)->cameraptr) {
    PRIVATE(this)->cameraptr->unref();
    PRIVATE(this)->cameraptr = NULL;
  }
  if (camera) {
    PRIVATE(this)->initcamera = (SoCamera *) camera->copy(FALSE);
    PRIVATE(this)->initcamera->ref();
    PRIVATE(this)->cameraptr = camera;
    PRIVATE(this)->cameraptr->ref();
  }
}

/*!
  Returns the pointer the the camera that is to be manipulated.

  \sa setCamera
*/

SoCamera *
SoNavigationControl::getCamera(void) const
{
  return PRIVATE(this)->cameraptr;
}

/*!
  This method lets you set the up vector.  Default value is (0, 0, 0)
  which means no up vector is set.

  \sa getUpVector, resetRoll
*/

void
SoNavigationControl::setUpVector(const SbVec3f & up)
{
  PRIVATE(this)->upvec = up;
}

/*!
  This method returns the up vector.  Default value is (0, 0, 0) which
  is interpreted as no up direction.

  \sa setUpVector, resetRoll
*/

const SbVec3f &
SoNavigationControl::getUpVector(void) const
{
  return PRIVATE(this)->upvec;
}

/*!
  This method <i>saves</i> the current camera position and orientation.
  You can later restore the camera back to this position by calling
  restoreCamera().

  \sa restoreCamera
*/

void
SoNavigationControl::saveCamera(void) const
{
  if (!PRIVATE(this)->cameraptr || !PRIVATE(this)->initcamera) return;
  PRIVATE(this)->initcamera->copyFieldValues(PRIVATE(this)->cameraptr);
}

/*!
  This method restores the camera back to its last <i>set</i> position,
  or its initial position if saveCamera() has not been used yet.

  \sa saveCamera
*/

void
SoNavigationControl::restoreCamera(void) const
{
  if (PRIVATE(this)->cameraptr == NULL) return;
  PRIVATE(this)->cameraptr->copyFieldValues(PRIVATE(this)->initcamera);
}

/*!
  Performs a pick in the scene.

  \a pos is the 2D locationon the viewport.
  \a pickpos is the returned 3D pick position.

  The method returns the SoPath to the picked object if a point was
  picked, and NULL otherwise.
*/

SoPath *
SoNavigationControl::pick(SbVec2s pos, SbVec3d & pickpos) const
{
  SoCamera * camera = this->getCamera();
  SoNode * scene = this->getSceneGraph();
  if (!camera || !scene) return NULL;

  SbViewportRegion vp;
  vp.setWindowSize(this->getViewportSize());
  if (!PRIVATE(this)->rpaction) {
    PRIVATE(this)->rpaction = new SoRayPickAction(vp);
  } else {
    PRIVATE(this)->rpaction->reset();
    PRIVATE(this)->rpaction->setViewportRegion(vp);
  }
  PRIVATE(this)->rpaction->setPoint(pos);
  PRIVATE(this)->rpaction->apply(scene);

  SoPickedPoint * pp = PRIVATE(this)->rpaction->getPickedPoint();
  if (!pp) {
    PRIVATE(this)->rpaction->reset();
    pickpos.setValue(0.0, 0.0, 0.0);
    return NULL;
  }

  pickpos.setValue(pp->getPoint());

  SoPath * path = pp->getPath();
  if (PRIVATE(this)->utmcamtype != SoType::badType() &&
      camera->isOfType(PRIVATE(this)->utmcamtype)) {
    SoSearchAction sa;
    SbBool searchingchildren = SoBaseKit::isSearchingChildren();
    SoBaseKit::setSearchingChildren(TRUE);
    sa.setType(PRIVATE(this)->utmpostype);
    sa.setInterest(SoSearchAction::LAST);
    sa.apply(path);
    SoPath * pospath = sa.getPath();
    if (pospath) {
      SoNode * utmposnode = ((SoFullPath *)pospath)->getTail();
      SoSFVec3d * camposfield = (SoSFVec3d *) camera->getField("utmposition");
      SoSFVec3d * utmposfield = (SoSFVec3d *) utmposnode->getField("utmposition");
      assert(camposfield); assert(utmposfield);
      // pickpos -= utmposfield->getValue();
      pickpos += camposfield->getValue();
    } else {
      // probable incorrect utm setup
#if 0
      SoDebugError::postWarning("SoNavigationControl::pick",
                                "UTMCamera but no UTMPosition node. Probable "
                                "bogus UTM-based scene graph setup.");
#endif
    }
    SoBaseKit::setSearchingChildren(searchingchildren);
  }
  return path;
}

/*!
  Positions the camera so that the whole model gets in view.
*/

void
SoNavigationControl::viewAll(void) const
{
  SoCamera * camera = this->getCamera();
  SoNode * root = this->getSceneGraph();
  if (camera == NULL || root == NULL) return;

  if (PRIVATE(this)->utmcamtype != SoType::badType() &&
      camera->isOfType(PRIVATE(this)->utmcamtype)) {
    // we need to set UTMCamera->utmposition to some initial value not too far
    // off the world space limits to reduce floating point precision errors
    SoSearchAction sa;
    SbBool sc = SoBaseKit::isSearchingChildren();
    SoBaseKit::setSearchingChildren(TRUE);
    sa.setSearchingAll(TRUE);
    sa.setInterest(SoSearchAction::FIRST);
    sa.setType(PRIVATE(this)->utmpostype);
    sa.apply(root);
    if (sa.getPath()) {
      // introspective code since we don't #include class declarations
      SoFieldContainer * utmpos = ((SoFullPath *) sa.getPath())->getTail();
      SoSFVec3d * utmposfield = (SoSFVec3d *) utmpos->getField("utmposition");
      SoSFVec3d * camposfield = (SoSFVec3d *) camera->getField("utmposition");
      assert(camposfield && utmposfield);
      *camposfield = *utmposfield;
    }
    else {
      SoDebugError::postWarning("SoNavigationControl::viewAll",
                                "You're using UTMCamera. "
                                "Please consider supplying at least one "
                                "UTMPosition node in your scene graph.");
    }
    SoBaseKit::setSearchingChildren(sc);
  }
  SbViewportRegion vp(this->getViewportSize());
  camera->viewAll(root, vp);

  if (PRIVATE(this)->utmcamtype != SoType::badType() &&
      camera->isOfType(PRIVATE(this)->utmcamtype)) {
    // move from position to utmposition and set position to (0,0,0)
    // introspective code since we don't #include class declarations
    SoSFVec3d * camposfield = (SoSFVec3d *) camera->getField("utmposition");
    SbVec3d utmpos = camposfield->getValue();
    SbVec3d tmp;
    tmp.setValue(camera->position.getValue());
    utmpos += tmp;
    camera->position = SbVec3f(0.0f, 0.0f, 0.0f);
    camposfield->setValue(utmpos);
  }
}

void
SoNavigationControl::viewPart(SoPath * path, const SbVec3f & in, const SbVec3f & up) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL || path == NULL) return;

  int i;
  for ( i = 0; i < 2; i++ ) {
    SbBool oldsearch = SoBaseKit::isSearchingChildren();
    SoBaseKit::setSearchingChildren(TRUE);

    if (PRIVATE(this)->utmcamtype != SoType::badType() &&
        camera->isOfType(PRIVATE(this)->utmcamtype)) {
      
      SoSearchAction sa;
      sa.setSearchingAll(TRUE);
      sa.setInterest(SoSearchAction::FIRST);
      sa.setType(SoType::fromName("UTMPosition"));
      sa.apply(path);
      if (sa.getPath()) {
        SoFullPath * p = (SoFullPath *) sa.getPath();
        SoNode * utm = p->getTail();
        SoSFVec3d * camutmposition =
          (SoSFVec3d *) camera->getField("utmposition");
        SoSFVec3d * utmposition = (SoSFVec3d *) utm->getField("utmposition");
        *camutmposition = *utmposition;
      } else {
        SoDebugError::postWarning("SoNavigationControl::viewPart",
                                  "Using UTMCamera, but no UTMPosition node "
                                  "found in scene graph.");
      }
    }
    SoBaseKit::setSearchingChildren(oldsearch);

    SbViewportRegion vp(this->getViewportSize());
    camera->viewAll(path, vp);

    if (PRIVATE(this)->utmcamtype != SoType::badType() &&
        camera->isOfType(PRIVATE(this)->utmcamtype)) {
      
      SoSFVec3d * camutmposition = (SoSFVec3d *) camera->getField("utmposition");
      SbVec3d utmpos = camutmposition->getValue();
      SbVec3d tmp;
      tmp.setValue(camera->position.getValue());
      utmpos += tmp;
      camera->position = SbVec3f(0.0f, 0.0f, 0.0f);
      *camutmposition = utmpos;
    }
    
    // focalpoint is now center point
    
    if (PRIVATE(this)->utmcamtype != SoType::badType() &&
        camera->isOfType(PRIVATE(this)->utmcamtype)) {
      
      SoSFVec3d * camutmposition = (SoSFVec3d *) camera->getField("utmposition");
      SbVec3d focalpoint = camutmposition->getValue();
      
      const double focaldist = (double) camera->focalDistance.getValue();
      SbRotation camrot = camera->orientation.getValue();
      SbVec3f dir(0.0f, 0.0f, -1.0f);
      camrot.multVec(dir, dir);
      SbVec3d dird(dir[0], dir[1], dir[2]);
      focalpoint += dird * focaldist;
      
      SbVec3d invec(in[0], in[1], in[2]);
      invec.normalize();
      
      SbVec3d camerapos = focalpoint;
      camerapos -= invec * double(focaldist);
      
      camutmposition->setValue(camerapos);
      
      SbVec3f nin = in;
      nin.normalize();
      
      SbRotation newrot(SbVec3f(0, 0, -1), nin);
      camera->orientation.setValue(newrot);
    } else {
      // FIXME: implement
      // camera->pointAt(center);
    }
    
    SbVec3f oldup = PRIVATE(this)->upvec;
    PRIVATE(this)->upvec = up;
    this->resetRoll();
    PRIVATE(this)->upvec = oldup;
  }
}

/*!
  Orients the camera so that it points in the given direction.

  \sa reorientCamera
*/

void
SoNavigationControl::pointDir(const SbVec3f & dir) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;
  camera->orientation = SbRotation(SbVec3f(0.0f, 0.0f, -1.0f), dir);
}

/*!
  This method reorients the camera according to the given rotation.

  \sa saveCamera, restoreCamera, moveCamera
*/

void
SoNavigationControl::reorientCamera(const SbRotation & rot) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  // Find global coordinates of focal point.
  SbVec3f direction;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * direction;

  // Set new orientation value by accumulating the new rotation.
  camera->orientation = rot * camera->orientation.getValue();

  // Reposition camera so we are still pointing at the same old focal point.
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  camera->position = focalpoint - camera->focalDistance.getValue() * direction;

  if (PRIVATE(this)->utmcamtype != SoType::badType() &&
      camera->isOfType(PRIVATE(this)->utmcamtype)) {
    SbVec3d offset;
    offset.setValue(camera->position.getValue());
    SoSFVec3d * utmpositionfield =
      (SoSFVec3d *) camera->getField("utmposition");
    assert(utmpositionfield);
    utmpositionfield->setValue(utmpositionfield->getValue()+offset);
    camera->position.setValue(0.0f, 0.0f, 0.0f);
  }
}

/*!
  This method reorients the camera to point at the \a pointat location.
*/

void
SoNavigationControl::reorientCamera(const SbVec3f & pointat) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;
  SbRotation rot = camera->orientation.getValue();
  SbVec3f up;
  rot.multVec(SbVec3f(0, 1, 0), up);
  if (PRIVATE(this)->utmcamtype != SoType::badType() &&
      camera->isOfType(PRIVATE(this)->utmcamtype)) {
    SoSFVec3d * utmcamposfield = (SoSFVec3d *) camera->getField("utmposition");
    assert(utmcamposfield);
    SbVec3d campos = utmcamposfield->getValue();
    SbVec3f camposf((float)campos[0], (float)campos[1], (float)campos[2]);
    SbVec3f vec = pointat - camposf;
    SbVec3f point = pointat - camposf;
    camera->pointAt(point, up);
    camera->focalDistance = vec.length();
  } else {
    SbVec3f vec = pointat - camera->position.getValue();
    camera->pointAt(pointat, up);
    camera->focalDistance = vec.length();
  }
}

/*!
  This method reorients the camera in the pitch plane.

  \sa rollCamera, yawCamera
*/

void
SoNavigationControl::pitchCamera(float angle) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;
  SbMatrix camerarot;
  camerarot.setRotate(camera->orientation.getValue());
  SbMatrix pitchmat;
  pitchmat.setRotate(SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), angle));
  camerarot.multLeft(pitchmat);
  camera->orientation = SbRotation(camerarot);
}

/*!
  This method reorients the camera in the yaw plane.

  \sa pitchCamera, rollCamera
*/

void
SoNavigationControl::yawCamera(float angle) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;
  SbMatrix camerarot;
  camerarot.setRotate(camera->orientation.getValue());
  SbMatrix yawmat;
  yawmat.setRotate(SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), angle));
  camerarot.multLeft(yawmat);
  camera->orientation = SbRotation(camerarot);
}

/*!
  This method reorients the camera in the roll plane.

  \sa pitchCamera, yawCamera
*/

void
SoNavigationControl::rollCamera(float angle) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  SbMatrix camerarot;
  camerarot.setRotate(camera->orientation.getValue());
  SbMatrix rollmat;
  rollmat.setRotate(SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), angle));
  camerarot.multLeft(rollmat);
  camera->orientation = SbRotation(camerarot);
}

/*!
  This method resets the roll of the camera.  If no up vector has been
  set, this method does nothing.

  \sa rollCamera, setUpVector, getUpVector
*/

void
SoNavigationControl::resetRoll(void) const
{
  SoCamera * camera = this->getCamera();
  if (!camera) return;

  SbVec3f newy = PRIVATE(this)->upvec;
  if (newy == SbVec3f(0.0f, 0.0f, 0.0f)) return;

  SbMatrix camerarot;
  camerarot.setRotate(camera->orientation.getValue());

  SbVec3f Z;
  Z[0] = camerarot[2][0];
  Z[1] = camerarot[2][1];
  Z[2] = camerarot[2][2];

  if (fabs(Z.dot(newy)) > 0.99f) {
    // just give up
    return;
  }
  SbVec3f newx = newy.cross(Z);
  newy = Z.cross(newx);

  newx.normalize();
  newy.normalize();
  camerarot[0][0] = newx[0];
  camerarot[0][1] = newx[1];
  camerarot[0][2] = newx[2];
  camerarot[1][0] = newy[0];
  camerarot[1][1] = newy[1];
  camerarot[1][2] = newy[2];
  camera->orientation = SbRotation(camerarot);
}

/*!
  This method moves the camera.

  \sa reorientCamera, saveCamera, restoreCamera
*/

void
SoNavigationControl::moveCamera(const SbVec3f & distance) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  if (PRIVATE(this)->utmcamtype != SoType::badType() &&
      camera->isOfType(PRIVATE(this)->utmcamtype)) {
    SoSFVec3d * utmposfield = (SoSFVec3d *) camera->getField("utmposition");
    assert(utmposfield);
    SbVec3d distanced;
    distanced.setValue(distance);
    *utmposfield = utmposfield->getValue() + distanced;
  } else {
    camera->position = camera->position.getValue() + distance;
  }
}

/*!
  This camera moves the camera straight forwards towards the focal
  point.  The position of the focal point can be kept, or moved
  equally forwards as well.
*/

void
SoNavigationControl::moveCamera(float factor, const SbBool keepfocalpoint) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  SbVec3f vec;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), vec);
  vec.normalize();
  vec = vec * camera->focalDistance.getValue();
  if (PRIVATE(this)->utmcamtype != SoType::badType() &&
      camera->isOfType(PRIVATE(this)->utmcamtype)) {
    SoSFVec3d * utmposfield = (SoSFVec3d *) camera->getField("utmposition");
    assert(utmposfield);
    SbVec3d sizedvec;
    sizedvec.setValue(vec*factor);
    *utmposfield = utmposfield->getValue() + sizedvec;
  } else {
    camera->position = camera->position.getValue() + (vec * factor);
  }

  if (keepfocalpoint) {
    camera->focalDistance = camera->focalDistance.getValue() * (1.0f - factor);
  }
}

/*!
  This method sets the viewport information.
*/

void
SoNavigationControl::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->viewport = vp;
}

/*!
  Returns the size of the viewport in pixels.
*/

SbVec2s
SoNavigationControl::getViewportSize(void) const
{
  return PRIVATE(this)->viewport.getWindowSize();
}

/*!
  Returns the aspect ratio of the viewport - width divided by height.
*/

float
SoNavigationControl::getViewportAspect(void) const
{
  return PRIVATE(this)->viewport.getViewportAspectRatio();
}

#undef PRIVATE

// *************************************************************************
// private implementation

SoNavigationControlP::SoNavigationControlP(SoNavigationControl * api)
{
  this->scenegraph = NULL; 
  this->initcamera = NULL;
  this->cameraptr = NULL;
  this->upvec.setValue(0.0f, 0.0f, 0.0f);
  this->rpaction = NULL;
  // to identify certain custom nodes
  this->utmcamtype = SoType::fromName("UTMCamera");
  this->utmpostype = SoType::fromName("UTMPosition");
  this->frustumcamtype = SoType::fromName("FrustumCamera");
}

SoNavigationControlP::~SoNavigationControlP(void)
{
  if (this->initcamera) {
    this->initcamera->unref();
    this->initcamera = NULL;
  }
  if (this->cameraptr) {
    this->cameraptr->unref();
    this->cameraptr = NULL;
  }
  if (this->rpaction) {
    delete this->rpaction;
    this->rpaction = NULL;
  }
}

// *************************************************************************
