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
  \class SoMFUInt32 SoMFUInt32.h Inventor/fields/SoMFUInt32.h
  \brief The SoMFUInt32 class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/fields/SoSFUInt32.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/fields/SoSFString.h>

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

#include <iomanip.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFUInt32, uint32_t, uint32_t);


/*!
  Does initialization common for all objects of the
  SoMFUInt32 class. This includes setting up the
  type system, among other things.
*/
void
SoMFUInt32::initClass(void)
{
  SO_MFIELD_INIT_CLASS(SoMFUInt32, inherited);
}

SbBool
SoMFUInt32::read1Value(SoInput * in, int idx)
{
  SoSFUInt32 sfuint32;
  SbBool result;
  if ((result = sfuint32.readValue(in)))
    this->set1Value(idx, sfuint32.getValue());
  return result;
}

void
SoMFUInt32::write1Value(SoOutput * out, int idx) const
{
  SoSFUInt32 sfuint32;
  sfuint32.setValue((*this)[idx]);
  sfuint32.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFUInt32::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFUInt32::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFUInt32::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFUInt32 *)dest)->setValue((*this)[0]);
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
    SoDebugError::post("SoMFUInt32::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
