/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBCOLOR4F_H
#define COIN_SBCOLOR4F_H

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
