/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTrackballDragger SoTrackballDragger.h Inventor/draggers/SoTrackballDragger.h
  \brief The SoTrackballDragger provides the end-user with rotation and scaling interaction possibilities.
  \ingroup draggers

  \DRAGGER_DEFAULT_SCREENSHOT

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/trackball.png">
  </center>

  The dragger has three "bands" around a transparent sphere, which
  when clicked and dragged will rotate around the three principal
  axes. In addition, the user can define his own rotation axis by
  holding down a SHIFT key while clicking with the left mouse button
  and dragging. A new, distinctively colored axis and dragger band
  will be then be added.

  Free-form rotation on the sphere can be done by clicking and
  dragging \e outside the pre-defined "rotation bands".

  Scale operations are made available by holding down a CTRL key when
  clicking when the left mouse button. The dragger is then in scaling
  mode, and dragging while holding down the mouse button will \e
  uniformly scale the trackball sphere.

  For the application programmer's convenience, the Coin library also
  provides a manipulator class called SoTrackballManip, which wraps
  the SoTrackballDragger into the necessary mechanisms for making
  direct insertion of this dragger into a scenegraph possible with
  very little effort.

  \sa SoTrackballManip
*/

#include <Inventor/draggers/SoTrackballDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/projectors/SbCylinderPlaneProjector.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/SoPath.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/SoDB.h>
#include <Inventor/fields/SoSFTime.h>

#include <data/draggerDefaults/trackballDragger.h>

/*!
  \var SoSFRotation SoTrackballDragger::rotation

  This field is continuously updated to contain the rotation of the
  dragger's trackball.
*/

/*!
  \var SoSFVec3f SoTrackballDragger::scaleFactor

  Continuously updated to contain the current vector of scaling along
  the X, Y and Z axes. The three components will always be equal, as
  this dragger only supports uniform scale operations.
*/

/*!
  \var SoFieldSensor * SoTrackballDragger::rotFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoTrackballDragger::scaleFieldSensor
  \COININTERNAL
*/

// don't change these values!
#define WHATKIND_NONE          0
#define WHATKIND_ROTATOR       1
#define WHATKIND_XROTATOR      2
#define WHATKIND_YROTATOR      3
#define WHATKIND_ZROTATOR      4
#define WHATKIND_USERAXIS      5
#define WHATKIND_USERROTATOR   6
#define WHATKIND_SCALE         7

#define USER_AXIS_DISAPPEAR_LIMIT 0.98f

#ifndef DOXYGEN_SKIP_THIS
class SoTrackballDraggerP {
public:
  SoTrackballDraggerP(SoTrackballDragger * master)
    : master(master) { }
  SoTrackballDragger * master;
  SbSphereProjector * sphereProj;
  SbCylinderProjector * cylProj;
  SbLineProjector * lineProj;

  int whatkind;
  SbBool animationEnabled;
  SbVec2f prevMousePos;
  SbTime prevTime;
  SbTime animTime;
  SbVec3f animAxis;
  float animAngle;
  SbBool hasDragged;
  SbMatrix prevMotionMatrix;
  SbVec3f prevWorldHitPt;
  SoTimerSensor * timerSensor;

  void getSpaceMatrices(SbMatrix & ws2wk, SbMatrix & wk2ws,
                        SbMatrix & wk2loc, SbMatrix & loc2wk);
};

#endif // DOXYGEN_SKIP_THIS

//
// returns the current time. First tries the realTime field, then
// SbTime::getTimeOfDay().
//
static SbTime
get_current_time(void)
{
  SoField * realtime = SoDB::getGlobalField("realTime");
  if (realtime && realtime->isOfType(SoSFTime::getClassTypeId())) {
    return ((SoSFTime*)realtime)->getValue();
  }
  return SbTime::getTimeOfDay();
}


SO_KIT_SOURCE(SoTrackballDragger);

// doc in superclass
void
SoTrackballDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTrackballDragger, SO_FROM_INVENTOR_1);
}

