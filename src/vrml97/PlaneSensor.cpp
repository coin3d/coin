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
