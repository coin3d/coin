/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoSensor SoSensor.h Inventor/sensors/SoSensor.h
  \brief The SoSensor class is the abstract base class for all sensors.
  \ingroup sensors

  Sensors is a mechanism in Coin for scheduling jobs to be run upon
  specific events. The events in question could be particular points
  in time, or changes to entities in the scene graph.

  See documentation of subclasses for insight into exactly for what
  types of events we can trigger sensor callbacks.

  For most tasks where application programmers can use sensors, it is
  also possible to achieve the same results by using engines. There
  are a few important differences between sensors and engines, though:

  Engines are considered part of the scene graph, and is written to
  file upon SoWriteAction export operations. Sensors, on the other
  hand, are not included in export operations.

  Engines basically connects fields (and comes with a lot of builtin
  functionality for combining and converting inputs and outputs), you
  just decide which engine you want, connect inputs and output and
  forgets about it. Sensors are a lot more flexible in what you can do
  when they trigger, as control is transfered internally from Coin to
  your registered callback functions.

  \sa SoSensorManager, SoEngine
 */

#include <Inventor/sensors/SoSensor.h>
#include <stdlib.h> // NULL
#include <coindefs.h> // COIN_OBSOLETED()

/*!
  \var SoSensorCB * SoSensor::func
  Function to be called when a sensor triggers.
*/
/*!
  \var void * SoSensor::funcData
  Data passed to the callback function.
*/

/*!
  \fn virtual void SoSensor::schedule(void)

  Put the sensor in a queue to be triggered at a later time.

  \sa unschedule(), isScheduled()
 */
/*!
  \fn virtual void SoSensor::unschedule(void)

  Remove sensor from queue. The sensor will not be triggered unless it
  is later rescheduled.

  \sa schedule(), isScheduled()
 */
/*!
  \fn virtual SbBool SoSensor::isScheduled(void) const

  Check if this sensor is scheduled for triggering.

  \sa schedule(), unschedule()
 */
/*!
  \fn virtual SbBool SoSensor::isBefore(const SoSensor * s) const

  Returns \c TRUE if this sensor should precede sensor \a s in its
  sensor queue.
 */

/*!
  \typedef void SoSensorCB(void * data, SoSensor * sensor)

  Sensor callback functions must have this signature to be valid for
  registering with SoSensor.
*/


/*!
  Constructor.
 */
SoSensor::SoSensor(void)
  : func(NULL), funcData(NULL)
{
}

/*!
  Constructor taking as parameters the sensor callback function \a
  func and the user \a data pointer which will be passed the callback.

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
  Set the callback function pointer which will be used when the sensor
  is triggered.

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
  Set the user-supplied data pointer which will be used as the first
  argument to the sensor callback function.

  \sa getData(), setFunction()
 */
void
SoSensor::setData(void * callbackdata)
{
  this->funcData = callbackdata;
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
  Trigger the sensor's callback function.
 */
void
SoSensor::trigger(void)
{
  if (this->func) this->func(this->funcData, this);
}

/*!
  \COININTERNAL
  Open Inventor function not implemented in Coin.
 */
void
SoSensor::setNextInQueue(SoSensor * next)
{
  COIN_OBSOLETED();
}

/*!
  \COININTERNAL
  Open Inventor function not implemented in Coin.
 */
SoSensor *
SoSensor::getNextInQueue(void) const
{
  COIN_OBSOLETED();
  return NULL;
}

/*!
  Sets up initialization for static data for the sensors. Called by
  SoDB::init().
 */
void
SoSensor::initClass(void)
{
}
