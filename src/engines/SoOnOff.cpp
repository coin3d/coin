/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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

// Documented in superclass.
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

// Documented in superclass.
void
SoOnOff::evaluate(void)
{
  SO_ENGINE_OUTPUT(isOn, SoSFBool, setValue(this->state));
  SO_ENGINE_OUTPUT(isOff, SoSFBool, setValue(!this->state));
}

// Documented in superclass.
void
SoOnOff::inputChanged(SoField * which)
{
  if (which == &on) this->state = TRUE;
  else if (which == &off) this->state = FALSE;
  if (which == &toggle) this->state = this->state ? FALSE : TRUE;
}
