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
  \class SoMFShort SoMFShort.h Inventor/fields/SoMFShort.h
  \brief The SoMFShort class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFShort.h>
#include <Inventor/fields/SoSFShort.h>

#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <assert.h>
#include <Inventor/fields/SoSFString.h>
#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFShort, short, short);


/*!
  Does initialization common for all objects of the
  SoMFShort class. This includes setting up the
  type system, among other things.
*/
void
SoMFShort::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFShort);
}

SbBool
SoMFShort::read1Value(SoInput * in, int idx)
{
  SoSFShort sfshort;
  SbBool result;
  if ((result = sfshort.readValue(in)))
    this->set1Value(idx, sfshort.getValue());
  return result;
}

void
SoMFShort::write1Value(SoOutput * out, int idx) const
{
  SoSFShort sfshort;
  sfshort.setValue((*this)[idx]);
  sfshort.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFShort::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFShort::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFShort::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFShort *)dest)->setValue((*this)[0]);
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
    SoDebugError::post("SoMFRotation::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