#undef THIS
#define THIS this->pimpl
#undef THISP
#define THISP thisp->pimpl

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoTrackballDragger
  -->"this"
        "callbackList"
        "topSeparator"
           "motionMatrix"
  -->      "surroundScale"
  -->      "antiSquish"
           "geomSeparator"
  -->         "rotatorSwitch"
  -->            "rotator"
  -->            "rotatorActive"
  -->         "XRotatorSwitch"
  -->            "XRotator"
  -->            "XRotatorActive"
  -->         "YRotatorSwitch"
  -->            "YRotator"
  -->            "YRotatorActive"
  -->         "ZRotatorSwitch"
  -->            "ZRotator"
  -->            "ZRotatorActive"
  -->         "userAxisRotation"
  -->         "userAxisSwitch"
  -->            "userAxis"
  -->            "userAxisActive"
  -->         "userRotatorSwitch"
  -->            "userRotator"
  -->            "userRotatorActive"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoTrackballDragger
  PVT   "this",  SoTrackballDragger  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
  PVT   "motionMatrix",  SoMatrixTransform  --- 
        "surroundScale",  SoSurroundScale  --- 
        "antiSquish",  SoAntiSquish  --- 
  PVT   "geomSeparator",  SoSeparator  --- 
  PVT   "rotatorSwitch",  SoSwitch  --- 
        "rotator",  SoSeparator  --- 
        "rotatorActive",  SoSeparator  --- 
  PVT   "XRotatorSwitch",  SoSwitch  --- 
        "XRotator",  SoSeparator  --- 
        "XRotatorActive",  SoSeparator  --- 
  PVT   "YRotatorSwitch",  SoSwitch  --- 
        "YRotator",  SoSeparator  --- 
        "YRotatorActive",  SoSeparator  --- 
  PVT   "ZRotatorSwitch",  SoSwitch  --- 
        "ZRotator",  SoSeparator  --- 
        "ZRotatorActive",  SoSeparator  --- 
  PVT   "userAxisRotation",  SoRotation  --- 
  PVT   "userAxisSwitch",  SoSwitch  --- 
        "userAxis",  SoSeparator  --- 
        "userAxisActive",  SoSeparator  --- 
  PVT   "userRotatorSwitch",  SoSwitch  --- 
        "userRotator",  SoSeparator  --- 
        "userRotatorActive",  SoSeparator  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/
