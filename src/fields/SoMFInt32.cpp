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
  \class SoMFInt32 SoMFInt32.h Inventor/fields/SoMFInt32.h
  \brief The SoMFInt32 class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFInt32.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/fields/SoSFString.h>

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_MFIELD_SOURCE_MALLOC(SoMFInt32, int32_t, int32_t);


/*!
  Does initialization common for all objects of the
  SoMFInt32 class. This includes setting up the
  type system, among other things.
*/
void
SoMFInt32::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFInt32);
}

SbBool
SoMFInt32::read1Value(SoInput * in, int idx)
{
  SoSFInt32 sfint32;
  SbBool result;
  if ((result = sfint32.readValue(in)))
    this->set1Value(idx, sfint32.getValue());
  return result;
}

void
SoMFInt32::write1Value(SoOutput * out, int idx) const
{
  SoSFInt32 sfint32;
  sfint32.setValue((*this)[idx]);
  sfint32.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
int
SoMFInt32::getNumValuesPerLine(void) const
{
  return 8;
}

void
SoMFInt32::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFInt32 *)dest)->setValue((*this)[0]);
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
    SoDebugError::post("SoMFInt32::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
