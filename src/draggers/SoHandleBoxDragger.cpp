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

#include <Inventor/draggers/SoHandleBoxDragger.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbRotation.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/SoPath.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <coindefs.h> // COIN_STUB()


#define WHATKIND_NONE       0
#define WHATKIND_TRANSLATOR 1
#define WHATKIND_EXTRUDER   2
#define WHATKIND_UNIFORM    3

#define CONSTRAINT_OFF  0
#define CONSTRAINT_WAIT 1
#define CONSTRAINT_X    2
#define CONSTRAINT_Y    3
#define CONSTRAINT_Z    4

static int uniform_ctrl_lookup[8][6] = {
  { 1,4,5,2,3,6 },
  { 1,4,6,2,3,5 },
  { 2,4,5,1,3,6 },
  { 2,4,6,1,3,5 },
  { 1,3,5,2,4,6 },
  { 1,3,6,2,4,5 },
  { 2,3,5,1,4,6 },
  { 2,3,6,1,4,5 }
};

SO_KIT_SOURCE(SoHandleBoxDragger);


void
SoHandleBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoHandleBoxDragger);
}

SoHandleBoxDragger::SoHandleBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoHandleBoxDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(drawStyle, SoDrawStyle, TRUE, geomSeparator, translator1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Switch, SoSwitch, TRUE, geomSeparator, translator2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1, SoSeparator, TRUE, translator1Switch, translator1Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Active, SoSeparator, TRUE, translator1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Switch, SoSwitch, TRUE, geomSeparator, translator3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2, SoSeparator, TRUE, translator2Switch, translator2Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Active, SoSeparator, TRUE, translator2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Switch, SoSwitch, TRUE, geomSeparator, translator4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3, SoSeparator, TRUE, translator3Switch, translator3Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Active, SoSeparator, TRUE, translator3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Switch, SoSwitch, TRUE, geomSeparator, translator5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4, SoSeparator, TRUE, translator4Switch, translator4Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Active, SoSeparator, TRUE, translator4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Switch, SoSwitch, TRUE, geomSeparator, translator6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5, SoSeparator, TRUE, translator5Switch, translator5Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Active, SoSeparator, TRUE, translator5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Switch, SoSwitch, TRUE, geomSeparator, extruder1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6, SoSeparator, TRUE, translator6Switch, translator6Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Active, SoSeparator, TRUE, translator6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder1Switch, SoSwitch, TRUE, geomSeparator, extruder2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder1, SoSeparator, TRUE, extruder1Switch, extruder1Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder1Active, SoSeparator, TRUE, extruder1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder2Switch, SoSwitch, TRUE, geomSeparator, extruder3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder2, SoSeparator, TRUE, extruder2Switch, extruder2Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder2Active, SoSeparator, TRUE, extruder2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder3Switch, SoSwitch, TRUE, geomSeparator, extruder4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder3, SoSeparator, TRUE, extruder3Switch, extruder3Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder3Active, SoSeparator, TRUE, extruder3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder4Switch, SoSwitch, TRUE, geomSeparator, extruder5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder4, SoSeparator, TRUE, extruder4Switch, extruder4Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder4Active, SoSeparator, TRUE, extruder4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder5Switch, SoSwitch, TRUE, geomSeparator, extruder6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder5, SoSeparator, TRUE, extruder5Switch, extruder5Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder5Active, SoSeparator, TRUE, extruder5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder6Switch, SoSwitch, TRUE, geomSeparator, uniform1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder6, SoSeparator, TRUE, extruder6Switch, extruder6Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder6Active, SoSeparator, TRUE, extruder6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform1Switch, SoSwitch, TRUE, geomSeparator, uniform2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform1, SoSeparator, TRUE, uniform1Switch, uniform1Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform1Active, SoSeparator, TRUE, uniform1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform2Switch, SoSwitch, TRUE, geomSeparator, uniform3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform2, SoSeparator, TRUE, uniform2Switch, uniform2Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform2Active, SoSeparator, TRUE, uniform2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform3Switch, SoSwitch, TRUE, geomSeparator, uniform4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform3, SoSeparator, TRUE, uniform3Switch, uniform3Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform3Active, SoSeparator, TRUE, uniform3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform4Switch, SoSwitch, TRUE, geomSeparator, uniform5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform4, SoSeparator, TRUE, uniform4Switch, uniform4Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform4Active, SoSeparator, TRUE, uniform4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform5Switch, SoSwitch, TRUE, geomSeparator, uniform6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform5, SoSeparator, TRUE, uniform5Switch, uniform5Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform5Active, SoSeparator, TRUE, uniform5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform6Switch, SoSwitch, TRUE, geomSeparator, uniform7Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform6, SoSeparator, TRUE, uniform6Switch, uniform6Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform6Active, SoSeparator, TRUE, uniform6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform7Switch, SoSwitch, TRUE, geomSeparator, uniform8Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform7, SoSeparator, TRUE, uniform7Switch, uniform7Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform7Active, SoSeparator, TRUE, uniform7Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform8Switch, SoSwitch, TRUE, geomSeparator, arrowTranslation, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform8, SoSeparator, TRUE, uniform8Switch, uniform8Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform8Active, SoSeparator, TRUE, uniform8Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrowTranslation, SoTranslation, TRUE, geomSeparator, arrow1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow1Switch, SoSwitch, TRUE, geomSeparator, arrow2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow1, SoSeparator, TRUE, arrow1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow2Switch, SoSwitch, TRUE, geomSeparator, arrow3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow2, SoSeparator, TRUE, arrow2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow3Switch, SoSwitch, TRUE, geomSeparator, arrow4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow3, SoSeparator, TRUE, arrow3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow4Switch, SoSwitch, TRUE, geomSeparator, arrow5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow4, SoSeparator, TRUE, arrow4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow5Switch, SoSwitch, TRUE, geomSeparator, arrow6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow5, SoSeparator, TRUE, arrow5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow6Switch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow6, SoSeparator, TRUE, arrow6Switch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("handleBoxDragger.iv", NULL, 0);
  }

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  this->setPartAsDefault("translator1", "handleBoxTranslator1");
  this->setPartAsDefault("translator2", "handleBoxTranslator2");
  this->setPartAsDefault("translator3", "handleBoxTranslator3");
  this->setPartAsDefault("translator4", "handleBoxTranslator4");
  this->setPartAsDefault("translator5", "handleBoxTranslator5");
  this->setPartAsDefault("translator6", "handleBoxTranslator6");

  this->setPartAsDefault("translator1Active", "handleBoxTranslator1Active");
  this->setPartAsDefault("translator2Active", "handleBoxTranslator2Active");
  this->setPartAsDefault("translator3Active", "handleBoxTranslator3Active");
  this->setPartAsDefault("translator4Active", "handleBoxTranslator4Active");
  this->setPartAsDefault("translator5Active", "handleBoxTranslator5Active");
  this->setPartAsDefault("translator6Active", "handleBoxTranslator6Active");

  this->setPartAsDefault("extruder1", "handleBoxExtruder1");
  this->setPartAsDefault("extruder2", "handleBoxExtruder2");
  this->setPartAsDefault("extruder3", "handleBoxExtruder3");
  this->setPartAsDefault("extruder4", "handleBoxExtruder4");
  this->setPartAsDefault("extruder5", "handleBoxExtruder5");
  this->setPartAsDefault("extruder6", "handleBoxExtruder6");

  this->setPartAsDefault("extruder1Active", "handleBoxExtruder1Active");
  this->setPartAsDefault("extruder2Active", "handleBoxExtruder2Active");
  this->setPartAsDefault("extruder3Active", "handleBoxExtruder3Active");
  this->setPartAsDefault("extruder4Active", "handleBoxExtruder4Active");
  this->setPartAsDefault("extruder5Active", "handleBoxExtruder5Active");
  this->setPartAsDefault("extruder6Active", "handleBoxExtruder6Active");

  this->setPartAsDefault("uniform1", "handleBoxUniform1");
  this->setPartAsDefault("uniform2", "handleBoxUniform2");
  this->setPartAsDefault("uniform3", "handleBoxUniform3");
  this->setPartAsDefault("uniform4", "handleBoxUniform4");
  this->setPartAsDefault("uniform5", "handleBoxUniform5");
  this->setPartAsDefault("uniform6", "handleBoxUniform6");
  this->setPartAsDefault("uniform7", "handleBoxUniform7");
  this->setPartAsDefault("uniform8", "handleBoxUniform8");

  this->setPartAsDefault("uniform1Active", "handleBoxUniform1Active");
  this->setPartAsDefault("uniform2Active", "handleBoxUniform2Active");
  this->setPartAsDefault("uniform3Active", "handleBoxUniform3Active");
  this->setPartAsDefault("uniform4Active", "handleBoxUniform4Active");
  this->setPartAsDefault("uniform5Active", "handleBoxUniform5Active");
  this->setPartAsDefault("uniform6Active", "handleBoxUniform6Active");
  this->setPartAsDefault("uniform7Active", "handleBoxUniform7Active");
  this->setPartAsDefault("uniform8Active", "handleBoxUniform8Active");

  this->setPartAsDefault("arrow1", "handleBoxArrow1");
  this->setPartAsDefault("arrow2", "handleBoxArrow2");
  this->setPartAsDefault("arrow3", "handleBoxArrow3");
  this->setPartAsDefault("arrow4", "handleBoxArrow4");
  this->setPartAsDefault("arrow5", "handleBoxArrow5");
  this->setPartAsDefault("arrow6", "handleBoxArrow6");

  this->constraintState = CONSTRAINT_OFF;
  this->whatkind = WHATKIND_NONE;

  this->setAllPartsActive(FALSE);

  this->planeProj = new SbPlaneProjector;
  this->lineProj = new SbLineProjector;

  this->addStartCallback(SoHandleBoxDragger::startCB);
  this->addMotionCallback(SoHandleBoxDragger::motionCB);
  this->addFinishCallback(SoHandleBoxDragger::finishCB);
  this->addValueChangedCallback(SoHandleBoxDragger::valueChangedCB);
  this->addOtherEventCallback(SoHandleBoxDragger::metaKeyChangeCB);

  this->translFieldSensor = new SoFieldSensor(SoHandleBoxDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoHandleBoxDragger::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoHandleBoxDragger::~SoHandleBoxDragger()
{
  delete this->lineProj;
  delete this->planeProj;
  delete this->translFieldSensor;
  delete this->scaleFieldSensor;
}

SbBool
SoHandleBoxDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoHandleBoxDragger::fieldSensorCB(this, NULL);

    if (this->translFieldSensor->getAttachedField() != &this->translation) {
      this->translFieldSensor->attach(&this->translation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }

  }
  else {
    if (this->translFieldSensor->getAttachedField() != NULL) {
      this->translFieldSensor->detach();
    }
    if (this->scaleFieldSensor->getAttachedField() != NULL) {
      this->scaleFieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoHandleBoxDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
  inherited::setDefaultOnNonWritingFields();
}

void
SoHandleBoxDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoHandleBoxDragger *thisp = (SoHandleBoxDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoHandleBoxDragger::valueChangedCB(void * f, SoDragger * d)
{
  SoHandleBoxDragger *thisp = (SoHandleBoxDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);

  thisp->translFieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->translFieldSensor->attach(&thisp->translation);

  thisp->scaleFieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->scaleFieldSensor->attach(&thisp->scaleFactor);
}

void
SoHandleBoxDragger::startCB(void *, SoDragger * d)
{
  SoHandleBoxDragger *thisp = (SoHandleBoxDragger*)d;
  thisp->dragStart();
}

void
SoHandleBoxDragger::motionCB(void *, SoDragger * d)
{
  SoHandleBoxDragger *thisp = (SoHandleBoxDragger*)d;
  thisp->drag();
}

void
SoHandleBoxDragger::finishCB(void *, SoDragger * d)
{
  SoHandleBoxDragger *thisp = (SoHandleBoxDragger*)d;
  thisp->dragFinish();
}

void
SoHandleBoxDragger::metaKeyChangeCB(void *, SoDragger * d)
{
  SoHandleBoxDragger *thisp = (SoHandleBoxDragger*)d;
  if (!thisp->isActive.getValue()) return;

  const SoEvent *event = thisp->getEvent();
  if (SO_KEY_RELEASE_EVENT(event, LEFT_SHIFT) ||
      SO_KEY_RELEASE_EVENT(event, RIGHT_SHIFT)) {
    if (thisp->constraintState != CONSTRAINT_OFF) thisp->drag();
  }
  else if (thisp->ctrlDown != event->wasCtrlDown()) {
    thisp->ctrlDown = !thisp->ctrlDown;
    thisp->updateSwitches();
  }
}

void
SoHandleBoxDragger::dragStart(void)
{
  static const char translatorname[] = "translator";
  static const char extrudername[] = "extruder";
  static const char uniformname[] = "uniform";

  const SoPath *pickpath = this->getPickPath();

  SbBool found = FALSE;
  this->whatkind = WHATKIND_NONE;
  this->whatnum = 0;

  int i;
  SbString str;
  if (!found) {
    for (i = 1; i <= 6; i++) {
      str.sprintf("%s%d", translatorname, i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString())) >= 0) break;
    }
    if (i <= 6) {
      found = TRUE;
      this->whatkind = WHATKIND_TRANSLATOR;
      this->whatnum = i;
    }
  }

  if (!found) {
    for (i = 1; i <= 6; i++) {
      str.sprintf("%s%d", extrudername, i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString()))>= 0) break;
    }
    if (i <= 6) {
      found = TRUE;
      this->whatkind = WHATKIND_EXTRUDER;
      this->whatnum = i;
    }
  }
  if (!found) {
    for (i = 1; i <= 8; i++) {
      str.sprintf("%s%d", uniformname, i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString()))>= 0) break;
    }
    if (i <= 8) {
      found = TRUE;
      this->whatkind = WHATKIND_UNIFORM;
      this->whatnum = i;
    }
  }
  assert(found);
  if (!found) return;

  SbVec3f startPt = this->getLocalStartingPoint();
  SoSwitch *sw;

  switch(this->whatkind) {
  case WHATKIND_TRANSLATOR:
    {
      SbVec3f n;
      if (this->whatnum <= 2) {
        n = SbVec3f(0.0f, 1.0f, 0.0f);
      }
      else if (this->whatnum <= 4) {
        n = SbVec3f(1.0f, 0.0f, 0.0f);
      }
      else {
        n = SbVec3f(0.0f, 0.0f, 1.0f);
      }
      SbVec3f localPt;
      {
        SbMatrix mat, inv;
        this->getSurroundScaleMatrices(mat, inv);
        inv.multVecMatrix(startPt, localPt);
      }
      this->planeProj->setPlane(SbPlane(n, startPt));
      SbLine myline(SbVec3f(0.0f, 0.0f, 0.0f), n);
      SoTranslation *t = SO_GET_ANY_PART(this, "arrowTranslation", SoTranslation);
      t->translation = myline.getClosestPoint(localPt);
      if (this->getEvent()->wasShiftDown()) {
        this->getLocalToWorldMatrix().multVecMatrix(startPt, this->worldRestartPt);
        this->constraintState = CONSTRAINT_WAIT;
      }
    }
    break;
  case WHATKIND_EXTRUDER:
    this->lineProj->setLine(SbLine(this->getDraggerCenter(), startPt));
    this->ctrlOffset = this->calcCtrlOffset(startPt);
    break;
  case WHATKIND_UNIFORM:
    this->lineProj->setLine(SbLine(this->getDraggerCenter(), startPt));
    this->ctrlOffset = this->calcCtrlOffset(startPt);
    break;
  }
  this->ctrlDown = this->getEvent()->wasCtrlDown();
  this->updateSwitches();
}

