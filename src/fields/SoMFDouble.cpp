/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoMFDouble SoMFDouble.h Inventor/fields/SoMFDouble.h
  \brief The SoMFDouble class is a container for double float precision point values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a group of multiple floating point values.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFDouble
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoMFDouble.h>

#include <cassert>

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/shared.h"
#include "fields/SoSubFieldP.h"

// *************************************************************************

SO_MFIELD_SOURCE_MALLOC(SoMFDouble, double, double);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFDouble, double, double);

// *************************************************************************

// Override from parent.
void
SoMFDouble::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFDouble);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFDouble::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  return in->read(this->values[idx]);
}

void
SoMFDouble::write1Value(SoOutput * out, int idx) const
{
  sosfdouble_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


// Store more than the default single value on each line for ASCII
// format export.
int
SoMFDouble::getNumValuesPerLine(void) const
{
  return 4;
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoMFDouble field;
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
  BOOST_CHECK_EQUAL(field.getNum(), 0);
}

#endif // COIN_TEST_SUITE
