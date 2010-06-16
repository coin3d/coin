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
  \class SoSFVec3s SoSFVec3s.h Inventor/fields/SoSFVec3s.h
  \brief The SoSFVec3s class is a container for an SbVec3s vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with three elements.

  \sa SbVec3s, SoMFVec3s
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec3s.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/SoSubFieldP.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec3s, SbVec3s, const SbVec3s &);

// *************************************************************************

// Override from parent class.
void
SoSFVec3s::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec3s);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec3s::readValue(SoInput * in)
{
  return
    in->read(this->value[0]) &&
    in->read(this->value[1]) &&
    in->read(this->value[2]);
}

void
SoSFVec3s::writeValue(SoOutput * out) const
{
  const SbVec3s v(this->getValue());
  out->write(v[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(v[2]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFVec3s::setValue(short x, short y, short z)
{
  this->setValue(SbVec3s(x, y, z));
}

/*!
  Set value of vector.
*/
void
SoSFVec3s::setValue(const short xyz[3])
{
  this->setValue(SbVec3s(xyz));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec3s field;
  BOOST_CHECK_MESSAGE(SoSFVec3s::getClassTypeId() != SoType::badType(),
                      "SoSFVec3s class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