void
SoHandleBoxDragger::drag(void)
{
  SbVec3f startPt = this->getLocalStartingPoint();

  if (this->whatkind == WHATKIND_TRANSLATOR) {
    this->planeProj->setViewVolume(this->getViewVolume());
    this->planeProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f projPt = this->planeProj->project(this->getNormalizedLocaterPosition());

    const SoEvent *event = this->getEvent();
    if (event->wasShiftDown() && this->constraintState == CONSTRAINT_OFF) {
      this->constraintState = CONSTRAINT_WAIT;
      this->setStartLocaterPosition(event->getPosition());
      this->getLocalToWorldMatrix().multVecMatrix(projPt, this->worldRestartPt);
    }
    else if (!event->wasShiftDown() && this->constraintState != CONSTRAINT_OFF) {
      this->constraintState = CONSTRAINT_OFF;
      this->updateArrows();
    }

    SbVec3f motion, localrestartpt;
    if (this->constraintState != CONSTRAINT_OFF) {
      this->getWorldToLocalMatrix().multVecMatrix(this->worldRestartPt,
                                                  localrestartpt);
      motion = localrestartpt - startPt;
    }
    else motion = projPt - startPt;
    switch(this->constraintState) {
    case CONSTRAINT_OFF:
      break;
    case CONSTRAINT_WAIT:
      if (this->isAdequateConstraintMotion()) {
        SbVec3f newmotion = projPt - localrestartpt;
        int biggest = 0;
        double bigval = fabs(newmotion[0]);
        if (fabs(newmotion[1]) > bigval) {
          biggest = 1;
          bigval = fabs(newmotion[1]);
        }
        if (fabs(newmotion[2]) > bigval) {
          biggest = 2;
        }
        motion[biggest] += newmotion[biggest];
        this->constraintState = CONSTRAINT_X + biggest;
        this->updateArrows();
      }
      else {
        return;
      }
      break;
    case CONSTRAINT_X:
      motion[0] += projPt[0] - localrestartpt[0];
      break;
    case CONSTRAINT_Y:
      motion[1] += projPt[1] - localrestartpt[1];
      break;
    case CONSTRAINT_Z:
      motion[2] += projPt[2] - localrestartpt[2];
    }
    this->setMotionMatrix(this->appendTranslation(this->getStartMotionMatrix(), motion));
  }
  else {
    this->lineProj->setViewVolume(this->getViewVolume());
    this->lineProj->setWorkingSpace(this->getLocalToWorldMatrix());
    SbVec3f projPt = this->lineProj->project(this->getNormalizedLocaterPosition());
    SbVec3f center = this->getDraggerCenter();
    if (this->getEvent()->wasCtrlDown()) {
      center += this->ctrlOffset;
    }

    float orglen = (startPt-center).length();
    float currlen = (projPt-center).length();
    float scale = 0.0f;
    
    if (orglen > 0.0f) scale = currlen / orglen;
    if (scale > 0.0f && (startPt-center).dot(projPt-center) <= 0.0f) scale = 0.0f;
    
    SbVec3f scalevec(scale, scale, scale);
    if (this->whatkind == WHATKIND_EXTRUDER) {
      if (this->whatnum <= 2) scalevec[0] = scalevec[2] = 1.0f;
      else if (this->whatnum <= 4) scalevec[1] = scalevec[2] = 1.0f;
      else scalevec[0] = scalevec[1] = 1.0f;
    }

    this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                            scalevec,
                                            center));
  }
}

