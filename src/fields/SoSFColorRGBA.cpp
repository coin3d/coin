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
  \class SoSFColorRGBA SoSFColorRGBA.h Inventor/fields/SoSFColorRGBA.h
  \brief The SoSFColorRGBA class is a container for an SbColor4f value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single color value (i.e. Red + Green + Blue + Alpha).

  Fields of this type stores their value to file as a "R G B A" quadruple
  component, where each color component value is between 0.0 and 1.0.

  \sa SbColor4f, SoMFColorRGBA
*/

// *************************************************************************

#include <Inventor/fields/SoSFColorRGBA.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_SFIELD_SOURCE(SoSFColorRGBA, SbColor4f, const SbColor4f &);

// *************************************************************************

// Override from parent.
void
SoSFColorRGBA::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFColorRGBA);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoSFColorRGBA::readValue(SoInput * in)
{
  return
    in->read(this->value[0]) &&
    in->read(this->value[1]) &&
    in->read(this->value[2]) &&
    in->read(this->value[3]);
}

void
SoSFColorRGBA::writeValue(SoOutput * out) const
{
  sosfvec4f_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set color value from a vector with three elements. The three elements
  will be interpreted as red, green and blue, respectively.
*/
void
SoSFColorRGBA::setValue(const SbVec4f & vec)
{
  this->value.setValue(vec[0], vec[1], vec[2], vec[3]);
  this->valueChanged();
}

/*!
  Set color value from \a red, \a green and \a blue. Value range
  for each component is between 0.0 and 1.0.
*/
void
SoSFColorRGBA::setValue(float red, float green, float blue, float alpha)
{
  this->value.setValue(red, green, blue, alpha);
  this->valueChanged();
}

/*!
  Set color value from a floating point number array with three
  elements. The three elements will be interpreted as red, green and
  blue, respectively.
*/
void
SoSFColorRGBA::setValue(const float rgba[4])
{
  this->value.setValue(rgba);
  this->valueChanged();
}

/*!
  Set color value from \a h, \a s and \a v, where \a is "hue", \a s
  is "saturation" and \a v is "value".
*/
void
SoSFColorRGBA::setHSVValue(float h, float s, float v, float a)
{
  this->value.setHSVValue(h, s, v, a);
  this->valueChanged();
}

/*!
  Set color value from a floating point number array with three
  elements. The three elements will be interpreted as hue,
  saturation and value, respectively.
*/
void
SoSFColorRGBA::setHSVValue(const float hsva[4])
{
  this->value.setHSVValue(hsva);
  this->valueChanged();
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoSFColorRGBA field;
  BOOST_CHECK_MESSAGE(SoSFColorRGBA::getClassTypeId() != SoType::badType(),
                      "SoSFColorRGBA class not initialized");
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
}

BOOST_AUTO_TEST_CASE(textinput)
{
  SbBool ok;
  SoSFColorRGBA field;
  ok = field.set("0.0 .5 0 1");
  BOOST_CHECK_MESSAGE(ok == TRUE, "could not set value");
  BOOST_CHECK_EQUAL(field.getValue(), SbColorRGBA(0, .5, 0, 1));
  ok = field.set("0 0.5 1");
  BOOST_CHECK_MESSAGE(ok == FALSE, "accepted invalid (missing component) value");
  ok = field.set("1 2 3 4");
  //BOOST_CHECK_MESSAGE(ok == FALSE, "accepted out-of-range value");
}

#endif // COIN_TEST_SUITE
