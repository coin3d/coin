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
  \class SoTransformBoxDragger SoTransformBoxDragger.h Inventor/draggers/SoTransformBoxDragger.h
  \brief The SoTransformBoxDragger provides a box which can be translated, scaled and rotated.
  \ingroup draggers

  \DRAGGER_DEFAULT_SCREENSHOT

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/transformbox.png">
  </center>

  Translate the dragger by clicking and dragging any of the
  (invisible) sides. Scaling is done by dragging the corner
  cubes. Only uniform scaling is supported. Rotation is done by
  dragging any of the 12 beams connecting the corner cubes.

  This dragger consists of a rigid framework for doing all the usual
  interaction operations on scene geometry. The "user interface" of
  the dragger is very simple, providing little room for the end-user
  to make mistakes.

  For the application programmer's convenience, the Coin library also
  provides a manipulator class called SoTransformBoxManip, which wraps
  the SoTransformBoxDragger into the necessary mechanisms for making
  direct insertion of this dragger into a scenegraph possible with
  very little effort.

  \sa SoTransformBoxManip
*/

#include <Inventor/draggers/SoTransformBoxDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/transformBoxDragger.h>

/*!
  \var SoSFRotation SoTransformBoxDragger::rotation

  This field is continuously updated to contain the rotation of the
  dragger's box.
*/
/*!
  \var SoSFVec3f SoTransformBoxDragger::translation

  The dragger's offset position from the local origo.
*/
/*!
  \var SoSFVec3f SoTransformBoxDragger::scaleFactor

  Continuously updated to contain the current vector of scaling along
  the X, Y and Z axes. The three components will always be equal, as
  this dragger only supports uniform scale operations.
*/

/*!
  \var SoFieldSensor * SoTransformBoxDragger::rotFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoTransformBoxDragger::translFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoTransformBoxDragger::scaleFieldSensor
  \COININTERNAL
*/


SO_KIT_SOURCE(SoTransformBoxDragger);


// doc in superclass
void
SoTransformBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTransformBoxDragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoTransformBoxDragger
  -->"this"
        "callbackList"
        "topSeparator"
           "motionMatrix"
  -->      "surroundScale"
  -->      "antiSquish"
  -->      "scaler"
  -->      "rotator1Sep"
  -->         "rotator1Rot"
  -->         "rotator1"
  -->      "rotator2Sep"
  -->         "rotator2Rot"
  -->         "rotator2"
  -->      "rotator3Sep"
  -->         "rotator3Rot"
  -->         "rotator3"
  -->      "translator1Sep"
  -->         "translator1Rot"
  -->         "translator1"
  -->      "translator2Sep"
  -->         "translator2Rot"
  -->         "translator2"
  -->      "translator3Sep"
  -->         "translator3Rot"
  -->         "translator3"
  -->      "translator4Sep"
  -->         "translator4Rot"
  -->         "translator4"
  -->      "translator5Sep"
  -->         "translator5Rot"
  -->         "translator5"
  -->      "translator6Sep"
  -->         "translator6Rot"
  -->         "translator6"
           "geomSeparator"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoTransformBoxDragger
  PVT   "this",  SoTransformBoxDragger  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
  PVT   "motionMatrix",  SoMatrixTransform  --- 
        "surroundScale",  SoSurroundScale  --- 
        "antiSquish",  SoAntiSquish  --- 
        "scaler",  SoScaleUniformDragger  --- 
  PVT   "rotator1Sep",  SoSeparator  --- 
  PVT   "rotator1Rot",  SoRotation  --- 
        "rotator1",  SoRotateCylindricalDragger  --- 
  PVT   "rotator2Sep",  SoSeparator  --- 
  PVT   "rotator2Rot",  SoRotation  --- 
        "rotator2",  SoRotateCylindricalDragger  --- 
  PVT   "rotator3Sep",  SoSeparator  --- 
  PVT   "rotator3Rot",  SoRotation  --- 
        "rotator3",  SoRotateCylindricalDragger  --- 
  PVT   "translator1Sep",  SoSeparator  --- 
  PVT   "translator1Rot",  SoRotation  --- 
        "translator1",  SoTranslate2Dragger  --- 
  PVT   "translator2Sep",  SoSeparator  --- 
  PVT   "translator2Rot",  SoRotation  --- 
        "translator2",  SoTranslate2Dragger  --- 
  PVT   "translator3Sep",  SoSeparator  --- 
  PVT   "translator3Rot",  SoRotation  --- 
        "translator3",  SoTranslate2Dragger  --- 
  PVT   "translator4Sep",  SoSeparator  --- 
  PVT   "translator4Rot",  SoRotation  --- 
        "translator4",  SoTranslate2Dragger  --- 
  PVT   "translator5Sep",  SoSeparator  --- 
  PVT   "translator5Rot",  SoRotation  --- 
        "translator5",  SoTranslate2Dragger  --- 
  PVT   "translator6Sep",  SoSeparator  --- 
  PVT   "translator6Rot",  SoRotation  --- 
        "translator6",  SoTranslate2Dragger  --- 
  PVT   "geomSeparator",  SoSeparator  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/
