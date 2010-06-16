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
  \class SoSFVec4us SoSFVec4us.h Inventor/fields/SoSFVec4us.h
  \brief The SoSFVec4us class is a container for an SbVec4us vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single vector with four elements.

  \sa SbVec4us, SoMFVec4us
  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFVec4us.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFVec4us, SbVec4us, const SbVec4us &);

// *************************************************************************

// Override from parent class.
void
SoSFVec4us::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFVec4us);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFVec4us::readValue(SoInput * in)
{
  return
    in->read(this->value[0]) &&
    in->read(this->value[1]) &&
    in->read(this->value[2]) &&
    in->read(this->value[3]);
}

void
SoSFVec4us::writeValue(SoOutput * out) const
{
  sosfvec4us_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set value of vector.
*/
void
SoSFVec4us::setValue(unsigned short x, unsigned short y, unsigned short z, unsigned short w)
{
  this->setValue(SbVec4us(x, y, z, w));
}

/*!
  Set value of vector.
*/
void
SoSFVec4us::setValue(const unsigned short xyzw[4])
{
  this->setValue(SbVec4us(xyzw));
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFVec4us field;
  BOOST_CHECK_MESSAGE(SoSFVec4us::getClassTypeId() != SoType::badType(),
                      "SoSFVec4us class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

BOOST_AUTO_TEST_CASE(textinput)
{
  SoSFVec4us field;
  field.set("1 2 3 4");
  BOOST_CHECK_EQUAL(field.getValue(), SbVec4us(1, 2, 3, 4));
  const char * filters[] = { "read error", NULL }; // all read error messages
  TestSuite::ResetReadErrorCount();
  // TestSuite::PushMessageSuppressFilters(filters);
  SbBool ok;
  ok = field.set("-3 4 32 3"); // should emit error message on '-3'
  BOOST_CHECK_EQUAL(ok, FALSE);
  //BOOST_CHECK_EQUAL(TestSuite::GetReadErrorCount(), 1);
  ok = field.set("3 525 32 3"); // should emit error message on '525'
  //BOOST_CHECK_EQUAL(ok, FALSE);
  //BOOST_CHECK_EQUAL(TestSuite::GetReadErrorCount(), 2);
  ok = field.set("3 32 3"); // error on account of too few numbers
  BOOST_CHECK_EQUAL(ok, FALSE);
  //BOOST_CHECK_EQUAL(TestSuite::GetReadErrorCount(), 3);
  // TestSuite::PopMessageSuppressFilters();
  TestSuite::ResetReadErrorCount();
}

#endif // COIN_TEST_SUITE
