/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLSensor SoVRMLSensor.h Inventor/VRMLnodes/SoVRMLSensor.h
  \brief The SoVRMLSensor class is an abstract superclass for VRML sensors.
*/

#include <Inventor/VRMLnodes/SoVRMLSensor.h>

#include <Inventor/VRMLnodes/SoVRMLMacros.h>

#include "nodes/SoSubNodeP.h"

SO_NODE_ABSTRACT_SOURCE(SoVRMLSensor);

void
SoVRMLSensor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLSensor, SO_VRML97_NODE_TYPE);
}

SoVRMLSensor::SoVRMLSensor(void)
{
  SO_NODE_CONSTRUCTOR(SoVRMLSensor);

  SO_VRMLNODE_ADD_EVENT_OUT(isActive);
  // DragSensor reads from this field. Initialize it.
  this->isActive = FALSE;
  SO_VRMLNODE_ADD_EXPOSED_FIELD(enabled, (TRUE));
}

SoVRMLSensor::~SoVRMLSensor()
{
}

#endif // HAVE_VRML97
