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
  \class SoMFMatrix SoMFMatrix.h Inventor/fields/SoMFMatrix.h
  \brief The SoMFMatrix class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFMatrix.h>
#include <Inventor/fields/SoSFMatrix.h>

#include <Inventor/fields/SoSFString.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <assert.h>

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_MFIELD_SOURCE_MALLOC(SoMFMatrix, SbMatrix, const SbMatrix &);


/*!
  Does initialization common for all objects of the
  SoMFMatrix class. This includes setting up the
  type system, among other things.
*/
void
SoMFMatrix::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFMatrix);
}

SbBool
SoMFMatrix::read1Value(SoInput * in, int idx)
{
  SoSFMatrix sfmatrix;
  SbBool result;
  if ((result = sfmatrix.readValue(in)))
    this->set1Value(idx, sfmatrix.getValue());
  return result;
}

void
SoMFMatrix::write1Value(SoOutput * out, int idx) const
{
  SoSFMatrix sfmatrix;
  sfmatrix.setValue((*this)[idx]);
  out->incrementIndent();
  if (idx == 0) out->incrementIndent();
  sfmatrix.writeValue(out);
  if (idx == 0) out->decrementIndent();
  out->decrementIndent();
}

/*!
  FIXME: write function documentation
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
  this->setValue(SbMatrix(a11,a12,a13,a14,
                          a21,a22,a23,a24,
                          a31,a32,a33,a34,
                          a41,a42,a43,a44));
}

void
SoMFMatrix::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFMatrix::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFMatrix *)dest)->setValue((*this)[0]);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    int num=this->getNum();
    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      ostr << this->values[i][0][0] << " " <<
        this->values[i][0][1] << " " <<
        this->values[i][0][2] << " " <<
        this->values[i][0][3] << "\n\t";
      if (num>1) ostr << "  ";
      ostr << this->values[i][1][0] << " " <<
        this->values[i][1][1] << " " <<
        this->values[i][1][2] << " " <<
        this->values[i][1][3] << "\n\t";
      if (num>1) ostr << "  ";
      ostr << this->values[i][2][0] << " " <<
        this->values[i][2][1] << " " <<
        this->values[i][2][2] << " " <<
        this->values[i][2][3] << "\n\t";
      if (num>1) ostr << "  ";
      ostr << this->values[i][3][0] << " " <<
        this->values[i][3][1] << " " <<
        this->values[i][3][2] << " " <<
        this->values[i][3][3];
      if (i<num-1)
        ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFMatrix::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