void
SoHandleBoxDragger::dragFinish(void)
{
  this->constraintState = CONSTRAINT_OFF;
  this->whatkind = WHATKIND_NONE;
  this->setAllPartsActive(FALSE);
}

void
SoHandleBoxDragger::setAllPartsActive(SbBool onoroff)
{
  int i;
  int val = onoroff ? 1 : 0;
  SoSwitch *sw;
  SbString str;
  for (i = 1; i <= 6; i++) {
    str.sprintf("translator%dSwitch", i);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, val);
  }
  for (i = 1; i <= 6; i++) {
    str.sprintf("extruder%dSwitch", i);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, val);
  }
  for (i = 1; i <= 8; i++) {
    str.sprintf("uniform%dSwitch", i);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, val);
  }
  this->updateArrows();
}

SoNode *
SoHandleBoxDragger::getNodeFieldNode(const char *fieldname)
{
  SoField *field = this->getField(fieldname);
  assert(field != NULL);
  assert(field->isOfType(SoSFNode::getClassTypeId()));
  assert(((SoSFNode*)field)->getValue() != NULL);
  return ((SoSFNode*)field)->getValue();
}

void
SoHandleBoxDragger::updateSwitches()
{
  int i;
  SbString str;
  SoSwitch *sw;

  if (this->whatkind == WHATKIND_UNIFORM) {
    if (this->ctrlDown) {
      const int *ptr = uniform_ctrl_lookup[this->whatnum-1];
      for (i = 0; i < 6; i++) {
        str.sprintf("extruder%dSwitch", ptr[i]);
        sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
        SoInteractionKit::setSwitchValue(sw, i < 3 ? 1 : 0);
      }
    }
    else {
      for (i = 1; i <= 6; i++) {
        str.sprintf("extruder%dSwitch", i);
        sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
        SoInteractionKit::setSwitchValue(sw, 1);
      }
    }
    str.sprintf("uniform%dSwitch", this->whatnum);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
  }
  else if (this->whatkind == WHATKIND_EXTRUDER) {
    int othernum = ((this->whatnum-1) & ~1) + 1;
    if (othernum == this->whatnum) othernum++;

    str.sprintf("extruder%dSwitch", this->whatnum);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, 1);
    str.sprintf("extruder%dSwitch", othernum);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, this->ctrlDown ? 0 : 1);
  }
  else {
    this->setAllPartsActive(TRUE);
    this->updateArrows();
  }
}

