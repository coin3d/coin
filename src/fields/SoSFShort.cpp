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
  \class SoSFShort SoSFShort.h Inventor/fields/SoSFShort.h
  \brief The SoSFShort class is a container for a short integer value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single short integer value.

  \sa SoMFShort
*/

#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_SFIELD_SOURCE(SoSFShort, short, short);


// Override parent class.
void
SoSFShort::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFShort);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFShort class.
SbBool
sosfshort_read_value(SoInput * in, short & val)
{
  if (!in->read(val)) {
    SoReadError::post(in, "Couldn't read short value");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFShort::readValue(SoInput * in)
{
  short val;
  if (!sosfshort_read_value(in, val)) return FALSE;
  this->setValue(val);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFShort
// class.
void
sosfshort_write_value(SoOutput * out, short val)
{
  out->write(val);
}

void
SoSFShort::writeValue(SoOutput * out) const
{
  sosfshort_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
