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
  \class SoVRMLCylinderSensor SoVRMLCylinderSensor.h Inventor/VRMLnodes/SoVRMLCylinderSensor
  \brief The SoVRMLCylinderSensor class maps pointer motion into rotations around the Y axis.
*/

/*!
  \var SoSFFloat SoVRMLCylinderSensor::diskAngle
  Angle where sensor starts to behave like a disk.
*/

/*!
  \var SoSFFloat SoVRMLCylinderSensor::maxAngle
  Maximum angle around Y-axis.
*/

/*!
  \var SoSFFloat SoVRMLCylinderSensor::minAngle
  Minimum angle around Y axis.
*/

/*!
  \var SoSFFloat SoVRMLCylinderSensor::offset
  Current rotation value. Initial value is 0.0.
*/

/*!
  \var SoSFRotation SoVRMLCylinderSensor::rotation_changed
  This eventOut is signaled during sensor interaction.
*/

#include <Inventor/VRMLnodes/SoVRMLCylinderSensor.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/projectors/SbCylinderPlaneProjector.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbCylinder.h>

SO_NODE_SOURCE(SoVRMLCylinderSensor);

// Doc in parent
void
SoVRMLCylinderSensor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLCylinderSensor, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLCylinderSensor::SoVRMLCylinderSensor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLCylinderSensor);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(diskAngle, (0.262f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(maxAngle, (-1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(minAngle, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(offset, (0.0f));
  SO_VRMLNODE_ADD_EVENT_OUT(rotation_changed);

  this->cylinderproj = new SbCylinderPlaneProjector();
}

/*!
  Destructor.
*/
SoVRMLCylinderSensor::~SoVRMLCylinderSensor()
{
  delete this->cylinderproj;
}

// Doc in parent
SbBool
SoVRMLCylinderSensor::dragStart(void)
{
  // FIXME: heed the minAngle/maxAngle parameters.
  SbVec3f hitpt = this->getLocalStartingPoint();
  SbLine line(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f));
  SbVec3f ptonline = line.getClosestPoint(hitpt);
  if (ptonline != hitpt) {
    this->cylinderproj->setCylinder(SbCylinder(line, (ptonline-hitpt).length()));
    this->rotation_changed = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), this->offset.getValue());
    return TRUE;
  }
  return FALSE;
}

// Doc in parent
void
SoVRMLCylinderSensor::drag(void)
{
  this->cylinderproj->setViewVolume(this->getViewVolume());
  this->cylinderproj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projpt = this->cylinderproj->project(this->getNormalizedLocaterPosition());
  SbVec3f startpt = this->getLocalStartingPoint();

  SbRotation rot = this->cylinderproj->getRotation(startpt, projpt);
  this->rotation_changed = rot * SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), this->offset.getValue());
}

// Doc in parent
void
SoVRMLCylinderSensor::dragFinish(void)
{
  if (this->autoOffset.getValue())
    this->offset = findAngle(this->rotation_changed.getValue());
}

//
// Private method that finds the angle around the Y axis.
//
float
SoVRMLCylinderSensor::findAngle(const SbRotation & rot)
{
  SbVec3f axis;
  float angle;
  rot.getValue(axis, angle);
  if (axis[1] >= 0.0f) return angle;
  return -angle;
}
