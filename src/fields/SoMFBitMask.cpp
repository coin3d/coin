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
  \class SoMFBitMask SoMFBitMask.h Inventor/fields/SoMFBitMask.h
  \brief The SoMFBitMask class is a container for a set of bitmasks.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple bitmasks with values from an enumerated set.

  \sa SoSFBitMask

*/

#include <Inventor/fields/SoMFBitMask.h>

#include <cassert>

#include <Inventor/fields/SoSFBitMask.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "fields/SoSubFieldP.h"

SO_MFIELD_DERIVED_SOURCE(SoMFBitMask, not_used, not_used);

// Override from parent class.
void
SoMFBitMask::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFBitMask);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFBitMask::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  SoSFBitMask sfbitmask;
  sfbitmask.setEnums(this->numEnums, this->enumValues, this->enumNames);
  SbBool result;
  if ((result = sfbitmask.readValue(in))) {
    this->values[idx] = sfbitmask.getValue();
  }
  return result;
}

void
SoMFBitMask::write1Value(SoOutput * out, int idx) const
{
  SoSFBitMask sfbitmask;
  sfbitmask.setEnums(this->numEnums, this->enumValues, this->enumNames);
  sfbitmask.setValue((*this)[idx]);
  sfbitmask.writeValue(out);
}

#endif // DOXYGEN_SKIP_THIS

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoMFBitMask field;
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
  BOOST_CHECK_EQUAL(field.getNum(), 0);
}

#endif // COIN_TEST_SUITE
