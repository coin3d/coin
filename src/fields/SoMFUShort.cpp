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
  \class SoMFUShort SoMFUShort.h Inventor/fields/SoMFUShort.h
  \brief The SoMFUShort class is a container for unsigned short integer values.
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFUShort.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFUShort, unsigned short, unsigned short);

// Override from parent class.
void
SoMFUShort::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFUShort);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFUShort class.
extern SbBool sosfushort_read_value(SoInput * in, unsigned short & val);
extern void sosfushort_write_value(SoOutput * out, const unsigned short val);

SbBool
SoMFUShort::read1Value(SoInput * in, int idx)
{
  unsigned short val;
  if (!sosfushort_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFUShort::write1Value(SoOutput * out, int idx) const
{
  sosfushort_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFUShort::getNumValuesPerLine(void) const
{
  return 8;
}
