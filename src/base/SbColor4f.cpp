/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SbColor4f SbColor4f.h Inventor/SbColor4f.h
  \brief The SbColor4f class contains the red, green, blue and alpha
  components which make up a color value.
  \ingroup base

  This class is used internally within other classes in Coin.  It contains
  a 4 component vector as a position in the RGB cube with an additional
  transparency value.

  The red, green and blue values should be between 0.0 and 1.0, where
  0.0 is interpreted as minimum intensity, and 1.0 is maximum intensity.
  The transparency value is also between 0.0 and 1.0.

  SbColor4f contains methods for convenient handling of setting and
  getting color values as 32 bit packed values or as HSV values.

  Note: this class is not part of Open Inventor, but is an extension to
  the API. Don't use it if you want your code to be compatible with
  Open Inventor.

  \sa SbColor
*/

#include <assert.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/SbVec4f.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  Default constructor. The color value will be uninitialized until
  explicitly set.
 */
SbColor4f::SbColor4f(void)
{
}

/*!
  Construct and initialize an SbColor4f with the red, green, blue
  and alpha values given by the \c v vector.
 */
SbColor4f::SbColor4f(const SbVec4f& v)
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  this->vec[2] = v[2];
  this->vec[3] = v[3];
}

/*!
  Construct and initialize an SbColor4f with the red, green, blue
  and alpha taken from given \c rgba array
 */
SbColor4f::SbColor4f(const float* const rgba)
{
  this->vec[0] = rgba[0];
  this->vec[1] = rgba[1];
  this->vec[2] = rgba[2];
  this->vec[3] = rgba[3];
}

/*!
  Construct and initialize an SbColor4f with the red, green and blue
  components from the SbColor \c rgb, and the alpha value from the
  supplied \c alpha argument.
*/
SbColor4f::SbColor4f(const SbColor &rgb, const float alpha)
{
  this->vec[0] = rgb[0];
  this->vec[1] = rgb[1];
  this->vec[2] = rgb[2];
  this->vec[3] = alpha;
}

/*!
  Construct and initialize an SbColor4f with the given red, green, blue
  and alpha values.
 */
SbColor4f::SbColor4f(const float r, const float g, const float b,
                   const float a)
{
  this->vec[0] = r;
  this->vec[1] = g;
  this->vec[2] = b;
  this->vec[3] = a;
}


/*!
  Set a new color.
 */
void
SbColor4f::setValue(const float r, const float g, const float b,
                   const float a)
{
  this->vec[0] = r;
  this->vec[1] = g;
  this->vec[2] = b;
  this->vec[3] = a;
}

/*!
  Set a new color. The elements of the array will be read in turned
  as red, green, blue and transparency.
 */
void
SbColor4f::setValue(const float col[4])
{
  this->vec[0] = col[0];
  this->vec[1] = col[1];
  this->vec[2] = col[2];
  this->vec[3] = col[3];
}

/*!
  Return pointer to array of 4 float values representing the red, green,
  blue and transparency values of the color.
 */
const float *
SbColor4f::getValue() const
{
  return this->vec;
}

/*!
  Return components of the stored color.
 */
void
SbColor4f::getValue(float &r, float &g, float &b, float &a)
{
  r = this->vec[0];
  g = this->vec[1];
  b = this->vec[2];
  a = this->vec[3];
}


/*!
  Set the color value as a 32 bit combined red/green/blue/alpha value.
  Each component is 8 bit wide (i.e. from 0x00 to 0xff), and the red
  value should be stored leftmost, like this: 0xRRGGBBAA.

  \sa getPackedValue().
 */
SbColor4f&
SbColor4f::setPackedValue(const uint32_t rgba)
{
  this->setValue((rgba >> 24)/255.0f,
                 ((rgba >> 16)&0xff)/255.0f,
                 ((rgba >> 8)&0xff)/255.0f,
                 (rgba & 0xff)/255.0f);
  return *this;
}

/*!
  Return color as a 32 bit packed integer in the form 0xRRGGBBAA.
  \sa setPackedValue().
 */
