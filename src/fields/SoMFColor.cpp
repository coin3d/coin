/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoMFColor SoMFColor.h Inventor/fields/SoMFColor.h
  \brief The SoMFColor class is a container for SbColor values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple color values (i.e. "Red Green Blue"
  triplets).

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFColor

*/

#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoSubFieldP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE(SoMFColor, SbColor, const SbColor &);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFColor, SbColor, float);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFColor, SbColor, SbColor);

// Override from parent.
void
SoMFColor::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFColor);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFColor class.
extern SbBool sosfcolor_read_value(SoInput * in, SbColor & val);
extern void sosfcolor_write_value(SoOutput * out, const SbColor & val);

SbBool
SoMFColor::read1Value(SoInput * in, int idx)
{
  SbColor val;
  if (!sosfcolor_read_value(in, val)) return FALSE;
  this->set1Value(idx, val);
  return TRUE;
}

void
SoMFColor::write1Value(SoOutput * out, int idx) const
{
  sosfcolor_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set \a num RGB color values, starting at index \a start.
*/
void
SoMFColor::setValues(const int start, const int num, const float rgb[][3])
{
  if(start+num > this->maxNum) this->makeRoom(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(rgb[i]);
  this->valueChanged();
}

/*!
  Set \a num HSV color values, starting at index \a start.
*/
void
SoMFColor::setHSVValues(const int start, const int num, const float hsv[][3])
{
  if(start+num > this->maxNum) this->makeRoom(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setHSVValue(hsv[i]);
  this->valueChanged();
}

/*!
  Set the color array to a single value. \a vec is interpreted as
  a three element vector with the red, green and blue components,
  respectively.
*/
void
SoMFColor::setValue(const SbVec3f & vec)
{
  this->setValue(vec[0], vec[1], vec[2]);
}

/*!
  Set the color array to a single value. \a r, \a g and \a b are the
  red, green and blue components, respectively.
*/
void
SoMFColor::setValue(const float r, const float g, const float b)
{
  this->setValue(SbColor(r, g, b));
}

/*!
  Set the color array to a single value. \a rgb is a three element
  vector with the red, green and blue components, respectively.
*/
void
SoMFColor::setValue(const float rgb[3])
{
  this->setValue(SbColor(rgb));
}

/*!
  Set the color array to a single value. \a h, \a s and \a v are the
  hue, saturation and value components, respectively.
*/
void
SoMFColor::setHSVValue(const float h, const float s, const float v)
{
  SbColor col;
  col.setHSVValue(h, s, v);
  this->setValue(col);
}

/*!
  Set the color array to a single value. \a hsv is a three element
  vector with the hue, saturation and value components, respectively.
*/
void
SoMFColor::setHSVValue(const float hsv[3])
{
  this->setHSVValue(hsv[0], hsv[1], hsv[2]);
}

/*!
  Set the color at \a idx. \a vec is interpreted as a three element
  vector with the red, green and blue components, respectively.
*/
void
SoMFColor::set1Value(const int idx, const SbVec3f & vec)
{
  this->set1Value(idx, SbColor(vec));
}

/*!
  Set the color at \a idx. \a r, \a g and \a b is the red, green and
  blue components, respectively.
*/
void
SoMFColor::set1Value(const int idx, const float r, const float g, const float b)
{
  this->set1Value(idx, SbColor(r, g, b));
}

/*!
  Set the color at \a idx. \a rgb is interpreted as a three element
  vector with the red, green and blue components, respectively.
*/
void
SoMFColor::set1Value(const int idx, const float rgb[3])
{
  this->set1Value(idx, SbColor(rgb));
}

/*!
  Set the color at \a idx. \a h, \a s and \a v is the hue, saturation and
  value components, respectively.
*/
void
SoMFColor::set1HSVValue(const int idx,
                        const float h, const float s, const float v)
{
  SbColor col;
  col.setHSVValue(h, s, v);
  this->set1Value(idx, col);
}

/*!
  Set the color at \a idx. \a hsv is a three element vector with the
  hue, saturation and value components, respectively.
*/
void
SoMFColor::set1HSVValue(const int idx, const float hsv[3])
{
  this->set1HSVValue(idx, hsv[0], hsv[1], hsv[2]);
}
