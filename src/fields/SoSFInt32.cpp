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
  \brief The SoSFInt32 class is a container for a 32-bit integer value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single 32-bit integer value.

  \sa SoMFInt32
*/

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFInt32, int32_t, const int32_t);


// Override from parent.
void
SoSFInt32::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFInt32);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read integer value from input stream, return TRUE if
// successful. Also used from SoMFInt32 class.
SbBool
sosfint32_read_value(SoInput * in, int32_t & val)
{
  if (!in->read(val)) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFInt32::readValue(SoInput * in)
{
  int32_t val;
  if (!sosfint32_read_value(in, val)) return FALSE;
  this->setValue(val);
  return TRUE;
}

// Write integer value to output stream. Also used from SoMFInt32
// class.
void
sosfint32_write_value(SoOutput * out, const int32_t val)
{
  out->write(val);
}

void
SoSFInt32::writeValue(SoOutput * out) const
{
  sosfint32_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
