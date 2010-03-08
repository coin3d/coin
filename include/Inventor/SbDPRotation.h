#ifndef COIN_SBDPROTATION_H
#define COIN_SBDPROTATION_H

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
#include <Inventor/SbVec4d.h>

class SbDPMatrix;
class SbVec3d;

class COIN_DLL_API SbDPRotation {
public:
  SbDPRotation(void);
  SbDPRotation(const SbVec3d & axis, const double radians);
  SbDPRotation(const double q[4]);
  SbDPRotation(const double q0, const double q1, const double q2, const double q3);
  SbDPRotation(const SbDPMatrix & m);
  SbDPRotation(const SbVec3d & rotateFrom, const SbVec3d & rotateTo);
  const double * getValue(void) const;
  void getValue(double & q0, double & q1, double & q2, double & q3) const;
  SbDPRotation & setValue(const double q0, const double q1,
                        const double q2, const double q3);
  void getValue(SbVec3d & axis, double & radians) const;
  void getValue(SbDPMatrix & matrix) const;
  SbDPRotation & invert(void);
  SbDPRotation inverse(void) const;
  SbDPRotation & setValue(const double q[4]);
  SbDPRotation & setValue(const SbDPMatrix & m);
  SbDPRotation & setValue(const SbVec3d & axis, const double radians);
  SbDPRotation & setValue(const SbVec3d & rotateFrom, const SbVec3d & rotateTo);
  SbBool equals(const SbDPRotation & r, double tolerance) const;
  void multVec(const SbVec3d & src, SbVec3d & dst) const;

  void scaleAngle(const double scaleFactor);
  static SbDPRotation slerp(const SbDPRotation & rot0, const SbDPRotation & rot1,
                          double t);
  static SbDPRotation identity(void);

  void print(FILE * fp) const;

  SbDPRotation & operator*=(const SbDPRotation & q);
  SbDPRotation & operator*=(const double s);
  friend COIN_DLL_API int operator==(const SbDPRotation & q1, const SbDPRotation & q2);
  friend COIN_DLL_API int operator!=(const SbDPRotation & q1, const SbDPRotation & q2);
  friend COIN_DLL_API SbDPRotation operator *(const SbDPRotation & q1, const SbDPRotation & q2);
private:
  SbVec4d quat;
};

typedef SbDPRotation SbRotationd;


COIN_DLL_API int operator ==(const SbDPRotation & q1, const SbDPRotation & q2);
COIN_DLL_API int operator !=(const SbDPRotation & q1, const SbDPRotation & q2);
COIN_DLL_API SbDPRotation operator *(const SbDPRotation & q1, const SbDPRotation & q2);

#endif // !COIN_SBDPROTATION_H
