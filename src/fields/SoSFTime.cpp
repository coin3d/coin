/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoSFTime SoSFTime.h Inventor/fields/SoSFTime.h
  \brief The SoSFTime class is a container for an SbTime value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single time representation.

  A field of this type stores its value to file as a floating
  point number.

  \sa SoMFTime

*/

#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFTime, SbTime, const SbTime &);


// Override from parent class.
void
SoSFTime::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFTime);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read SbTime from input stream, return TRUE if successful. Also
// used from SoMFTime class.
SbBool
sosftime_read_value(SoInput * in, SbTime & t)
{
  double val;
  if (!in->read(val)) {
    SoReadError::post(in, "Couldn't read double value");
    return FALSE;
  }

  t.setValue(val);
  return TRUE;
}

SbBool
SoSFTime::readValue(SoInput * in)
{
  SbTime t;
  if (!sosftime_read_value(in, t)) return FALSE;
  this->setValue(t);
  return TRUE;
}

// Write SbTime value to output stream. Also used from SoMFTime class.
void
sosftime_write_value(SoOutput * out, const SbTime & p)
{
  out->write(p.getValue());
}

void
SoSFTime::writeValue(SoOutput * out) const
{
  sosftime_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS
