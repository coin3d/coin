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
  \class SoRotateCylindricalDragger SoRotateCylindricalDragger.h Inventor/draggers/SoRotateCylindricalDragger.h
  \brief The SoRotateCylindricalDragger class is for rotating geometry around a single axis.
  \ingroup draggers

  \DRAGGER_DEFAULT_SCREENSHOT

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/rotatecylindrical.png">
  </center>

  Use an instance of this dragger class in your scenegraph to let the
  end-users of your application rotate geometry around a pre-defined
  axis vector in 3D.

  For the dragger orientation and positioning itself, use some kind of
  transformation node in your scenegraph, as usual.
*/

#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/projectors/SbCylinderPlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/rotateCylindricalDragger.h>

SO_KIT_SOURCE(SoRotateCylindricalDragger);

/*!
  \var SoSFRotation SoRotateCylindricalDragger::rotation

  This field is continuously updated to contain the rotation of the
  current direction vector of the dragger.

  The application programmer using this dragger in his scenegraph
  should connect the relevant node fields in the scene to this field
  to make them follow the dragger orientation.
*/

/*!
  \var SoFieldSensor * SoRotateCylindricalDragger::fieldSensor
  \COININTERNAL
*/
/*!
  \var SbCylinderProjector * SoRotateCylindricalDragger::cylinderProj
  \COININTERNAL
*/
/*!
  \var SbBool SoRotateCylindricalDragger::userProj
  \COININTERNAL
*/


// doc in superclass
void
SoRotateCylindricalDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateCylindricalDragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoRotateCylindricalDragger
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
  CLASS SoRotateCylindricalDragger
  PVT   "this",  SoRotateCylindricalDragger  --- 
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
SoRotateCylindricalDragger::SoRotateCylindricalDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateCylindricalDragger);

  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("rotateCylindricalDragger.iv",
                                       ROTATECYLINDRICALDRAGGER_draggergeometry,
                                       sizeof(ROTATECYLINDRICALDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("rotator", "rotateCylindricalRotator");
  this->setPartAsDefault("rotatorActive", "rotateCylindricalRotatorActive");
  this->setPartAsDefault("feedback", "rotateCylindricalFeedback");
  this->setPartAsDefault("feedbackActive", "rotateCylindricalFeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->cylinderProj = new SbCylinderPlaneProjector();
  this->userProj = FALSE;
  this->addStartCallback(SoRotateCylindricalDragger::startCB);
  this->addMotionCallback(SoRotateCylindricalDragger::motionCB);
  this->addFinishCallback(SoRotateCylindricalDragger::doneCB);

  this->addValueChangedCallback(SoRotateCylindricalDragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoRotateCylindricalDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoRotateCylindricalDragger::~SoRotateCylindricalDragger()
{
  delete this->fieldSensor;
  if (!this->userProj) delete this->cylinderProj;
}

// Doc in superclass.
SbBool
SoRotateCylindricalDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoRotateCylindricalDragger::fieldSensorCB(this, NULL);

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
SoRotateCylindricalDragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f t, s;
  SbRotation r, so;

  matrix.getTransform(t, r, s, so);
  r = thisp->rotation.getValue();
  matrix.setTransform(t, r, s, so);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoRotateCylindricalDragger::valueChangedCB(void *, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
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
  Replace the default cylinder projection strategy. You may want to do
  this if you change the dragger geometry, for instance.

  The default cylinder projection is an SbCylinderPlaneProjector.

  \sa SbCylinderSectionProjector, SbCylinderSheetProjector
*/
void
SoRotateCylindricalDragger::setProjector(SbCylinderProjector * p)
{
  if (!this->userProj) delete this->cylinderProj;
  this->cylinderProj = p;
}

/*!
  Returns projector instance used for converting from user interaction
  dragger movements to 3D dragger re-orientation.

  \sa setProjector()
*/
const SbCylinderProjector *
SoRotateCylindricalDragger::getProjector(void) const
{
  return this->cylinderProj;
}

// Doc in superclass.
void
SoRotateCylindricalDragger::copyContents(const SoFieldContainer * fromfc,
                                         SbBool copyconnections)
{
  inherited::copyContents(fromfc, copyconnections);
  
  assert(fromfc->isOfType(SoRotateCylindricalDragger::getClassTypeId()));
  SoRotateCylindricalDragger *from = (SoRotateCylindricalDragger *)fromfc;
  if (!this->userProj) {
    delete this->cylinderProj; 
  }
  this->cylinderProj = NULL;
  
  if (from->cylinderProj) {
    this->cylinderProj = (SbCylinderProjector*) 
      from->cylinderProj->copy();
  }
  else {
    // just create a new one
    this->cylinderProj = new SbCylinderPlaneProjector();
  }
  // we copied or created a new one, and need to delete it.
  this->userProj = FALSE;
}

/*! \COININTERNAL */
void
SoRotateCylindricalDragger::startCB(void * f, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoRotateCylindricalDragger::motionCB(void * f, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoRotateCylindricalDragger::doneCB(void * f, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoRotateCylindricalDragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  SbLine line(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f));
  SbVec3f ptOnLine = line.getClosestPoint(hitPt);
  this->cylinderProj->setCylinder(SbCylinder(line, (ptOnLine-hitPt).length()));

  this->cylinderProj->setViewVolume(this->getViewVolume());
  this->cylinderProj->setWorkingSpace(this->getLocalToWorldMatrix());

  switch (this->getFrontOnProjector()) {
  case FRONT:
    this->cylinderProj->setFront(TRUE);
    break;
  case BACK:
    this->cylinderProj->setFront(TRUE);
    break;
  default: // avoid warnings
  case USE_PICK:
    this->cylinderProj->setFront(this->cylinderProj->isPointInFront(hitPt));
    break;
  }

}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoRotateCylindricalDragger::drag(void)
{
  this->cylinderProj->setViewVolume(this->getViewVolume());
  this->cylinderProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = cylinderProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();

  SbRotation rot = this->cylinderProj->getRotation(startPt, projPt);

  this->setMotionMatrix(this->appendRotation(this->getStartMotionMatrix(),
                                             rot, SbVec3f(0.0f, 0.0f, 0.0f)));
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoRotateCylindricalDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
