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
  \class SoTabBoxDragger SoTabBoxDragger.h Inventor/draggers/SoTabBoxDragger.h
  \brief The SoTabBoxDragger wraps a box around geometry you can then translate and scale.
  \ingroup draggers

  This dragger lets the end-user do translation and non-uniform
  scaling of geometry with an easy to understand interface.

  Click and drag any side of the box to translate (hold down a SHIFT
  key to lock to one axis) and click and drag any of the tab markers
  in the corners to scale. The way the different tabs influences the
  scale operation should be straight-forward and intuitive to the
  end-user.

  The SoTabBoxDragger is a composite dragger, implemented with 6
  SoTabPlaneDragger instances set up as the sides of a box.
*/

#include <Inventor/draggers/SoTabBoxDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoTabPlaneDragger.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/tabBoxDragger.h>

/*!
  \var SoSFVec3f SoTabBoxDragger::translation

  Continuously updated to contain the current translation from the
  dragger's local origo position.
*/
/*!
  \var SoSFVec3f SoTabBoxDragger::scaleFactor

  Continuously updated to contain the current vector of scaling along
  the X, Y and Z axes.
*/

/*!
  \var SoFieldSensor * SoTabBoxDragger::translFieldSensor
  \internal
*/
/*!
  \var SoFieldSensor * SoTabBoxDragger::scaleFieldSensor
  \internal
*/

SO_KIT_SOURCE(SoTabBoxDragger);


// doc in superclass
void
SoTabBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTabBoxDragger, SO_FROM_INVENTOR_2_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  Default constructor, sets up the dragger nodekit catalog with the
  interaction and feedback geometry.
 */
