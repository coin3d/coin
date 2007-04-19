/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoMFTime SoMFTime.h Inventor/fields/SoMFTime.h
  \brief The SoMFTime class is a container for SbTime values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple time representations.

  \sa SoSFTime
*/

// *************************************************************************

#include <assert.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/errors/SoDebugError.h>

#include "shared.h"

// *************************************************************************

SO_MFIELD_SOURCE(SoMFTime, SbTime, const SbTime &);

// *************************************************************************

// Override from parent class.
void
SoMFTime::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFTime);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFTime::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  SbTime p;
  if (!sosftime_read_value(in, p)) return FALSE;
  this->values[idx] = p;
  return TRUE;
}

void
SoMFTime::write1Value(SoOutput * out, int idx) const
{
  sosftime_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************
