#ifndef COIN_SBBOX2F_H
#define COIN_SBBOX2F_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2f.h>

class COIN_DLL_API SbBox2f {
public:
  SbBox2f(void);
  SbBox2f(float xmin, float ymin, float xmax, float ymax);
  SbBox2f(const SbVec2f & min, const SbVec2f & max);
  ~SbBox2f(void);

  const SbVec2f & getMin(void) const;
  const SbVec2f & getMax(void) const;
  SbVec2f getCenter(void) const;
  void extendBy(const SbVec2f & point);
  void extendBy(const SbBox2f & box);
  SbBool intersect(const SbVec2f & point) const;
  SbBool intersect(const SbBox2f & box) const;
  SbVec2f getClosestPoint(const SbVec2f & p) const;
  void setBounds(float xmin, float ymin, float xmax, float ymax);
  void setBounds(const SbVec2f & min, const SbVec2f & max);
  void getBounds(float & xmin, float & ymin, float & xmax, float & ymax) const;
  void getBounds(SbVec2f & min, SbVec2f & max) const;
  void getOrigin(float & x0, float & y0) const;
  void getSize(float & w, float & h) const;
  float getAspectRatio(void) const;
  void makeEmpty(void);
  SbBool isEmpty(void) const;
  SbBool hasArea(void) const;

  friend COIN_DLL_API int operator ==(const SbBox2f & b1, const SbBox2f & b2);
  friend COIN_DLL_API int operator !=(const SbBox2f & b1, const SbBox2f & b2);

private:
  SbVec2f minpt, maxpt;

  float width(void) const;
  float height(void) const;
};

COIN_DLL_API int operator ==(const SbBox2f & b1, const SbBox2f & b2);
COIN_DLL_API int operator !=(const SbBox2f & b1, const SbBox2f & b2);

#endif // !COIN_SBBOX2F_H