SoTransformBoxDragger::SoTransformBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTransformBoxDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, scaler, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoScaleUniformDragger, TRUE, topSeparator, rotator1Sep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1Sep, SoSeparator, FALSE, topSeparator, rotator2Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1Rot, SoRotation, TRUE, rotator1Sep, rotator1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1, SoRotateCylindricalDragger, TRUE, rotator1Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2Sep, SoSeparator, FALSE, topSeparator, rotator3Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2Rot, SoRotation, TRUE, rotator2Sep, rotator2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2, SoRotateCylindricalDragger, TRUE, rotator2Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3Sep, SoSeparator, FALSE, topSeparator, translator1Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3Rot, SoRotation, TRUE, rotator3Sep, rotator3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3, SoRotateCylindricalDragger, TRUE, rotator3Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Sep, SoSeparator, FALSE, topSeparator, translator2Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Rot, SoRotation, TRUE, translator1Sep, translator1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1, SoTranslate2Dragger, TRUE, translator1Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Sep, SoSeparator, FALSE, topSeparator, translator3Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Rot, SoRotation, TRUE, translator2Sep, translator2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2, SoTranslate2Dragger, TRUE, translator2Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Sep, SoSeparator, FALSE, topSeparator, translator4Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Rot, SoRotation, TRUE, translator3Sep, translator3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3, SoTranslate2Dragger, TRUE, translator3Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Sep, SoSeparator, FALSE, topSeparator, translator5Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Rot, SoRotation, TRUE, translator4Sep, translator4, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4, SoTranslate2Dragger, TRUE, translator4Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Sep, SoSeparator, FALSE, topSeparator, translator6Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Rot, SoRotation, TRUE, translator5Sep, translator5, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5, SoTranslate2Dragger, TRUE, translator5Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Sep, SoSeparator, FALSE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Rot, SoRotation, TRUE, translator6Sep, translator6, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6, SoTranslate2Dragger, TRUE, translator6Sep, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("transformBoxDragger.iv",
                                       TRANSFORMBOXDRAGGER_draggergeometry,
                                       sizeof(TRANSFORMBOXDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  SO_GET_ANY_PART(this, "scaler", SoScaleUniformDragger);
  SO_GET_ANY_PART(this, "rotator1", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "rotator2", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "rotator3", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "translator1", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator2", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator3", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator4", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator5", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator6", SoTranslate2Dragger);

  SoRotation *rot;
  rot = SO_GET_ANY_PART(this, "rotator1Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "rotator2Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "rotator3Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);

  rot = SO_GET_ANY_PART(this, "translator1Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "translator2Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), -M_PI/2.0f);

  rot = SO_GET_ANY_PART(this, "translator3Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "translator4Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), -M_PI/2.0f);

  rot = SO_GET_ANY_PART(this, "translator5Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI);
  rot = SO_GET_ANY_PART(this, "translator6Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f);

  SoAntiSquish *squish = SO_GET_ANY_PART(this, "antiSquish", SoAntiSquish);
  squish->sizing = SoAntiSquish::BIGGEST_DIMENSION;
  squish->recalcAlways = FALSE;

  this->addValueChangedCallback(SoTransformBoxDragger::valueChangedCB);
  this->rotFieldSensor = new SoFieldSensor(SoTransformBoxDragger::fieldSensorCB, this);
  this->rotFieldSensor->setPriority(0);
  this->translFieldSensor = new SoFieldSensor(SoTransformBoxDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoTransformBoxDragger::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);
  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoTransformBoxDragger::~SoTransformBoxDragger()
{
  delete this->rotFieldSensor;
  delete this->translFieldSensor;
  delete this->scaleFieldSensor;
}

// Doc in super.
SbBool
SoTransformBoxDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    int i;
    SbString str;
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *child = (SoDragger*) this->getAnyPart("scaler", FALSE);
    child->setPartAsDefault("scaler", "transformBoxScalerScaler");
    child->setPartAsDefault("scalerActive", "transformBoxScalerScalerActive");
    child->setPartAsDefault("feedback", "transformBoxScalerFeedback");
    child->setPartAsDefault("feedbackActive", "transformBoxScalerFeedbackActive");
    this->addChildDragger(child);

    for (i = 1; i <= 3; i++) {
      str.sprintf("rotator%d", i);
      child = (SoDragger*)this->getAnyPart(str.getString(), FALSE);
      child->setPartAsDefault("rotator", "transformBoxRotatorRotator");
      child->setPartAsDefault("rotatorActive", "transformBoxRotatorRotatorActive");
      child->setPartAsDefault("feedback", "transformBoxRotatorFeedback");
      child->setPartAsDefault("feedbackActive", "transformBoxRotatorFeedbackActive");
      this->addChildDragger(child);
    }

    for (i = 1; i <= 6; i++) {
      str.sprintf("translator%d", i);
      child = (SoDragger*)this->getAnyPart(str.getString(), FALSE);
      child->setPartAsDefault("translator", "transformBoxTranslatorTranslator");
      child->setPartAsDefault("translatorActive", "transformBoxTranslatorTranslatorActive");
      child->setPartAsDefault("xAxisFeedback", "transformBoxTranslatorXAxisFeedback");
      child->setPartAsDefault("yAxisFeedback", "transformBoxTranslatorYAxisFeedback");
      this->addChildDragger(child);
    }
    if (this->translFieldSensor->getAttachedField() != &this->translation) {
      this->translFieldSensor->attach(&this->translation);
    }
    if (this->rotFieldSensor->getAttachedField() != &this->rotation) {
      this->rotFieldSensor->attach(&this->rotation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }
  }
  else {
    this->removeChildDragger((SoDragger*)this->getAnyPart("scaler", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("rotator1", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("rotator2", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("rotator3", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("translator1", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("translator2", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("translator3", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("translator4", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("translator5", FALSE));
    this->removeChildDragger((SoDragger*)this->getAnyPart("translator6", FALSE));

    if (this->translFieldSensor->getAttachedField() != NULL) {
      this->translFieldSensor->detach();
    }
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

// Convenience method used to call setDefault on similar fields.
//
// Note: keep the function name prefix to avoid name clashes with
// other dragger .cpp files for "--enable-compact" builds.
//
// FIXME: should collect these methods in a common method visible to
// all draggers implementing the exact same functionality. 20010826 mortene.
static void 
SoTransformBoxDragger_set_default(SoDragger * dragger, const char * fmt,
                                  int minval, int maxval)
{
  SbString str;
  for (int i = minval; i <= maxval; i++) {
    str.sprintf(fmt, i);
    SoField * f = dragger->getField(str.getString());
    assert(f);
    f->setDefault(TRUE);
  }
}

// Doc in superclass.
void
SoTransformBoxDragger::setDefaultOnNonWritingFields(void)
{
  this->surroundScale.setDefault(TRUE);
  
  SoTransformBoxDragger_set_default(this, "rotator%d", 1, 3);
  SoTransformBoxDragger_set_default(this, "translator%d", 1, 6);

  inherited::setDefaultOnNonWritingFields();
}

/*! \COININTERNAL */
void
SoTransformBoxDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoTransformBoxDragger *thisp = (SoTransformBoxDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoTransformBoxDragger::valueChangedCB(void *, SoDragger * d)
{
  SoTransformBoxDragger *thisp = (SoTransformBoxDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);

  thisp->translFieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->translFieldSensor->attach(&thisp->translation);

  thisp->rotFieldSensor->detach();
  if (thisp->rotation.getValue() != rot)
    thisp->rotation = rot;
  thisp->rotFieldSensor->attach(&thisp->rotation);

  thisp->scaleFieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->scaleFieldSensor->attach(&thisp->scaleFactor);
}

// private
void 
SoTransformBoxDragger::addChildDragger(SoDragger * child)
{
  child->addStartCallback(invalidateSurroundScaleCB, this);
  child->addFinishCallback(invalidateSurroundScaleCB, this);
  this->registerChildDragger(child);
}

// private
void 
SoTransformBoxDragger::removeChildDragger(SoDragger * child)
{
  child->removeStartCallback(invalidateSurroundScaleCB, this);
  child->removeFinishCallback(invalidateSurroundScaleCB, this);
  this->unregisterChildDragger(child);
}

/*! \COININTERNAL */
void
SoTransformBoxDragger::invalidateSurroundScaleCB(void *, SoDragger * d)
{
  SoTransformBoxDragger * thisp = (SoTransformBoxDragger*) d;
  SoSurroundScale * surround = SO_CHECK_PART(thisp, "surroundScale", SoSurroundScale);
  if (surround) surround->invalidate();

  SoAntiSquish * squish = SO_CHECK_ANY_PART(thisp, "antiSquish", SoAntiSquish);
  if (squish) squish->recalc();
}
