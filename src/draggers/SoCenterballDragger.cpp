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
  \class SoCenterballDragger SoCenterballDragger.h Inventor/draggers/SoCenterballDragger.h
  \brief The SoCenterballDragger class is a dragger you can rotate and translate.
  \ingroup draggers

  When translating, instead of modifying the translation part of the motion
  matrix, a \e center field is updated, and the geometry is moved using
  an internal transformation node. FIXME: more doc.
*/

#include <Inventor/draggers/SoCenterballDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <coindefs.h> // COIN_STUB()

SO_KIT_SOURCE(SoCenterballDragger);


void
SoCenterballDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoCenterballDragger);
}

/*!
  Constructor.
*/
SoCenterballDragger::SoCenterballDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoCenterballDragger);
  SO_KIT_ADD_CATALOG_ENTRY(XAxis, SoSeparator, TRUE, XAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XAxisSwitch, SoSwitch, TRUE, geomSeparator, YAxisSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(XCenterChanger, SoTranslate2Dragger, TRUE, topSeparator, rot2X90, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotator, SoRotateCylindricalDragger, TRUE, topSeparator, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YAxis, SoSeparator, TRUE, YAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YAxisSwitch, SoSwitch, TRUE, geomSeparator, ZAxisSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(YCenterChanger, SoTranslate2Dragger, TRUE, topSeparator, rotY90, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotator, SoRotateCylindricalDragger, TRUE, topSeparator, ZCenterChanger, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZAxis, SoSeparator, TRUE, ZAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZAxisSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(ZCenterChanger, SoTranslate2Dragger, TRUE, topSeparator, rotX90, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotator, SoRotateCylindricalDragger, TRUE, topSeparator, YCenterChanger, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, lightModel, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(lightModel, SoLightModel, TRUE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rot2X90, SoRotation, TRUE, topSeparator, XRotator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotX90, SoRotation, TRUE, topSeparator, ZRotator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotY90, SoRotation, TRUE, topSeparator, XCenterChanger, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, YRotator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translateToCenter, SoMatrixTransform, TRUE, topSeparator, surroundScale, TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("centerballDragger.iv", NULL, 0);
  }

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f, 0.0f));

  SO_KIT_INIT_INSTANCE();

  // create subdraggers
  SO_GET_ANY_PART(this, "XCenterChanger", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "YCenterChanger", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "ZCenterChanger", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "XRotator", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "YRotator", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "ZRotator", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "rotator", SoRotateSphericalDragger);

  // create default parts
  this->setPartAsDefault("XAxis", "centerballXAxis");
  this->setPartAsDefault("YAxis", "centerballYAxis");
  this->setPartAsDefault("ZAxis", "centerballZAxis");

  // initialize some nodes
  SoRotation *rot;
  rot = SO_GET_ANY_PART(this, "rot2X90", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI*0.5f);
  rot = SO_GET_ANY_PART(this, "rotX90", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI*0.5f);
  rot = SO_GET_ANY_PART(this, "rotY90", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), M_PI*0.5f);

  SoAntiSquish *squish = SO_GET_ANY_PART(this, "antiSquish", SoAntiSquish);
  squish->sizing = SoAntiSquish::LONGEST_DIAGONAL;

  SoMatrixTransform *mt = SO_GET_ANY_PART(this, "translateToCenter", SoMatrixTransform);
  mt->matrix = SbMatrix::identity();

  this->addValueChangedCallback(SoCenterballDragger::valueChangedCB);

  this->rotFieldSensor = new SoFieldSensor(SoCenterballDragger::fieldSensorCB, this);
  this->centerFieldSensor = new SoFieldSensor(SoCenterballDragger::fieldSensorCB, this);
  this->setUpConnections(TRUE, TRUE);
}

/*!
  Destructor.
*/
SoCenterballDragger::~SoCenterballDragger()
{
  delete this->rotFieldSensor;
  delete this->centerFieldSensor;
}

