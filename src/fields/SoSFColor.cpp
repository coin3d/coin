/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoSFColor SoSFColor.h Inventor/fields/SoSFColor.h
  \brief The SoSFColor class is a container for an SbColor value.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a single color value (i.e. Red + Green + Blue).

  Fields of this type stores their value to file as a "R G B" triple
  component, where each color component value is between 0.0 and 1.0.

  \sa SoMFColor

*/

#include <Inventor/fields/SoSFColor.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoSFVec3f.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_SFIELD_SOURCE(SoSFColor, SbColor, const SbColor &);

// Override from parent.
void
SoSFColor::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFColor);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Read SbColor value from input stream, return TRUE if
// successful. Also used from SoMFColor class.
SbBool
sosfcolor_read_value(SoInput * in, SbColor & val)
{
  if (in->read(val[0]) && in->read(val[1]) && in->read(val[2]))
    return TRUE;

  SoReadError::post(in, "Premature end of file");
  return FALSE;
}

SbBool
SoSFColor::readValue(SoInput * in)
{
  SbColor val;
  if (!sosfcolor_read_value(in, val)) return FALSE;
  this->value = val;
  this->valueChanged();
  return TRUE;
}

// Write SbColor value to output stream. Also used from SoMFColor
// class.
void
sosfcolor_write_value(SoOutput * out, const SbColor & val)
{
  out->write(val[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(val[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(val[2]);
}

void
SoSFColor::writeValue(SoOutput * out) const
{
  sosfcolor_write_value(out, this->getValue());
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set color value from a vector with three elements. The three elements
  will be interpreted as red, green and blue, respectively.
*/
void
SoSFColor::setValue(const SbVec3f & vec)
{
  this->value.setValue(vec[0], vec[1], vec[2]);
  this->valueChanged();
}

/*!
  Set color value from \a red, \a green and \a blue. Value range
  for each component is between 0.0 and 1.0.
*/
void
SoSFColor::setValue(const float red, const float green, const float blue)
{
  this->value.setValue(red, green, blue);
  this->valueChanged();
}

/*!
  Set color value from a floating point number array with three
  elements. The three elements will be interpreted as red, green and
  blue, respectively.
*/
void
SoSFColor::setValue(const float rgb[3])
{
  this->value.setValue(rgb);
  this->valueChanged();
}

/*!
  Set color value from \a h, \a s and \a v, where \a is "hue", \a s
  is "saturation" and \a v is "value".
*/
void
SoSFColor::setHSVValue(const float h, const float s, const float v)
{
  this->value.setHSVValue(h, s, v);
  this->valueChanged();
}

/*!
  Set color value from a floating point number array with three
  elements. The three elements will be interpreted as hue,
  saturation and value, respectively.
*/
void
SoSFColor::setHSVValue(const float hsv[3])
{
  this->value.setHSVValue(hsv);
  this->valueChanged();
}

void
SoSFColor::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFVec3f::getClassTypeId()) {
    ((SoSFVec3f *)dest)->setValue(this->getValue());
  }
#if 0 // OBSOLETED: don't use libstdc++ stuff. 20000219 mortene
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    ostr << this->getValue()[0] << " " <<
      this->getValue()[1] << " " <<
      this->getValue()[2] << '\0';
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // OBSOLETED
  else if (dest->getTypeId()==SoMFColor::getClassTypeId()) {
    ((SoMFColor *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFColor::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
