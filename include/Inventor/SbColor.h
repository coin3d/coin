#ifndef COIN_SBCOLOR_H
#define COIN_SBCOLOR_H

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
#include <Inventor/SbVec3f.h>

class COIN_DLL_API SbColor : public SbVec3f {
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
