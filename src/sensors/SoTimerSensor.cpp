/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTimerSensor SoTimerSensor.h Inventor/sensors/SoTimerSensor.h
  \brief The SoTimerSensor class is a sensor which will trigger at given intervals.
  \ingroup sensors

  Use sensors of this class when you want a job repeated at a certain
  interval, without explicitly needing to reschedule the sensor
  (i.e. SoTimerSensor automatically re-schedules itself after it has
  been triggered).

  SoTimerSensor instances is commonly used to trigger animation
  updates at a constant framerate.
*/

#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SoDB.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#define DEBUG_TIMERSENSOR_TRACE 0

/*!
  Default constructor. Sets up an interval of 1/30th of a second.
 */
SoTimerSensor::SoTimerSensor(void)
{
  this->interval.setValue(1.0f/30.0f);
  this->setbasetime = FALSE;
  this->istriggering = FALSE;
  this->wasunscheduled = FALSE;
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoTimerSensor::SoTimerSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
  this->interval.setValue(1.0f/30.0f);
  this->setbasetime = FALSE;
  this->istriggering = FALSE;
  this->wasunscheduled = FALSE;
}

/*!
  Destructor.
*/
SoTimerSensor::~SoTimerSensor(void)
{
}

/*!
  Set the \a base trigger time.

  If you use this method, the trigger times will be on intervals from
  the given value.

  Without an explicitly set base time, the next trigger invocation
  after a trigger has happened will be on the current time plus the
  interval time.

  \sa getBaseTime()
 */
void
SoTimerSensor::setBaseTime(const SbTime & base)
{
  this->base = base;
  this->setbasetime = TRUE;
  // FIXME: reschedule? 19990425 mortene.
}

/*!
  Returns the base trigger time.

  \sa setBaseTime()
 */
const SbTime &
SoTimerSensor::getBaseTime(void) const
{
  return this->base;
}

/*!
  Sets the time interval between each time the sensor triggers.

  \sa getInterval()
 */
void
SoTimerSensor::setInterval(const SbTime & interval)
{
  this->interval = interval;
  // FIXME: reschedule? 19990425 mortene.
}

/*!
  Returns the timer trigger interval.

  \sa setInterval()
 */
const SbTime &
SoTimerSensor::getInterval(void) const
{
  return this->interval;
}

/*!
  Set new trigger time based on the given schedule time.
 */
void
SoTimerSensor::reschedule(const SbTime & schedtime)
{
  if (!this->setbasetime) {
    this->base = schedtime;
    this->setTriggerTime(this->base + this->interval);
#if DEBUG_TIMERSENSOR_TRACE // debug
    SoDebugError::postInfo("SoTimerSensor::reschedule",
                           "(setbasetime) base: %lf, new trigger time: %lf",
                           this->base.getValue(), this->getTriggerTime());
#endif // debug
  }
  else {
    int intervals = (int)((schedtime - this->base)/this->interval);
    this->setTriggerTime(this->base + (intervals+1) * this->interval);
#if DEBUG_TIMERSENSOR_TRACE // debug
    SoDebugError::postInfo("SoTimerSensor::reschedule",
                           "base: %lf, new trigger time: %lf",
                           this->base.getValue(), this->getTriggerTime());
#endif // debug
  }
}

/*!
  Overrides the virtual schedule() method to be able to set up the
  base time, if this was not done by the user.

  If no base time was set, base time will then equal the current time.

  \sa unschedule(), isScheduled()
 */
void
SoTimerSensor::schedule(void)
{
#if DEBUG_TIMERSENSOR_TRACE // debug
  SoDebugError::postInfo("SoTimerSensor::schedule", "");
#endif // debug

#if COIN_DEBUG
  if (this->isScheduled()) {
    SoDebugError::postWarning("SoTimerSensor::schedule",
                              "was already scheduled!");
    return;
  }
#endif // COIN_DEBUG

  this->reschedule(SbTime::getTimeOfDay());
  if (this->istriggering) this->wasunscheduled = FALSE;
  else inherited::schedule();
}

/*!
  Overrides the virtual unschedule() method to handle unschedule()
  calls during triggering.
 */
void
SoTimerSensor::unschedule(void)
{
#if DEBUG_TIMERSENSOR_TRACE // debug
  SoDebugError::postInfo("SoTimerSensor::unschedule", "%p start", this);
#endif // debug

#if COIN_DEBUG
  if (!this->isScheduled()) {
    SoDebugError::postWarning("SoTimerSensor::unschedule",
                              "%p not scheduled (istriggering=%s)",
                              this, this->istriggering ? "TRUE" : "FALSE");
    return;
  }
#endif // COIN_DEBUG

  if (this->istriggering) this->wasunscheduled = TRUE;
  else inherited::unschedule();

#if DEBUG_TIMERSENSOR_TRACE // debug
  SoDebugError::postInfo("SoTimerSensor::unschedule", "%p done", this);
#endif // debug
}

/*!
  Overides the virtual trigger() method to be able to reschedule
  ourselves after we've been triggered.
*/
void
SoTimerSensor::trigger(void)
{
#if DEBUG_TIMERSENSOR_TRACE // debug
  SoDebugError::postInfo("SoTimerSensor::trigger", "%p start", this);
#endif // debug

  this->istriggering = TRUE;
  this->wasunscheduled = FALSE;

  if (this->getFunction()) this->getFunction()(this->getData(), this);

  if (this->wasunscheduled) {
    SoDB::getSensorManager()->removeRescheduledTimer(this);
    this->scheduled = FALSE;
  }
  else {
    SoDB::getSensorManager()->rescheduleTimer(this);
  }

  this->istriggering = FALSE;
  this->wasunscheduled = FALSE;

#if DEBUG_TIMERSENSOR_TRACE // debug
  SoDebugError::postInfo("SoTimerSensor::trigger", "%p done", this);
#endif // debug
}

/*!
  Need to overload this method in SoTimerSensor, otherwise we can't
  return the correct schedule status during triggering.
 */
SbBool
SoTimerSensor::isScheduled(void) const
{
  if (this->istriggering) return this->wasunscheduled ? FALSE : TRUE;
  return inherited::isScheduled();
}
