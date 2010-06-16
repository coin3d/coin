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
  \class SoSFMatrix SoSFMatrix.h Inventor/fields/SoSFMatrix.h
  \brief The SoSFMatrix class is a container for an SbMatrix.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a 4x4 matrix.

  Fields of this type stores their value to file as a set of 16
  floating point values, written from the matrix in row-major mode.

  \sa SoMFMatrix
*/

// *************************************************************************

#include <Inventor/fields/SoSFMatrix.h>

#include <Inventor/SoInput.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFMatrix, SbMatrix, const SbMatrix &);

// *************************************************************************

// Override from parent.
void
SoSFMatrix::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFMatrix);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFMatrix::readValue(SoInput * in)
{
  float * ptr = this->value[0];
  for (int i = 0; i < 16; i++) {
    if (!in->read(ptr[i])) return FALSE;
  }
  return TRUE;
}

void
SoSFMatrix::writeValue(SoOutput * out) const
{
  sosfmatrix_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set matrix elements.
*/
void
SoSFMatrix::setValue(const float a11, const float a12,
                     const float a13, const float a14,
                     const float a21, const float a22,
                     const float a23, const float a24,
                     const float a31, const float a32,
                     const float a33, const float a34,
                     const float a41, const float a42,
                     const float a43, const float a44)
{
  this->setValue(SbMatrix(a11,a12,a13,a14,a21,a22,a23,a24,
                          a31,a32,a33,a34,a41,a42,a43,a44));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFMatrix field;
  BOOST_CHECK_MESSAGE(SoSFMatrix::getClassTypeId() != SoType::badType(),
                      "SoSFMatrix class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
