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
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoTriggerAny);

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
