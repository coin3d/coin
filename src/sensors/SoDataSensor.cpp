/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoDataSensor SoDataSensor.h Inventor/sensors/SoDataSensor.h
  \brief The SoDataSensor class is the abstract base class for sensors
  monitoring changes in a scene graph.
  \ingroup sensors

  TODO: doc
 */

#include <Inventor/sensors/SoDataSensor.h>
#include <stdlib.h> // NULL
#include <assert.h>

/*!
  \fn void SoDataSensor::dyingReference(void) = 0
  FIXME: write doc
*/


/*!
  Constructor.
 */
SoDataSensor::SoDataSensor(void)
{
  this->cbfunc = NULL;
  this->cbdata = NULL;
  this->findpath = FALSE;
}

/*!
  Constructor taking as parameters the sensor callback function and the
  userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoDataSensor::SoDataSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
  this->cbfunc = NULL;
  this->cbdata = NULL;
  this->findpath = FALSE;
}

/*!
  Destructor.
*/
SoDataSensor::~SoDataSensor(void)
{
}

/*!
  If an object monitored by a data sensor is deleted, the given callback
  function will be called with the given userdata.
 */
void
SoDataSensor::setDeleteCallback(SoSensorCB * function, void * data)
{
  this->cbfunc = function;
  this->cbdata = data;
}

/*!
  FIXME: write doc
 */
SoNode *
SoDataSensor::getTriggerNode(void) const
{
  // TODO: implement
  assert(0);
  return NULL;
}

/*!
  FIXME: write doc
 */
SoField *
SoDataSensor::getTriggerField(void) const
{
  // TODO: implement
  assert(0);
  return NULL;
}

/*!
  FIXME: write doc
 */
SoPath *
SoDataSensor::getTriggerPath(void) const
{
  // TODO: implement
  assert(0);
  return NULL;
}

/*!
  This flag indicates whether or not the path should be queried whenever
  a node triggers the data sensor.

  This flag is provided because finding a node path through a scene graph
  is an expensive operation.

  \sa getTriggerPathFlag(), getTriggerPath()
 */
void
SoDataSensor::setTriggerPathFlag(SbBool flag)
{
  this->findpath = flag;
}

/*!
  Returns whether or not any node induced trigger operations will make
  the sensor find the path of the node which caused it.

  \sa setTriggerPathFlag(), getTriggerPath()
 */
SbBool
SoDataSensor::getTriggerPathFlag(void) const
{
  return this->findpath;
}

/*!
  FIXME: write doc
 */
void
SoDataSensor::notify(SoNotList * /* list */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoDataSensor::invokeDeleteCallback(void)
{
  assert(0 && "FIXME: not implemented");
}
