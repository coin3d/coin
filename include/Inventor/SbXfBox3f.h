/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBXFBOX3F_H
#define COIN_SBXFBOX3F_H

#include <stdio.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>

class COIN_DLL_API SbXfBox3f : public SbBox3f {
  typedef SbBox3f inherited;

public:
  SbXfBox3f();
  SbXfBox3f(const SbVec3f &_min, const SbVec3f &_max);
  SbXfBox3f(const SbBox3f &box);
  ~SbXfBox3f();

  void setTransform(const SbMatrix &m);
  const SbMatrix &getTransform() const;
  const SbMatrix &getInverse() const;
  SbVec3f getCenter() const;
  void extendBy(const SbVec3f &pt);
  void extendBy(const SbBox3f &bb);
  void extendBy(const SbXfBox3f &bb);
  SbBool intersect(const SbVec3f &pt) const;
  SbBool intersect(const SbBox3f &bb) const;
  void getSpan(const SbVec3f &direction, float &dMin, float &dMax) const;
  SbBox3f project() const;
  friend COIN_DLL_API int operator ==(const SbXfBox3f &b1, const SbXfBox3f &b2);
  friend COIN_DLL_API int operator !=(const SbXfBox3f &b1, const SbXfBox3f &b2);
  // Must overload the transform() method from SbBox3f, as the box and
  // the transform matrix are supposed to be kept separate in
  // SbXfBox3f. --mortene
  void transform(const SbMatrix & m);
  // Overloaded from SbBox3f
  float getVolume(void) const;

  void print(FILE * file) const;

private:
  // incorrect for SbXfBox3f. Hide them
  const SbVec3f & getMin(void) const {return SbBox3f::getMin(); }
  const SbVec3f & getMax(void) const { return SbBox3f::getMax(); }

  void calcInverse(void) const;
  void makeInvInvalid(void);

  SbMatrix matrix, invertedmatrix;
};

COIN_DLL_API int operator ==(const SbXfBox3f &b1, const SbXfBox3f &b2);
COIN_DLL_API int operator !=(const SbXfBox3f &b1, const SbXfBox3f &b2);

#endif // !COIN_SBXFBOX3F_H
