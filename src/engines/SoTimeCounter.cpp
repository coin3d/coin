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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTimeCounter SoTimeCounter.h Inventor/engines/SoTimeCounter.h
  \brief The SoTimeCounter class is an integer counter engine.
  \ingroup engines

  The SoTimeCounter engine is by default connected to the realTime
  global field, and does stepwise updates on the SoTimeCounter::output
  field according to the field settings.
*/

/*!
  \var SoSFTime SoTimeCounter::timeIn
  Running time. Connected to the \e realTime field by default.
*/

/*!
  \var SoSFShort SoTimeCounter::min
  Minimum counter value.
*/

/*!
  \var SoSFShort SoTimeCounter::max
  Maximum counter value.
*/

/*!
  \var SoSFShort SoTimeCounter::step
  Counter step size.
*/

/*!
  \var SoSFBool SoTimeCounter::on
  Set to \c FALSE to pause the counter.
*/

/*!
  \var SoSFFloat SoTimeCounter::frequency
  Number of complete cycles from the min value to the max value per
  second.
*/

/*!
  \var SoMFFloat SoTimeCounter::duty
  Used to weight step times. Supply one weight value per step.
*/

/*!
  \var SoSFShort SoTimeCounter::reset

  Manually set the counter to some value. If SoTimeCounter::reset is
  set below the SoTimeCounter::min value or above the
  SoTimeCounter::max value it will be clamped to the closest boundary
  value.
*/

/*!
  \var SoSFTrigger SoTimeCounter::syncIn
  Restart counter at the minimum value.
*/

/*!
  \var SoEngineOutput SoTimeCounter::output
  (SoSFShort) The counter value.
*/

/*!
  \var SoEngineOutput SoTimeCounter::syncOut
  (SoSFTrigger) Triggers every cycle start.
*/

#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoSubEngineP.h>
#include <coindefs.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_ENGINE_SOURCE(SoTimeCounter);

/*!
  Default constructor.
*/
SoTimeCounter::SoTimeCounter(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoTimeCounter);

  SO_ENGINE_ADD_INPUT(timeIn, (SbTime::zero()));
  SO_ENGINE_ADD_INPUT(min, (0));
  SO_ENGINE_ADD_INPUT(max, (1));
  SO_ENGINE_ADD_INPUT(step, (1));
  SO_ENGINE_ADD_INPUT(on, (TRUE));
  SO_ENGINE_ADD_INPUT(frequency, (1.0f));
  SO_ENGINE_ADD_INPUT(duty, (1.0f));
  SO_ENGINE_ADD_INPUT(reset, (0));
  SO_ENGINE_ADD_INPUT(syncIn, ());

  SO_ENGINE_ADD_OUTPUT(output, SoSFShort);
  SO_ENGINE_ADD_OUTPUT(syncOut, SoSFTrigger);

  this->syncOut.enable(FALSE);

  SoField * realtime = SoDB::getGlobalField("realTime");
  this->starttime = ((SoSFTime *)realtime)->getValue().getValue();
  this->firstoutputenable = TRUE;
  this->outputvalue = 0;
  this->cyclelen = 1.0;
  this->numsteps = 2;
  this->stepnum = 0;
  this->prevon = TRUE;
  this->ispaused = FALSE;

  this->timeIn.connectFrom(realtime);
}

// overloaded from parent
void
SoTimeCounter::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoTimeCounter);
}


// private destructor
SoTimeCounter::~SoTimeCounter()
{
}

// Overloaded to not write connection to realTime global field.
void
SoTimeCounter::writeInstance(SoOutput * out)
{
  // Disconnect from realTime field.
  SoField * connectfield = NULL;
  SbBool connectfromrealTime =
    this->timeIn.getConnectedField(connectfield) &&
    connectfield == SoDB::getGlobalField("realTime");
  SbBool defaultflag = this->timeIn.isDefault();
  if (connectfromrealTime) {
    this->timeIn.disconnect();
    this->timeIn.setDefault(TRUE);
  }

  inherited::writeInstance(out);

  // Re-connect to realTime field.
  if (connectfromrealTime) {
    this->timeIn.connectFrom(connectfield);
    this->timeIn.setDefault(defaultflag);
  }
}

// doc in parent
void
SoTimeCounter::evaluate(void)
{
  if (this->output.isEnabled()) {
    SO_ENGINE_OUTPUT(output, SoSFShort, setValue(this->outputvalue));
    // disable until new value is available in outputvalue
    this->output.enable(FALSE);
  }
}