void
SoHandleBoxDragger::updateArrows()
{
  int i;
  SbString str;
  SoSwitch *sw;

  if (this->constraintState >= CONSTRAINT_X) {
    int onval = -1;
    switch (this->constraintState) {
    case CONSTRAINT_X:
      onval = 3;
      break;
    case CONSTRAINT_Y:
      onval = 1;
      break;
    case CONSTRAINT_Z:
      onval = 5;
      break;
    }
    for (i = 1; i <= 6; i++) {
      str.sprintf("arrow%dSwitch", i);
      sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
      if (i == onval || i == onval + 1) {
        SoInteractionKit::setSwitchValue(sw, 0);
      }
      else {
        SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
      }
    }
  }
  else if (this->whatkind == WHATKIND_TRANSLATOR) {
    int num = (this->whatnum-1) & ~1;
    for (i = 0; i < 6; i++) {
      str.sprintf("arrow%dSwitch", i+1);
      sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
      if (i == num || i == num+1) {
        SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
      }
      else {
        SoInteractionKit::setSwitchValue(sw, 0);
      }
    }
  }
  else {
    for (i = 1; i <= 6; i++) {
      str.sprintf("arrow%dSwitch", i);
      sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
      SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
    }
  }
}

void
SoHandleBoxDragger::getSurroundScaleMatrices(SbMatrix &mat, SbMatrix &inv)
{
  SoSurroundScale *ss = SO_CHECK_ANY_PART(this, "surroundScale", SoSurroundScale);
  if (ss) {
    SoGetMatrixAction ma(this->getViewportRegion());
    ma.apply(ss);
    mat = ma.getMatrix();
    inv = ma.getInverse();
  }
  else {
    mat = SbMatrix::identity();
    inv = SbMatrix::identity();
  }
}

SbVec3f 
SoHandleBoxDragger::getDraggerCenter()
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  return SbVec3f(mat[3][0], mat[3][1], mat[3][2]);
}

SbVec3f 
SoHandleBoxDragger::calcCtrlOffset(const SbVec3f startpt)
{
  SbMatrix m, inv;
  this->getSurroundScaleMatrices(m, inv);
  SbVec3f v = SbVec3f(m[3][0], m[3][1], m[3][2]) - startpt;
  
  for (int i = 0; i < 3; i++) {
    v[i] *= inv[i][i];
    if (v[i] < -0.95) v[i] = -1.0f;
    else if (v[i] > 0.95) v[i] = 1.0f;
    else v[i] = 0.0f;
    v[i] *= m[i][i];
  }
  return v;
}



