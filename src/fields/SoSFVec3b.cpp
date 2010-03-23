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
  \class SoSFVec3b SoSFVec3b.h Inventor/fields/SoSFVec3b.h
  \brief The SoSFVec3b class is a container for an SbVec3b vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with three elements.

  \sa SbVec3b, SoMFVec3b
  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec3b.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec3b, SbVec3b, SbVec3b);

// *************************************************************************

// Override from parent class.
void
SoSFVec3b::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec3b);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec3b::readValue(SoInput * in)
{
  return
    in->readByte(this->value[0]) &&
    in->readByte(this->value[1]) &&
    in->readByte(this->value[2]);
}

void
SoSFVec3b::writeValue(SoOutput * out) const
{
  sosfvec3b_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFVec3b::setValue(int8_t x, int8_t y, int8_t z)
{
  this->setValue(SbVec3b(x, y, z));
}

/*!
  Set value of vector.
*/
void
SoSFVec3b::setValue(const int8_t xyz[3])
{
  this->setValue(SbVec3b(xyz));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec3b field;
  BOOST_CHECK_MESSAGE(SoSFVec3b::getClassTypeId() != SoType::badType(),
                      "SoSFVec3b class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
