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
  \class SoOneShot SoOneShot.h Inventor/engines/SoOneShot.h
  \brief The SoOneShot class is a timer that runs for a configurable time and then stops.
  \ingroup engines
*/

#include <Inventor/engines/SoOneShot.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/engines/SoSubEngineP.h>

/*!
  \var SoSFTime SoOneShot::timeIn

  Input time source. Connected by default to the realTime global
  field, but the application programmer is free to change this to any
  other time source.
*/
/*!
  \var SoSFTime SoOneShot::duration

  The time the engine should run for when it is triggered. Defaults to
  1 second.
*/
/*!
  \var SoSFTrigger SoOneShot::trigger

  A pulse on this input field starts the engine.
*/
/*!
  \var SoSFBitMask SoOneShot::flags

  Control flags.

  There are two flags available: \c RETRIGGERABLE will cause the
  engine to restart at 0 if the SoOneShot::trigger field is activated
  during a run (otherwise the trigger is ignored during runs).

  \c HOLD_FINAL will make the engine outputs keep their values after a
  run, instead of resetting them.
*/
/*!
  \var SoSFBool SoOneShot::disable

  Set to \c TRUE to disable the engine completely.
*/
/*!
  \var SoEngineOutput SoOneShot::timeOut

  (SoSFTime) Output time. Will run from 0 to the value of
  SoOneShot::duration.
*/
/*!
  \var SoEngineOutput SoOneShot::ramp
  (SoSFFloat) Will run from 0 to 1 during the active period.
*/
/*!
  \var SoEngineOutput SoOneShot::isActive
  (SoSFBool) \c TRUE while the engine is running, \c FALSE otherwise.
*/


SO_ENGINE_SOURCE(SoOneShot);

// overloaded from parent
void
SoOneShot::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoOneShot);
}

/*!
  Default constructor.
*/
SoOneShot::SoOneShot(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoOneShot);

  SO_ENGINE_ADD_INPUT(timeIn, (SbTime::zero()));
  SO_ENGINE_ADD_INPUT(duration, (SbTime(1.0)));
  SO_ENGINE_ADD_INPUT(trigger, ());
  SO_ENGINE_ADD_INPUT(flags, (0));
  SO_ENGINE_ADD_INPUT(disable, (FALSE));

  SO_ENGINE_ADD_OUTPUT(timeOut, SoSFTime);
  SO_ENGINE_ADD_OUTPUT(isActive, SoSFBool);
  SO_ENGINE_ADD_OUTPUT(ramp, SoSFFloat);

  SoField * realtime = SoDB::getGlobalField("realTime");
  this->timeIn.connectFrom(realtime);

  this->running = FALSE;
}

/*!
  Destructor is protected to avoid explicit destruction.
*/
SoOneShot::~SoOneShot()
{
}

// overloaded from parent
void
SoOneShot::evaluate(void)
{
  SbTime elapsed = this->timeIn.getValue() - this->starttime;
  SbTime durationVal = this->duration.getValue();

  SO_ENGINE_OUTPUT(isActive, SoSFBool, setValue(this->running));
  if (this->running) {
    if (elapsed < durationVal) {
      SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(elapsed));
      SO_ENGINE_OUTPUT(ramp, SoSFFloat,
                       setValue(float(elapsed.getValue())/float(durationVal.getValue())));
    }
    else {
      SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(durationVal));
      SO_ENGINE_OUTPUT(ramp, SoSFFloat, setValue(1.0));
      if (this->flags.getValue() & SoOneShot::HOLD_FINAL)
        this->holdduration = durationVal;
      this->running = FALSE;
    }
  }
  else {
    if (this->flags.getValue() & SoOneShot::HOLD_FINAL) {
      SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(this->holdduration));
      SO_ENGINE_OUTPUT(ramp, SoSFFloat, setValue(1.0));
    }
    else {
      SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(0.0));
      SO_ENGINE_OUTPUT(ramp, SoSFFloat, setValue(0.0));
    }
  }
}

//FIXME: messages (kintel 19990611)

// overloaded from parent
void
SoOneShot::inputChanged(SoField * which)
{
  if (which == &this->trigger) {
#if 0 // FIXME: doesn't compile. 19990620 mortene.
      fprintf(stdout, "(trigger): %x\n", this->flags.getValue());
#endif // disabled
    if ((!this->running ||
         this->flags.getValue() & SoOneShot::RETRIGGERABLE) &&
        !this->disable.getValue()) {
#if 0 // FIXME: doesn't compile. 19990620 mortene.
      fprintf(stdout, "XXX Retrigger XXX\n");
#endif // disabled
      this->starttime = this->timeIn.getValue();
      this->running = TRUE;
      SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(0.0));
      SO_ENGINE_OUTPUT(ramp, SoSFFloat, setValue(0.0));
    }
  }
  else if (which == &this->disable) {
    if (this->disable.getValue())
      this->running = FALSE;
    this->timeOut.enable(!this->disable.getValue());
    this->ramp.enable(!this->disable.getValue());
    this->isActive.enable(!this->disable.getValue());
  }
}
