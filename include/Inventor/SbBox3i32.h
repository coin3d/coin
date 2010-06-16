#ifndef COIN_SBBOX3I32_H
#define COIN_SBBOX3I32_H

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

#include <Inventor/SbVec3i32.h>
#include <Inventor/SbVec3f.h>

class SbBox3s;
class SbBox3f;
class SbBox3d;
class SbMatrix;

class COIN_DLL_API SbBox3i32 {
public:
  SbBox3i32(void) { makeEmpty(); }
  SbBox3i32(int32_t xmin, int32_t ymin, int32_t zmin, int32_t xmax, int32_t ymax, int32_t zmax)
    : minpt(xmin, ymin, zmin), maxpt(xmax, ymax, zmax) { }
  SbBox3i32(const SbVec3i32 & minpoint, const SbVec3i32 & maxpoint)
    : minpt(minpoint), maxpt(maxpoint) { }
  explicit SbBox3i32(const SbBox3s & box) { setBounds(box); }
  explicit SbBox3i32(const SbBox3f & box) { setBounds(box); }
  explicit SbBox3i32(const SbBox3d & box) { setBounds(box); }

  SbBox3i32 & setBounds(int32_t xmin, int32_t ymin, int32_t zmin, int32_t xmax, int32_t ymax, int32_t zmax)
    { minpt.setValue(xmin, ymin, zmin); maxpt.setValue(xmax, ymax, zmax); return *this; }
  SbBox3i32 & setBounds(const SbVec3i32 & minpoint, const SbVec3i32 & maxpoint)
    { minpt = minpoint; maxpt = maxpoint; return *this; }
  SbBox3i32 & setBounds(const SbBox3s & box);
  SbBox3i32 & setBounds(const SbBox3f & box);
  SbBox3i32 & setBounds(const SbBox3d & box);

  void getBounds(int32_t & xmin, int32_t & ymin, int32_t & zmin, int32_t & xmax, int32_t & ymax, int32_t & zmax) const
    { minpt.getValue(xmin, ymin, zmin); maxpt.getValue(xmax, ymax, zmax); }
  void getBounds(SbVec3i32 & minpoint, SbVec3i32 & maxpoint) const
    { minpoint = minpt; maxpoint = maxpt; }

  const SbVec3i32 & getMin(void) const { return minpt; }
  SbVec3i32 & getMin(void) { return minpt; }
  const SbVec3i32 & getMax(void) const { return maxpt; }
  SbVec3i32 & getMax(void) { return maxpt; }

  void extendBy(const SbVec3i32 & pt);
  void extendBy(const SbBox3i32 & bb);
  void extendBy(const SbVec3f & pt);
  void transform(const SbMatrix & m);
  void makeEmpty(void);
  SbBool isEmpty(void) const { return (maxpt[0] < minpt[0]); }
  SbBool hasVolume(void) const
    { return ((maxpt[0] > minpt[0]) && (maxpt[1] > minpt[1]) && (maxpt[2] > minpt[2])); }
  float getVolume(void) const
    { int32_t dx = 0, dy = 0, dz = 0; getSize(dx, dy, dz); return (float(dx) * float(dy) * float(dz)); }

  SbBool intersect(const SbVec3i32 & pt) const;
  SbBool intersect(const SbBox3i32 & bb) const;
  SbBool intersect(const SbVec3f & pt) const;

  SbBool outside(const SbMatrix & MVP, int & cullBits) const;
  SbVec3f getClosestPoint(const SbVec3f & pt) const;

  SbVec3f getCenter(void) const { return SbVec3f(minpt + maxpt) * 0.5f; }
  void getOrigin(int32_t & originX, int32_t & originY, int32_t & originZ) const
    { minpt.getValue(originX, originY, originZ); }
  void getSize(int32_t & sizeX, int32_t & sizeY, int32_t & sizeZ) const
    { if (isEmpty()) { sizeX = sizeY = sizeZ = 0; }
      else { sizeX = maxpt[0] - minpt[0]; sizeY = maxpt[1] - minpt[1]; sizeZ = maxpt[2] - minpt[2]; } }

  SbVec3i32 getSize(void) const {
    SbVec3i32 v;
    this->getSize(v[0], v[1], v[2]);
    return v;
  }

  void getSpan(const SbVec3f & direction, float & dmin, float & dmax) const;

protected:
  SbVec3i32 minpt, maxpt;

}; // SbBox3i32

COIN_DLL_API inline int operator == (const SbBox3i32 & b1, const SbBox3i32 & b2)
{
  return ((b1.getMin() == b2.getMin()) && (b1.getMax() == b2.getMax()));
}

COIN_DLL_API inline int operator != (const SbBox3i32 & b1, const SbBox3i32 & b2)
{
  return !(b1 == b2);
}

#endif // !COIN_SBBOX3I32_H
