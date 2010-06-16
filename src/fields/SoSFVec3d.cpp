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
  \class SoSFVec3d SoSFVec3d.h Inventor/fields/SoSFVec3d.h
  \brief The SoSFVec3d class is a container for an SbVec3d vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with three elements.

  \sa SbVec3d, SoMFVec3d
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec3d.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec3d, SbVec3d, const SbVec3d &);

// *************************************************************************

// Override from parent class.
void
SoSFVec3d::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec3d);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec3d::readValue(SoInput * in)
{
  SbVec3d v;
  if (!sosfvec3d_read_value(in, v)) return FALSE;
  this->setValue(v);
  return TRUE;
}

void
SoSFVec3d::writeValue(SoOutput * out) const
{
  sosfvec3d_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

/*!
  Set value of vector.
*/
void
SoSFVec3d::setValue(const double x, const double y, const double z)
{
  this->setValue(SbVec3d(x, y, z));
}

/*!
  Set value of vector.
*/
void
SoSFVec3d::setValue(const double xyz[3])
{
  this->setValue(SbVec3d(xyz));
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec3d field;
  BOOST_CHECK_MESSAGE(SoSFVec3d::getClassTypeId() != SoType::badType(),
                      "SoSFVec3d class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