SoTrackballDragger::SoTrackballDragger(void)
{
  THIS = new SoTrackballDraggerP(this);

  SO_KIT_INTERNAL_CONSTRUCTOR(SoTrackballDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, XRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotatorSwitch, SoSwitch, TRUE, geomSeparator, YRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotator, SoSeparator, TRUE, XRotatorSwitch, XRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotatorActive, SoSeparator, TRUE, XRotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotatorSwitch, SoSwitch, TRUE, geomSeparator, ZRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotator, SoSeparator, TRUE, YRotatorSwitch, YRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotatorActive, SoSeparator, TRUE, YRotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotatorSwitch, SoSwitch, TRUE, geomSeparator, userAxisRotation, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotator, SoSeparator, TRUE, ZRotatorSwitch, ZRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotatorActive, SoSeparator, TRUE, ZRotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxisRotation, SoRotation, TRUE, geomSeparator, userAxisSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxisSwitch, SoSwitch, TRUE, geomSeparator, userRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxis, SoSeparator, TRUE, userAxisSwitch, userAxisActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxisActive, SoSeparator, TRUE, userAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userRotatorSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(userRotator, SoSeparator, TRUE, userRotatorSwitch, userRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userRotatorActive, SoSeparator, TRUE, userRotatorSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("trackballDragger.iv",
                                       TRACKBALLDRAGGER_draggergeometry,
                                       sizeof(TRACKBALLDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  this->setPartAsDefault("rotator", "trackballRotator");
  this->setPartAsDefault("rotatorActive", "trackballRotatorActive");
  this->setPartAsDefault("XRotator", "trackballXRotator");
  this->setPartAsDefault("XRotatorActive", "trackballXRotatorActive");
  this->setPartAsDefault("YRotator", "trackballYRotator");
  this->setPartAsDefault("YRotatorActive", "trackballYRotatorActive");
  this->setPartAsDefault("ZRotator", "trackballZRotator");
  this->setPartAsDefault("ZRotatorActive", "trackballZRotatorActive");
  this->setPartAsDefault("userAxis", "trackballUserAxis");
  this->setPartAsDefault("userAxisActive", "trackballUserAxisActive");
  this->setPartAsDefault("userRotator", "trackballUserRotator");
  this->setPartAsDefault("userRotatorActive", "trackballUserRotatorActive");

  SoAntiSquish *squish = SO_GET_ANY_PART(this, "antiSquish", SoAntiSquish);
  squish->sizing = SoAntiSquish::LONGEST_DIAGONAL;

  THIS->animationEnabled = TRUE;
  THIS->whatkind = WHATKIND_NONE;
  THIS->sphereProj = new SbSpherePlaneProjector;
  THIS->cylProj = new SbCylinderPlaneProjector;
  THIS->lineProj = new SbLineProjector();

  this->setAllPartsActive(FALSE);
  this->updateUserAxisSwitches();


  this->addStartCallback(SoTrackballDragger::startCB);
  this->addMotionCallback(SoTrackballDragger::motionCB);
  this->addFinishCallback(SoTrackballDragger::finishCB);
  this->addValueChangedCallback(SoTrackballDragger::valueChangedCB);
  this->addOtherEventCallback(SoTrackballDragger::metaKeyChangeCB);

  this->rotFieldSensor = new SoFieldSensor(SoTrackballDragger::fieldSensorCB, this);
  this->rotFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoTrackballDragger::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);

  THIS->timerSensor = new SoTimerSensor(SoTrackballDragger::timerSensorCB, this);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoTrackballDragger::~SoTrackballDragger()
{
  delete this->rotFieldSensor;
  delete this->scaleFieldSensor;
  delete THIS->sphereProj;
  delete THIS->cylProj;
  delete THIS->lineProj;

  delete THIS;
}

// Doc in superclass.
SbBool
SoTrackballDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoTrackballDragger::fieldSensorCB(this, NULL);

    if (this->rotFieldSensor->getAttachedField() != &this->rotation) {
      this->rotFieldSensor->attach(&this->rotation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }
  }
  else {
    if (this->rotFieldSensor->getAttachedField() != NULL) {
      this->rotFieldSensor->detach();
    }
    if (this->scaleFieldSensor->getAttachedField() != NULL) {
      this->scaleFieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

// Doc in superclass.
void
SoTrackballDragger::setDefaultOnNonWritingFields(void)
{
  this->antiSquish.setDefault(TRUE);
  this->surroundScale.setDefault(TRUE);

  SoSwitch * sw = (SoSwitch*) this->userAxisSwitch.getValue();
  if (sw && sw->whichChild.getValue() == SO_SWITCH_NONE)
    this->userAxisSwitch.setDefault(TRUE);

  inherited::setDefaultOnNonWritingFields();
}

/*! \COININTERNAL */
void
SoTrackballDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoTrackballDragger::valueChangedCB(void *, SoDragger * d)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);

  thisp->rotFieldSensor->detach();
  if (thisp->rotation.getValue() != rot)
    thisp->rotation = rot;
  thisp->rotFieldSensor->attach(&thisp->rotation);

  thisp->scaleFieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->scaleFieldSensor->attach(&thisp->scaleFactor);
}

/*!
  Whether or not the trackball will start a spin animation after a
  quick drag-and-release.

  Note: this feature is not supported yet in the Coin library, so even
  though if \c TRUE is returned, spin animations will not be possible
  to trigger.
*/
SbBool
SoTrackballDragger::isAnimationEnabled(void)
{
  return THIS->animationEnabled;
}

/*!
  Set a flag which will decide whether or not the trackball will start
  a spin animation after a quick drag-and-release.
  
  Note: this feature is not supported yet in the Coin library.
*/
void
SoTrackballDragger::setAnimationEnabled(SbBool newval)
{
  THIS->animationEnabled = newval;
}

// Invalidate surround scale node, if it exists.
//
// Note: keep the function name prefix to avoid name clashes with
// other dragger .cpp files for "--enable-compact" builds.
//
// FIXME: should collect these methods in a common method visible to
// all draggers implementing the exact same functionality. 20010826 mortene.
static void
SoTrackballDragger_invalidate_surroundscale(SoBaseKit * kit)
{
  SoSurroundScale * ss = (SoSurroundScale*)
    kit->getPart("surroundScale", FALSE);
  if (ss) ss->invalidate();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoTrackballDragger::dragStart(void)
{
  SoTrackballDragger_invalidate_surroundscale(this);

  if (THIS->timerSensor->isScheduled()) {
    THIS->timerSensor->unschedule();
  }
  const SoPath *pickpath = this->getPickPath();
  const SoEvent *event = this->getEvent();
  THIS->whatkind = WHATKIND_NONE;
  SoSwitch *sw;

  if (event->wasCtrlDown()) {
    THIS->whatkind = WHATKIND_SCALE;
    this->setAllPartsActive(TRUE);
  }
  else if (event->wasShiftDown()) {
    THIS->whatkind = WHATKIND_USERAXIS;
    this->updateUserAxisSwitches(TRUE);
  }

  SbVec3f axis(0.0f, 1.0f, 0.0f);
  if (!THIS->whatkind && (pickpath->findNode(this->getNodeFieldNode("rotator")) >= 0 ||
                          this->getSurrogatePartPickedName() == "rotator")) {
    THIS->whatkind = WHATKIND_ROTATOR;
    this->setAllPartsActive(TRUE);
  }
  if (!THIS->whatkind && (pickpath->findNode(this->getNodeFieldNode("XRotator")) >= 0 ||
                          this->getSurrogatePartPickedName() == "XRotator")) {
    THIS->whatkind = WHATKIND_XROTATOR;
    sw = SO_GET_ANY_PART(this, "XRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(1.0f, 0.0f, 0.0f);
  }
  if (!THIS->whatkind && (pickpath->findNode(this->getNodeFieldNode("YRotator")) >= 0 ||
                          this->getSurrogatePartPickedName() == "YRotator")) {
    THIS->whatkind = WHATKIND_YROTATOR;
    sw = SO_GET_ANY_PART(this, "YRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(0.0f, 1.0f, 0.0f);
  }
  if (!THIS->whatkind && (pickpath->findNode(this->getNodeFieldNode("ZRotator")) >= 0 ||
                          this->getSurrogatePartPickedName() == "ZRotator")) {
    THIS->whatkind = WHATKIND_ZROTATOR;
    sw = SO_GET_ANY_PART(this, "ZRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(0.0f, 0.0f, 1.0f);
  }
  if (!THIS->whatkind && (pickpath->findNode(this->getNodeFieldNode("userRotator")) >= 0 ||
                          this->getSurrogatePartPickedName() == "userRotation")) {
    THIS->whatkind = WHATKIND_USERROTATOR;
    sw = SO_GET_ANY_PART(this, "userRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(0.0f, 1.0f, 0.0f);
    SbRotation rot =
      SO_GET_ANY_PART(this, "userAxisRotation", SoRotation)->rotation.getValue();
    rot.multVec(axis, axis);
  }

  // wk = Work Space (the space the dragger geometry is in, including
  //                  SoSurroundScale and SoAntiSquish nodes) 
  // ws = World Space 
  // loc = Local Space (the space the dragger node is in)
  SbMatrix wk2ws, ws2wk, loc2wk, wk2loc;
  THIS->getSpaceMatrices(ws2wk, wk2ws, loc2wk, wk2loc);
  SbVec3f hitPt = this->getWorldStartingPoint();
  ws2wk.multVecMatrix(hitPt, hitPt);

  if (THIS->whatkind == WHATKIND_ROTATOR || THIS->whatkind == WHATKIND_USERAXIS) {
    THIS->sphereProj->setSphere(SbSphere(SbVec3f(0.0f, 0.0f, 0.0f),
                                         hitPt.length()));

    if (THIS->whatkind == WHATKIND_USERAXIS) {
      hitPt.normalize();
      SO_GET_ANY_PART(this, "userAxisRotation", SoRotation)->rotation =
        SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), hitPt);
    }
    else {
      THIS->sphereProj->setViewVolume(this->getViewVolume());
      THIS->sphereProj->setWorkingSpace(wk2ws);
      switch (this->getFrontOnProjector()) {
      case FRONT:
        THIS->sphereProj->setFront(TRUE);
        break;
      case BACK:
        THIS->sphereProj->setFront(TRUE);
        break;
      default: // avoid warnings
      case USE_PICK:
        THIS->sphereProj->setFront(THIS->sphereProj->isPointInFront(hitPt));
        break;
      }
      SbVec3f projPt = THIS->sphereProj->project(this->getNormalizedLocaterPosition());
      wk2ws.multVecMatrix(projPt, THIS->prevWorldHitPt);
      THIS->prevMotionMatrix = this->getMotionMatrix();
      this->setStartingPoint(THIS->prevWorldHitPt);
    }
  }
  else if (THIS->whatkind == WHATKIND_XROTATOR ||
           THIS->whatkind == WHATKIND_YROTATOR ||
           THIS->whatkind == WHATKIND_ZROTATOR ||
           THIS->whatkind == WHATKIND_USERROTATOR) {
    THIS->cylProj->setCylinder(SbCylinder(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), axis),
                                          hitPt.length()));
    THIS->cylProj->setViewVolume(this->getViewVolume());
    THIS->cylProj->setWorkingSpace(wk2ws);
    hitPt = THIS->cylProj->project(this->getNormalizedLocaterPosition());
    wk2ws.multVecMatrix(hitPt, THIS->prevWorldHitPt);
    this->setStartingPoint(THIS->prevWorldHitPt);
    THIS->prevMotionMatrix = this->getMotionMatrix();
  }
  else if (THIS->whatkind == WHATKIND_SCALE) {
    THIS->lineProj->setLine(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), hitPt));
  }
  THIS->prevTime = get_current_time();
  THIS->prevMousePos = this->getNormalizedLocaterPosition();
  THIS->hasDragged = FALSE;
}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoTrackballDragger::drag(void)
{
  THIS->hasDragged = TRUE;

  SbMatrix wk2ws, ws2wk, loc2wk, wk2loc;
  THIS->getSpaceMatrices(ws2wk, wk2ws, loc2wk, wk2loc);

  SbVec3f startPt = this->getWorldStartingPoint();
  ws2wk.multVecMatrix(startPt, startPt);

  if (THIS->whatkind == WHATKIND_USERAXIS) {
    THIS->sphereProj->setViewVolume(this->getViewVolume());
    THIS->sphereProj->setWorkingSpace(wk2ws);
    SbVec3f vec = THIS->sphereProj->project(this->getNormalizedLocaterPosition());
    vec.normalize();
    SO_GET_ANY_PART(this, "userAxisRotation", SoRotation)->rotation =
      SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), vec);
  }

  else if (THIS->whatkind == WHATKIND_ROTATOR) {
    THIS->sphereProj->setViewVolume(this->getViewVolume());
    THIS->sphereProj->setWorkingSpace(wk2ws);
    ws2wk.multVecMatrix(THIS->prevWorldHitPt, startPt);
    SbVec3f projPt = THIS->sphereProj->project(this->getNormalizedLocaterPosition());
    wk2ws.multVecMatrix(projPt, THIS->prevWorldHitPt);
    SbRotation rot = THIS->sphereProj->getRotation(startPt, projPt);
    THIS->prevMotionMatrix = this->appendRotation(THIS->prevMotionMatrix, rot,
                                                  SbVec3f(0.0f, 0.0f, 0.0f),
                                                  &wk2loc);
    this->setMotionMatrix(THIS->prevMotionMatrix);
  }
  else if (THIS->whatkind == WHATKIND_XROTATOR ||
           THIS->whatkind == WHATKIND_YROTATOR ||
           THIS->whatkind == WHATKIND_ZROTATOR ||
           THIS->whatkind == WHATKIND_USERROTATOR) {
    THIS->cylProj->setViewVolume(this->getViewVolume());
    THIS->cylProj->setWorkingSpace(wk2ws);
    ws2wk.multVecMatrix(THIS->prevWorldHitPt, startPt);
    SbVec3f projPt = THIS->cylProj->project(this->getNormalizedLocaterPosition());
    wk2ws.multVecMatrix(projPt, THIS->prevWorldHitPt);
    SbRotation rot = THIS->cylProj->getRotation(startPt, projPt);
    THIS->prevMotionMatrix = this->appendRotation(THIS->prevMotionMatrix, rot,
                                                  SbVec3f(0.0f, 0.0f, 0.0f),
                                                  &wk2loc);
    this->setMotionMatrix(THIS->prevMotionMatrix);
  }
  else if (THIS->whatkind == WHATKIND_SCALE) {
    THIS->lineProj->setViewVolume(this->getViewVolume());
    THIS->lineProj->setWorkingSpace(wk2ws);
    SbVec3f startPt = this->getWorldStartingPoint();
    ws2wk.multVecMatrix(startPt, startPt);
    SbVec3f projPt = THIS->lineProj->project(this->getNormalizedLocaterPosition());
    
    float orglen = startPt.length();
    float currlen = projPt.length();

    float scale = 0.0f;
    if (orglen > 0.0f) scale = currlen / orglen;
    if (scale > 0.0f && startPt.dot(projPt) < 0.0f) scale = 0.0f;

    this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                            SbVec3f(scale, scale, scale),
                                            SbVec3f(0.0f, 0.0f, 0.0f),
                                            &wk2loc));
  }
  THIS->prevTime = get_current_time();
  THIS->prevMousePos = this->getNormalizedLocaterPosition();
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoTrackballDragger::dragFinish(void)
{
  THIS->whatkind = WHATKIND_NONE;
  this->setAllPartsActive(FALSE);
  this->updateUserAxisSwitches();

  if (THIS->hasDragged &&
      THIS->animationEnabled &&
      THIS->whatkind != WHATKIND_SCALE &&
      THIS->whatkind != WHATKIND_USERAXIS) {
    SbVec2f pos = this->getNormalizedLocaterPosition();

    if (pos == THIS->prevMousePos) return;

    //
    // FIXME: it might be better to calculate animAngle and animAxis in
    // drag(). Will test more later, pederb, 20000210
    //

    SbTime currtime = get_current_time();
    THIS->animTime = currtime - THIS->prevTime;

    if (THIS->whatkind == WHATKIND_ROTATOR) {
      SbVec3f pt1, pt2;
      pt1 = THIS->sphereProj->project(THIS->prevMousePos);
      pt2 = THIS->sphereProj->project(pos);
      SbRotation rot(pt1, pt2);
      rot.getValue(THIS->animAxis, THIS->animAngle);
    }
    else {
      SbVec3f pt1, pt2;
      pt1 = THIS->cylProj->project(THIS->prevMousePos);
      pt2 = THIS->cylProj->project(pos);
      SbRotation rot = THIS->cylProj->getRotation(pt1, pt2);
      rot.getValue(THIS->animAxis, THIS->animAngle);
    }
    this->saveStartParameters(); // store new startMotionMatrix
    SoTrackballDragger::timerSensorCB(this, NULL);
    THIS->prevTime = get_current_time();
    THIS->timerSensor->setBaseTime(THIS->prevTime);
    // FIXME: get animation frame rate from somewhere?
    // pederb, 20000210
    THIS->timerSensor->setInterval(SbTime(1.0/50.0));
    THIS->timerSensor->schedule();
  }
  SoTrackballDragger_invalidate_surroundscale(this);
}

