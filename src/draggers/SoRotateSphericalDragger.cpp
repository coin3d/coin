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
  \class SoRotateSphericalDragger SoRotateSphericalDragger.h Inventor/draggers/SoRotateSphericalDragger.h
  \brief The SoRotateSphericalDragger class is for rotating geometry in any direction.
  \ingroup draggers

  \DRAGGER_DEFAULT_SCREENSHOT

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/rotatespherical.png">
  </center>

  Use an instance of this dragger class in your scenegraph to let the
  end-users of your application rotate geometry freely in any
  direction.

  For the initial dragger orientation and the dragger geometry
  positioning itself, use some kind of transformation node in your
  scenegraph, as usual.
*/

#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/rotateSphericalDragger.h>

/*!
  \var SoSFRotation SoRotateSphericalDragger::rotation

  This field is continuously updated to contain the rotation of the
  current direction vector of the dragger.

  The application programmer using this dragger in his scenegraph
  should connect the relevant node fields in the scene to this field
  to make them follow the dragger orientation.
*/

/*!
  \var SoFieldSensor * SoRotateSphericalDragger::fieldSensor
  \COININTERNAL
*/
/*!
  \var SbSphereProjector * SoRotateSphericalDragger::sphereProj
  \COININTERNAL
*/
/*!
  \var SbBool SoRotateSphericalDragger::userProj
  \COININTERNAL
*/
/*!
  \var SbMatrix SoRotateSphericalDragger::prevMotionMatrix
  \COININTERNAL
*/
/*!
  \var SbVec3f SoRotateSphericalDragger::prevWorldHitPt
  \COININTERNAL
*/

SO_KIT_SOURCE(SoRotateSphericalDragger);


// doc in superclass
void
SoRotateSphericalDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateSphericalDragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoRotateSphericalDragger
  -->"this"
        "callbackList"
        "topSeparator"
           "motionMatrix"
           "geomSeparator"
  -->         "rotatorSwitch"
  -->            "rotator"
  -->            "rotatorActive"
  -->         "feedbackSwitch"
  -->            "feedback"
  -->            "feedbackActive"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoRotateSphericalDragger
  PVT   "this",  SoRotateSphericalDragger  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
  PVT   "motionMatrix",  SoMatrixTransform  --- 
  PVT   "geomSeparator",  SoSeparator  --- 
  PVT   "rotatorSwitch",  SoSwitch  --- 
        "rotator",  SoSeparator  --- 
        "rotatorActive",  SoSeparator  --- 
  PVT   "feedbackSwitch",  SoSwitch  --- 
        "feedback",  SoSeparator  --- 
        "feedbackActive",  SoSeparator  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/
