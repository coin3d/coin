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
#include <SoSubFieldPrivate.h>


SO_SFIELD_CONSTRUCTOR_SOURCE(SoSFTrigger);
// The following code is the internals of the
// SO_SFIELD_REQUIRED_SOURCE macro, with a minor modification
// (setValue() takes a void argument).
PRIVATE_TYPEID_SOURCE(SoSFTrigger);
PRIVATE_EQUALITY_SOURCE(SoSFTrigger);
const SoSFTrigger &
SoSFTrigger::operator=(const SoSFTrigger & field)
{
  this->setValue();
  return *this;
}

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
  SoNotList l;
  this->notify(&l);
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