// doc in parent
void
SoTimeCounter::inputChanged(SoField * which)
{
#if COIN_DEBUG && 0 // debug
  SbName fieldname;
  which->getContainer()->getFieldName(which, fieldname);
  SoDebugError::postInfo("SoTimeCounter::inputChanged", "field=%p, \"%s\"",
                         which, fieldname.getString());
#endif // debug

  if (which == &this->timeIn) {
    if (this->ispaused) return;

    double currtime = this->timeIn.getValue().getValue();
    double difftime = currtime - this->starttime;

    if (difftime > this->cyclelen) {
      this->syncOut.enable(TRUE);
      SO_ENGINE_OUTPUT(syncOut, SoSFTrigger, setValue());
      this->syncOut.enable(TRUE);
      double num = difftime / this->cyclelen;
      this->starttime += this->cyclelen * floor(num);
      difftime = currtime - this->starttime;
    }
    this->setOutputValue(this->findOutputValue(difftime));
  }
  else if (which == &this->on) {
    if (this->on.getValue() != this->prevon) {
      if (this->on.getValue() == FALSE) {
        this->ispaused = TRUE;
        this->output.enable(FALSE);
        this->pausetimeincycle =
          this->timeIn.getValue().getValue() - this->starttime;
        this->prevon = FALSE;
      }
      else {
        this->starttime =
          this->timeIn.getValue().getValue() - this->pausetimeincycle;
        this->ispaused = FALSE;
        this->prevon = TRUE;
        this->inputChanged(&this->timeIn); // fake it
      }
    }
  }
  else if (which == &this->frequency) {
    this->cyclelen = 1.0 / this->frequency.getValue();
    this->calcDutySteps();
  }

  else if (which == &this->duty) {
    this->calcDutySteps();
  }
  else if (which == &this->reset) {
    short minval = this->min.getValue();
    short val = SbClamp(this->reset.getValue(),
                      minval,
                      this->max.getValue());
    short offset = val - minval;
    short stepval = this->step.getValue();
    if ((offset % stepval) != 0) {
      val = minval + (offset / stepval) * stepval;
    }
    this->calcStarttime(val);
    this->setOutputValue(val);
  }
  else if (which == &this->syncIn) {
    this->starttime = this->timeIn.getValue().getValue();
    this->setOutputValue(this->min.getValue());
  }
  else if (which == &this->max) {
    if (this->max.getValue() < this->min.getValue())
      this->min.setValue(this->max.getValue());
    this->calcNumSteps();
    this->calcDutySteps();
    if (this->max.getValue() < this->outputvalue) {
      this->starttime = this->timeIn.getValue().getValue();
      this->setOutputValue(this->min.getValue());
    }
  }
  else if (which == &this->min) {
    if (this->max.getValue() < this->min.getValue())
      this->max.setValue(this->min.getValue());
    this->calcNumSteps();
    this->calcDutySteps();
    short value = this->min.getValue() + this->step.getValue() * this->stepnum;
    if (value > this->max.getValue()) {
      this->starttime = this->timeIn.getValue().getValue();
      value = this->min.getValue();
    }
    this->setOutputValue(value);
  }
  else if (which == &this->step) {
    this->calcNumSteps();
    this->calcDutySteps();
    short value = this->min.getValue() + this->step.getValue() * this->stepnum;
    if (value > this->max.getValue()) {
      this->starttime = this->timeIn.getValue().getValue();
      value = this->min.getValue();
    }
    this->setOutputValue(value);
  }
}


// calculates # of steps in cycle
void
SoTimeCounter::calcNumSteps(void)
{
  short stepval = this->step.getValue();
  this->numsteps = (this->max.getValue() - this->min.getValue()) / stepval + 1;
}

// recalculate duty steps.
void
SoTimeCounter::calcDutySteps(void)
{
  if (this->duty.getNum() == this->numsteps) {
    int i;
    double dutysum = 0.0;
    for (i = 0; i < numsteps; i++) {
      dutysum += (double)this->duty[i];
    }
    double currsum = 0.0;
    dutylimits.truncate(0);
    for (i = 0; i < numsteps; i++) {
      currsum += (double) this->duty[i];
      this->dutylimits.append(currsum/dutysum * this->cyclelen);
    }
  }
  else {
    // ignore duty values
    dutylimits.truncate(0);
  }
}

// calculates current output value based on time in cycle
short
SoTimeCounter::findOutputValue(double timeincycle) const
{
  assert(timeincycle <= cyclelen);

  short val;
  short minval = this->min.getValue();
  short maxval = this->max.getValue();
  short stepval = this->step.getValue();

  if (this->dutylimits.getLength()) {
    // FIXME: use binary search if > 64 or something values
    int i = 0;
    for (i = 0; i < this->numsteps; i++) {
      if (timeincycle <= this->dutylimits[i]) break;
    }
    val = minval + i * stepval;
  }
  else {
    double steptime = this->cyclelen / this->numsteps;
    val = minval + (short)(timeincycle / steptime) * stepval;
    if (val > maxval) val = maxval;
  }
  assert(val >= minval && val <= maxval);
  return val;
}

// sets current value to output. Enables output if new value
void
SoTimeCounter::setOutputValue(short value)
{
  if (value != this->outputvalue) {
    if (value == this->outputvalue + this->step.getValue()) { // common case
      this->stepnum++;
    }
    else { // either reset, wrap-around or a delay somewhere
      short offset = value - this->min.getValue();
      this->stepnum = offset / this->step.getValue();
    }
    this->outputvalue = value;
    this->output.enable(TRUE);
  }

  // FIXME: is this really necessary? Can't we just set
  // this->output.enable(TRUE) in the constructor instead? 20000907 mortene.
  if (this->firstoutputenable) {
    this->firstoutputenable = FALSE;
    this->output.enable(TRUE);
  }
}

// calculates cycle starttime based on counter value and timeIn.
// also sets stepnum. value must be in legal range
void
SoTimeCounter::calcStarttime(short value)
{
  this->stepnum = (value - this->min.getValue()) / this->step.getValue();
  if (this->dutylimits.getLength()) {
   this->starttime = this->timeIn.getValue().getValue()
     - this->dutylimits[this->stepnum];
  }
  else {
    this->starttime = this->timeIn.getValue().getValue() -
      double(this->stepnum) * this->cyclelen / double(this->numsteps);
  }
}
