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
  \class SoTriggerAny SoTriggerAny.h Inventor/engines/SoTriggerAny.h
  \brief The SoTriggerAny class is a fan-in engine for triggers.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoTriggerAny.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoTriggerAny);

/*!
  Default constructor.
*/
SoTriggerAny::SoTriggerAny()
{
  SO_ENGINE_CONSTRUCTOR(SoTriggerAny);

  SO_ENGINE_ADD_INPUT(input0,());
  SO_ENGINE_ADD_INPUT(input1,());
  SO_ENGINE_ADD_INPUT(input2,());
  SO_ENGINE_ADD_INPUT(input3,());
  SO_ENGINE_ADD_INPUT(input4,());
  SO_ENGINE_ADD_INPUT(input5,());
  SO_ENGINE_ADD_INPUT(input6,());
  SO_ENGINE_ADD_INPUT(input7,());
  SO_ENGINE_ADD_INPUT(input8,());
  SO_ENGINE_ADD_INPUT(input9,());

  SO_ENGINE_ADD_OUTPUT(output,SoSFTrigger);
}

// overloaded from parent
void
SoTriggerAny::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoTriggerAny);
}

//
// private members
//
SoTriggerAny::~SoTriggerAny()
{
}

// overloaded from parent
void
SoTriggerAny::evaluate()
{
  // FIXME: there should probably be some code here :) pederb, 20000309
}
