#ifndef COIN_SBBOX2S_H
#define COIN_SBBOX2S_H

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

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>

class SbBox2i32;
class SbBox2f;
class SbBox2d;

class COIN_DLL_API SbBox2s {
public:
  SbBox2s(void) { makeEmpty(); }
  SbBox2s(short xmin, short ymin, short xmax, short ymax)
    : minpt(xmin, ymin), maxpt(xmax, ymax) { }
  SbBox2s(const SbVec2s & minpoint, const SbVec2s & maxpoint)
    : minpt(minpoint), maxpt(maxpoint) { }
  explicit SbBox2s(const SbBox2i32 & box) { setBounds(box); }
  explicit SbBox2s(const SbBox2f & box) { setBounds(box); }
  explicit SbBox2s(const SbBox2d & box) { setBounds(box); }

  SbBox2s & setBounds(short xmin, short ymin, short xmax, short ymax)
    { minpt.setValue(xmin, ymin); maxpt.setValue(xmax, ymax); return *this; }
  SbBox2s & setBounds(const SbVec2s & minpoint, const SbVec2s & maxpoint)
    { minpt = minpoint; maxpt = maxpoint; return *this; }
  SbBox2s & setBounds(const SbBox2i32 & box);
  SbBox2s & setBounds(const SbBox2f & box);
  SbBox2s & setBounds(const SbBox2d & box);

  void getBounds(short & xmin, short & ymin, short & xmax, short & ymax) const
    { minpt.getValue(xmin, ymin); maxpt.getValue(xmax, ymax); }
  void getBounds(SbVec2s & minpoint, SbVec2s & maxpoint) const
    { minpoint = minpt; maxpoint = maxpt; }

  const SbVec2s & getMin(void) const { return minpt; }
  SbVec2s & getMin(void) { return minpt; }
  const SbVec2s & getMax(void) const { return maxpt; }
  SbVec2s & getMax(void) { return maxpt; }

  void extendBy(const SbVec2s & point);
  void extendBy(const SbBox2s & box);
  void makeEmpty(void);
  SbBool isEmpty(void) const { return (maxpt[0] < minpt[0]); }
  SbBool hasArea(void) const { return ((maxpt[0] > minpt[0]) && (maxpt[1] > minpt[1])); }

  SbBool intersect(const SbVec2s & point) const;
  SbBool intersect(const SbBox2s & box) const;

  SbVec2f getCenter(void) const { return SbVec2f((minpt[0] + maxpt[0]) * 0.5f, (minpt[1] + maxpt[1]) * 0.5f); }
  void getOrigin(short & originX, short & originY) const
    { minpt.getValue(originX, originY); }
  void getSize(short & sizeX, short & sizeY) const
    { if (isEmpty()) { sizeX = sizeY = 0; }
      else { sizeX = maxpt[0] - minpt[0]; sizeY = maxpt[1] - minpt[1]; } }
  SbVec2s getSize(void) const {
    SbVec2s v;
    this->getSize(v[0], v[1]);
    return v;
  }

  float getAspectRatio(void) const
    { SbDividerChk("SbBox2s::getAspectRatio()", maxpt[1] - minpt[1]);
      return (float(maxpt[0] - minpt[0]) / float(maxpt[1] - minpt[1])); }

private:
  SbVec2s minpt, maxpt;

}; // SbBox2s

COIN_DLL_API inline int operator == (const SbBox2s & b1, const SbBox2s & b2) {
  return ((b1.getMin() == b2.getMin()) && (b1.getMax() == b2.getMax()));
}

COIN_DLL_API inline int operator != (const SbBox2s & b1, const SbBox2s & b2) {
  return !(b1 == b2);
}

#endif // !COIN_SBBOX2S_H
