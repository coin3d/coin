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
  \class SoSFBool SoSFBool.h Inventor/fields/SoSFBool.h
  \brief The SoSFBool class is a container for an SbBool value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a boolean on/off or TRUE/FALSE value, like for
  instance as the "on" field of the SoPointLight, SoSpotLight and
  SoDirectionalLight node classes.

  \sa SoMFBool
*/

// *************************************************************************

#include <Inventor/fields/SoSFBool.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFBool, SbBool, SbBool);

// *************************************************************************

// Override from parent.
void
SoSFBool::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBool);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFBool::readValue(SoInput * in)
{
  SbBool val;
  if (!sosfbool_read_value(in, val)) return FALSE;
  this->value = val;
  return TRUE;
}

void
SoSFBool::writeValue(SoOutput * out) const
{
  sosfbool_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFBool field;
  BOOST_CHECK_MESSAGE(SoSFBool::getClassTypeId() != SoType::badType(),
                      "SoSFBool class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

BOOST_AUTO_TEST_CASE(textinput)
{
  SbBool ok;
  SoSFBool field;
  ok = field.set("TRUE");
  BOOST_CHECK_MESSAGE(ok == TRUE, "did not accept 'TRUE'");
  BOOST_CHECK_EQUAL(field.getValue(), TRUE);
  ok = field.set("FALSE");
  BOOST_CHECK_MESSAGE(ok == TRUE, "did not accept 'FALSE'");
  BOOST_CHECK_EQUAL(field.getValue(), FALSE);

  TestSuite::ResetReadErrorCount();
  static const char * filters[] = { "Invalid value", NULL };
  TestSuite::PushMessageSuppressFilters(filters);
  ok = field.set("MAYBE"); // emits two error messages
  BOOST_CHECK_MESSAGE(ok == FALSE, "did accept 'MAYBE'");
  BOOST_CHECK_MESSAGE(TestSuite::GetReadErrorCount() == 1, "did not emit error");
  TestSuite::PopMessageSuppressFilters();
  TestSuite::ResetReadErrorCount();

  ok = field.set("0");
  BOOST_CHECK_MESSAGE(ok == TRUE, "did not accept '0'");
  BOOST_CHECK_EQUAL(field.getValue(), FALSE);
  ok = field.set("1");
  BOOST_CHECK_MESSAGE(ok == TRUE, "did not accept '1'");
  BOOST_CHECK_EQUAL(field.getValue(), TRUE);

  static const char * filters2[] = { "Illegal value", NULL };
  TestSuite::PushMessageSuppressFilters(filters2);
  ok = field.set("2");
  BOOST_CHECK_MESSAGE(ok == FALSE, "did accept '2'");
  TestSuite::PopMessageSuppressFilters();
  TestSuite::ResetReadErrorCount();
}

#endif // COIN_TEST_SUITE
