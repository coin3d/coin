/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoSFMatrix SoSFMatrix.h Inventor/fields/SoSFMatrix.h
  \brief The SoSFMatrix class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoInput.h>
#include <Inventor/SbName.h>

#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFMatrix.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_SFIELD_SOURCE(SoSFMatrix, SbMatrix, const SbMatrix &);


/*!
  Does initialization common for all objects of the
  SoSFMatrix class. This includes setting up the
  type system, among other things.
*/
void
SoSFMatrix::initClass(void)
{
  SO_SFIELD_INIT_CLASS(SoSFMatrix, inherited);
}

SbBool
SoSFMatrix::readValue(SoInput * in)
{
  SbMat mat;

  SbBool result =
    in->read(mat[0][0]) && in->read(mat[0][1]) &&
    in->read(mat[0][2]) && in->read(mat[0][3]) &&
    in->read(mat[1][0]) && in->read(mat[1][1]) &&
    in->read(mat[1][2]) && in->read(mat[1][3]) &&
    in->read(mat[2][0]) && in->read(mat[2][1]) &&
    in->read(mat[2][2]) && in->read(mat[2][3]) &&
    in->read(mat[3][0]) && in->read(mat[3][1]) &&
    in->read(mat[3][2]) && in->read(mat[3][3]);

  if (result) this->value = mat;
  return result;
}

void
SoSFMatrix::writeValue(SoOutput * out) const
{
  if (out->isBinary()) {
    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
        out->write(this->getValue()[i][j]);
      }
    }
    return;
  }


  for(int k=0; k < 4; k++) {
    out->write(this->getValue()[0][k]);
    if(k != 3) out->write(' ');
  }

  out->write('\n'); // FIXME: ok on mac/win32?
  out->incrementIndent();

  for(int i=1; i < 4; i++) {
    out->indent();
    for(int j=0; j < 4; j++) {
      out->write(this->getValue()[i][j]);
      if(j != 3) out->write(' ');
    }
    if(i != 3) out->write('\n'); // FIXME: ok on mac/win32?
  }

  out->decrementIndent();
}

/*!
  FIXME: write function documentation
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

void
SoSFMatrix::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFRotation::getClassTypeId()) {
    ((SoSFRotation *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const SbMatrix mat=this->getValue();
    ostr << mat[0][0] << " " <<
      mat[0][1] << " " <<
      mat[0][2] << " " <<
      mat[0][3] << "\n\t" <<
      mat[1][0] << " " <<
      mat[1][1] << " " <<
      mat[1][2] << " " <<
      mat[1][3] << "\n\t" <<
      mat[2][0] << " " <<
      mat[2][1] << " " <<
      mat[2][2] << " " <<
      mat[2][3] << "\n\t" <<
      mat[3][0] << " " <<
      mat[3][1] << " " <<
      mat[3][2] << " " <<
      mat[3][3] << ends;

    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFMatrix::getClassTypeId()) {
    ((SoMFMatrix *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFMatrix::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
