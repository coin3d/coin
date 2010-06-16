#ifndef COIN_SBBOX3F_H
#define COIN_SBBOX3F_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/SbVec3f.h>

class SbBox3d;
class SbBox3i32;
class SbBox3s;

class SbMatrix;

class COIN_DLL_API SbBox3f {
public:
  SbBox3f(void) { makeEmpty(); }
  SbBox3f(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
    : minpt(xmin, ymin, zmin), maxpt(xmax, ymax, zmax) { }
  SbBox3f(const SbVec3f & minpoint, const SbVec3f & maxpoint)
    : minpt(minpoint), maxpt(maxpoint) { }
  explicit SbBox3f(const SbBox3d & box) { setBounds(box); }
  explicit SbBox3f(const SbBox3s & box) { setBounds(box); }
  explicit SbBox3f(const SbBox3i32 & box) { setBounds(box); }

  SbBox3f & setBounds(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
    { minpt.setValue(xmin, ymin, zmin); maxpt.setValue(xmax, ymax, zmax); return *this; }
  SbBox3f & setBounds(const SbVec3f & minpoint, const SbVec3f & maxpoint)
    { minpt = minpoint; maxpt = maxpoint; return *this; }
  SbBox3f & setBounds(const SbBox3d & box);
  SbBox3f & setBounds(const SbBox3s & box);
  SbBox3f & setBounds(const SbBox3i32 & box);

  void getBounds(float & xmin, float & ymin, float & zmin, float & xmax, float & ymax, float & zmax) const
    { minpt.getValue(xmin, ymin, zmin); maxpt.getValue(xmax, ymax, zmax); }
  void getBounds(SbVec3f & minpoint, SbVec3f & maxpoint) const
    { minpoint = minpt; maxpoint = maxpt; }

  const SbVec3f & getMin(void) const { return minpt; }
  SbVec3f & getMin(void) { return minpt; }
  const SbVec3f & getMax(void) const { return maxpt; }
  SbVec3f & getMax(void) { return maxpt; }

  void extendBy(const SbVec3f & pt);
  void extendBy(const SbBox3f & box);
  void transform(const SbMatrix & matrix);
  void makeEmpty(void);
  SbBool isEmpty(void) const { return maxpt[0] < minpt[0]; }
  SbBool hasVolume(void) const
    { return ((maxpt[0] > minpt[0]) && (maxpt[1] > minpt[1]) && (maxpt[2] > minpt[2])); }
  float getVolume(void) const
    { float dx = 0.0f, dy = 0.0f, dz = 0.0f; getSize(dx, dy, dz); return (dx * dy * dz); }

  SbBool intersect(const SbVec3f & pt) const;
  SbBool intersect(const SbBox3f & box) const;
  SbVec3f getClosestPoint(const SbVec3f & point) const;
  SbBool outside(const SbMatrix & mvp, int & cullbits) const;

  SbVec3f getCenter(void) const { return (minpt + maxpt) * 0.5f; }
  void getOrigin(float & originX, float & originY, float & originZ) const
    { minpt.getValue(originX, originY, originZ); }
  void getSize(float & sizeX, float & sizeY, float & sizeZ) const
    { if (isEmpty()) { sizeX = sizeY = sizeZ = 0; }
      else { sizeX = maxpt[0] - minpt[0]; sizeY = maxpt[1] - minpt[1]; sizeZ = maxpt[2] - minpt[2]; } }

  SbVec3f getSize(void) const {
    SbVec3f v;
    this->getSize(v[0], v[1], v[2]);
    return v;
  }
  void getSpan(const SbVec3f & dir, float & dmin, float & dmax) const;

  void print(FILE * file) const;

private:
  SbVec3f minpt, maxpt;

}; // SbBox3f

COIN_DLL_API inline int operator == (const SbBox3f & b1, const SbBox3f & b2) {
  return ((b1.getMin() == b2.getMin()) && (b1.getMax() == b2.getMax()));
}

COIN_DLL_API inline int operator != (const SbBox3f & b1, const SbBox3f & b2) {
  return !(b1 == b2);
}

#endif // !COIN_SBBOX3F_H