uint32_t
SbColor4f::getPackedValue() const
{
  return (((uint32_t)(red()*255.0f + 0.5f) << 24) |
          ((uint32_t)(green()*255.0f + 0.5f) << 16) |
          ((uint32_t)(blue()*255.0f + 0.5f) << 8) |
          (uint32_t)(alpha()*255.0f + 0.5f));
}

/*!
  Sets the RGB components of the color. The alpha component is
  left unchanged.
*/
SbColor4f&
SbColor4f::setRGB(const SbColor &col)
{
  this->vec[0] = col[0];
  this->vec[1] = col[1];
  this->vec[2] = col[2];
  return *this;
}

/*!
  Returns the RGB components of this color.
*/
void
SbColor4f::getRGB(SbColor &color)
{
  color[0] = this->red();
  color[1] = this->green();
  color[2] = this->blue();
}

/*!
  Set the color as a \c hue, \c saturation, \c value triplet.
  The hue component should be normalized to within [0, 1] before you
  call this method, where 0 is equal to 0° and 1 is equal to 360°.

  \sa getHSVValue().
 */
SbColor4f&
SbColor4f::setHSVValue(float hue, float saturation,
                      float value, float alpha)
{
#if COIN_DEBUG
  if (!(hue>=0.0f && hue<=1.0f)) {
    SoDebugError::postWarning("SbColor4f::setHSVValue",
                              "'hue' (%f) not within [0.0,1.0]; clamping.",
                              hue);
    if (hue<0.0f) hue=0.0f;
    else if (hue>1.0f) hue=1.0f;
  }

  if (!(saturation>=0.0f && saturation<=1.0f)) {
    SoDebugError::postWarning("SbColor4f::setHSVValue",
                              "'saturation' (%f) not within [0.0,1.0]; "
                              "clamping.", saturation);
    if (saturation<0.0f) saturation=0.0f;
    else if (saturation>1.0f) saturation=1.0f;
  }

  if (!(value>=0.0f && value<=1.0f)) {
    SoDebugError::postWarning("SbColor4f::setHSVValue",
                              "'value' (%f) not within [0.0,1.0]; clamping.",
                              value);
    if (value<0.0f) value=0.0f;
    else if (value>1.0f) value=1.0f;
  }
  if (!(alpha >= 0.0f && alpha <= 1.0f)) {
    SoDebugError::postWarning("SbColor4f::setHSVValue",
                              "'alpha' (%f) not within [0.0,1.0]; clamping.",
                              alpha);
    alpha = SbClamp(alpha, 0.0f, 1.0f);
  }

#endif // COIN_DEBUG

  SbColor col;
  col.setHSVValue(hue, saturation, value);
  this->vec[0] = col[0];
  this->vec[1] = col[1];
  this->vec[2] = col[2];
  this->vec[3] = alpha;
  return *this;
}

/*!
  Return the color as a \c hue, \c saturation, \c value triplet. Alpha
  component is ignored.

  \sa setHSVValue().
 */
void
SbColor4f::getHSVValue(float &h, float &s, float &v) const
{
  SbColor col;
  col[0] = this->vec[0];
  col[1] = this->vec[1];
  col[2] = this->vec[2];
  col.getHSVValue(h, s, v);
}

/*!
  Set the color as a \c hue, \c saturation, \c value triplet.
  The hue component should be normalized to within [0, 1] before you
  call this method, where 0 is equal to 0° and 1 is equal to 360°.

  \sa getHSVValue().
 */
SbColor4f&
SbColor4f::setHSVValue(const float hsv[3], float a)
{
  return this->setHSVValue(hsv[0], hsv[1], hsv[2], a);
}

/*!
  Return the color as a \c hue, \c saturation, \c value triplet. Alpha
  component is ignored.

  \sa setHSVValue().
 */
void
SbColor4f::getHSVValue(float hsv[3]) const
{
  this->getHSVValue(hsv[0], hsv[1], hsv[2]);
}

