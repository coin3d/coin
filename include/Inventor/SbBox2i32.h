#ifndef COIN_SBBOX2I32_H
#define COIN_SBBOX2I32_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2i32.h>

class COIN_DLL_API SbBox2i32 {
public:
  SbBox2i32(void);
  SbBox2i32(int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax);
  SbBox2i32(const SbVec2i32 & boxmin, const SbVec2i32 & boxmax);
  ~SbBox2i32(void);

  const SbVec2i32 & getMin(void) const;
  const SbVec2i32 & getMax(void) const;
  void extendBy(const SbVec2i32 & point);
  void extendBy(const SbBox2i32 & box);
  SbBool intersect(const SbVec2i32 & point) const;
  SbBool intersect(const SbBox2i32 & box) const;
  void setBounds(int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax);
  void setBounds(const SbVec2i32 & boxmin, const SbVec2i32 & boxmax);
  void getBounds(int32_t & xmin, int32_t & ymin, int32_t & xmax, int32_t & ymax) const;
  void getBounds(SbVec2i32 & boxmin, SbVec2i32 & boxmax) const;
  void getOrigin(int32_t & originX, int32_t & originY) const;
  void getSize(int32_t & sizeX, int32_t & sizeY) const;
  float getAspectRatio(void) const;
  void makeEmpty(void);
  friend COIN_DLL_API int operator ==(const SbBox2i32 & b1, const SbBox2i32 & b2);
  friend COIN_DLL_API int operator !=(const SbBox2i32 & b1, const SbBox2i32 & b2);

private:
  SbVec2i32 minpt, maxpt;

  int32_t width(void) const;
  int32_t height(void) const;
  SbBool hasArea(void) const;
};

COIN_DLL_API int operator ==(const SbBox2i32 & b1, const SbBox2i32 & b2);
COIN_DLL_API int operator !=(const SbBox2i32 & b1, const SbBox2i32 & b2);

#endif // !COIN_SBBOX2I32_H
