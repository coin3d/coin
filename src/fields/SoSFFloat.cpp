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
  \class SoSFFloat SoSFFloat.h Inventor/fields/SoSFFloat.h
  \brief The SoSFFloat class is a container for a floating point value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single floating point value.

  \sa SoMFFloat
*/

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_SFIELD_SOURCE(SoSFFloat, float, const float);


// Override from parent.
void
SoSFFloat::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFFloat);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read floating point value from input stream, return TRUE if
// successful. Also used from SoMFFloat class.
SbBool
sosffloat_read_value(SoInput * in, float & val)
{
  if (!in->read(val)) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }
  return TRUE;
}

SbBool
SoSFFloat::readValue(SoInput * in)
{
  float val;
  if (!sosffloat_read_value(in, val)) return FALSE;
  this->setValue(val);
  return TRUE;
}

// Write floating point value to output stream. Also used from
// SoMFFloat class.
void
sosffloat_write_value(SoOutput * out, const float val)
{
  out->write(val);
}

void
SoSFFloat::writeValue(SoOutput * out) const
{
  sosffloat_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
