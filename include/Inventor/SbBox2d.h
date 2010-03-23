#ifndef COIN_SBBOX2D_H
#define COIN_SBBOX2D_H

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

#include <Inventor/SbVec2d.h>

class SbBox2f;
class SbBox2s;
class SbBox2i32;

class COIN_DLL_API SbBox2d {
public:
  SbBox2d(void) { makeEmpty(); }
  SbBox2d(double xmin, double ymin, double xmax, double ymax)
    : minpt(xmin, ymin), maxpt(xmax, ymax) { }
  SbBox2d(const SbVec2d & minpoint, const SbVec2d & maxpoint)
    : minpt(minpoint), maxpt(maxpoint) { }
  explicit SbBox2d(const SbBox2f & box) { setBounds(box); }
  explicit SbBox2d(const SbBox2s & box) { setBounds(box); }
  explicit SbBox2d(const SbBox2i32 & box) { setBounds(box); }

  SbBox2d & setBounds(double xmin, double ymin, double xmax, double ymax)
    { minpt.setValue(xmin, ymin); maxpt.setValue(xmax, ymax); return *this; }
  SbBox2d & setBounds(const SbVec2d & minpoint, const SbVec2d & maxpoint)
    { minpt = minpoint; maxpt = maxpoint; return *this; }
  SbBox2d & setBounds(const SbBox2f & box);
  SbBox2d & setBounds(const SbBox2s & box);
  SbBox2d & setBounds(const SbBox2i32 & box);

  void getBounds(double & xmin, double & ymin, double & xmax, double & ymax) const
    { minpt.getValue(xmin, ymin); maxpt.getValue(xmax, ymax); }
  void getBounds(SbVec2d & minpoint, SbVec2d & maxpoint) const
    { minpoint = minpt; maxpoint = maxpt; }

  const SbVec2d & getMin(void) const { return minpt; }
  SbVec2d & getMin(void) { return minpt; }
  const SbVec2d & getMax(void) const { return maxpt; }
  SbVec2d & getMax(void) { return maxpt; }

  void extendBy(const SbVec2d & point);
  void extendBy(const SbBox2d & box);
  void makeEmpty(void);
  SbBool isEmpty(void) const { return (maxpt[0] < minpt[0]); }
  SbBool hasArea(void) const { return ((maxpt[0] > minpt[0]) && (maxpt[1] > minpt[1])); }

  SbBool intersect(const SbVec2d & point) const;
  SbBool intersect(const SbBox2d & box) const;
  SbVec2d getClosestPoint(const SbVec2d & p) const;
  SbBool findIntersection(const SbVec2d & a, const SbVec2d & b, SbVec2d & ia, SbVec2d & ib) const;

  SbVec2d getCenter(void) const { return (minpt + maxpt) * 0.5; }
  void getOrigin(double & originX, double & originY) const
    { minpt.getValue(originX, originY); }
  void getSize(double & sizeX, double & sizeY) const
    { if (isEmpty()) { sizeX = sizeY = 0.0; }
      else { sizeX = maxpt[0] - minpt[0]; sizeY = maxpt[1] - minpt[1]; } }
  SbVec2d getSize(void) const {
    SbVec2d v;
    this->getSize(v[0], v[1]);
    return v;
  }
  double getAspectRatio(void) const
    { SbDividerChk("SbBox2d::getAspectRatio()", maxpt[1] - minpt[1]);
      return (maxpt[0] - minpt[0]) / (maxpt[1] - minpt[1]); }

private:
  SbVec2d minpt, maxpt;

}; // SbBox2d

COIN_DLL_API inline int operator == (const SbBox2d & b1, const SbBox2d & b2) {
  return ((b1.getMin() == b2.getMin()) && (b1.getMax() == b2.getMax()));
}

COIN_DLL_API inline int operator != (const SbBox2d & b1, const SbBox2d & b2) {
  return !(b1 == b2);
}

#endif // !COIN_SBBOX2D_H