/*!
  Returns the color component represented by the given index \a idx.
  0 is red, 1 is green, 2 is blue and 3 is the transparency value.
 */
//$ EXPORT INLINE
float
SbColor4f::operator[](const int idx) const
{
  return this->vec[idx];
}

/*!
  Returns the color component represented by the given index \a idx.
  0 is red, 1 is green, 2 is blue and 3 is the transparency value.
 */
//$ EXPORT INLINE
float &
SbColor4f::operator[](const int idx)
{
  return this->vec[idx];
}

/*!
  Multiplies the RGB components by \c d. The alpha component is left
  unchanged.
*/
SbColor4f &
SbColor4f::operator*=(const float d)
{
  this->vec[0] *= d;
  this->vec[1] *= d;
  this->vec[2] *= d;
  return *this;
}

/*!
  Divides the RGB components by \c d. The alpha component is left
  unchanged.
*/
SbColor4f &
SbColor4f::operator/=(const float d)
{
  this->vec[0] /= d;
  this->vec[1] /= d;
  this->vec[2] /= d;
  return *this;
}

/*!
  Adds the RGB components. Alpha is ignored.
*/
SbColor4f &
SbColor4f::operator+=(const SbColor4f &c)
{
  this->vec[0] += c[0];
  this->vec[1] += c[1];
  this->vec[2] += c[2];
  return *this;
}

/*!
  Subtracts the RGB components. Alpha is ignored.
*/
SbColor4f &
SbColor4f::operator-=(const SbColor4f &c)
{
  this->vec[0] -= c[0];
  this->vec[1] -= c[1];
  this->vec[2] -= c[2];
  return *this;
}

/*!
  Multiplies the RGB components by \c d. Alpha is left unchanged.
*/
SbColor4f
operator *(const SbColor4f &c, const float d)
{
  return SbColor4f(c.vec[0]*d, c.vec[1]*d, c.vec[2]*d, c.vec[3]);
}

/*!
  Multiplies the RGB components by \c d. Alpha is left unchanged.
*/
SbColor4f
operator *(const float d, const SbColor4f &c)
{
  return SbColor4f(c.vec[0]*d, c.vec[1]*d, c.vec[2]*d, c.vec[3]);
}

/*!
  Divides the RGB components by \c d. Alpha is left unchanged.
*/
SbColor4f
operator /(const SbColor4f &c, const float d)
{
  assert(d != 0.0f);
  float inv = 1.0f / d;
  return SbColor4f(c.vec[0]*inv,
                  c.vec[1]*inv,
                  c.vec[2]*inv,
                  c.vec[3]);
}

/*!
  Adds the RGB components of the two colors. Alpha is taken from the
  first color (\c v1).
*/
SbColor4f
operator +(const SbColor4f &v1, const SbColor4f &v2)
{
  return SbColor4f(v1.vec[0] + v2.vec[0],
                  v1.vec[1] + v2.vec[1],
                  v1.vec[2] + v2.vec[2],
                  v1.vec[3]);
}

/*!
  Subtracts the RGB components of the two colors. Alpha is taken from the
  first color (\c v1).
*/
SbColor4f
operator -(const SbColor4f &v1, const SbColor4f &v2)
{
  return SbColor4f(v1.vec[0] - v2.vec[0],
                  v1.vec[1] - v2.vec[1],
                  v1.vec[2] - v2.vec[2],
                  v1.vec[3]);
}

/*!
  Check if two colors are equal. Returns 1 if equal, 0 if unequal.
 */
int
operator ==(const SbColor4f &v1, const SbColor4f &v2)
{
  return (v1.vec[0] == v2.vec[0] &&
          v1.vec[1] == v2.vec[1] &&
          v1.vec[2] == v2.vec[2] &&
          v1.vec[3] == v2.vec[3]);
}

/*!
  Check if two colors are unequal. Returns 0 if equal, 1 if unequal.
 */
//$ EXPORT INLINE
int
operator !=(const SbColor4f &v1, const SbColor4f &v2)
{
  return !(v1 == v2);
}
