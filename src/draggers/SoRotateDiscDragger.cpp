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
  \class SoRotateDiscDragger SoRotateDiscDragger.h Inventor/draggers/SoRotateDiscDragger.h
  \brief The SoRotateDiscDragger class is for rotating geometry around a single axis.
  \ingroup draggers

  Use an instance of this dragger class in your scenegraph to let the
  end-users of your application rotate geometry around a pre-defined
  axis vector in 3D.

  The special feature of this dragger that sets it apart from the
  other draggers that provides rotation around an axis (like the
  SoRotateCylindricalDragger) is that it provides a convenient
  interface to rotate around an axis that is pointing in approximately
  the same direction as the camera. This is useful for interacting
  with for instance something like a "volume" knob.

  For the dragger orientation and positioning itself, use some kind of
  transformation node in your scenegraph, as usual.
*/

#include <Inventor/draggers/SoRotateDiscDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <math.h>

#include <data/draggerDefaults/rotateDiscDragger.h>

/*!
  \var SoSFRotation SoRotateDiscDragger::rotation

  This field is continuously updated to contain the rotation of the
  current direction vector of the dragger.

  The application programmer using this dragger in his scenegraph
  should connect the relevant node fields in the scene to this field
  to make them follow the dragger orientation.
*/

/*!
  \var SoFieldSensor * SoRotateDiscDragger::fieldSensor
  \COININTERNAL
*/
/*!
  \var SbPlaneProjector * SoRotateDiscDragger::planeProj
  \COININTERNAL
*/


SO_KIT_SOURCE(SoRotateDiscDragger);


// doc in superclass
void
SoRotateDiscDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateDiscDragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoRotateDiscDragger
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
  CLASS SoRotateDiscDragger
  PVT   "this",  SoRotateDiscDragger  --- 
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
SoRotateDiscDragger::SoRotateDiscDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateDiscDragger);

  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("rotateDiscDragger.iv",
                                       ROTATEDISCDRAGGER_draggergeometry,
                                       sizeof(ROTATEDISCDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("rotator", "rotateDiscRotator");
  this->setPartAsDefault("rotatorActive", "rotateDiscRotatorActive");
  this->setPartAsDefault("feedback", "rotateDiscFeedback");
  this->setPartAsDefault("feedbackActive", "rotateDiscFeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->planeProj = new SbPlaneProjector();
  this->addStartCallback(SoRotateDiscDragger::startCB);
  this->addMotionCallback(SoRotateDiscDragger::motionCB);
  this->addFinishCallback(SoRotateDiscDragger::doneCB);

  this->addValueChangedCallback(SoRotateDiscDragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoRotateDiscDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoRotateDiscDragger::~SoRotateDiscDragger()
{
  delete this->fieldSensor;
  delete this->planeProj;
}

// Doc in superclass.
SbBool
SoRotateDiscDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoRotateDiscDragger::fieldSensorCB(this, NULL);

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
SoRotateDiscDragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
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
SoRotateDiscDragger::valueChangedCB(void *, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->rotation.getValue() != rot)
    thisp->rotation = rot;
  thisp->fieldSensor->attach(&thisp->rotation);
}

/*! \COININTERNAL */
void
SoRotateDiscDragger::startCB(void *, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoRotateDiscDragger::motionCB(void *, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoRotateDiscDragger::doneCB(void * f, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoRotateDiscDragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  this->planeProj->setPlane(SbPlane(SbVec3f(0.0f, 0.0f, 1.0f),
                                    hitPt));
}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoRotateDiscDragger::drag(void)
{
  this->planeProj->setViewVolume(this->getViewVolume());
  this->planeProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = planeProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();

  SbPlane plane(SbVec3f(0.0f, 0.0f, 1.0f), startPt);
  SbLine line(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 1.0f));
  SbVec3f center;
  plane.intersect(line, center);
  startPt -= center;
  projPt -= center;
  startPt.normalize();
  projPt.normalize();
  SbVec3f dir = startPt.cross(projPt);
  float angle = acos(SbClamp(startPt.dot(projPt), -1.0f, 1.0f));
  if (dir[2] < 0.0f) angle = -angle;

  this->setMotionMatrix(this->appendRotation(this->getStartMotionMatrix(),
                                             SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), angle),
                                             SbVec3f(0.0f, 0.0f, 0.0f)));
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoRotateDiscDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