// private
void
SoTrackballDragger::timerSensorCB(void *d, SoSensor *)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;

  SbTime currtime = get_current_time();
  SbTime difftime = currtime - THISP->prevTime;
  THISP->prevTime = currtime;
  float angle = THISP->animAngle * float(difftime.getValue()/THISP->animTime.getValue());
  SbRotation rot(THISP->animAxis, angle);
  thisp->setMotionMatrix(thisp->appendRotation(thisp->getStartMotionMatrix(),
                                               rot, SbVec3f(0.0f, 0.0f, 0.0f)));
}

/*!
  If the input argument is \c TRUE, all the geometry parts of the
  dragger will become highligthed, as when active.

  Used internally during scaling operations and free-form rotations.
*/
void
SoTrackballDragger::setAllPartsActive(SbBool onoroff)
{
  SoSwitch *sw;
  int val = onoroff ? 1 : 0;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, val);
  sw = SO_GET_ANY_PART(this, "XRotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, val);
  sw = SO_GET_ANY_PART(this, "YRotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, val);
  sw = SO_GET_ANY_PART(this, "ZRotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, val);
}

/*! \COININTERNAL */
void
SoTrackballDragger::startCB(void *, SoDragger * d)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoTrackballDragger::motionCB(void *, SoDragger * d)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoTrackballDragger::finishCB(void *, SoDragger * d)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL */
void
SoTrackballDragger::metaKeyChangeCB(void *, SoDragger *)
{
  //
  // FIXME: do we need this callback? pederb, 20000210
  //
}

// private
SoNode *
SoTrackballDragger::getNodeFieldNode(const char *fieldname)
{
  SoField *field = this->getField(fieldname);
  assert(field != NULL);
  assert(field->isOfType(SoSFNode::getClassTypeId()));
  assert(((SoSFNode*)field)->getValue() != NULL);
  return ((SoSFNode*)field)->getValue();
}

// private
void
SoTrackballDragger::updateUserAxisSwitches(const SbBool setactive)
{
  SoSwitch *sw;
  int val = setactive ? 1 : 0;

  if (!setactive) {
    SbVec3f vec(0.0f, 1.0f, 0.0f);
    SbRotation rot =
      SO_GET_ANY_PART(this, "userAxisRotation", SoRotation)->rotation.getValue();
    rot.multVec(vec, vec);
    vec.normalize();

    if (vec[0] >= USER_AXIS_DISAPPEAR_LIMIT) val = SO_SWITCH_NONE;
    else if (vec[1] >= USER_AXIS_DISAPPEAR_LIMIT) val = SO_SWITCH_NONE;
    else if (vec[2] >= USER_AXIS_DISAPPEAR_LIMIT) val = SO_SWITCH_NONE;
  }

  sw = SO_GET_ANY_PART(this, "userAxisSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, val);
  sw = SO_GET_ANY_PART(this, "userRotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, val);
}


// Undefine these again, as some of them are also used in other
// dragger sourcecode files (which causes trouble when using the
// compact build hack where all .cpp files are included into all.cpp).

#undef WHATKIND_NONE
#undef WHATKIND_ROTATOR
#undef WHATKIND_XROTATOR
#undef WHATKIND_YROTATOR
#undef WHATKIND_ZROTATOR
#undef WHATKIND_USERAXIS
#undef WHATKIND_USERROTATOR
#undef WHATKIND_SCALE

#undef USER_AXIS_DISAPPEAR_LIMIT
#undef THIS
#undef THISP

#ifndef DOXYGEN_SKIP_THIS

void 
SoTrackballDraggerP::getSpaceMatrices(SbMatrix & ws2wk, SbMatrix & wk2ws,
                                      SbMatrix & loc2wk, SbMatrix & wk2loc)
{
  this->master->getPartToLocalMatrix("antiSquish", wk2loc, loc2wk);
  wk2ws = this->master->getLocalToWorldMatrix();
  wk2ws.multLeft(wk2loc);
  ws2wk = this->master->getWorldToLocalMatrix();
  ws2wk.multRight(loc2wk);
}

#endif // DOXYGEN_SKIP_THIS
