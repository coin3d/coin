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
  \class SoMFBitMask SoMFBitMask.h Inventor/fields/SoMFBitMask.h
  \brief The SoMFBitMask class is a container for a set of bitmasks.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple bitmasks with values from an enumerated set.

  \sa SoSFBitMask

*/

#include <Inventor/fields/SoMFBitMask.h>
#include <SoSubFieldPrivate.h>
#include <Inventor/fields/SoSFBitMask.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_DERIVED_SOURCE(SoMFBitMask, not_used, not_used);

// Override from parent class.
void
SoMFBitMask::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFBitMask);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFBitMask::read1Value(SoInput * in, int idx)
{
  SoSFBitMask sfbitmask;
  sfbitmask.setEnums(this->numEnums, this->enumValues, this->enumNames);
  SbBool result;
  if ((result = sfbitmask.readValue(in)))
    this->set1Value(idx, sfbitmask.getValue());
  return result;
}

void
SoMFBitMask::write1Value(SoOutput * out, int idx) const
{
  SoSFBitMask sfbitmask;
  sfbitmask.setEnums(this->numEnums, this->enumValues, this->enumNames);
  sfbitmask.setValue((*this)[idx]);
  sfbitmask.writeValue(out);
}

#endif // DOXYGEN_SKIP_THIS
