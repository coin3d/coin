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
  \class SoSFUInt32 SoSFUInt32.h Inventor/fields/SoSFUInt32.h
  \brief The SoSFUInt32 class is a container for a 32-bit unsigned integer value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single 32-bit unsigned integer value.

  \sa SoMFUInt32
*/

// *************************************************************************

#include <Inventor/fields/SoSFUInt32.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFUInt32, uint32_t, uint32_t);

// *************************************************************************

// Override from parent class.
void
SoSFUInt32::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFUInt32);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFUInt32::readValue(SoInput * in)
{
  unsigned int tmp;
  if (!in->read(tmp)) return FALSE;
  this->value = tmp;
  return TRUE;
}

void
SoSFUInt32::writeValue(SoOutput * out) const
{
  sosfuint32_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFUInt32 field;
  BOOST_CHECK_MESSAGE(SoSFUInt32::getClassTypeId() != SoType::badType(),
                      "SoSFUInt32 class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
