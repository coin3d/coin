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
  \class SoSFVec2b SoSFVec2b.h Inventor/fields/SoSFVec2b.h
  \brief The SoSFVec2b class is a container for an SbVec2b vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with two elements.

  \sa SbVec2b, SoMFVec2b
  \since 20070518
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec2b.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSubFieldP.h>

#include "shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec2b, SbVec2b, SbVec2b);

// *************************************************************************

// Override from parent class.
void
SoSFVec2b::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec2b);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec2b::readValue(SoInput * in)
{
  return 
    in->readByte(this->value[0]) &&
    in->readByte(this->value[1]);
}

void
SoSFVec2b::writeValue(SoOutput * out) const
{
  sosfvec2b_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFVec2b::setValue(int8_t x, int8_t y)
{
  this->setValue(SbVec2b(x, y));
}

/*!
  Set value of vector.
*/
void
SoSFVec2b::setValue(const int8_t xy[2])
{
  this->setValue(SbVec2b(xy));
}

// *************************************************************************
