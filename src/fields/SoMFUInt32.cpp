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
  \class SoMFUInt32 SoMFUInt32.h Inventor/fields/SoMFUInt32.h
  \brief The SoMFUInt32 class is a container for 32-bit unsigned integer values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple 32-bit unsigned integer values.

  \sa SoSFUInt32
*/

#include <Inventor/fields/SoMFUInt32.h>
#include <SoSubFieldPrivate.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFUInt32, uint32_t, uint32_t);


// Override from parent class.
void
SoMFUInt32::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFUInt32);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFUInt32 class.
extern SbBool sosfuint32_read_value(SoInput * in, uint32_t & val);
extern void sosfuint32_write_value(SoOutput * out, const uint32_t val);

SbBool
SoMFUInt32::read1Value(SoInput * in, int idx)
{
  uint32_t val;
  if (!sosfuint32_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFUInt32::write1Value(SoOutput * out, int idx) const
{
  sosfuint32_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFUInt32::getNumValuesPerLine(void) const
{
  return 8;
}
