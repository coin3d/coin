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
  \class SoMFFloat SoMFFloat.h Inventor/fields/SoMFFloat.h
  \brief The SoMFFloat class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFFloat.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <malloc.h>
#include <Inventor/fields/SoSFString.h>

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_MFIELD_SOURCE_MALLOC(SoMFFloat, float, float);


/*!
  Does initialization common for all objects of the
  SoMFFloat class. This includes setting up the
  type system, among other things.
*/
void
SoMFFloat::initClass(void)
{
  SO_MFIELD_INIT_CLASS(SoMFFloat, inherited);
}

SbBool
SoMFFloat::read1Value(SoInput * in, int idx)
{
  SoSFFloat sffloat;
  SbBool result;
  if ((result = sffloat.readValue(in)))
    this->set1Value(idx, sffloat.getValue());
  return result;
}

void
SoMFFloat::write1Value(SoOutput * out, int idx) const
{
  SoSFFloat sffloat;
  sffloat.setValue((*this)[idx]);
  sffloat.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFFloat::getNumValuesPerLine(void) const
{
  return 4;
}

void
SoMFFloat::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFFloat *)dest)->setValue((*this)[0]);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    const int num=this->getNum();
    ostrstream ostr;
    if (num!=1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      ostr << (*this)[i];
      if (i<num-1) ostr << ", ";
    }
    if (num!=1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFFloat::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
