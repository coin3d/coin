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

#include <Inventor/engines/SoOnOff.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoSFBool.h>

SO_ENGINE_SOURCE(SoOnOff);

SoOnOff::SoOnOff()
{
  SO_ENGINE_CONSTRUCTOR(SoOnOff);

  SO_ENGINE_ADD_INPUT(on,());
  SO_ENGINE_ADD_INPUT(off,());
  SO_ENGINE_ADD_INPUT(toggle,());

  SO_ENGINE_ADD_OUTPUT(isOn,SoSFBool);
  SO_ENGINE_ADD_OUTPUT(isOff,SoSFBool);

  state=FALSE;
}

void
SoOnOff::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoOnOff);
}

//
// private members
//
SoOnOff::~SoOnOff()
{
}

void
SoOnOff::evaluate()
{
  SO_ENGINE_OUTPUT(isOn,SoSFBool,setValue(this->state));
  SO_ENGINE_OUTPUT(isOff,SoSFBool,setValue(!this->state));
}

void
SoOnOff::inputChanged(SoField *which)
{
  if (which==&on) this->state=TRUE;
  else if (which==&off) this->state=FALSE;
  if (which==&toggle) this->state=this->state?FALSE:TRUE;
}
