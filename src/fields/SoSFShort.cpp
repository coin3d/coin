/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
