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

#include <Inventor/engines/SoCounter.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoSFBool.h>

SO_ENGINE_SOURCE(SoCounter);

SoCounter::SoCounter()
{
  SO_ENGINE_CONSTRUCTOR(SoCounter);

  SO_ENGINE_ADD_INPUT(min,(0));
  SO_ENGINE_ADD_INPUT(max,(1));
  SO_ENGINE_ADD_INPUT(step,(1));
  SO_ENGINE_ADD_INPUT(trigger,());
  SO_ENGINE_ADD_INPUT(reset,(0));

  SO_ENGINE_ADD_OUTPUT(output,SoSFShort);
  SO_ENGINE_ADD_OUTPUT(syncOut,SoSFTrigger);

  this->syncOut.enable(FALSE);  //Disable notification
}

void
SoCounter::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoCounter);
}

//
// private members
//
SoCounter::~SoCounter()
{
}

void
SoCounter::evaluate()
{
  SO_ENGINE_OUTPUT(output,SoSFShort,setValue(this->value));
}

void
SoCounter::inputChanged(SoField *which)
{
  if (which==&this->trigger) {
    this->numSteps+=1;
    this->value+=this->step.getValue();
    if (this->value>this->max.getValue()) {
      this->value=this->min.getValue();
      this->numSteps=0;
      this->syncOut.enable(TRUE);
      SO_ENGINE_OUTPUT(syncOut,SoSFTrigger,setValue());
      this->syncOut.enable(FALSE);
    }
  }
  else if (which==&this->reset) {
    short resetVal=this->reset.getValue();
    if (resetVal>this->max.getValue()) this->value=this->max.getValue();
    else if (resetVal<this->min.getValue()) this->value=this->min.getValue();
    else {
      int mod=(resetVal-this->min.getValue())%this->step.getValue();
      if (mod!=0) this->value=resetVal-mod;
    }
  }
  else if (which==&this->max) {
    if (this->max.getValue()<this->min.getValue()) {
      short tmp=this->max.getValue();
      this->max.setValue(this->min.getValue());
      this->min.setValue(tmp);
    }
    if (this->max.getValue()<this->value) {
      this->value=this->min.getValue();
      //sync
    }
  }
  else if (which==&this->min) {
    if (this->max.getValue()<this->min.getValue()) {
      short tmp=this->max.getValue();
      this->max.setValue(this->min.getValue());
      this->min.setValue(tmp);
    }
    this->value=this->min.getValue()+this->step.getValue()*this->numSteps;
    if (this->value>this->max.getValue()) {
      this->numSteps=0;
      this->value=this->min.getValue();
    }
  }
  else if (which==&this->step) {
    this->value=this->min.getValue()+this->step.getValue()*this->numSteps;
    if (this->value>this->max.getValue()) {
      this->numSteps=0;
      this->value=this->min.getValue();
    }
  }
}
