/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoMFShort SoMFShort.h Inventor/fields/SoMFShort.h
  \brief The SoMFShort class is a container for short integer values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple short integer values.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFShort

*/

#include <Inventor/fields/SoMFShort.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_MFIELD_SOURCE_MALLOC(SoMFShort, short, short);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFShort, short, short);

// Override from parent class.
void
SoMFShort::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFShort);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFShort class.
extern SbBool sosfshort_read_value(SoInput * in, short & val);
extern void sosfshort_write_value(SoOutput * out, short val);

SbBool
SoMFShort::read1Value(SoInput * in, int idx)
{
  short val;
  if (!sosfshort_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFShort::write1Value(SoOutput * out, int idx) const
{
  sosfshort_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFShort::getNumValuesPerLine(void) const
{
  return 8;
}
