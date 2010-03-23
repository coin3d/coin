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
  \class SoSFVec4b SoSFVec4b.h Inventor/fields/SoSFVec4b.h
  \brief The SoSFVec4b class is a container for an SbVec4b vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with four elements.

  \sa SbVec4b, SoMFVec4b
  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec4b.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec4b, SbVec4b, SbVec4b);

// *************************************************************************

// Override from parent class.
void
SoSFVec4b::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec4b);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec4b::readValue(SoInput * in)
{
  return
    in->readByte(this->value[0]) &&
    in->readByte(this->value[1]) &&
    in->readByte(this->value[2]) &&
    in->readByte(this->value[3]);
}

void
SoSFVec4b::writeValue(SoOutput * out) const
{
  sosfvec4b_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set value of vector.
*/
void
SoSFVec4b::setValue(int8_t x, int8_t y, int8_t z, int8_t w)
{
  this->setValue(SbVec4b(x, y, z, w));
}

/*!
  Set value of vector.
*/
void
SoSFVec4b::setValue(const int8_t xyzw[4])
{
  this->setValue(SbVec4b(xyzw));
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec4b field;
  BOOST_CHECK_MESSAGE(SoSFVec4b::getClassTypeId() != SoType::badType(),
                      "SoSFVec4b class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
