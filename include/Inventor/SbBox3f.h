#ifndef COIN_SBBOX3F_H
#define COIN_SBBOX3F_H

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

#include <stdio.h>

#include <Inventor/SbVec3f.h>

class SbMatrix;

class COIN_DLL_API SbBox3f {
public:
  SbBox3f(void);
  SbBox3f(const float minx, const float miny, const float minz,
          const float maxx, const float maxy, const float maxz);
  SbBox3f(const SbVec3f & min, const SbVec3f & max);
  ~SbBox3f();

  const SbVec3f & getMin(void) const;
  const SbVec3f & getMax(void) const;
  SbVec3f & getMin(void);
  SbVec3f & getMax(void);

  SbVec3f getCenter(void) const;

  void extendBy(const SbVec3f & point);
  void extendBy(const SbBox3f & box);

  SbBool intersect(const SbVec3f & point) const;
  SbBool intersect(const SbBox3f & box) const;

  void setBounds(const float minx, const float miny, const float minz,
                 const float maxx, const float maxy, const float maxz);
  void setBounds(const SbVec3f & min, const SbVec3f & max);
  void getBounds(float & minx, float & miny, float & minz,
                 float & maxx, float & maxy, float & maxz) const;
  void getBounds(SbVec3f & min, SbVec3f & max) const;

  void getOrigin(float & x0, float & y0, float & z0) const;
  void getSize(float & dx, float & dy, float & dz) const;

  void makeEmpty(void);
  SbBool isEmpty(void) const;
  SbBool hasVolume(void) const;

  void getSpan(const SbVec3f & dir, float & dmin, float & dmax) const;
  void transform(const SbMatrix & matrix);
  float getVolume(void) const;

  SbBool outside(const SbMatrix & mvp, int & cullbits) const;
  SbVec3f getClosestPoint(const SbVec3f & point) const;

  friend COIN_DLL_API int operator ==(const SbBox3f & b1, const SbBox3f & b2);
  friend COIN_DLL_API int operator !=(const SbBox3f & b1, const SbBox3f & b2);

  void print(FILE * file) const;

private:
  SbVec3f min, max;
};

COIN_DLL_API int operator ==(const SbBox3f & b1, const SbBox3f & b2);
COIN_DLL_API int operator !=(const SbBox3f & b1, const SbBox3f & b2);

#endif // !COIN_SBBOX3F_H
