/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#include <Inventor/fields/SoSubFieldP.h>
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
