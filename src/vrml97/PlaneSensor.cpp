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
  \class SoVRMLPlaneSensor SoVRMLPlaneSensor.h Inventor/VRMLnodes/SoVRMLPlaneSensor
  \brief The SoVRMLPlaneSensor class maps pointer motion into object space translations.
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim
  PlaneSensor {
    exposedField SFBool  autoOffset          TRUE
    exposedField SFBool  enabled             TRUE
    exposedField SFVec2f maxPosition         -1 -1     # (-,)
    exposedField SFVec2f minPosition         0 0       # (-,)
    exposedField SFVec3f offset              0 0 0     # (-,)
    eventOut     SFBool  isActive
    eventOut     SFVec3f trackPoint_changed
    eventOut     SFVec3f translation_changed
  }
  \endverbatim

  The PlaneSensor node maps pointing device motion into
  two-dimensional translation in a plane parallel to the Z=0 plane of
  the local coordinate system. The PlaneSensor node uses the
  descendent geometry of its parent node to determine whether it is
  liable to generate events.

  The \e enabled exposedField enables and disables the PlaneSensor.
  If \e enabled is TRUE, the sensor reacts appropriately to user events.
  If \e enabled is FALSE, the sensor does not track user input or send
  events. 

  If \e enabled receives a FALSE event and \e isActive is
  TRUE, the sensor becomes disabled and deactivated, and outputs an \e isActive
  FALSE event. 
  
  If enabled receives a TRUE event, the sensor is enabled
  and made ready for user activation.

  
  The PlaneSensor node generates events when the pointing device is
  activated while the pointer is indicating any descendent geometry
  nodes of the sensor's parent group. See 4.6.7.5, Activating and
  manipulating sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.5),
  for details on using the pointing device to activate the PlaneSensor.

  Upon activation of the pointing device (e.g., mouse button down)
  while indicating the sensor's geometry, an \e isActive TRUE event is
  sent. Pointer motion is mapped into relative translation in the
  tracking plane, (a plane parallel to the sensor's local Z=0 plane
  and coincident with the initial point of intersection). For each
  subsequent movement of the bearing, a translation_changed event is
  output which corresponds to the sum of the relative translation from
  the original intersection point to the intersection point of the new
  bearing in the plane plus the offset value. The sign of the
  translation is defined by the Z=0 plane of the sensor's coordinate
  system. 

  \e trackPoint_changed events reflect the unclamped drag
  position on the surface of this plane. When the pointing device is
  deactivated and \e autoOffset is TRUE, offset is set to the last
  translation_changed value and an offset_changed event is generated.
  More details are provided in 4.6.7.4, Drag sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.4).

  When the sensor generates an isActive TRUE event, it grabs all
  further motion events from the pointing device until it is
  deactivated and generates an isActive FALSE event. Other pointing-device sensors shall not
  generate events during this time. Motion of the pointing device
  while isActive is TRUE is referred to as a "drag". 

  If a 2D pointing device is in use, isActive events typically reflect
  the state of the primary button associated with the device (i.e.,
  isActive is TRUE when the primary button is pressed, and is FALSE
  when it is released).  

  If a 3D pointing device (e.g., wand) is in
  use, isActive events typically reflect whether the pointer is within
  or in contact with the sensor's geometry.  

  \e minPosition and \e maxPosition may be set to clamp
  translation_changed events to a range of values as measured from the
  origin of the Z=0 plane.  If the X or Y component of minPosition is
  greater than the corresponding component of maxPosition,
  translation_changed events are not clamped in that dimension. If the
  X or Y component of minPosition is equal to the corresponding
  component of maxPosition, that component is constrained to the given
  value. This technique provides a way to implement a line sensor that
  maps dragging motion into a translation in one dimension.  

  While the
  pointing device is activated and moved, \e trackPoint_changed and
  \e translation_changed events are sent. \e trackPoint_changed events
  represent the unclamped intersection points on the surface of the
  tracking plane. If the pointing device is dragged off of the
  tracking plane while activated (e.g., above horizon line), browsers
  may interpret this in a variety ways (e.g., clamp all values to the
  horizon). Each movement of the pointing device, while isActive is
  TRUE, generates \e trackPoint_changed and \e translation_changed events.
  Further information about this behaviour can be found in 4.6.7.3,
  Pointing-device sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.3),
  4.6.7.4, Drag sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.4),
  and 4.6.7.5, Activating and manipulating sensors
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7.5).
  
*/

/*!
  \var SoSFVec2f SoVRMLPlaneSensor::minPosition
  The minimum position.
*/

/*!
  \var SoSFVec2f SoVRMLPlaneSensor::maxPosition
  The maximum position.
*/

/*!
  \var SoSFVec3f SoVRMLPlaneSensor::offset
  Current position. Initial value is (0, 0).
*/

/*!
  \var SoSFVec3f SoVRMLPlaneSensor::translation_changed
  An eventOut that is sent during interaction.
*/

#include <Inventor/VRMLnodes/SoVRMLPlaneSensor.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/projectors/SbPlaneProjector.h>

SO_NODE_SOURCE(SoVRMLPlaneSensor);

// Doc in parent
void
SoVRMLPlaneSensor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLPlaneSensor, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLPlaneSensor::SoVRMLPlaneSensor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLPlaneSensor);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(minPosition, (0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(maxPosition, (-1.0f, -1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(offset, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EVENT_OUT(translation_changed);

  this->planeproj = new SbPlaneProjector;
}

/*!
  Destructor.
*/
SoVRMLPlaneSensor::~SoVRMLPlaneSensor()
{
  delete this->planeproj;
}

// doc in parent
SbBool
SoVRMLPlaneSensor::dragStart(void)
{
  SbVec3f hitpt = this->getLocalStartingPoint();
  this->planeproj->setPlane(SbPlane(SbVec3f(0.0f, 0.0f, 1.0f), hitpt));
  this->translation_changed = this->offset.getValue();

  return TRUE;
}

// doc in parent
void
SoVRMLPlaneSensor::drag(void)
{
  this->planeproj->setViewVolume(this->getViewVolume());
  this->planeproj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projpt = this->planeproj->project(this->getNormalizedLocaterPosition());

  SbVec3f startpt = this->getLocalStartingPoint();
  SbVec3f motion = projpt - startpt;

  SbVec2f maxp = this->maxPosition.getValue();
  SbVec2f minp = this->minPosition.getValue();

  motion += this->offset.getValue();

  if (minp[0] <= maxp[0]) motion[0] = SbClamp(motion[0], minp[0], maxp[0]);
  if (minp[1] <= maxp[1]) motion[1] = SbClamp(motion[1], minp[1], maxp[1]);

  this->translation_changed = motion;
}

// doc in parent
void
SoVRMLPlaneSensor::dragFinish(void)
{
  if (this->autoOffset.getValue()) {
    this->offset = this->translation_changed;
  }
}
