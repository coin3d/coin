/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLSphereSensor SoVRMLSphereSensor.h Inventor/VRMLnodes/SoVRMLSphereSensor
  \brief The SoVRMLSphereSensor class maps pointer motion into rotations on a sphere.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  SphereSensor {
    exposedField SFBool     autoOffset        TRUE
    exposedField SFBool     enabled           TRUE
    exposedField SFRotation offset            0 1 0 0  # [-1,1],(-,)
    eventOut     SFBool     isActive
    eventOut     SFRotation rotation_changed
    eventOut     SFVec3f    trackPoint_changed
  }
  \endverbatim
  
  The SphereSensor node maps pointing device motion into spherical rotation
  about the origin of the local coordinate system. The SphereSensor node uses
  the descendent geometry of its parent node to determine whether it is liable
  to generate events.

  The \e enabled exposed field enables and disables the SphereSensor
  node. If \e enabled is TRUE, the sensor reacts appropriately to user
  events. If \e enabled is FALSE, the sensor does not track user input
  or send events. If \e enabled receives a FALSE event and \e isActive
  is TRUE, the sensor becomes disabled and deactivated, and outputs an \e isActive
  FALSE event. If \e enabled receives a TRUE event the sensor is enabled
  and ready for user activation.
  
  The SphereSensor node generates events when the pointing device is activated
  while the pointer is indicating any descendent geometry nodes of the sensor's
  parent group. See 4.6.7.5, Activating and manipulating sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.5),
  for details on using the pointing device to activate the SphereSensor.

  Upon activation of the pointing device (e.g., mouse button down)
  over the sensor's geometry, an \e isActive TRUE event is sent. The
  vector defined by the initial point of intersection on the
  SphereSensor's geometry and the local origin determines the radius
  of the sphere that is used to map subsequent pointing device motion
  while dragging. The virtual sphere defined by this radius and the
  local origin at the time of activation is used to interpret
  subsequent pointing device motion and is not affected by any changes
  to the sensor's coordinate system while the sensor is active.  

  For each position of the bearing, a rotation_changed event is sent
  which corresponds to the sum of the relative rotation from the
  original intersection point plus the offset
  value. trackPoint_changed events reflect the unclamped drag position
  on the surface of this sphere.  When the pointing device is
  deactivated and autoOffset is TRUE, offset is set to the last
  rotation_changed value and an offset_changed event is generated. See
  4.6.7.4, Drag sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.4),
  for more details.

  When the sensor generates an \e isActive TRUE event, it grabs all
  further motion events from the pointing device until it is released
  and generates an isActive FALSE event (other pointing-device sensors
  shall not generate events during this time). Motion of the pointing
  device while isActive is TRUE is termed a "drag". If a 2D pointing
  device is in use, isActive events will typically reflect the state
  of the primary button associated with the device (i.e., isActive is
  TRUE when the primary button is pressed and FALSE when it is
  released).  If a 3D pointing device (e.g., wand) is in use, isActive
  events will typically reflect whether the pointer is within (or in
  contact with) the sensor's geometry.

  While the pointing device is activated, trackPoint_changed and
  rotation_changed events are output. trackPoint_changed events
  represent the unclamped intersection points on the surface of the
  invisible sphere. If the pointing device is dragged off the sphere
  while activated, browsers may interpret this in a variety of ways
  (e.g., clamp all values to the sphere or continue to rotate as the
  point is dragged away from the sphere).

  Each movement of the pointing device while \e isActive is TRUE
  generates \e trackPoint_changed and \e rotation_changed events.  Further
  information about this behaviour can be found in 4.6.7.3
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.3),
  Pointing-device sensors, 4.6.7.4, Drag sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.4),
  and 4.6.7.5, Activating and manipulating sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.5).
  
*/

/*!
  \var SoSFRotation SoVRMLSphereSensor::offset
  The sensor rotation value.
*/

/*!
  \var SoSFRotation SoVRMLSphereSensor::rotation_changed

  An eventOut that is updated during interaction. Holds the differense
  between the current rotation and the sensor rotation value.

*/

#include <Inventor/VRMLnodes/SoVRMLSphereSensor.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>

SO_NODE_SOURCE(SoVRMLSphereSensor);

// Doc in parent
void
SoVRMLSphereSensor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLSphereSensor, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLSphereSensor::SoVRMLSphereSensor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLSphereSensor);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(offset, (0.0f, 1.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EVENT_OUT(rotation_changed);

  this->sphereproj = new SbSpherePlaneProjector;
}

/*!
  Destructor.
*/
SoVRMLSphereSensor::~SoVRMLSphereSensor()
{
  delete this->sphereproj;
}

// Doc in parent
SbBool
SoVRMLSphereSensor::dragStart(void)
{
  SbVec3f hitpt = this->getLocalStartingPoint();
  float radius = hitpt.length();
  if (radius == 0.0f) return FALSE;

  this->sphereproj->setSphere(SbSphere(SbVec3f(0.0f, 0.0f, 0.0f), radius));
  this->sphereproj->setViewVolume(this->getViewVolume());
  this->sphereproj->setWorkingSpace(this->getLocalToWorldMatrix());
  SbVec3f projpt = hitpt;
  this->getLocalToWorldMatrix().multVecMatrix(hitpt, this->prevworldhitpt);
  this->prevrotation = SbRotation::identity();
  this->rotation_changed = this->offset.getValue();

  return TRUE;
}

// Doc in parent
void
SoVRMLSphereSensor::drag(void)
{
  this->sphereproj->setViewVolume(this->getViewVolume());
  this->sphereproj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f startpt;
  this->getWorldToLocalMatrix().multVecMatrix(this->prevworldhitpt, startpt);
  SbVec3f projpt = this->sphereproj->project(this->getNormalizedLocaterPosition());
  this->getLocalToWorldMatrix().multVecMatrix(projpt, this->prevworldhitpt);

  SbRotation rot = this->sphereproj->getRotation(startpt, projpt);
  this->prevrotation *= rot;
  this->rotation_changed = this->prevrotation * this->offset.getValue();
}

// Doc in parent
void
SoVRMLSphereSensor::dragFinish(void)
{
  if (this->autoOffset.getValue()) {
    this->offset = this->rotation_changed.getValue();
  }
}
