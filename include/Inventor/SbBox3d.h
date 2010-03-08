#ifndef COIN_SBBOX3D_H
#define COIN_SBBOX3D_H

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

#include <Inventor/SbVec3d.h>

class SbBox3f;
class SbBox3s;
class SbBox3i32;

class SbDPMatrix;

class COIN_DLL_API SbBox3d {
public:
  SbBox3d(void) { makeEmpty(); }
  SbBox3d(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
    : minpt(xmin, ymin, zmin), maxpt(xmax, ymax, zmax) { }
  SbBox3d(const SbVec3d & minpoint, const SbVec3d & maxpoint)
    : minpt(minpoint), maxpt(maxpoint) { }
  explicit SbBox3d(const SbBox3f & box) { setBounds(box); }
  explicit SbBox3d(const SbBox3s & box) { setBounds(box); }
  explicit SbBox3d(const SbBox3i32 & box) { setBounds(box); }

  SbBox3d & setBounds(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
    { minpt.setValue(xmin, ymin, zmin); maxpt.setValue(xmax, ymax, zmax); return *this; }
  SbBox3d & setBounds(const SbVec3d & minpoint, const SbVec3d & maxpoint)
    { minpt = minpoint; maxpt = maxpoint; return *this; }
  SbBox3d & setBounds(const SbBox3f & box);
  SbBox3d & setBounds(const SbBox3s & box);
  SbBox3d & setBounds(const SbBox3i32 & box);

  void getBounds(double & xmin, double & ymin, double & zmin, double & xmax, double & ymax, double & zmax) const
    { minpt.getValue(xmin, ymin, zmin); maxpt.getValue(xmax, ymax, zmax); }
  void getBounds(SbVec3d & minpoint, SbVec3d & maxpoint) const
    { minpoint = minpt; maxpoint = maxpt; }

  const SbVec3d & getMin(void) const { return minpt; }
  SbVec3d & getMin(void) { return minpt; }
  const SbVec3d & getMax(void) const { return maxpt; }
  SbVec3d & getMax(void) { return maxpt; }

  void extendBy(const SbVec3d & pt);
  void extendBy(const SbBox3d & box);
  void transform(const SbDPMatrix & matrix);
  void makeEmpty(void);
  SbBool isEmpty(void) const { return (maxpt[0] < minpt[0]); }
  SbBool hasVolume(void) const
    { return ((maxpt[0] > minpt[0]) && (maxpt[1] > minpt[1]) && (maxpt[2] > minpt[2])); }
  double getVolume(void) const
    { double dx = 0.0, dy = 0.0, dz = 0.0; getSize(dx, dy, dz); return (dx * dy * dz); }

  SbBool intersect(const SbVec3d & point) const;
  SbBool intersect(const SbBox3d & box) const;
  SbVec3d getClosestPoint(const SbVec3d & point) const;
  SbBool outside(const SbDPMatrix & mvp, int & cullbits) const;

  SbVec3d getCenter(void) const { return (minpt + maxpt) * 0.5; }
  void getOrigin(double & origoX, double & origoY, double & origoZ) const
    { minpt.getValue(origoX, origoY, origoZ); }
  void getSize(double & sizeX, double & sizeY, double & sizeZ) const
    { if (isEmpty()) { sizeX = sizeY = sizeZ = 0.0; }
      else { sizeX = maxpt[0] - minpt[0]; sizeY = maxpt[1] - minpt[1]; sizeZ = maxpt[2] - minpt[2]; } }
  SbVec3d getSize(void) const {
    SbVec3d v;
    this->getSize(v[0], v[1], v[2]);
    return v;
  }
  void getSpan(const SbVec3d & dir, double & dmin, double & dmax) const;

  void print(FILE * file) const;

protected:
  SbVec3d minpt, maxpt;

}; // SbBox3d

COIN_DLL_API inline int operator == (const SbBox3d & b1, const SbBox3d & b2) {
  return ((b1.getMin() == b2.getMin()) && (b1.getMax() == b2.getMax()));
}

COIN_DLL_API inline int operator != (const SbBox3d & b1, const SbBox3d & b2) {
  return !(b1 == b2);
}

#endif // !COIN_SBBOX3D_H
