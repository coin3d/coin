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
  \class SoSFUInt32 SoSFUInt32.h Inventor/fields/SoSFUInt32.h
  \brief The SoSFUInt32 class is a container for a 32-bit unsigned integer value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single 32-bit unsigned integer value.

  \sa SoMFUInt32
*/

#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFUInt32, uint32_t, uint32_t);


// Override from parent class.
void
SoSFUInt32::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFUInt32);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFUInt32 class.
SbBool
sosfuint32_read_value(SoInput * in, uint32_t & val)
{
  if (!in->read(val)) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFUInt32::readValue(SoInput * in)
{
  uint32_t val;
  if (!sosfuint32_read_value(in, val)) return FALSE;
  this->setValue(val);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFUInt32
// class.
void
sosfuint32_write_value(SoOutput * out, uint32_t val)
{
  out->write(val);
}

void
SoSFUInt32::writeValue(SoOutput * out) const
{
  sosfuint32_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
