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
  \class SoMFPlane SoMFPlane.h Inventor/fields/SoMFPlane.h
  \brief The SoMFPlane class is a container for SbPlane values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple 3D plane definitions.

  \sa SoSFPlane

*/

#include <Inventor/fields/SoMFPlane.h>
#include <SoSubFieldPrivate.h>
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