/*!
  overloaded to save some information necessary to move center correctly.
*/
void
SoCenterballDragger::saveStartParameters(void)
{
  inherited::saveStartParameters();
  const SbMatrix &m = this->getMotionMatrix();
  this->savedtransl[0] = m[3][0];
  this->savedtransl[1] = m[3][1];
  this->savedtransl[2] = m[3][2];
  this->savedcenter = this->center.getValue();
}

/*!
  Should be called after motion matrix has been updated by a child dragger.
*/
void
SoCenterballDragger::transferCenterDraggerMotion(SoDragger * childdragger)
{
  if ((SoNode*)childdragger == XCenterChanger.getValue() ||
      (SoNode*)childdragger == YCenterChanger.getValue() ||
      (SoNode*)childdragger == ZCenterChanger.getValue()) {
    // translate part of matrix should not change. Move motion
    // into center instead.

    SbVec3f transl;
    SbMatrix matrix = this->getMotionMatrix();
    transl[0] = matrix[3][0];
    transl[1] = matrix[3][1];
    transl[2] = matrix[3][2];

    SbVec3f difftransl = transl - this->savedtransl;
    { // consider rotation before translating
      SbRotation rot = this->rotation.getValue();
      SbMatrix tmp;
      tmp.setRotate(rot.inverse());
      tmp.multVecMatrix(difftransl, difftransl);
    }

    this->centerFieldSensor->detach();
    this->center = difftransl + this->savedcenter;
    this->centerFieldSensor->attach(&this->center);

    matrix[3][0] = this->savedtransl[0];
    matrix[3][1] = this->savedtransl[1];
    matrix[3][2] = this->savedtransl[2];

    SbBool oldval = this->enableValueChangedCallbacks(FALSE);
    this->setMotionMatrix(matrix);
    this->enableValueChangedCallbacks(oldval);

    SoMatrixTransform *mt = SO_GET_ANY_PART(this, "translateToCenter", SoMatrixTransform);
    matrix.setTranslate(this->center.getValue());
    mt->matrix = matrix;
  }
}

