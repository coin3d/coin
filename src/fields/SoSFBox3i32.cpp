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
  \class SoSFBox3i32 SoSFBox3i32.h Inventor/fields/SoSFBox3i32.h
  \brief The SoSFBox3i32 class is a container for an SbBox3i32 vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a box.

  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFBox3i32.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFBox3i32, SbBox3i32, const SbBox3i32 &);

// *************************************************************************

// Override from parent class.
void
SoSFBox3i32::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBox3i32);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS


SbBool
SoSFBox3i32::readValue(SoInput * in)
{
  int32_t min[3];
  int32_t max[3];
  if (!in->read(min[0]) ||
      !in->read(min[1]) ||
      !in->read(min[2]) ||
      !in->read(max[0]) ||
      !in->read(max[1]) ||
      !in->read(max[2])) {
    SoReadError::post(in, "Couldn't read SoSFBox3i32");
    return FALSE;
  }
  this->setValue(min[0], min[1], min[2], max[0], max[1], max[2]);
  return TRUE;
}

void
SoSFBox3i32::writeValue(SoOutput * out) const
{
  int32_t min[3];
  int32_t max[3];
  SbBox3i32 b = this->getValue();
  b.getBounds(min[0], min[1], min[2], max[0], max[1], max[2]);
  out->write(min[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(min[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(min[2]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[2]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFBox3i32::setValue(int32_t xmin, int32_t ymin, int32_t zmin,
                      int32_t xmax, int32_t ymax, int32_t zmax)
{
  this->setValue(SbBox3i32(xmin, ymin, zmin, xmax, ymax, zmax));
}


/*!
  Set value of vector.
*/
void
SoSFBox3i32::setValue(const SbVec3i32 & minvec, const SbVec3i32 & maxvec)
{
  this->setValue(SbBox3i32(minvec, maxvec));
}


/*!
  Set value of vector.
*/
void
SoSFBox3i32::getValue(SbBox3i32 & box) const
{
  box = value;
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFBox3i32 field;
  BOOST_CHECK_MESSAGE(SoSFBox3i32::getClassTypeId() != SoType::badType(),
                      "SoSFBox3i32 class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
