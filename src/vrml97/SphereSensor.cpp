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
