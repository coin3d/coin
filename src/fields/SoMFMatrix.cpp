/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoMFMatrix SoMFMatrix.h Inventor/fields/SoMFMatrix.h
  \brief The SoMFMatrix class is a container for SbMatrix values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store matrices.

  \sa SoSFMatrix

*/

#include <Inventor/fields/SoMFMatrix.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_SOURCE(SoMFMatrix, SbMatrix, const SbMatrix &);


// Override from parent.
void
SoMFMatrix::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFMatrix);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFMatrix class.
extern SbBool sosfmatrix_read_value(SoInput * in, SbMatrix & val);
extern void sosfmatrix_write_value(SoOutput * out, const SbMatrix & val);

SbBool
SoMFMatrix::read1Value(SoInput * in, int idx)
{
  SbMatrix m;
  if (!sosfmatrix_read_value(in, m)) return FALSE;
  this->set1Value(idx, m);
  return TRUE;
}

void
SoMFMatrix::write1Value(SoOutput * out, int idx) const
{
  out->incrementIndent();
  if (idx == 0) out->incrementIndent();
  sosfmatrix_write_value(out, (*this)[idx]);
  if (idx == 0) out->decrementIndent();
  out->decrementIndent();
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set field value array to a single matrix with the given components.
*/
void
SoMFMatrix::setValue(const float a11, const float a12,
                     const float a13, const float a14,
                     const float a21, const float a22,
                     const float a23, const float a24,
                     const float a31, const float a32,
                     const float a33, const float a34,
                     const float a41, const float a42,
                     const float a43, const float a44)
{
  this->setValue(SbMatrix(a11,a12,a13,a14, a21,a22,a23,a24,
                          a31,a32,a33,a34, a41,a42,a43,a44));
}
