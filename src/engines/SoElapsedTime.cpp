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
  \class SoElapsedTime SoElapsedTime.h Inventor/engines/SoElapsedTime.h
  \brief The SoElapsedTime class is a controllable time source engine.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>

SO_ENGINE_SOURCE(SoElapsedTime);

/*!
  Default constructor.
*/
SoElapsedTime::SoElapsedTime()
{
  SO_ENGINE_CONSTRUCTOR(SoElapsedTime);

  SO_ENGINE_ADD_INPUT(timeIn,(SbTime::zero()));
  SO_ENGINE_ADD_INPUT(speed,(1));
  SO_ENGINE_ADD_INPUT(on,(TRUE));
  SO_ENGINE_ADD_INPUT(pause,(FALSE));
  SO_ENGINE_ADD_INPUT(reset, ());

  SO_ENGINE_ADD_OUTPUT(timeOut,SoSFTime);

  SoField *realtime=SoDB::getGlobalField("realTime");
  this->timeIn.connectFrom(realtime);

  this->currTime=SbTime::zero();
  this->lastTime=((SoSFTime *)realtime)->getValue();
  this->status=SoElapsedTime::RUNNING;
  this->firstTime=TRUE;
}

// overloaded from parent
void
SoElapsedTime::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoElapsedTime);
}


//
// private members
//
SoElapsedTime::~SoElapsedTime()
{
}

// overloaded from parent
void
SoElapsedTime::evaluate()
{
  //  if (this->firstTime) return;
  if (this->status==SoElapsedTime::STOPPED) {
    SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(this->currTime));
  }
  else {
    SbTime now=this->timeIn.getValue();
    this->currTime+=(now-this->lastTime)*this->speed.getValue();
    this->lastTime=now;
    if (this->status==SoElapsedTime::PAUSED) {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(this->pauseTime));
    }
    else {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(this->currTime));
    }
  }
}

// overloaded from parent
void
SoElapsedTime::inputChanged(SoField *which)
{
  if (which==&this->timeIn) return;
//   if (which==&this->timeIn && this->firstTime) {
//     this->lastTime=this->timeIn.getValue();
//     this->firstTime=FALSE;
//     printf("!!!!!!!\n");
//   }
  else if (which==&this->reset) {
    this->currTime=SbTime::zero();
    this->lastTime=this->timeIn.getValue();
  }
  else if (which==&this->pause) {
    if (this->pause.getValue() && this->status==SoElapsedTime::RUNNING) {
      this->status=SoElapsedTime::PAUSED;
      this->pauseTime=this->currTime;
    }
    else if (!this->pause.getValue() && this->status==SoElapsedTime::PAUSED) {
      this->status=SoElapsedTime::RUNNING;
    }
  }
  else if (which==&this->on) {
    if (this->on.getValue() && this->status==SoElapsedTime::STOPPED) {
      this->status=SoElapsedTime::RUNNING;
      this->lastTime=this->timeIn.getValue();
    }
    else if (!this->on.getValue() && this->status!=SoElapsedTime::STOPPED) {
      this->status=SoElapsedTime::STOPPED;
    }
  }
}
