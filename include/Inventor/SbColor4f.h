/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SBCOLOR4F_H__
#define __SBCOLOR4F_H__

#include <Inventor/system/inttypes.h>
#include <Inventor/SbColor.h>

class SbVec4f;

class SbColor4f {
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

  friend SbColor4f operator *(const SbColor4f &c, const float d);
  friend SbColor4f operator *(const float d, const SbColor4f &c);
  friend SbColor4f operator /(const SbColor4f &c, const float d);
  friend SbColor4f operator +(const SbColor4f &v1, const SbColor4f &v2);
  friend SbColor4f operator -(const SbColor4f &v1, const SbColor4f &v2);
  friend int operator ==(const SbColor4f &v1, const SbColor4f &v2);
  friend int operator !=(const SbColor4f &v1, const SbColor4f &v2);

private:
  float vec[4];
  float red() const { return this->vec[0]; }
  float green() const { return this->vec[1]; }
  float blue() const { return this->vec[2]; }
  float alpha() const { return this->vec[3]; }
};

#endif // !__SBCOLOR4F_H__
