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
  \class SoTimerSensor SoTimerSensor.h Inventor/sensors/SoTimerSensor.h
  \brief The SoTimerSensor class is a sensor which will trigger repeatedly at
  certain intervals.
  \ingroup sensors

  TODO: doc
 */

#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SoDB.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#define DEBUG_TIMERSENSOR_TRACE 0

/*!
  Constructor.
 */
SoTimerSensor::SoTimerSensor(void)
{
  this->interval.setValue(1.0f/30.0f);
  this->notsetBaseTime = TRUE;
  this->istriggering = FALSE;
  this->wasunscheduled = FALSE;
}

/*!
  Constructor taking as parameters the sensor callback function and the
  userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoTimerSensor::SoTimerSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
  this->interval.setValue(1.0f/30.0f);
  this->notsetBaseTime = TRUE;
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
  Set the base trigger time.

  \sa getBaseTime()
 */
void
SoTimerSensor::setBaseTime(const SbTime & base)
{
  this->base = base;
  this->notsetBaseTime = FALSE;
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
  if (this->notsetBaseTime) {
    this->base = schedtime;
    this->setTriggerTime(this->base + this->interval);
#if DEBUG_TIMERSENSOR_TRACE // debug
    SoDebugError::postInfo("SoTimerSensor::reschedule",
                           "(notsetBaseTime) base: %lf, new trigger time: %lf",
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
  Overrides the virtual unschedule() method to handle unschedule() calls
  during triggering.
 */
void
SoTimerSensor::unschedule(void)
{
#if DEBUG_TIMERSENSOR_TRACE // debug
  SoDebugError::postInfo("SoTimerSensor::unschedule", "");
#endif // debug

#if COIN_DEBUG
  if (!this->isScheduled()) {
    SoDebugError::postWarning("SoTimerSensor::unschedule",
                              "not scheduled!");
    return;
  }
#endif // COIN_DEBUG

  if (this->istriggering) this->wasunscheduled = TRUE;
  else inherited::unschedule();
}

/*!
  Overides the virtual trigger() method to be able to reschedule ourselves
  after we've been triggered.
*/
void
SoTimerSensor::trigger(void)
{
#if DEBUG_TIMERSENSOR_TRACE // debug
  SoDebugError::postInfo("SoTimerSensor::trigger", "");
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
}

/*!
  Need to overload this method in SoTimerSensor, otherwise we can't return
  the correct schedule status during triggering.
 */
SbBool
SoTimerSensor::isScheduled(void)
{
  if (this->istriggering) return this->wasunscheduled ? FALSE : TRUE;
  return inherited::isScheduled();
}
