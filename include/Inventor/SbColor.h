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

#ifndef COIN_SBCOLOR_H
#define COIN_SBCOLOR_H

#include <Inventor/system/inttypes.h>
#include <Inventor/SbVec3f.h>

class SbColor : public SbVec3f {
public:
  SbColor(void);
  SbColor(const SbVec3f& v);
  SbColor(const float* const rgb);
  SbColor(const float r, const float g, const float b);

  SbColor & setHSVValue(float h, float s, float v);
  SbColor & setHSVValue(const float hsv[3]);
  void getHSVValue(float &h, float &s, float &v) const;
  void getHSVValue(float hsv[3]) const;
  SbColor & setPackedValue(const uint32_t rgba, float& transparency);
  uint32_t getPackedValue(const float transparency = 0.0f) const;

private:
  float red(void) const { return (*this)[0]; }
  float green(void) const { return (*this)[1]; }
  float blue(void) const { return (*this)[2]; }
  uint32_t convertToUInt(const float val) { return (uint32_t)(val*255.0f);}
};

#endif // !COIN_SBCOLOR_H
