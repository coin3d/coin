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
