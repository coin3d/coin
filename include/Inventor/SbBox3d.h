#ifndef COIN_SBBOX3D_H
#define COIN_SBBOX3D_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/SbVec3d.h>

class SbDPMatrix;

class COIN_DLL_API SbBox3d {
public:
  SbBox3d(void);
  SbBox3d(const double minx, const double miny, const double minz,
          const double maxx, const double maxy, const double maxz);
  SbBox3d(const SbVec3d & min, const SbVec3d & max);
  ~SbBox3d();

  const SbVec3d & getMin(void) const;
  const SbVec3d & getMax(void) const;
  SbVec3d & getMin(void);
  SbVec3d & getMax(void);

  SbVec3d getCenter(void) const;

  void extendBy(const SbVec3d & point);
  void extendBy(const SbBox3d & box);

  SbBool intersect(const SbVec3d & point) const;
  SbBool intersect(const SbBox3d & box) const;

  void setBounds(const double minx, const double miny, const double minz,
                 const double maxx, const double maxy, const double maxz);
  void setBounds(const SbVec3d & min, const SbVec3d & max);
  void getBounds(double & minx, double & miny, double & minz,
                 double & maxx, double & maxy, double & maxz) const;
  void getBounds(SbVec3d & min, SbVec3d & max) const;

  void getOrigin(double & x0, double & y0, double & z0) const;
  void getSize(double & dx, double & dy, double & dz) const;

  void makeEmpty(void);
  SbBool isEmpty(void) const;
  SbBool hasVolume(void) const;

  void getSpan(const SbVec3d & dir, double & dmin, double & dmax) const;
  void transform(const SbDPMatrix & matrix);
  double getVolume(void) const;

  SbBool outside(const SbDPMatrix & mvp, int & cullbits) const;
  SbVec3d getClosestPoint(const SbVec3d & point) const;

  friend COIN_DLL_API int operator ==(const SbBox3d & b1, const SbBox3d & b2);
  friend COIN_DLL_API int operator !=(const SbBox3d & b1, const SbBox3d & b2);

  void print(FILE * file) const;

private:
  SbVec3d min, max;
};

COIN_DLL_API int operator ==(const SbBox3d & b1, const SbBox3d & b2);
COIN_DLL_API int operator !=(const SbBox3d & b1, const SbBox3d & b2);

#endif // !COIN_SBBOX3D_H
