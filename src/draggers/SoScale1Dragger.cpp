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
  \class SoScale1Dragger SoScale1Dragger.h Inventor/draggers/SoScale1Dragger.h
  \brief The SoScale1Dragger class provides a mechanism for the end-user to scale in one dimension.
  \ingroup draggers

  Use this dragger to allow the end-user of your application to scale
  along the X-axis. (Use a transformation node in front of the dragger
  to position it and re-orient it to scale along any vector.)

  \sa SoScale2Dragger, SoScaleUniformDragger, SoScale2UniformDragger
*/

#include <Inventor/draggers/SoScale1Dragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/SbRotation.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/scale1Dragger.h>

/*!
  \var SoSFVec3f SoScale1Dragger::scaleFactor

  Continuously updated to contain the current vector of scaling along
  the X, Y and Z axes.

  For the SoScale1Dragger, only the X component is used, the Y and Z
  components will always be equal to 1 (ie no scaling).
*/

/*!
  \var SoFieldSensor * SoScale1Dragger::fieldSensor
  \internal
*/
/*!
  \var SbLineProjector * SoScale1Dragger::lineProj
  \internal
*/

SO_KIT_SOURCE(SoScale1Dragger);


// doc in superclass
void
SoScale1Dragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoScale1Dragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  Default constructor, sets up the dragger nodekit catalog with the
  interaction and feedback geometry.
 */
SoScale1Dragger::SoScale1Dragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoScale1Dragger);

  SO_KIT_ADD_CATALOG_ENTRY(scalerSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoSeparator, TRUE, scalerSwitch, scalerActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scalerActive, SoSeparator, TRUE, scalerSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("scale1Dragger.iv",
                                       SCALE1DRAGGER_draggergeometry,
                                       sizeof(SCALE1DRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("scaler", "scale1Scaler");
  this->setPartAsDefault("scalerActive", "scale1ScalerActive");
  this->setPartAsDefault("feedback", "scale1Feedback");
  this->setPartAsDefault("feedbackActive", "scale1FeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->lineProj = new SbLineProjector();
  this->addStartCallback(SoScale1Dragger::startCB);
  this->addMotionCallback(SoScale1Dragger::motionCB);
  this->addFinishCallback(SoScale1Dragger::finishCB);

  this->addValueChangedCallback(SoScale1Dragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoScale1Dragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoScale1Dragger::~SoScale1Dragger()
{
  delete this->lineProj;
  delete this->fieldSensor;
}

// Doc in superclass.
SbBool
SoScale1Dragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoScale1Dragger::fieldSensorCB(this, NULL);

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

/*! \internal */
void
SoScale1Dragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f t, s;
  SbRotation r, so;

  matrix.getTransform(t, r, s, so);
  s = thisp->scaleFactor.getValue();
  matrix.setTransform(t, r, s, so);
  thisp->setMotionMatrix(matrix);
}

/*! \internal */
void
SoScale1Dragger::valueChangedCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->fieldSensor->attach(&thisp->scaleFactor);
}

/*! \internal */
void
SoScale1Dragger::startCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  thisp->dragStart();
}

/*! \internal */
void
SoScale1Dragger::motionCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  thisp->drag();
}

/*! \internal */
void
SoScale1Dragger::finishCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  thisp->dragFinish();
}

/*! \internal
  Called when dragger is selected (picked) by the user.
*/
void
SoScale1Dragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  this->lineProj->setLine(SbLine(hitPt, hitPt + SbVec3f(1.0f, 0.0f, 0.0f)));
}

/*! \internal
  Called when user drags the mouse after picking the dragger.
*/
void
SoScale1Dragger::drag(void)
{
  this->lineProj->setViewVolume(this->getViewVolume());
  this->lineProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = lineProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();

  float motion = projPt[0];
  if (startPt[0] != 0.0f)
    motion /= startPt[0];
  else
    motion = 0.0f;

  if (motion < 0.0f) motion = 0.0f;

  this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                          SbVec3f(fabs(motion), 1.0f, 1.0f),
                                          SbVec3f(0.0f, 0.0f, 0.0f)));
}

/*! \internal
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoScale1Dragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
