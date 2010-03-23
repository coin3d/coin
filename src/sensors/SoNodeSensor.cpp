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

/*!
  \class SoNodeSensor SoNodeSensor.h Inventor/sensors/SoNodeSensor.h
  \brief The SoNodeSensor class detects changes to nodes.
  \ingroup sensors

  Attach a node to a sensor of this type to put it under surveillance,
  so you can act upon changes to the node.

  Any modification to the node's fields will trigger the sensor, as
  will changes to node's children (if any), including if nodes are
  added or removed as children below the node in the subgraph.

  An SoNodeSensor can also act for delete-callback purposes alone and
  does not need a regular notification-based callback.
*/

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/errors/SoDebugError.h>

/*!
  Default constructor.
*/
SoNodeSensor::SoNodeSensor(void)
{
  this->convict = NULL;
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
*/
SoNodeSensor::SoNodeSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
  this->convict = NULL;
}

/*!
  Destructor.
*/
SoNodeSensor::~SoNodeSensor(void)
{
  if (this->convict) this->detach();
}

/*!
  Attach sensor to a node. Whenever any data in the node (or its
  children, if it's an SoGroup or SoGroup derived node) changes, the
  sensor will be triggered and call the callback function.

  Attaching a node sensor to a node will \e not increase the node's
  reference count (and conversely, detach()'ing the node sensor will
  not decrease the reference count, either).

  When the attached node is deleted, the sensor will be automatically
  detached().

  \sa detach()
*/
void
SoNodeSensor::attach(SoNode * node)
{
  if (this->convict != NULL) {
    this->detach();
    SoDebugError::postWarning("SoNodeSensor::attach", 
                              "Attaching node sensor that is already attached.", 
                              this);
  }
  this->convict = node;
  node->addAuditor(this, SoNotRec::SENSOR);
}

/*!
  Detach sensor from node. As long as an SoNodeSensor is detached, it
  will never call its callback function.

  \sa attach()
*/
void
SoNodeSensor::detach(void)
{
  if (this->convict) this->convict->removeAuditor(this, SoNotRec::SENSOR);
  this->convict = NULL;
}

/*!
  Returns a pointer to the node connected to the sensor.

  \sa attach(), detach()
*/
SoNode *
SoNodeSensor::getAttachedNode(void) const
{
  return this->convict;
}

// Doc from superclass.
void
SoNodeSensor::dyingReference(void)
{
  SoNode * dyingnode = this->getAttachedNode();
  this->invokeDeleteCallback();
  if (dyingnode == this->getAttachedNode()) {
    this->detach();
  }
}
