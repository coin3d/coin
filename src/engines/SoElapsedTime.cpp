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

  FIXME: doc
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
  FIXME
*/
/*!
  \var SoSFFloat SoElapsedTime::speed
  FIXME
*/
/*!
  \var SoSFBool SoElapsedTime::on
  FIXME
*/
/*!
  \var SoSFBool SoElapsedTime::pause
  FIXME
*/
/*!
  \var SoSFTrigger SoElapsedTime::reset
  FIXME
*/

/*!
  SoEngineOutput SoElapsedTime::timeOut
  (SoSFTime) FIXME
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

  SO_ENGINE_ADD_INPUT(timeIn,(SbTime::zero()));
  SO_ENGINE_ADD_INPUT(speed,(1));
  SO_ENGINE_ADD_INPUT(on,(TRUE));
  SO_ENGINE_ADD_INPUT(pause,(FALSE));
  SO_ENGINE_ADD_INPUT(reset, ());

  SO_ENGINE_ADD_OUTPUT(timeOut,SoSFTime);

  SoField * realtime = SoDB::getGlobalField("realTime");
  this->timeIn.connectFrom(realtime);

  this->currtime = SbTime::zero();
  this->lasttime = ((SoSFTime *)realtime)->getValue();
  this->status = SoElapsedTime::RUNNING;
  this->firsttime = TRUE;
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
  //  if (this->firsttime) return;
  if (this->status == SoElapsedTime::STOPPED) {
    SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(this->currtime));
  }
  else {
    SbTime now = this->timeIn.getValue();
    this->currtime += (now-this->lasttime) * this->speed.getValue();
    this->lasttime = now;
    if (this->status == SoElapsedTime::PAUSED) {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(this->pausetime));
    }
    else {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(this->currtime));
    }
  }
}

// overloaded from parent
void
SoElapsedTime::inputChanged(SoField * which)
{
  if (which == &this->timeIn) return;

//   if (which == &this->timeIn && this->firsttime) {
//     this->lasttime = this->timeIn.getValue();
//     this->firsttime = FALSE;
//     printf("!!!!!!!\n");
//   }
  else if (which == &this->reset) {
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
