/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
*/

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoNode.h>

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

  \sa detach()
*/
void
SoNodeSensor::attach(SoNode * node)
{
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
  this->detach();
}
