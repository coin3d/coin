/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoSFTrigger SoSFTrigger.h Inventor/fields/SoSFTrigger.h
  \brief The SoSFTrigger class is the "void" class used for detecting field changes.
  \ingroup fields

  Connect this field to a master field (or engine output) to detect
  when the master field changes its value.

  This is useful if you want to automatically trigger an update from
  the node or engine (or other field container) this field is
  part of whenever another field changes -- and you are not particularly
  interested in the actual value of the master field.

  \sa SoFieldSensor
*/

#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSubFieldP.h>


SO_SFIELD_CONSTRUCTOR_SOURCE(SoSFTrigger);
// The following code is the internals of the
// SO_SFIELD_REQUIRED_SOURCE macro, with a minor modification
// (setValue() takes a void argument).
PRIVATE_TYPEID_SOURCE(SoSFTrigger);
PRIVATE_EQUALITY_SOURCE(SoSFTrigger);
// (Declarations hidden in SO_[S|M]FIELD_HEADER macro in header file,
// so don't use Doxygen commenting.)
#ifndef DOXYGEN_SKIP_THIS
const SoSFTrigger &
SoSFTrigger::operator=(const SoSFTrigger & field)
{
  this->setValue();
  return *this;
}
#endif // DOXYGEN_SKIP_THIS

/*!
  This field class does not actually contain any value, so we
  just triggers an update by calling touch() within this method.
 */
void
SoSFTrigger::setValue(void)
{
  this->touch();
}

/*!
  Field doesn't contain any value, so this method does nothing.
 */
void
SoSFTrigger::getValue(void) const
{
  // Does nothing.
}

// Override from parent class.
void
SoSFTrigger::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFTrigger);
}

void
SoSFTrigger::touch(void)
{
  this->startNotify();
}

/*!
  Since SoSFTrigger fields doesn't have any value, they are all
  equal. So this method always returns \c TRUE.
 */
int
SoSFTrigger::operator==(const SoSFTrigger & trigger) const
{
  return 1;
}

/*!
  Since SoSFTrigger fields doesn't have any value, they are all
  equal. So this method always returns \c FALSE.
*/
int
SoSFTrigger::operator!=(const SoSFTrigger & trigger) const
{
  return 0;
}

void
SoSFTrigger::startNotify(void)
{
  inherited::startNotify();
}

void
SoSFTrigger::notify(SoNotList * l)
{
  inherited::notify(l);
}

// No value to read, so just return TRUE.
SbBool
SoSFTrigger::readValue(SoInput * in)
{
  return TRUE;
}

// No value to write, just return.
void
SoSFTrigger::writeValue(SoOutput * out) const
{
  return;
}
