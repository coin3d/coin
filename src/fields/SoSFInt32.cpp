/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoSFInt32 SoSFInt32.h Inventor/fields/SoSFInt32.h
  \brief The SoSFInt32 class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFUShort.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFInt32.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_SFIELD_SOURCE(SoSFInt32, int32_t, const int32_t);


/*!
  Does initialization common for all objects of the
  SoSFInt32 class. This includes setting up the
  type system, among other things.
*/
void
SoSFInt32::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFInt32);
}

SbBool
SoSFInt32::readValue(SoInput * in)
{
  return in->read(this->value);
}

void
SoSFInt32::writeValue(SoOutput * out) const
{
  out->write(this->getValue());
}

void
SoSFInt32::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFBool::getClassTypeId()) {
    ((SoSFBool *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFFloat::getClassTypeId()) {
    ((SoSFFloat *)dest)->setValue(float(this->getValue()));
  }
  else if (dest->getTypeId()==SoSFShort::getClassTypeId()) {
    ((SoSFShort *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFUInt32::getClassTypeId()) {
    ((SoSFUInt32 *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFUShort::getClassTypeId()) {
    ((SoSFUShort *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue() << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
  else if (dest->getTypeId()==SoMFInt32::getClassTypeId()) {
    ((SoMFInt32 *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFInt32::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
