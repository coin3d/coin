/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoSFUShort SoSFUShort.h Inventor/fields/SoSFUShort.h
  \brief The SoSFUShort class is a container for a unsigned short integer value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single short unsigned integer value.

  \sa SoMFUShort
*/

#include <Inventor/fields/SoSFUShort.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFUShort, unsigned short, unsigned short);


// Override from parent class.
void
SoSFUShort::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFUShort);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFUShort class.
SbBool
sosfushort_read_value(SoInput * in, unsigned short & val)
{
  if (!in->read(val)) {
    SoReadError::post(in, "Couldn't read unsigned short value");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFUShort::readValue(SoInput * in)
{
  unsigned short val;
  if (!sosfushort_read_value(in, val)) return FALSE;
  this->setValue(val);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFUShort
// class.
void
sosfushort_write_value(SoOutput * out, unsigned short val)
{
  out->write(val);
}

void
SoSFUShort::writeValue(SoOutput * out) const
{
  sosfushort_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