SoRotateSphericalDragger::SoRotateSphericalDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateSphericalDragger);

  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("rotateSphericalDragger.iv",
                                       ROTATESPHERICALDRAGGER_draggergeometry,
                                       sizeof(ROTATESPHERICALDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("rotator", "rotateSphericalRotator");
  this->setPartAsDefault("rotatorActive", "rotateSphericalRotatorActive");
  this->setPartAsDefault("feedback", "rotateSphericalFeedback");
  this->setPartAsDefault("feedbackActive", "rotateSphericalFeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->sphereProj = new SbSpherePlaneProjector();
  this->userProj = FALSE;
  this->addStartCallback(SoRotateSphericalDragger::startCB);
  this->addMotionCallback(SoRotateSphericalDragger::motionCB);
  this->addFinishCallback(SoRotateSphericalDragger::doneCB);

  this->addValueChangedCallback(SoRotateSphericalDragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoRotateSphericalDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoRotateSphericalDragger::~SoRotateSphericalDragger()
{
  delete this->fieldSensor;
  if (!this->userProj) delete this->sphereProj;
}

// Doc in superclass.
SbBool
SoRotateSphericalDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoRotateSphericalDragger::fieldSensorCB(this, NULL);

    if (this->fieldSensor->getAttachedField() != &this->rotation) {
      this->fieldSensor->attach(&this->rotation);
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
SoRotateSphericalDragger::fieldSensorCB(void *d, SoSensor *)
{
  assert(d);
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  matrix.setTransform(trans, thisp->rotation.getValue(), scale, scaleOrient);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoRotateSphericalDragger::valueChangedCB(void *, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->rotation.getValue() != rot)
    thisp->rotation = rot;
  thisp->fieldSensor->attach(&thisp->rotation);
}

/*!
  Replace the default sphere projection strategy. You may want to do
  this if you change the dragger geometry, for instance.

  The default projection is an SbSpherePlaneProjector.

  \sa SbSphereSectionProjector, SbSphereSheetProjector
*/
void
SoRotateSphericalDragger::setProjector(SbSphereProjector * p)
{
  if (!this->userProj) delete this->sphereProj;
  this->userProj = TRUE;
  this->sphereProj = p;
}

/*!
  Returns projector instance used for converting from user interaction
  dragger movements to 3D dragger re-orientation.

  \sa setProjector()
*/
const SbSphereProjector *
SoRotateSphericalDragger::getProjector(void) const
{
  return this->sphereProj;
}

// Doc in superclass.
void
SoRotateSphericalDragger::copyContents(const SoFieldContainer * fromfc,
                                       SbBool copyconnections)
{
  inherited::copyContents(fromfc, copyconnections);
  assert(fromfc->isOfType(SoRotateSphericalDragger::getClassTypeId()));
  SoRotateSphericalDragger *from = (SoRotateSphericalDragger*) fromfc;

  if (!this->userProj) {
    delete this->sphereProj;
  }
  if (from->sphereProj) {
    this->sphereProj = (SbSphereProjector*)from->sphereProj->copy();
  }
  else {
    this->sphereProj = new SbSpherePlaneProjector();
  }
  // we copied or created a new one, and need to delete it
  this->userProj = FALSE;
}

/*! \COININTERNAL */
void
SoRotateSphericalDragger::startCB(void *, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoRotateSphericalDragger::motionCB(void *, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoRotateSphericalDragger::doneCB(void * f, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoRotateSphericalDragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();

  float radius = hitPt.length();
  this->sphereProj->setSphere(SbSphere(SbVec3f(0.0f, 0.0f, 0.0f), radius));

  this->sphereProj->setViewVolume(this->getViewVolume());
  this->sphereProj->setWorkingSpace(this->getLocalToWorldMatrix());

  switch (this->getFrontOnProjector()) {
  case FRONT:
    this->sphereProj->setFront(TRUE);
    break;
  case BACK:
    this->sphereProj->setFront(TRUE);
    break;
  default: // avoid warnings
  case USE_PICK:
    this->sphereProj->setFront(this->sphereProj->isPointInFront(hitPt));
    break;
  }
  SbVec3f projPt = this->sphereProj->project(this->getNormalizedLocaterPosition());
  this->getLocalToWorldMatrix().multVecMatrix(projPt, this->prevWorldHitPt);
  this->prevMotionMatrix = this->getMotionMatrix();
}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoRotateSphericalDragger::drag(void)
{
  this->sphereProj->setViewVolume(this->getViewVolume());
  this->sphereProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f startPt;
  this->getWorldToLocalMatrix().multVecMatrix(this->prevWorldHitPt, startPt);
  SbVec3f projPt = this->sphereProj->project(this->getNormalizedLocaterPosition());
  this->getLocalToWorldMatrix().multVecMatrix(projPt, this->prevWorldHitPt);

  SbRotation rot = this->sphereProj->getRotation(startPt, projPt);

  this->prevMotionMatrix = this->appendRotation(this->prevMotionMatrix, rot,
                                                SbVec3f(0.0f, 0.0f, 0.0f));
  this->setMotionMatrix(this->prevMotionMatrix);
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoRotateSphericalDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
