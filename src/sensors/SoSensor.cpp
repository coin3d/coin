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
  \class SoSensor SoSensor.h Inventor/sensors/SoSensor.h
  \brief The SoSensor class is the abstract base class which the other
  sensor classes are built upon.
  \ingroup sensors

  TODO: doc on what sensors are

  \sa SoSensorManager
  \sa SoTimerQueueSensor SoDelayQueueSensor
  \sa SoTimerSensor SoAlarmSensor
  \sa SoIdleSensor SoDataSensor SoOneShotSensor
  \sa SoPathSensor SoFieldSensor SoNodeSensor
 */

#include <Inventor/sensors/SoSensor.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <stdlib.h> // to get NULL definition
#include <assert.h>

/*!
  \var SoSensorCB * SoSensor::func
  Function to be called when a sensor triggers.
*/
/*!
  \var void * SoSensor::funcData
  Data passed to the callback function.
*/

/*!
  \fn virtual void SoSensor::schedule(void) = 0
  Put the sensor in a queue to be triggered at a later time.
  \sa unschedule(), isScheduled()
 */
/*!
  \fn virtual void SoSensor::unschedule(void) = 0
  Remove sensor from queue. The sensor will not be triggered again unless
  it is re-scheduled.
  \sa schedule(), isScheduled()
 */
/*!
  \fn virtual SbBool SoSensor::isScheduled(void) const = 0
  Check if this sensor is scheduled for triggering.
  \sa schedule(), unschedule()
 */
/*!
  \fn virtual SbBool SoSensor::isBefore(const SoSensor * s) const = 0
  Returns \a TRUE if this sensor should precede sensor \a s in its
  sensor queue.
 */


/*!
  Constructor.
 */
SoSensor::SoSensor(void)
  : func(NULL), funcData(NULL)
{
}

/*!
  Constructor taking as parameters the sensor callback function and the
  userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoSensor::SoSensor(SoSensorCB * func, void * data)
{
  this->setData(data);
  this->setFunction(func);
}

/*!
  Destructor.
 */
SoSensor::~SoSensor(void)
{
}

/*!
  Set the callback function pointer which will be used when the sensor is
  triggered.

  \sa getFunction(), setData()
 */
void
SoSensor::setFunction(SoSensorCB * callbackfunction)
{
  this->func = callbackfunction;
}

/*!
  Returns the callback function pointer.

  \sa setFunction()
 */
SoSensorCB *
SoSensor::getFunction(void) const
{
  return this->func;
}

/*!
  Set the user-supplied data pointer which will be used as one of the
  arguments to the sensor callback function.

  \sa getData(), setFunction()
 */
void
SoSensor::setData(void * callbackData)
{
  this->funcData = callbackData;
}

/*!
  Returns the user-supplied callback function data pointer.

  \sa setData()
 */
void *
SoSensor::getData(void) const
{
  return this->funcData;
}

/*!
  \internal

  First unschedule and then trigger the callback function.
 */
void
SoSensor::trigger(void)
{
  this->unschedule();
  if(this->func) this->func(this->funcData, this);
}

/*!
  FIXME: write doc
 */
void
SoSensor::setNextInQueue(SoSensor * /* next */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
SoSensor *
SoSensor::getNextInQueue(void) const
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write doc
 */
void
SoSensor::initClass(void)
{
  COIN_STUB();
}
