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
  \class SoSFFloat SoSFFloat.h Inventor/fields/SoSFFloat.h
  \brief The SoSFFloat class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFUShort.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFTime.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_SFIELD_SOURCE(SoSFFloat, float, const float);


/*!
  Does initialization common for all objects of the
  SoSFFloat class. This includes setting up the
  type system, among other things.
*/
void
SoSFFloat::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFFloat);
}

SbBool
SoSFFloat::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFFloat::writeValue(SoOutput * out) const
{
  out->write(this->value);
}

void
SoSFFloat::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFBool::getClassTypeId()) {
    ((SoSFBool *)dest)->setValue((SbBool)this->getValue());
  }
  else if (dest->getTypeId()==SoSFInt32::getClassTypeId()) {
    ((SoSFInt32 *)dest)->setValue((int32_t)this->getValue());
  }
  else if (dest->getTypeId()==SoSFShort::getClassTypeId()) {
    ((SoSFShort *)dest)->setValue((short)this->getValue());
  }
  else if (dest->getTypeId()==SoSFUInt32::getClassTypeId()) {
    ((SoSFUInt32 *)dest)->setValue((uint32_t)this->getValue());
  }
  else if (dest->getTypeId()==SoSFUShort::getClassTypeId()) {
    ((SoSFUShort *)dest)->setValue((unsigned short)this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue() << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFFloat::getClassTypeId()) {
    ((SoMFFloat *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFTime::getClassTypeId()) {
    ((SoSFTime *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFFloat::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