SoTabBoxDragger::SoTabBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTabBoxDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, tabPlane1Sep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane1Sep, SoSeparator, FALSE, topSeparator, tabPlane2Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane1Xf, SoTransform, TRUE, tabPlane1Sep, tabPlane1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane1, SoTabPlaneDragger, TRUE, tabPlane1Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane2Sep, SoSeparator, FALSE, topSeparator, tabPlane3Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane2Xf, SoTransform, TRUE, tabPlane2Sep, tabPlane2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane2, SoTabPlaneDragger, TRUE, tabPlane2Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane3Sep, SoSeparator, FALSE, topSeparator, tabPlane4Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane3Xf, SoTransform, TRUE, tabPlane3Sep, tabPlane3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane3, SoTabPlaneDragger, TRUE, tabPlane3Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane4Sep, SoSeparator, FALSE, topSeparator, tabPlane5Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane4Xf, SoTransform, TRUE, tabPlane4Sep, tabPlane4, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane4, SoTabPlaneDragger, TRUE, tabPlane4Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane5Sep, SoSeparator, FALSE, topSeparator, tabPlane6Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane5Xf, SoTransform, TRUE, tabPlane5Sep, tabPlane5, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane5, SoTabPlaneDragger, TRUE, tabPlane5Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane6Sep, SoSeparator, FALSE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane6Xf, SoTransform, TRUE, tabPlane6Sep, tabPlane6, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane6, SoTabPlaneDragger, TRUE, tabPlane6Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(boxGeom, SoSeparator, TRUE, geomSeparator, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("tabBoxDragger.iv",
                                       TABBOXDRAGGER_draggergeometry,
                                       sizeof(TABBOXDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  // create subdraggers
  SO_GET_ANY_PART(this, "tabPlane1", SoTabPlaneDragger);
  SO_GET_ANY_PART(this, "tabPlane2", SoTabPlaneDragger);
  SO_GET_ANY_PART(this, "tabPlane3", SoTabPlaneDragger);
  SO_GET_ANY_PART(this, "tabPlane4", SoTabPlaneDragger);
  SO_GET_ANY_PART(this, "tabPlane5", SoTabPlaneDragger);
  SO_GET_ANY_PART(this, "tabPlane6", SoTabPlaneDragger);

  this->setPartAsDefault("boxGeom", "tabBoxBoxGeom");

  this->initTransformNodes();

  this->addValueChangedCallback(SoTabBoxDragger::valueChangedCB);

  this->scaleFieldSensor = new SoFieldSensor(SoTabBoxDragger::fieldSensorCB, this);
  this->translFieldSensor = new SoFieldSensor(SoTabBoxDragger::fieldSensorCB, this);
  this->setUpConnections(TRUE, TRUE);
}


/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoTabBoxDragger::~SoTabBoxDragger()
{
  delete this->scaleFieldSensor;
  delete this->translFieldSensor;
}

// Doc in superclass.
SbBool
SoTabBoxDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbString str;
  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *child;
    for (int i = 1; i <= 6; i++) {
      str.sprintf("tabPlane%d", i);
      child = (SoDragger*)this->getAnyPart(str.getString(), FALSE);
      child->setPartAsDefault("translator", "tabBoxTranslator");
      child->setPartAsDefault("scaleTabMaterial", "tabBoxScaleTabMaterial");
      child->setPartAsDefault("scaleTabHints", "tabBoxScaleTabHints");
      child->addStartCallback(SoTabBoxDragger::invalidateSurroundScaleCB, this);
      child->addFinishCallback(SoTabBoxDragger::invalidateSurroundScaleCB, this);
      this->registerChildDragger(child);
    }

    if (this->translFieldSensor->getAttachedField() != &this->translation) {
      this->translFieldSensor->attach(&this->translation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }
  }
  else {
    SoDragger *child;
    for (int i = 1; i <= 6; i++) {
      str.sprintf("tabPlane%d", i);
      child = (SoDragger*)this->getAnyPart(str.getString(), FALSE);
      child->removeStartCallback(SoTabBoxDragger::invalidateSurroundScaleCB, this);
      child->removeFinishCallback(SoTabBoxDragger::invalidateSurroundScaleCB, this);
      this->unregisterChildDragger(child);
    }

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

// Doc in superclass.
void
SoTabBoxDragger::setDefaultOnNonWritingFields(void)
{
  this->surroundScale.setDefault(TRUE);

  this->tabPlane1.setDefault(TRUE);
  this->tabPlane2.setDefault(TRUE);
  this->tabPlane3.setDefault(TRUE);
  this->tabPlane4.setDefault(TRUE);
  this->tabPlane5.setDefault(TRUE);
  this->tabPlane6.setDefault(TRUE);

  inherited::setDefaultOnNonWritingFields();
}

/*! \internal */
void
SoTabBoxDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoTabBoxDragger *thisp = (SoTabBoxDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

/*! \internal */
void
SoTabBoxDragger::valueChangedCB(void *, SoDragger *d)
{
  SoTabBoxDragger *thisp = (SoTabBoxDragger*)d;

  const SbMatrix &matrix = thisp->getMotionMatrix();
  SbVec3f t, s;
  SbRotation r, so;
  matrix.getTransform(t, r, s, so);

  thisp->translFieldSensor->detach();
  if (thisp->translation.getValue() != t) {
    thisp->translation = t;
  }
  thisp->translFieldSensor->attach(&thisp->translation);

  thisp->scaleFieldSensor->detach();
  if (thisp->scaleFactor.getValue() != s) {
    thisp->scaleFactor = s;
  }
  thisp->scaleFieldSensor->attach(&thisp->scaleFactor);
}

/*!
  \e surroundScale is invalidated every time a child dragger is
  activated/deactivated using this callback
*/
void
SoTabBoxDragger::invalidateSurroundScaleCB(void * d, SoDragger *)
{
  SoTabBoxDragger *thisp = (SoTabBoxDragger*)d;
  SoSurroundScale *ss = SO_CHECK_PART(thisp, "surroundScale", SoSurroundScale);
  if (ss) ss->invalidate();
}

// private
void
SoTabBoxDragger::initTransformNodes(void)
{
  SoTransform *tf;
  tf = SO_GET_ANY_PART(this, "tabPlane1Xf", SoTransform);
  tf->translation = SbVec3f(0.0f, 0.0f, 1.0f);
  tf = SO_GET_ANY_PART(this, "tabPlane2Xf", SoTransform);
  tf->translation = SbVec3f(0.0f, 0.0f, -1.0f);
  tf->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI);
  tf = SO_GET_ANY_PART(this, "tabPlane3Xf", SoTransform);
  tf->translation = SbVec3f(1.0f, 0.0f, 0.0f);
  tf->rotation = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), M_PI*0.5f);
  tf = SO_GET_ANY_PART(this, "tabPlane4Xf", SoTransform);
  tf->translation = SbVec3f(-1.0f, 0.0f, 0.0f);
  tf->rotation = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), -M_PI*0.5f);
  tf = SO_GET_ANY_PART(this, "tabPlane5Xf", SoTransform);
  tf->translation = SbVec3f(0.0f, 1.0f, 0.0f);
  tf->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), -M_PI*0.5f);
  tf = SO_GET_ANY_PART(this, "tabPlane6Xf", SoTransform);
  tf->translation = SbVec3f(0.0f, -1.0f, 0.0f);
  tf->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI*0.5f);
}

/*!
  Signals the dragger to recalculate the size of its tabs.

  Simply calls SoTabPlaneDragger::adjustScaleTabSize() for all 6
  planes this dragger consists of.
*/
void
SoTabBoxDragger::adjustScaleTabSize(void)
{
  SO_GET_ANY_PART(this, "tabPlane1", SoTabPlaneDragger)->adjustScaleTabSize();
  SO_GET_ANY_PART(this, "tabPlane2", SoTabPlaneDragger)->adjustScaleTabSize();
  SO_GET_ANY_PART(this, "tabPlane3", SoTabPlaneDragger)->adjustScaleTabSize();
  SO_GET_ANY_PART(this, "tabPlane4", SoTabPlaneDragger)->adjustScaleTabSize();
  SO_GET_ANY_PART(this, "tabPlane5", SoTabPlaneDragger)->adjustScaleTabSize();
  SO_GET_ANY_PART(this, "tabPlane6", SoTabPlaneDragger)->adjustScaleTabSize();
}
