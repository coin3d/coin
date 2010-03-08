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
  \class SoSFVec3f SoSFVec3f.h Inventor/fields/SoSFVec3f.h
  \brief The SoSFVec3f class is a container for an SbVec3f vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with three elements.

  \sa SbVec3f, SoMFVec3f
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec3f.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec3f, SbVec3f, const SbVec3f &);

// *************************************************************************

// Override from parent class.
void
SoSFVec3f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec3f);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec3f::readValue(SoInput * in)
{
  return
    in->read(this->value[0]) &&
    in->read(this->value[1]) &&
    in->read(this->value[2]);
}

void
SoSFVec3f::writeValue(SoOutput * out) const
{
  sosfvec3f_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFVec3f::setValue(float x, float y, float z)
{
  this->setValue(SbVec3f(x, y, z));
}

/*!
  Set value of vector.
*/
void
SoSFVec3f::setValue(const float xyz[3])
{
  this->setValue(SbVec3f(xyz));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec3f field;
  BOOST_CHECK_MESSAGE(SoSFVec3f::getClassTypeId() != SoType::badType(),
                      "SoSFVec3f class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
