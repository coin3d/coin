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
#include <coindefs.h> // COIN_STUB()

#include <data/draggerDefaults/trackballDragger.h>

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

SO_KIT_SOURCE(SoTrackballDragger);

void
SoTrackballDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTrackballDragger);
}

SoTrackballDragger::SoTrackballDragger(void)
{
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

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

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

  this->animationEnabled = TRUE;
  this->whatkind = WHATKIND_NONE;
  this->sphereProj = new SbSpherePlaneProjector;
  this->cylProj = new SbCylinderPlaneProjector;
  this->lineProj = new SbLineProjector();

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

  this->timerSensor = new SoTimerSensor(SoTrackballDragger::timerSensorCB, this);

  this->setUpConnections(TRUE, TRUE);
}


SoTrackballDragger::~SoTrackballDragger()
{
  delete this->rotFieldSensor;
  delete this->scaleFieldSensor;
  delete this->sphereProj;
  delete this->cylProj;
  delete this->lineProj;
}

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

void
SoTrackballDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
  inherited::setDefaultOnNonWritingFields();
}

void
SoTrackballDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

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

SbBool
SoTrackballDragger::isAnimationEnabled(void)
{
  return this->animationEnabled;
}

void
SoTrackballDragger::setAnimationEnabled(SbBool newval)
{
  this->animationEnabled = FALSE;
}

void
SoTrackballDragger::dragStart(void)
{
  if (this->timerSensor->isScheduled()) {
    this->timerSensor->unschedule();
  }
  const SoPath *pickpath = this->getPickPath();
  const SoEvent *event = this->getEvent();
  this->whatkind = WHATKIND_NONE;
  SoSwitch *sw;

  if (event->wasCtrlDown()) {
    this->whatkind = WHATKIND_SCALE;
    this->setAllPartsActive(TRUE);
  }
  else if (event->wasShiftDown()) {
    this->whatkind = WHATKIND_USERAXIS;
    this->updateUserAxisSwitches(TRUE);
  }

  SbVec3f axis(0.0f, 1.0f, 0.0f);
  if (!this->whatkind && pickpath->findNode(this->getNodeFieldNode("rotator")) >= 0) {
    this->whatkind = WHATKIND_ROTATOR;
    this->setAllPartsActive(TRUE);
  }
  if (!this->whatkind && pickpath->findNode(this->getNodeFieldNode("XRotator")) >= 0) {
    this->whatkind = WHATKIND_XROTATOR;
    sw = SO_GET_ANY_PART(this, "XRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(1.0f, 0.0f, 0.0f);
  }
  if (!this->whatkind && pickpath->findNode(this->getNodeFieldNode("YRotator")) >= 0) {
    this->whatkind = WHATKIND_YROTATOR;
    sw = SO_GET_ANY_PART(this, "YRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(0.0f, 1.0f, 0.0f);
  }
  if (!this->whatkind && pickpath->findNode(this->getNodeFieldNode("ZRotator")) >= 0) {
    this->whatkind = WHATKIND_ZROTATOR;
    sw = SO_GET_ANY_PART(this, "ZRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(0.0f, 0.0f, 1.0f);
  }
  if (!this->whatkind && pickpath->findNode(this->getNodeFieldNode("userRotator")) >= 0) {
    this->whatkind = WHATKIND_USERROTATOR;
    sw = SO_GET_ANY_PART(this, "userRotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    axis.setValue(0.0f, 1.0f, 0.0f);
    SbRotation rot =
      SO_GET_ANY_PART(this, "userAxisRotation", SoRotation)->rotation.getValue();
    rot.multVec(axis, axis);
  }

  SbVec3f hitPt = this->getLocalStartingPoint();

  if (this->whatkind == WHATKIND_ROTATOR || this->whatkind == WHATKIND_USERAXIS) {
    this->sphereProj->setSphere(SbSphere(SbVec3f(0.0f, 0.0f, 0.0f),
                                         hitPt.length()));

    if (this->whatkind == WHATKIND_USERAXIS) {
      hitPt.normalize();
      SO_GET_ANY_PART(this, "userAxisRotation", SoRotation)->rotation =
        SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), hitPt);
    }
  }
  else if (this->whatkind == WHATKIND_XROTATOR ||
           this->whatkind == WHATKIND_YROTATOR ||
           this->whatkind == WHATKIND_ZROTATOR ||
           this->whatkind == WHATKIND_USERROTATOR) {
    this->cylProj->setCylinder(SbCylinder(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), axis),
                                          hitPt.length()));
    this->cylProj->setViewVolume(this->getViewVolume());
    this->cylProj->setWorkingSpace(this->getLocalToWorldMatrix());
    hitPt = this->cylProj->project(this->getNormalizedLocaterPosition());
    SbVec3f worldPt;
    this->getLocalToWorldMatrix().multVecMatrix(hitPt, worldPt);
    this->setStartingPoint(worldPt);
  }
  else if (this->whatkind == WHATKIND_SCALE) {
    this->lineProj->setLine(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), hitPt));
  }
  this->prevTime = SbTime::getTimeOfDay();
  this->prevMousePos = this->getNormalizedLocaterPosition();
  this->hasDragged = FALSE;
}

