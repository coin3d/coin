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
  \class SoSFMatrix SoSFMatrix.h Inventor/fields/SoSFMatrix.h
  \brief The SoSFMatrix class is a container for an SbMatrix.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a 4x4 matrix.

  Fields of this type stores their value to file as a set of 16
  floating point values, written from the matrix in row-major mode.

  \sa SoMFMatrix
*/

#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFMatrix, SbMatrix, const SbMatrix &);


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

// Read matrix from input stream, return TRUE if successful. Also used
// from SoMFMatrix class.
SbBool
sosfmatrix_read_value(SoInput * in, SbMatrix & m)
{
  if (in->read(m[0][0]) && in->read(m[0][1]) &&
      in->read(m[0][2]) && in->read(m[0][3]) &&
      in->read(m[1][0]) && in->read(m[1][1]) &&
      in->read(m[1][2]) && in->read(m[1][3]) &&
      in->read(m[2][0]) && in->read(m[2][1]) &&
      in->read(m[2][2]) && in->read(m[2][3]) &&
      in->read(m[3][0]) && in->read(m[3][1]) &&
      in->read(m[3][2]) && in->read(m[3][3]))
    return TRUE;

  SoReadError::post(in, "Premature end of file");
  return FALSE;
}

SbBool
SoSFMatrix::readValue(SoInput * in)
{
  SbMatrix m;
  if (!sosfmatrix_read_value(in, m)) return FALSE;
  this->setValue(m);
  return TRUE;
}

// Write matrix to output stream. Also used from SoMFMatrix class.
void
sosfmatrix_write_value(SoOutput * out, const SbMatrix & m)
{
  if (out->isBinary()) {
    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
        out->write(m[i][j]);
      }
    }
    return;
  }


  for(int k=0; k < 4; k++) {
    out->write(m[0][k]);
    if(k != 3) out->write(' ');
  }

  out->write('\n');
  out->incrementIndent();

  for(int i=1; i < 4; i++) {
    out->indent();
    for(int j=0; j < 4; j++) {
      out->write(m[i][j]);
      if(j != 3) out->write(' ');
    }
    if(i != 3) out->write('\n');
  }

  out->decrementIndent();
}

void
SoSFMatrix::writeValue(SoOutput * out) const
{
  sosfmatrix_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


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
