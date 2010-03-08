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

// *************************************************************************

#include <Inventor/fields/SoMFBool.h>

#include <cassert>

#include <Inventor/errors/SoDebugError.h>

#include "fields/shared.h"
#include "fields/SoSubFieldP.h"

// *************************************************************************

SO_MFIELD_SOURCE_MALLOC(SoMFBool, SbBool, SbBool);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFBool, SbBool, SbBool);

// *************************************************************************

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

SbBool
SoMFBool::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  SbBool val;
  if (!sosfbool_read_value(in, val)) return FALSE;
  this->values[idx] = val;
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

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoMFBool field;
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
  BOOST_CHECK_EQUAL(field.getNum(), 0);
}

BOOST_AUTO_TEST_CASE(array_ops)
{
  SoMFBool field;
  field.set1Value(0, TRUE);
  field.set1Value(1, FALSE);
  field.set1Value(2, TRUE);
  BOOST_CHECK_EQUAL(field.getNum(), 3);
  field.deleteValues(1,1);
  BOOST_CHECK_EQUAL(field.getNum(), 2);
  BOOST_CHECK_EQUAL(field[0], TRUE);
  BOOST_CHECK_EQUAL(field[1], TRUE);
}

#endif // COIN_TEST_SUITE
