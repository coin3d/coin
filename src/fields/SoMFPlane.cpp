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
  \class SoMFPlane SoMFPlane.h Inventor/fields/SoMFPlane.h
  \brief The SoMFPlane class is a container for SbPlane values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple 3D plane definitions.

  \sa SoSFPlane

*/

#include <Inventor/fields/SoMFPlane.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_SOURCE(SoMFPlane, SbPlane, const SbPlane &);


// Override from parent class.
void
SoMFPlane::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFPlane);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFPlane class.
extern SbBool sosfplane_read_value(SoInput * in, SbPlane & val);
extern void sosfplane_write_value(SoOutput * out, const SbPlane & val);

SbBool
SoMFPlane::read1Value(SoInput * in, int idx)
{
  SbPlane p;
  if (!sosfplane_read_value(in, p)) return FALSE;
  this->set1Value(idx, p);
  return TRUE;
}

void
SoMFPlane::write1Value(SoOutput * out, int idx) const
{
  sosfplane_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFPlane::getNumValuesPerLine(void) const
{
  return 8;
}
