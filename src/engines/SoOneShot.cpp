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

#include <Inventor/engines/SoOneShot.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>

#include <Inventor/fields/SoSFFloat.h>


SO_ENGINE_SOURCE(SoOneShot);

SoOneShot::SoOneShot()
{
  SO_ENGINE_CONSTRUCTOR(SoOneShot);

  SO_ENGINE_ADD_INPUT(timeIn,(SbTime::zero()));
  SO_ENGINE_ADD_INPUT(duration,(SbTime(1.0)));
  SO_ENGINE_ADD_INPUT(trigger,());
  SO_ENGINE_ADD_INPUT(flags,(0));
  SO_ENGINE_ADD_INPUT(disable,(FALSE));

  SO_ENGINE_ADD_OUTPUT(timeOut,SoSFTime);
  SO_ENGINE_ADD_OUTPUT(isActive,SoSFBool);
  SO_ENGINE_ADD_OUTPUT(ramp,SoSFFloat);

  SoField *realtime=SoDB::getGlobalField("realTime");
  this->timeIn.connectFrom(realtime);

  this->running=FALSE;
}

void
SoOneShot::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoOneShot);
}


//
// private members
//
SoOneShot::~SoOneShot()
{
}

void
SoOneShot::evaluate()
{
  SbTime elapsed=this->timeIn.getValue()-this->startTime;
  SbTime durationVal=this->duration.getValue();

  SO_ENGINE_OUTPUT(isActive,SoSFBool,setValue(this->running));
  if (this->running) {
    if (elapsed<durationVal) {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(elapsed));
      SO_ENGINE_OUTPUT(ramp,SoSFFloat,
                       setValue(elapsed.getValue()/durationVal.getValue()));
    }
    else {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(durationVal));
      SO_ENGINE_OUTPUT(ramp,SoSFFloat,setValue(1.0));
      if (this->flags.getValue()&SoOneShot::HOLD_FINAL)
        this->holdDuration=durationVal;
      this->running=FALSE;
    }
  }
  else {
    if (this->flags.getValue()&SoOneShot::HOLD_FINAL) {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(this->holdDuration));
      SO_ENGINE_OUTPUT(ramp,SoSFFloat,setValue(1.0));
    }
    else {
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(0.0));
      SO_ENGINE_OUTPUT(ramp,SoSFFloat,setValue(0.0));
    }
  }
}

//FIXME: messages (kintel 19990611)
void
SoOneShot::inputChanged(SoField *which)
{
  if (which==&this->trigger) {
#if 0 // FIXME: doesn't compile. 19990620 mortene.
      cout << "(trigger): " <<
        this->flags.getValue() << endl;
#endif // disabled
    if ((!this->running ||
         this->flags.getValue()&SoOneShot::RETRIGGERABLE) &&
        !this->disable.getValue()) {
#if 0 // FIXME: doesn't compile. 19990620 mortene.
      cout << "XXX Retrigger XXX" << endl;
#endif // disabled
      this->startTime=this->timeIn.getValue();
      this->running=TRUE;
      SO_ENGINE_OUTPUT(timeOut,SoSFTime,setValue(0.0));
      SO_ENGINE_OUTPUT(ramp,SoSFFloat,setValue(0.0));
    }
  }
  else if (which==&this->disable) {
    if (this->disable.getValue())
      this->running=FALSE;
    this->timeOut.enable(!this->disable.getValue());
    this->ramp.enable(!this->disable.getValue());
    this->isActive.enable(!this->disable.getValue());
  }
}
