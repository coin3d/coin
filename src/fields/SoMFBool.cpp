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
  \class SoMFBool SoMFBool.h Inventor/fields/SoMFBool.h
  \brief The SoMFBool class is a container for SbBool values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple boolean on/off or TRUE/FALSE values.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFBool 
*/

#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_SOURCE_MALLOC(SoMFBool, SbBool, SbBool);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFBool, SbBool, SbBool);

// Override from parent.
void
SoMFBool::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFBool);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFBool class.
extern SbBool sosfbool_read_value(SoInput * in, SbBool & val);
extern void sosfbool_write_value(SoOutput * out, SbBool val);

SbBool
SoMFBool::read1Value(SoInput * in, int idx)
{
  SbBool val;
  if (!sosfbool_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFBool::write1Value(SoOutput * out, int idx) const
{
  sosfbool_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Don't store the default single value on each line for ASCII format
// export.
int
SoMFBool::getNumValuesPerLine(void) const
{
  return 8;
}
