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
  \class SoMFFloat SoMFFloat.h Inventor/fields/SoMFFloat.h
  \brief The SoMFFloat class is a container for floating point values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple floating point values.

  \sa SoSFFloat
*/

#include <Inventor/fields/SoMFFloat.h>
#include <SoSubFieldPrivate.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_SOURCE_MALLOC(SoMFFloat, float, float);


// Override from parent.
void
SoMFFloat::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFFloat);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFFloat class.
extern SbBool sosffloat_read_value(SoInput * in, float & val);
extern void sosffloat_write_value(SoOutput * out, const float val);

SbBool
SoMFFloat::read1Value(SoInput * in, int idx)
{
  float val;
  if (!sosffloat_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFFloat::write1Value(SoOutput * out, int idx) const
{
  sosffloat_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFFloat::getNumValuesPerLine(void) const
{
  return 4;
}
