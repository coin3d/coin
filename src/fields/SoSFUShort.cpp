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


SO_SFIELD_SOURCE(SoSFUShort, unsigned short, const unsigned short);


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
sosfushort_write_value(SoOutput * out, const unsigned short val)
{
  out->write(val);
}

void
SoSFUShort::writeValue(SoOutput * out) const
{
  sosfushort_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
