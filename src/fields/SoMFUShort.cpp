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
  \class SoMFUShort SoMFUShort.h Inventor/fields/SoMFUShort.h
  \brief The SoMFUShort class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFUShort.h>
#include <Inventor/fields/SoSFUShort.h>

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
#include <iomanip.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFUShort, unsigned short, unsigned short);


/*!
  Does initialization common for all objects of the
  SoMFUShort class. This includes setting up the
  type system, among other things.
*/
void
SoMFUShort::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFUShort);
}

SbBool
SoMFUShort::read1Value(SoInput * in, int idx)
{
  SoSFUShort sfushort;
  SbBool result;
  if ((result = sfushort.readValue(in)))
    this->set1Value(idx, sfushort.getValue());
  return result;
}

void
SoMFUShort::write1Value(SoOutput * out, int idx) const
{
  SoSFUShort sfushort;
  sfushort.setValue((*this)[idx]);
  sfushort.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFUShort::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFUShort::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFUShort::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFUShort *)dest)->setValue((*this)[0]);
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    const int num=this->getNum();
    ostrstream ostr;
    if (num!=1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      if ((*this)[i]>0) ostr.flags(ios::hex|ios::showbase);
      ostr << (*this)[i];
      if (i<num-1) ostr << ", ";
    }
    if (num!=1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFUShort::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
