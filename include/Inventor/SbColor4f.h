#ifndef COIN_SBCOLOR4F_H
#define COIN_SBCOLOR4F_H

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

#include <Inventor/system/inttypes.h>
#include <Inventor/SbColor.h>

class SbVec4f;

class COIN_DLL_API SbColor4f {
public:
  SbColor4f(void);
  SbColor4f(const SbColor &rgb, const float alpha);
  SbColor4f(const SbVec4f& v);
  SbColor4f(const float* const rgba);
  SbColor4f(const float r, const float g, const float b, const float a = 1.0f);

  void setValue(const float r, const float g, const float b,
                const float a = 1.0f);
  void setValue(const float col[4]);
  const float *getValue() const;
  void getValue(float &r, float &g, float &b, float &a);


  SbColor4f& setRGB(const SbColor &col);
  void getRGB(SbColor &color);
  SbColor4f& setHSVValue(float h, float s, float v, float a = 1.0f);
  SbColor4f& setHSVValue(const float hsv[3], float alpha = 1.0f);
  void getHSVValue(float &h, float &s, float &v) const;
  void getHSVValue(float hsv[3]) const;
  SbColor4f& setPackedValue(const uint32_t rgba);
  uint32_t getPackedValue() const;

  float operator[](const int idx) const;
  float &operator[](const int idx);

  SbColor4f &operator*=(const float d);
  SbColor4f &operator/=(const float d);
  SbColor4f &operator+=(const SbColor4f &c);
  SbColor4f &operator-=(const SbColor4f &c);

  friend COIN_DLL_API SbColor4f operator *(const SbColor4f &c, const float d);
  friend COIN_DLL_API SbColor4f operator *(const float d, const SbColor4f &c);
  friend COIN_DLL_API SbColor4f operator /(const SbColor4f &c, const float d);
  friend COIN_DLL_API SbColor4f operator +(const SbColor4f &v1, const SbColor4f &v2);
  friend COIN_DLL_API SbColor4f operator -(const SbColor4f &v1, const SbColor4f &v2);
  friend COIN_DLL_API int operator ==(const SbColor4f &v1, const SbColor4f &v2);
  friend COIN_DLL_API int operator !=(const SbColor4f &v1, const SbColor4f &v2);

private:
  float vec[4];
  float red() const { return this->vec[0]; }
  float green() const { return this->vec[1]; }
  float blue() const { return this->vec[2]; }
  float alpha() const { return this->vec[3]; }
};

COIN_DLL_API SbColor4f operator *(const SbColor4f &c, const float d);
COIN_DLL_API SbColor4f operator *(const float d, const SbColor4f &c);
COIN_DLL_API SbColor4f operator /(const SbColor4f &c, const float d);
COIN_DLL_API SbColor4f operator +(const SbColor4f &v1, const SbColor4f &v2);
COIN_DLL_API SbColor4f operator -(const SbColor4f &v1, const SbColor4f &v2);
COIN_DLL_API int operator ==(const SbColor4f &v1, const SbColor4f &v2);
COIN_DLL_API int operator !=(const SbColor4f &v1, const SbColor4f &v2);

#endif // !COIN_SBCOLOR4F_H
