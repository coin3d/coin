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
  \class SoMFColorRGBA SoMFColorRGBA.h Inventor/fields/SoMFColorRGBA.h
  \brief The SoMFColorRGBA class is a container for SbColor4f values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple color values (i.e. "Red Green Blue"
  triplets).

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SbColor4f, SoSFColorRGBA

*/

// *************************************************************************

#include <Inventor/fields/SoMFColorRGBA.h>

#include <cassert>

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/shared.h"
#include "fields/SoSubFieldP.h"

// *************************************************************************

SO_MFIELD_SOURCE(SoMFColorRGBA, SbColor4f, const SbColor4f &);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFColorRGBA, SbColor4f, float);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFColorRGBA, SbColor4f, SbColor4f);

// Override from parent.
void
SoMFColorRGBA::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFColorRGBA);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFColorRGBA::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  return
    in->read(this->values[idx][0]) &&
    in->read(this->values[idx][1]) &&
    in->read(this->values[idx][2]);
}

void
SoMFColorRGBA::write1Value(SoOutput * out, int idx) const
{
  sosfvec4f_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set \a num RGB color values, starting at index \a start.
*/
void
SoMFColorRGBA::setValues(int start, int numarg, const float rgba[][4])
{
  if(start+numarg > this->maxNum) this->makeRoom(start+numarg);
  else if(start+numarg > this->num) this->num = start+numarg;

  for(int i=0; i < numarg; i++) this->values[i+start].setValue(rgba[i]);
  this->valueChanged();
}

/*!
  Set \a num HSV color values, starting at index \a start.
*/
void
SoMFColorRGBA::setHSVValues(int start, int numarg, const float hsva[][4])
{
  if(start+numarg > this->maxNum) this->makeRoom(start+numarg);
  else if(start+numarg > this->num) this->num = start+numarg;

  for(int i=0; i < numarg; i++) this->values[i+start].setHSVValue(hsva[i]);
  this->valueChanged();
}

/*!
  Set the color array to a single value. \a vec is interpreted as
  a three element vector with the red, green and blue components,
  respectively.
*/
void
SoMFColorRGBA::setValue(const SbVec4f & vec)
{
  this->setValue(vec[0], vec[1], vec[2], vec[3]);
}

/*!
  Set the color array to a single value. \a r, \a g and \a b are the
  red, green and blue components, respectively.
*/
void
SoMFColorRGBA::setValue(float r, float g, float b, float a)
{
  this->setValue(SbColor4f(r, g, b, a));
}

/*!
  Set the color array to a single value. \a rgb is a three element
  vector with the red, green and blue components, respectively.
*/
void
SoMFColorRGBA::setValue(const float rgba[4])
{
  this->setValue(SbColor4f(rgba));
}

/*!
  Set the color array to a single value. \a h, \a s and \a v are the
  hue, saturation and value components, respectively.
*/
void
SoMFColorRGBA::setHSVValue(float h, float s, float v, float a)
{
  SbColor4f col;
  col.setHSVValue(h, s, v, a);
  this->setValue(col);
}

/*!
  Set the color array to a single value. \a hsv is a three element
  vector with the hue, saturation and value components, respectively.
*/
void
SoMFColorRGBA::setHSVValue(const float hsva[4])
{
  this->setHSVValue(hsva[0], hsva[1], hsva[2], hsva[3]);
}

/*!
  Set the color at \a idx. \a vec is interpreted as a three element
  vector with the red, green and blue components, respectively.
*/
void
SoMFColorRGBA::set1Value(int idx, const SbVec4f & vec)
{
  this->set1Value(idx, SbColor4f(vec));
}

/*!
  Set the color at \a idx. \a r, \a g and \a b is the red, green and
  blue components, respectively.
*/
void
SoMFColorRGBA::set1Value(int idx, float r, float g, float b, float a)
{
  this->set1Value(idx, SbColor4f(r, g, b, a));
}

/*!
  Set the color at \a idx. \a rgb is interpreted as a three element
  vector with the red, green and blue components, respectively.
*/
void
SoMFColorRGBA::set1Value(int idx, const float rgba[4])
{
  this->set1Value(idx, SbColor4f(rgba));
}

/*!
  Set the color at \a idx. \a h, \a s and \a v is the hue, saturation and
  value components, respectively.
*/
void
SoMFColorRGBA::set1HSVValue(int idx, float h, float s, float v, float a)
{
  SbColor4f col;
  col.setHSVValue(h, s, v, a);
  this->set1Value(idx, col);
}

/*!
  Set the color at \a idx. \a hsv is a three element vector with the
  hue, saturation and value components, respectively.
*/
void
SoMFColorRGBA::set1HSVValue(int idx, const float hsva[3])
{
  this->set1HSVValue(idx, hsva[0], hsva[1], hsva[2], hsva[3]);
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoMFColorRGBA field;
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
  BOOST_CHECK_EQUAL(field.getNum(), 0);
}

#endif // COIN_TEST_SUITE
