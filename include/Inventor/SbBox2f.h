#ifndef COIN_SBBOX2F_H
#define COIN_SBBOX2F_H

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

#include <Inventor/SbVec2f.h>

class SbBox2d;
class SbBox2s;
class SbBox2i32;

class COIN_DLL_API SbBox2f {
public:
  SbBox2f(void) { makeEmpty(); }
  SbBox2f(float xmin, float ymin, float xmax, float ymax)
    : minpt(xmin, ymin), maxpt(xmax, ymax) { }
  SbBox2f(const SbVec2f & minpoint, const SbVec2f & maxpoint)
    : minpt(minpoint), maxpt(maxpoint) { }
  explicit SbBox2f(const SbBox2d & box) { setBounds(box); }
  explicit SbBox2f(const SbBox2s & box) { setBounds(box); }
  explicit SbBox2f(const SbBox2i32 & box) { setBounds(box); }

  SbBox2f & setBounds(float xmin, float ymin, float xmax, float ymax)
    { minpt.setValue(xmin, ymin); maxpt.setValue(xmax, ymax); return *this; }
  SbBox2f & setBounds(const SbVec2f & minpoint, const SbVec2f & maxpoint)
    { minpt = minpoint; maxpt = maxpoint; return *this; }
  SbBox2f & setBounds(const SbBox2d & box);
  SbBox2f & setBounds(const SbBox2s & box);
  SbBox2f & setBounds(const SbBox2i32 & box);

  void getBounds(float & xmin, float & ymin, float & xmax, float & ymax) const
    { minpt.getValue(xmin, ymin); maxpt.getValue(xmax, ymax); }
  void getBounds(SbVec2f & minpoint, SbVec2f & maxpoint) const
    { minpoint = minpt; maxpoint = maxpt; }

  const SbVec2f & getMin(void) const { return minpt; }
  SbVec2f & getMin(void) { return minpt; }

  const SbVec2f & getMax(void) const { return maxpt; }
  SbVec2f & getMax(void) { return maxpt; }

  void extendBy(const SbVec2f & point);
  void extendBy(const SbBox2f & box);
  void makeEmpty(void);
  SbBool isEmpty(void) const { return (maxpt[0] < minpt[0]); }
  SbBool hasArea(void) const { return ((maxpt[0] > minpt[0]) && (maxpt[1] > minpt[1])); }

  SbBool intersect(const SbVec2f & point) const;
  SbBool intersect(const SbBox2f & box) const;
  SbVec2f getClosestPoint(const SbVec2f & point) const;

  SbVec2f getCenter(void) const { return (minpt + maxpt) * 0.5f; }
  void getOrigin(float & originX, float & originY) const
    { minpt.getValue(originX, originY); }
  void getSize(float & sizeX, float & sizeY) const
    { if (isEmpty()) { sizeX = sizeY = 0.0f; }
      else { sizeX = maxpt[0] - minpt[0]; sizeY = maxpt[1] - minpt[1]; } }
  SbVec2f getSize(void) const {
    SbVec2f v;
    this->getSize(v[0], v[1]);
    return v;
  }

  float getAspectRatio(void) const
    { SbDividerChk("SbBox2f::getAspectRatio()", maxpt[1] - minpt[1]);
      return (maxpt[0] - minpt[0]) / (maxpt[1] - minpt[1]); }

protected:
  SbVec2f minpt, maxpt;

}; // SbBox2f

COIN_DLL_API inline int operator == (const SbBox2f & b1, const SbBox2f & b2) {
  return ((b1.getMin() == b2.getMin()) && (b1.getMax() == b2.getMax()));
}

COIN_DLL_API inline int operator != (const SbBox2f & b1, const SbBox2f & b2) {
  return !(b1 == b2);
}

#endif // !COIN_SBBOX2F_H
