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
  \class SoSFString SoSFString.h Inventor/fields/SoSFString.h
  \brief The SoSFString class is a container for an SbString.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single string.

  Fields of this type stores their value to file as a the string
  within quotes.

  \sa SoMFString

*/

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFString, SbString, const SbString &);

// Override from parent class.
void
SoSFString::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFString);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read SbString value from input stream, return TRUE if
// successful. Also used from SoMFString class.
SbBool
sosfstring_read_value(SoInput * in, SbString & val)
{
  if (in->read(val)) return TRUE;
  SoReadError::post(in, "Couldn't read string");
  return FALSE;
}

SbBool
SoSFString::readValue(SoInput * in)
{
  SbString n;
  if (!sosfstring_read_value(in, n)) return FALSE;
  this->setValue(n);
  return TRUE;
}

// Write SbString value to output stream. Also used from SoMFString class.
void
sosfstring_write_value(SoOutput * out, const SbString & val)
{
  out->write(val);
}

void
SoSFString::writeValue(SoOutput * out) const
{
  sosfstring_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set string field value from \a str.
*/
void
SoSFString::setValue(const char * str)
{
  this->setValue(SbString(str));
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
