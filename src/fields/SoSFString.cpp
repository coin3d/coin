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
  \class SoSFString SoSFString.h Inventor/fields/SoSFString.h
  \brief The SoSFString class is a container for an SbString.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single string.

  Fields of this type stores their value to file as a the string
  within quotes.

  \sa SoMFString
*/

// *************************************************************************

#include <Inventor/fields/SoSFString.h>

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFString, SbString, const SbString &);

// *************************************************************************

// Override from parent class.
void
SoSFString::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFString);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFString::readValue(SoInput * in)
{
  return in->read(this->value);
}

void
SoSFString::writeValue(SoOutput * out) const
{
  sosfstring_write_value(this, out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set string field value from \a str.
*/
void
SoSFString::setValue(const char * str)
{
  this->setValue(SbString(str));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFString field;
  BOOST_CHECK_MESSAGE(SoSFString::getClassTypeId() != SoType::badType(),
                      "SoSFString class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
