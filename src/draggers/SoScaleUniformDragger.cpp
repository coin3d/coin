/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoScaleUniformDragger SoScaleUniformDragger.h Inventor/draggers/SoScaleUniformDragger.h
  \brief The SoScaleUniformDragger class is (FIXME: doc)
  \ingroup draggers

  FIXME: document class
*/

#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>

#include <Inventor/SbRotation.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/scaleUniformDragger.h>

SO_KIT_SOURCE(SoScaleUniformDragger);


// doc in superclass
void
SoScaleUniformDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoScaleUniformDragger);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  Default constructor, sets up the dragger nodekit catalog with the
  interaction and feedback geometry.
 */
SoScaleUniformDragger::SoScaleUniformDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoScaleUniformDragger);

  SO_KIT_ADD_CATALOG_ENTRY(scalerSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoSeparator, TRUE, scalerSwitch, scalerActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scalerActive, SoSeparator, TRUE, scalerSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("scaleUniformDragger.iv",
                                       SCALEUNIFORMDRAGGER_draggergeometry,
                                       sizeof(SCALEUNIFORMDRAGGER_draggergeometry));
  }
  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("scaler", "scaleUniformScaler");
  this->setPartAsDefault("scalerActive", "scaleUniformScalerActive");
  this->setPartAsDefault("feedback", "scaleUniformFeedback");
  this->setPartAsDefault("feedbackActive", "scaleUniformFeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->lineProj = new SbLineProjector();

  this->addStartCallback(SoScaleUniformDragger::startCB);
  this->addMotionCallback(SoScaleUniformDragger::motionCB);
  this->addFinishCallback(SoScaleUniformDragger::finishCB);

  this->addValueChangedCallback(SoScaleUniformDragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoScaleUniformDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);

}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoScaleUniformDragger::~SoScaleUniformDragger()
{
  delete this->lineProj;
  delete this->fieldSensor;
}

SbBool
SoScaleUniformDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoScaleUniformDragger::fieldSensorCB(this, NULL);

    if (this->fieldSensor->getAttachedField() != &this->scaleFactor) {
      this->fieldSensor->attach(&this->scaleFactor);
    }
  }
  else {
    if (this->fieldSensor->getAttachedField() != NULL) {
      this->fieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  this->connectionsSetUp = onoff;
  return oldval;
}

void
SoScaleUniformDragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoScaleUniformDragger *thisp = (SoScaleUniformDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f t, s;
  SbRotation r, so;

  matrix.getTransform(t, r, s, so);
  s = thisp->scaleFactor.getValue();
  matrix.setTransform(t, r, s, so);
  thisp->setMotionMatrix(matrix);
}

void
SoScaleUniformDragger::valueChangedCB(void *, SoDragger * d)
{
  SoScaleUniformDragger *thisp = (SoScaleUniformDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->fieldSensor->attach(&thisp->scaleFactor);
}

void
SoScaleUniformDragger::startCB(void *, SoDragger * d)
{
  SoScaleUniformDragger *thisp = (SoScaleUniformDragger*)d;
  thisp->dragStart();
}

void
SoScaleUniformDragger::motionCB(void *, SoDragger * d)
{
  SoScaleUniformDragger *thisp = (SoScaleUniformDragger*)d;
  thisp->drag();
}

void
SoScaleUniformDragger::finishCB(void *, SoDragger * d)
{
  SoScaleUniformDragger *thisp = (SoScaleUniformDragger*)d;
  thisp->dragFinish();
}

void
SoScaleUniformDragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f startPt = this->getLocalStartingPoint();
  this->lineProj->setLine(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), startPt));
}

void
SoScaleUniformDragger::drag(void)
{
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

void
SoScaleUniformDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
