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
  \class SoSFVec4i32 SoSFVec4i32.h Inventor/fields/SoSFVec4i32.h
  \brief The SoSFVec4i32 class is a container for an SbVec4i32 vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with four elements.

  \sa SbVec4i32, SoMFVec4i32
  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec4i32.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec4i32, SbVec4i32, const SbVec4i32 &);

// *************************************************************************

// Override from parent class.
void
SoSFVec4i32::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec4i32);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec4i32::readValue(SoInput * in)
{
  return
    in->read(this->value[0]) &&
    in->read(this->value[1]) &&
    in->read(this->value[2]) &&
    in->read(this->value[3]);
}

void
SoSFVec4i32::writeValue(SoOutput * out) const
{
  sosfvec4i32_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set value of vector.
*/
void
SoSFVec4i32::setValue(int32_t x, int32_t y, int32_t z, int32_t w)
{
  this->setValue(SbVec4i32(x, y, z, w));
}

/*!
  Set value of vector.
*/
void
SoSFVec4i32::setValue(const int32_t xyzw[4])
{
  this->setValue(SbVec4i32(xyzw));
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec4i32 field;
  BOOST_CHECK_MESSAGE(SoSFVec4i32::getClassTypeId() != SoType::badType(),
                      "SoSFVec4i32 class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
