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
  \class SoSFVec2i32 SoSFVec2i32.h Inventor/fields/SoSFVec2i32.h
  \brief The SoSFVec2i32 class is a container for an SbVec2i32 vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with two elements.

  \sa SbVec2i32, SoMFVec2i32
  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec2i32.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec2i32, SbVec2i32, const SbVec2i32 &);

// *************************************************************************

// Override from parent class.
void
SoSFVec2i32::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec2i32);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec2i32::readValue(SoInput * in)
{
  return
    in->read(this->value[0]) &&
    in->read(this->value[1]);
}

void
SoSFVec2i32::writeValue(SoOutput * out) const
{
  sosfvec2i32_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFVec2i32::setValue(int32_t x, int32_t y)
{
  this->setValue(SbVec2i32(x, y));
}

/*!
  Set value of vector.
*/
void
SoSFVec2i32::setValue(const int32_t xy[2])
{
  this->setValue(SbVec2i32(xy));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec2i32 field;
  BOOST_CHECK_MESSAGE(SoSFVec2i32::getClassTypeId() != SoType::badType(),
                      "SoSFVec2i32 class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