/*!
  Sets values for the internal SoSwitch parts. If \a activechild == NULL,
  all feedback is deactivated.
*/
void
SoCenterballDragger::setSwitches(SoDragger * activechild)
{
  SoSwitch *sw;

  if (activechild == NULL || (SoNode*)activechild == rotator.getValue()) {
    // special feedback when rotator is activated/deactiveated
    int switchval = activechild != NULL ? 1 : 0;
    sw = SO_GET_ANY_PART(this, "XCenterChanger.translatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, switchval);;
    sw = SO_GET_ANY_PART(this, "YCenterChanger.translatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, switchval);;
    sw = SO_GET_ANY_PART(this, "ZCenterChanger.translatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, switchval);;
    sw = SO_GET_ANY_PART(this, "XRotator.rotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, switchval);;
    sw = SO_GET_ANY_PART(this, "YRotator.rotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, switchval);;
    sw = SO_GET_ANY_PART(this, "ZRotator.rotatorSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, switchval);;
  }

  // internal feedback
  int vals[3] = { SO_SWITCH_NONE, SO_SWITCH_NONE, SO_SWITCH_NONE };

  if ((SoNode*)activechild == XRotator.getValue()) {
    vals[0] = 0;
  }
  else if ((SoNode*)activechild == YRotator.getValue()) {
    vals[1] = 0;
  }
  else if ((SoNode*)activechild == ZRotator.getValue()) {
    vals[2] = 0;
  }
  else if (activechild != NULL) {
    vals[0] = vals[1] = vals[2] = 0;
  }

  sw = SO_GET_ANY_PART(this, "XAxisSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, vals[0]);
  sw = SO_GET_ANY_PART(this, "YAxisSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, vals[1]);
  sw = SO_GET_ANY_PART(this, "ZAxisSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, vals[2]);

}

SbBool
SoCenterballDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  int i;
  SbString str;

  if (onoff) {
    SoSeparator *emptysep = new SoSeparator;

    inherited::setUpConnections(onoff, doitalways);
    SoDragger *child;
    child = (SoDragger*) this->getAnyPart("rotator", FALSE);
    child->setPartAsDefault("rotator",
                            "centerballRotator");
    child->setPartAsDefault("rotatorActive",
                            "centerballRotatorActive");
    child->setPartAsDefault("feedback", emptysep);
    child->setPartAsDefault("feedbackActive", emptysep);
    this->addChildDragger(child);

    for (i = 0; i < 3; i++) {
      str.sprintf("%cRotator", 'X' + i);
      child = (SoDragger*) this->getAnyPart(str.getString(), FALSE);
      child->setPartAsDefault("rotator",
                              "centerballStripe");
      child->setPartAsDefault("rotatorActive",
                              "centerballStripeActive");
      child->setPartAsDefault("feedback", emptysep);
      child->setPartAsDefault("feedbackActive", emptysep);
      this->addChildDragger(child);
    }

    for (i = 0; i < 3; i++) {
      str.sprintf("%cCenterChanger", 'X' + i);
      child = (SoDragger*) this->getAnyPart(str.getString(), FALSE);
      child->setPartAsDefault("translator",
                              "centerballCenterChanger");
      child->setPartAsDefault("translatorActive",
                              "centerballCenterChangerActive");
      child->setPartAsDefault("xAxisFeedback",
                              "centerballCenterXAxisFeedback");
      child->setPartAsDefault("yAxisFeedback",
                              "centerballCenterYAxisFeedback");
      this->addChildDragger(child);
    }

    if (this->rotFieldSensor->getAttachedField() != &this->rotation) {
      this->rotFieldSensor->attach(&this->rotation);
    }
    if (this->centerFieldSensor->getAttachedField() != &this->center) {
      this->centerFieldSensor->attach(&this->center);
    }
  }
  else {
    this->removeChildDragger("rotator");
    this->removeChildDragger("XRotator");
    this->removeChildDragger("YRotator");
    this->removeChildDragger("ZRotator");
    this->removeChildDragger("XCenterChanger");
    this->removeChildDragger("YCenterChanger");
    this->removeChildDragger("ZCenterChanger");

    if (this->rotFieldSensor->getAttachedField() != NULL) {
      this->rotFieldSensor->detach();
    }
    if (this->centerFieldSensor->getAttachedField() != NULL) {
      this->centerFieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoCenterballDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
  inherited::setDefaultOnNonWritingFields();
}

void
SoCenterballDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoCenterballDragger *thisp = (SoCenterballDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoCenterballDragger::valueChangedCB(void *, SoDragger * d)
{
  SoCenterballDragger *thisp = (SoCenterballDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f t, s;
  SbRotation r, so;
  matrix.getTransform(t, r, s, so, thisp->center.getValue());

  thisp->rotFieldSensor->detach();
  if (thisp->rotation.getValue() != r) {
    thisp->rotation = r;
  }
  thisp->rotFieldSensor->attach(&thisp->rotation);
  thisp->transferCenterDraggerMotion(thisp->getActiveChildDragger());
}

/*!
  Needed to activate some feedback.
*/
void
SoCenterballDragger::kidStartCB(void * d , SoDragger * child)
{
  SoCenterballDragger *thisp = (SoCenterballDragger*)d;
  thisp->setSwitches(child);
}

/*!
  Needed to deactive some feedback.
*/
void
SoCenterballDragger::kidFinishCB(void * d, SoDragger * child)
{
  SoCenterballDragger *thisp = (SoCenterballDragger*)d;
  thisp->setSwitches(NULL);
}

//
// convenience method that takes care of callbacks before registering child
//
void
SoCenterballDragger::addChildDragger(SoDragger *child)
{
  child->addStartCallback(SoCenterballDragger::kidStartCB, this);
  child->addFinishCallback(SoCenterballDragger::kidFinishCB, this);
  this->registerChildDragger(child);
}

//
// convenience method that removes callbacks before unregistering child
//
void
SoCenterballDragger::removeChildDragger(const char *childname)
{
  SoDragger *child = (SoDragger*) this->getAnyPart(childname, FALSE);
  child->removeStartCallback(SoCenterballDragger::kidStartCB, this);
  child->removeFinishCallback(SoCenterballDragger::kidFinishCB, this);
  this->unregisterChildDragger(child);
}
