/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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

#include "engines/SoSubEngineP.h"

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
