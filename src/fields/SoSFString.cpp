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
  \class SoSFString SoSFString.h Inventor/fields/SoSFString.h
  \brief The SoSFString class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFString, SbString, const SbString &);


/*!
  Does initialization common for all objects of the
  SoSFString class. This includes setting up the
  type system, among other things.
*/
void
SoSFString::initClass(void)
{
  SO_SFIELD_INIT_CLASS(SoSFString, inherited);
}

SbBool
SoSFString::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFString::writeValue(SoOutput * out) const
{
  out->write(this->getValue());
}

/*!
  FIXME: write function documentation
*/
void
SoSFString::setValue(const char * const string)
{
  this->setValue(SbString(string));
}

void
SoSFString::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFString::getClassTypeId()) {
    ((SoMFString *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFString::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