void
SoTrackballDragger::drag(void)
{
  this->hasDragged = TRUE;
  SbVec3f startPt = this->getLocalStartingPoint();

  if (this->whatkind == WHATKIND_USERAXIS) {
    this->sphereProj->setViewVolume(this->getViewVolume());
    this->sphereProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f vec = this->sphereProj->project(this->getNormalizedLocaterPosition());
    vec.normalize();
    SO_GET_ANY_PART(this, "userAxisRotation", SoRotation)->rotation =
      SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), vec);
  }

  else if (this->whatkind == WHATKIND_ROTATOR) {
    this->sphereProj->setViewVolume(this->getViewVolume());
    this->sphereProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f projPt = this->sphereProj->project(this->getNormalizedLocaterPosition());
    SbRotation rot = this->sphereProj->getRotation(startPt, projPt);
    this->setMotionMatrix(this->appendRotation(this->getStartMotionMatrix(),
                                               rot, SbVec3f(0.0f, 0.0f, 0.0f)));
  }
  else if (this->whatkind == WHATKIND_XROTATOR ||
           this->whatkind == WHATKIND_YROTATOR ||
           this->whatkind == WHATKIND_ZROTATOR ||
           this->whatkind == WHATKIND_USERROTATOR) {
    this->cylProj->setViewVolume(this->getViewVolume());
    this->cylProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f projPt = this->cylProj->project(this->getNormalizedLocaterPosition());
    SbRotation rot = this->cylProj->getRotation(startPt, projPt);
    this->setMotionMatrix(this->appendRotation(this->getStartMotionMatrix(),
                                               rot, SbVec3f(0.0f, 0.0f, 0.0f)));
  }
  else if (this->whatkind == WHATKIND_SCALE) {
    this->lineProj->setViewVolume(this->getViewVolume());
    this->lineProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f startPt = this->getLocalStartingPoint();
    SbVec3f projPt = lineProj->project(this->getNormalizedLocaterPosition());

    float orglen = startPt.length();
    float currlen = projPt.length();

    float scale = 0.0f;
    if (orglen > 0.0f) scale = currlen / orglen;
    if (scale > 0.0f && startPt.dot(projPt) < 0.0f) scale = 0.0f;

    this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                            SbVec3f(scale, scale, scale),
                                            SbVec3f(0.0f, 0.0f, 0.0f)));
  }
  this->prevTime = SbTime::getTimeOfDay();
  this->prevMousePos = this->getNormalizedLocaterPosition();
}

void
SoTrackballDragger::dragFinish(void)
{
  this->whatkind = WHATKIND_NONE;
  this->setAllPartsActive(FALSE);
  this->updateUserAxisSwitches();

  if (this->hasDragged &&
      this->animationEnabled &&
      this->whatkind != WHATKIND_SCALE &&
      this->whatkind != WHATKIND_USERAXIS) {
    SbVec2f pos = this->getNormalizedLocaterPosition();

    if (pos == this->prevMousePos) return;

    //
    // FIXME: it might be better to calculate animAngle and animAxis in
    // drag(). Will test more later, pederb, 20000210
    //

    SbTime currtime = SbTime::getTimeOfDay();
    this->animTime = currtime - this->prevTime;

    if (this->whatkind == WHATKIND_ROTATOR) {
      SbVec3f pt1, pt2;
      pt1 = this->sphereProj->project(this->prevMousePos);
      pt2 = this->sphereProj->project(pos);
      SbRotation rot(pt1, pt2);
      rot.getValue(this->animAxis, this->animAngle);
    }
    else {
      SbVec3f pt1, pt2;
      pt1 = this->cylProj->project(this->prevMousePos);
      pt2 = this->cylProj->project(pos);
      SbRotation rot = this->cylProj->getRotation(pt1, pt2);
      rot.getValue(this->animAxis, this->animAngle);
    }
    this->saveStartParameters(); // store new startMotionMatrix
    SoTrackballDragger::timerSensorCB(this, NULL);
    this->prevTime = SbTime::getTimeOfDay();
    this->timerSensor->setBaseTime(this->prevTime);
    // FIXME: get animation frame rate from somewhere?
    // pederb, 20000210
    this->timerSensor->setInterval(SbTime(1.0/50.0));
    this->timerSensor->schedule();
  }
}

void
SoTrackballDragger::timerSensorCB(void *d, SoSensor *)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;

  SbTime difftime = SbTime::getTimeOfDay() - thisp->prevTime;
  float angle = thisp->animAngle * float(difftime.getValue()/thisp->animTime.getValue());
  SbRotation rot(thisp->animAxis, angle);
  thisp->setMotionMatrix(thisp->appendRotation(thisp->getStartMotionMatrix(),
                                               rot, SbVec3f(0.0f, 0.0f, 0.0f)));
}

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

void
SoTrackballDragger::startCB(void *, SoDragger * d)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  thisp->dragStart();
}

void
SoTrackballDragger::motionCB(void *, SoDragger * d)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  thisp->drag();
}

void
SoTrackballDragger::finishCB(void *, SoDragger * d)
{
  SoTrackballDragger *thisp = (SoTrackballDragger*)d;
  thisp->dragFinish();
}

void
SoTrackballDragger::metaKeyChangeCB(void *, SoDragger *)
{
  //
  // FIXME: do we need this callback? pederb, 20000210
  //
}

SoNode *
SoTrackballDragger::getNodeFieldNode(const char *fieldname)
{
  SoField *field = this->getField(fieldname);
  assert(field != NULL);
  assert(field->isOfType(SoSFNode::getClassTypeId()));
  assert(((SoSFNode*)field)->getValue() != NULL);
  return ((SoSFNode*)field)->getValue();
}

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
