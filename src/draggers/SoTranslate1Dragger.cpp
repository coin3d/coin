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
  \class SoTranslate1Dragger SoTranslate1Dragger.h Inventor/draggers/SoTranslate1Dragger.h
  \brief The SoTranslate1Dragger class provides a mechanism for the end-user to translate along an axis.
  \ingroup draggers

  \DRAGGER_DEFAULT_SCREENSHOT

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/translate1.png">
  </center>

  Use this dragger to allow the end-user of your application to
  translate along the X-axis and the Y-axis at the same time, ie
  freely inside a 3D plane. (Use a transformation node in front of the
  dragger to position it and re-orient it to translate in any plane.)

  \sa SoTranslate2Dragger, SoDragPointDragger
*/
// FIXME: in class-doc, explain that one should always connect to the
// x-coordinate of this dragger. Also show by a code snippet how to 0)
// use a SoDecomposeVec3fEngine for extracting the x-coordinate, and
// 1) set up a vertical or "depth" version of the dragger by using a
// rotation transform. 20011021 mortene.

#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <data/draggerDefaults/translate1Dragger.h>

/*!
  \var SoSFVec3f SoTranslate1Dragger::translation

  Continuously updated to contain the current translation from the
  dragger's local origo position. Only the first component (the X
  value) can be changed by the end-user.

  The application programmer applying this dragger in his code should
  connect the relevant node fields in the scene to this field to make
  it follow the dragger.
*/

/*!
  \var SbLineProjector * SoTranslate1Dragger::lineProj

  The SbLineProjector instance used for projecting from 2D mouse
  cursor positions to 3D points.
*/

/*!
  \var SoFieldSensor * SoTranslate1Dragger::fieldSensor
  \COININTERNAL
*/


SO_KIT_SOURCE(SoTranslate1Dragger);


// doc in superclass
void
SoTranslate1Dragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTranslate1Dragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoTranslate1Dragger
  -->"this"
        "callbackList"
        "topSeparator"
           "motionMatrix"
           "geomSeparator"
  -->         "translatorSwitch"
  -->            "translator"
  -->            "translatorActive"
  -->         "feedbackSwitch"
  -->            "feedback"
  -->            "feedbackActive"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoTranslate1Dragger
  PVT   "this",  SoTranslate1Dragger  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
  PVT   "motionMatrix",  SoMatrixTransform  --- 
  PVT   "geomSeparator",  SoSeparator  --- 
  PVT   "translatorSwitch",  SoSwitch  --- 
        "translator",  SoSeparator  --- 
        "translatorActive",  SoSeparator  --- 
  PVT   "feedbackSwitch",  SoSwitch  --- 
        "feedback",  SoSeparator  --- 
        "feedbackActive",  SoSeparator  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/
SoTranslate1Dragger::SoTranslate1Dragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTranslate1Dragger);

  SO_KIT_ADD_CATALOG_ENTRY(translatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoSeparator, TRUE, translatorSwitch, translatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorActive, SoSeparator, TRUE, translatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("translate1Dragger.iv",
                                       TRANSLATE1DRAGGER_draggergeometry,
                                       sizeof(TRANSLATE1DRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));

  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("translator", "translate1Translator");
  this->setPartAsDefault("translatorActive", "translate1TranslatorActive");
  this->setPartAsDefault("feedback", "translate1Feedback");
  this->setPartAsDefault("feedbackActive", "translate1FeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->lineProj = new SbLineProjector();
  this->addStartCallback(SoTranslate1Dragger::startCB);
  this->addMotionCallback(SoTranslate1Dragger::motionCB);
  this->addFinishCallback(SoTranslate1Dragger::finishCB);

  this->addValueChangedCallback(SoTranslate1Dragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoTranslate1Dragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoTranslate1Dragger::~SoTranslate1Dragger()
{
  delete this->lineProj;
  delete this->fieldSensor;
}

// Doc in super.
SbBool
SoTranslate1Dragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoTranslate1Dragger::fieldSensorCB(this, NULL);

    if (this->fieldSensor->getAttachedField() != &this->translation) {
      this->fieldSensor->attach(&this->translation);
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
SoTranslate1Dragger::fieldSensorCB(void *d, SoSensor *)
{
  assert(d);
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f t = thisp->translation.getValue();
  matrix[3][0] = t[0];
  matrix[3][1] = t[1];
  matrix[3][2] = t[2];
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoTranslate1Dragger::valueChangedCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

#if 0 // FIXME: what's up here? 20011113 mortene.
  SbVec3f t;
  t[0] = matrix[3][0];
  t[1] = matrix[3][1];
  t[2] = matrix[3][2];

  thisp->fieldSensor->detach();
  if (thisp->translation.getValue() != t) {
    thisp->translation = t;
  }
  thisp->fieldSensor->attach(&thisp->translation);
#else
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->fieldSensor->attach(&thisp->translation);
#endif
}

/*! \COININTERNAL */
void
SoTranslate1Dragger::startCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoTranslate1Dragger::motionCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoTranslate1Dragger::finishCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoTranslate1Dragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  this->lineProj->setLine(SbLine(hitPt, hitPt + SbVec3f(1.0f, 0.0f, 0.0f)));
}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoTranslate1Dragger::drag(void)
{
  this->lineProj->setViewVolume(this->getViewVolume());
  this->lineProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = this->lineProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();
  SbVec3f motion = projPt - startPt;
  this->setMotionMatrix(this->appendTranslation(this->getStartMotionMatrix(), motion));
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoTranslate1Dragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
