#ifndef COIN_SBBOX2D_H
#define COIN_SBBOX2D_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2d.h>

class COIN_DLL_API SbBox2d {
public:
  SbBox2d(void);
  SbBox2d(double xmin, double ymin, double xmax, double ymax);
  SbBox2d(const SbVec2d & min, const SbVec2d & max);
  ~SbBox2d(void);

  const SbVec2d & getMin(void) const;
  const SbVec2d & getMax(void) const;
  SbVec2d getCenter(void) const;
  void extendBy(const SbVec2d & point);
  void extendBy(const SbBox2d & box);
  SbBool intersect(const SbVec2d & point) const;
  SbBool intersect(const SbBox2d & box) const;
  SbBool findIntersection(const SbVec2d & a, const SbVec2d & b, SbVec2d & ia, SbVec2d & ib) const;
  SbVec2d getClosestPoint(const SbVec2d & p) const;
  void setBounds(double xmin, double ymin, double xmax, double ymax);
  void setBounds(const SbVec2d & min, const SbVec2d & max);
  void getBounds(double & xmin, double & ymin, double & xmax, double & ymax) const;
  void getBounds(SbVec2d & min, SbVec2d & max) const;
  void getOrigin(double & x0, double & y0) const;
  void getSize(double & w, double & h) const;
  double getAspectRatio(void) const;
  void makeEmpty(void);
  SbBool isEmpty(void) const;
  SbBool hasArea(void) const;

  friend COIN_DLL_API int operator ==(const SbBox2d & b1, const SbBox2d & b2);
  friend COIN_DLL_API int operator !=(const SbBox2d & b1, const SbBox2d & b2);

private:
  SbVec2d minpt, maxpt;

  double width(void) const;
  double height(void) const;
};

COIN_DLL_API int operator == (const SbBox2d & b1, const SbBox2d & b2);
COIN_DLL_API int operator != (const SbBox2d & b1, const SbBox2d & b2);

#endif // !COIN_SBBOX2D_H
