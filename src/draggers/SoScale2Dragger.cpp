/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoScale2Dragger SoScale2Dragger.h Inventor/draggers/SoScale2Dragger.h
  \brief The SoScale2Dragger class provides a mechanism for the end-user to scale in two dimensions.
  \ingroup draggers

  \DRAGGER_DEFAULT_SCREENSHOT

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/scale2.png">
  </center>

  Use this dragger to allow the end-user of your application to scale
  along the X-axis and the Y-axis. (Use a transformation node in front
  of the dragger to position it and re-orient it to scale in any
  plane.)

  Scaling can be done in a non-uniform manner.

  \sa SoScale1Dragger, SoScaleUniformDragger, SoScale2UniformDragger
*/

#include <Inventor/draggers/SoScale2Dragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/SbRotation.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/scale2Dragger.h>


/*!
  \var SoSFVec3f SoScale2Dragger::scaleFactor

  Continuously updated to contain the current vector of scaling along
  the X, Y and Z axes.

  For the SoScale2Dragger, only the X and Y components are used, the Z
  component will always be equal to 1 (ie no scaling).
*/

/*!
  \var SoFieldSensor * SoScale2Dragger::fieldSensor
  \COININTERNAL
*/
/*!
  \var SbPlaneProjector * SoScale2Dragger::planeProj
  \COININTERNAL
*/

SO_KIT_SOURCE(SoScale2Dragger);


// doc in superclass
void
SoScale2Dragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoScale2Dragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoScale2Dragger
  -->"this"
        "callbackList"
        "topSeparator"
           "motionMatrix"
           "geomSeparator"
  -->         "scalerSwitch"
  -->            "scaler"
  -->            "scalerActive"
  -->         "feedbackSwitch"
  -->            "feedback"
  -->            "feedbackActive"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoScale2Dragger
  PVT   "this",  SoScale2Dragger  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
  PVT   "motionMatrix",  SoMatrixTransform  --- 
  PVT   "geomSeparator",  SoSeparator  --- 
  PVT   "scalerSwitch",  SoSwitch  --- 
        "scaler",  SoSeparator  --- 
        "scalerActive",  SoSeparator  --- 
  PVT   "feedbackSwitch",  SoSwitch  --- 
        "feedback",  SoSeparator  --- 
        "feedbackActive",  SoSeparator  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/
SoScale2Dragger::SoScale2Dragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoScale2Dragger);

  SO_KIT_ADD_CATALOG_ENTRY(scalerSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoSeparator, TRUE, scalerSwitch, scalerActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scalerActive, SoSeparator, TRUE, scalerSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("scale2Dragger.iv",
                                       SCALE2DRAGGER_draggergeometry,
                                       sizeof(SCALE2DRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("scaler", "scale2Scaler");
  this->setPartAsDefault("scalerActive", "scale2ScalerActive");
  this->setPartAsDefault("feedback", "scale2Feedback");
  this->setPartAsDefault("feedbackActive", "scale2FeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->planeProj = new SbPlaneProjector();
  this->addStartCallback(SoScale2Dragger::startCB);
  this->addMotionCallback(SoScale2Dragger::motionCB);
  this->addFinishCallback(SoScale2Dragger::finishCB);

  this->addValueChangedCallback(SoScale2Dragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoScale2Dragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoScale2Dragger::~SoScale2Dragger()
{
  delete this->planeProj;
  delete this->fieldSensor;
}

// Doc in superclass.
SbBool
SoScale2Dragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoScale2Dragger::fieldSensorCB(this, NULL);

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

/*! \COININTERNAL */
void
SoScale2Dragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoScale2Dragger *thisp = (SoScale2Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f t, s;
  SbRotation r, so;

  matrix.getTransform(t, r, s, so);
  s = thisp->scaleFactor.getValue();
  matrix.setTransform(t, r, s, so);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoScale2Dragger::valueChangedCB(void *, SoDragger * d)
{
  SoScale2Dragger *thisp = (SoScale2Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->fieldSensor->attach(&thisp->scaleFactor);
}

/*! \COININTERNAL */
void
SoScale2Dragger::startCB(void *, SoDragger * d)
{
  SoScale2Dragger *thisp = (SoScale2Dragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoScale2Dragger::motionCB(void *, SoDragger * d)
{
  SoScale2Dragger *thisp = (SoScale2Dragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoScale2Dragger::finishCB(void *, SoDragger * d)
{
  SoScale2Dragger *thisp = (SoScale2Dragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoScale2Dragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  this->planeProj->setPlane(SbPlane(SbVec3f(0.0f, 0.0f, 1.0f), hitPt));
}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoScale2Dragger::drag(void)
{
  this->planeProj->setViewVolume(this->getViewVolume());
  this->planeProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = this->planeProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();

  SbVec3f scale = projPt;
  scale[2] = 1.0f;

  if (startPt[0] != 0.0f)
    scale[0] /= startPt[0];
  else
    scale[0] = 0.0f;

  if (startPt[1] != 0.0f)
    scale[1] /= startPt[1];
  else
    scale[1] = 0.0f;

  this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                          scale,
                                          SbVec3f(0.0f, 0.0f, 0.0f)));
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoScale2Dragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
