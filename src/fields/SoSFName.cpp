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
  \class SoSFName SoSFName.h Inventor/fields/SoSFName.h
  \brief The SoSFName class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFName.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32



SO_SFIELD_SOURCE(SoSFName, SbName, const SbName);


/*!
  Does initialization common for all objects of the
  SoSFName class. This includes setting up the
  type system, among other things.
*/
void
SoSFName::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFName);
}

SbBool
SoSFName::readValue(SoInput * in)
{
  return in->read(value);
}

void
SoSFName::writeValue(SoOutput * out) const
{
  out->write(this->getValue());
}

/*!
  FIXME: write function documentation
*/
void
SoSFName::setValue(const char * const name)
{
  this->setValue(SbName(name));
}

void
SoSFName::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFName::getClassTypeId()) {
    ((SoMFName *)dest)->setValue(this->getValue());
  }
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << '"' << this->getValue().getString() << '"';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFName::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
