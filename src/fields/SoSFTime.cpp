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
#include <SoSubFieldPrivate.h>
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
