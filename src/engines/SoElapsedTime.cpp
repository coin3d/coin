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
  \class SoElapsedTime SoElapsedTime.h Inventor/engines/SoElapsedTime.h
  \brief The SoElapsedTime class is a controllable time source engine.
  \ingroup engines
*/

#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoSubEngineP.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFTime SoElapsedTime::timeIn
  Input time value. Default connected to the global realTime field.
*/
/*!
  \var SoSFFloat SoElapsedTime::speed

  Decides how fast the timer should run, measured versus "real time"
  from the timeIn field. Default value is 1.0.

  A negative value makes the time go backward.
*/
/*!
  \var SoSFBool SoElapsedTime::on

  Shuts timer on and off. Will restart at the same position.
*/
/*!
  \var SoSFBool SoElapsedTime::pause
  
  Shuts timer on and off. Will restart at the position it would have
  been if not paused (i.e. doesn't "lose time").
*/
/*!
  \var SoSFTrigger SoElapsedTime::reset

  Restart timer at 0.
*/

/*!
  SoEngineOutput SoElapsedTime::timeOut
  (SoSFTime) Output time value. Starts at 0.
*/

SO_ENGINE_SOURCE(SoElapsedTime);

// overloaded from parent
void
SoElapsedTime::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoElapsedTime);
}

/*!
  Default constructor.
*/
SoElapsedTime::SoElapsedTime(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoElapsedTime);

  SO_ENGINE_ADD_INPUT(timeIn, (SbTime::zero()));
  SO_ENGINE_ADD_INPUT(speed, (1));
  SO_ENGINE_ADD_INPUT(on, (TRUE));
  SO_ENGINE_ADD_INPUT(pause, (FALSE));
  SO_ENGINE_ADD_INPUT(reset, ());

  SO_ENGINE_ADD_OUTPUT(timeOut, SoSFTime);

  SoField * realtime = SoDB::getGlobalField("realTime");
  this->timeIn.connectFrom(realtime);

  this->currtime = SbTime::zero();
  this->lasttime = ((SoSFTime *)realtime)->getValue();
  this->status = SoElapsedTime::RUNNING;
}

/*!
  Destructor.
*/
SoElapsedTime::~SoElapsedTime()
{
}

// overloaded from parent
void
SoElapsedTime::evaluate(void)
{
  if (this->status == SoElapsedTime::STOPPED) {
    SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(this->currtime));
  }
  else {
    SbTime now = this->timeIn.getValue();
    this->currtime += (now-this->lasttime) * this->speed.getValue();
    this->lasttime = now;
    if (this->status == SoElapsedTime::PAUSED) {
      SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(this->pausetime));
    }
    else {
      SO_ENGINE_OUTPUT(timeOut, SoSFTime, setValue(this->currtime));
    }
  }
}

// overloaded from parent
void
SoElapsedTime::inputChanged(SoField * which)
{
  if (which == &this->timeIn) return;

  if (which == &this->reset) {
    this->currtime = SbTime::zero();
    this->lasttime = this->timeIn.getValue();
  }
  else if (which == &this->pause) {
    if (this->pause.getValue() && this->status == SoElapsedTime::RUNNING) {
      this->status = SoElapsedTime::PAUSED;
      this->pausetime = this->currtime;
    }
    else if (!this->pause.getValue() && this->status == SoElapsedTime::PAUSED) {
      this->status = SoElapsedTime::RUNNING;
    }
  }
  else if (which == &this->on) {
    if (this->on.getValue() && this->status == SoElapsedTime::STOPPED) {
      this->status = SoElapsedTime::RUNNING;
      this->lasttime = this->timeIn.getValue();
    }
    else if (!this->on.getValue() && this->status != SoElapsedTime::STOPPED) {
      this->status = SoElapsedTime::STOPPED;
    }
  }
}
