#ifndef COIN_SBXFBOX3D_H
#define COIN_SBXFBOX3D_H

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

#include <Inventor/SbBox3d.h>
#include <Inventor/SbDPMatrix.h>

class COIN_DLL_API SbXfBox3d : public SbBox3d {
  typedef SbBox3d inherited;

public:
  SbXfBox3d(void);
  SbXfBox3d(const SbVec3d & boxmin, const SbVec3d & boxmax);
  SbXfBox3d(const SbBox3d & box);

  void setTransform(const SbDPMatrix & matrix);

  const SbDPMatrix & getTransform(void) const { return matrix; }
  const SbDPMatrix & getInverse(void) const;
  SbVec3d getCenter(void) const;

  void extendBy(const SbVec3d & pt);
  void extendBy(const SbBox3d & bb);
  void extendBy(const SbXfBox3d & bb);
  SbBool intersect(const SbVec3d & pt) const;
  SbBool intersect(const SbBox3d & bb) const;
  SbBool intersect(const SbXfBox3d & bb) const;
  SbBox3d project(void) const;
  void getSpan(const SbVec3d & direction, double & dMin, double & dMax) const;

  // Must override the transform() method from SbBox3f, as the box and
  // the transform matrix are supposed to be kept separate in
  // SbXfBox3f. --mortene
  void transform(const SbDPMatrix & matrix);

  // Overridden from SbBox3d
  double getVolume(void) const;

private:
  // These are incorrect for SbXfBox3d. Privatize/hide them.
  using SbBox3d::getMin;
  using SbBox3d::getMax;

  void calcInverse(void) const;
  void makeInvInvalid(void);

  SbDPMatrix matrix;
  mutable SbDPMatrix invertedmatrix; // lazy cache

}; // SbXfBox3d

COIN_DLL_API int operator == (const SbXfBox3d & b1, const SbXfBox3d & b2);
COIN_DLL_API int operator != (const SbXfBox3d & b1, const SbXfBox3d & b2);

#endif // !COIN_SBXFBOX3D_H
