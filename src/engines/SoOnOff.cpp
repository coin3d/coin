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
  \class SoOnOff SoOnOff.h Inventor/engines/SoOnOff.h
  \brief The SoOnOff engine is a simple on/off switch.
  \ingroup engines
*/

#include <Inventor/engines/SoOnOff.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/engines/SoSubEngineP.h>

/*!
  \var SoSFTrigger SoOnOff::on

  An input signal on this trigger makes the SoOnOff::isOn output
  become \c TRUE.
*/
/*!
  \var SoSFTrigger SoOnOff::off

  An input signal on this trigger makes the SoOnOff::isOn output
  become \c FALSE.
*/
/*!
  \var SoSFTrigger SoOnOff::toggle

  An input signal on this trigger toggles the \c TRUE or \c FALSE
  value of the SoOnOff::isOn output.
*/

/*!
  \var SoEngineOutput SoOnOff::isOn
  (SoSFBool) Whether the engine is "on" or not.
*/
/*!
  \var SoEngineOutput SoOnOff::isOff
  (SoSFBool) Always the opposite value of SoOnOff::isOn.
*/

SO_ENGINE_SOURCE(SoOnOff);

/*!
  Default constructor.
*/
SoOnOff::SoOnOff(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoOnOff);

  SO_ENGINE_ADD_INPUT(on, ());
  SO_ENGINE_ADD_INPUT(off, ());
  SO_ENGINE_ADD_INPUT(toggle, ());

  SO_ENGINE_ADD_OUTPUT(isOn, SoSFBool);
  SO_ENGINE_ADD_OUTPUT(isOff, SoSFBool);

  this->state = FALSE;
}

// overloaded from parent.
void
SoOnOff::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoOnOff);
}

/*!
  Destructor is protected because engines are reference counted.
*/
SoOnOff::~SoOnOff()
{
}

// overloaded from parent.
void
SoOnOff::evaluate(void)
{
  SO_ENGINE_OUTPUT(isOn, SoSFBool, setValue(this->state));
  SO_ENGINE_OUTPUT(isOff, SoSFBool, setValue(!this->state));
}

// overloaded from parent
void
SoOnOff::inputChanged(SoField * which)
{
  if (which == &on) this->state = TRUE;
  else if (which == &off) this->state = FALSE;
  if (which == &toggle) this->state = this->state ? FALSE : TRUE;
}
