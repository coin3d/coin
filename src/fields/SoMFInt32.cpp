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
  \class SoMFInt32 SoMFInt32.h Inventor/fields/SoMFInt32.h
  \brief The SoMFInt32 class is a container for 32-bit integer values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple 32-bit integer values.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFInt32
*/


#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_SOURCE_MALLOC(SoMFInt32, int32_t, int32_t);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFInt32, int32_t, int32_t);


// Override from parent.
void
SoMFInt32::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFInt32);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFInt32 class.
extern SbBool sosfint32_read_value(SoInput * in, int32_t & val);
extern void sosfint32_write_value(SoOutput * out, int32_t val);

SbBool
SoMFInt32::read1Value(SoInput * in, int idx)
{
  int32_t val;
  if (!sosfint32_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFInt32::write1Value(SoOutput * out, int idx) const
{
  sosfint32_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFInt32::getNumValuesPerLine(void) const
{
  return 8;
}
