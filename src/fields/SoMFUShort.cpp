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
  \class SoMFUShort SoMFUShort.h Inventor/fields/SoMFUShort.h
  \brief The SoMFUShort class is a container for unsigned short integer values.
  \ingroup fields

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFUShort.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFUShort, unsigned short, unsigned short);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFUShort, unsigned short, unsigned short);

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
extern void sosfushort_write_value(SoOutput * out, unsigned short val);

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
