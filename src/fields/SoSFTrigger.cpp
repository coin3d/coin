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

#include "coindefs.h"

#include "fields/SoSubFieldP.h"

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
SoSFTrigger::operator=(const SoSFTrigger & COIN_UNUSED_ARG(field))
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
  // call SoField::startNotify() to notify our container
  SoField::startNotify();
}

/*!
  Since SoSFTrigger fields doesn't have any value, they are all
  equal. So this method always returns \c TRUE.
 */
int
SoSFTrigger::operator==(const SoSFTrigger & COIN_UNUSED_ARG(trigger)) const
{
  return 1;
}

/*!
  Since SoSFTrigger fields doesn't have any value, they are all
  equal. So this method always returns \c FALSE.
*/
int
SoSFTrigger::operator!=(const SoSFTrigger & COIN_UNUSED_ARG(trigger)) const
{
  return 0;
}

void
SoSFTrigger::startNotify(void)
{
  // if notification starts here, we should not notify out container
  SoFieldContainer * oldcontainer = this->getContainer();
  this->setContainer(NULL);
  inherited::startNotify();
  this->setContainer(oldcontainer);
}

void
SoSFTrigger::notify(SoNotList * l)
{
  inherited::notify(l);
}

// No value to read, so just return TRUE.
SbBool
SoSFTrigger::readValue(SoInput * COIN_UNUSED_ARG(in))
{
  return TRUE;
}

// No value to write, just return.
void
SoSFTrigger::writeValue(SoOutput * COIN_UNUSED_ARG(out)) const
{
  return;
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFTrigger field;
  BOOST_CHECK_MESSAGE(SoSFTrigger::getClassTypeId() != SoType::badType(),
                      "SoSFTrigger class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
