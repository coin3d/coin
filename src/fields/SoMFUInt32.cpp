/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoMFUInt32 SoMFUInt32.h Inventor/fields/SoMFUInt32.h
  \brief The SoMFUInt32 class is a container for 32-bit unsigned integer values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple 32-bit unsigned integer values.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFUInt32
*/

#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_MFIELD_SOURCE_MALLOC(SoMFUInt32, uint32_t, uint32_t);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFUInt32, uint32_t, uint32_t);


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
extern void sosfuint32_write_value(SoOutput * out, uint32_t val);

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
