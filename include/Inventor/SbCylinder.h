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

#ifndef COIN_SBCYLINDER_H
#define COIN_SBCYLINDER_H

#include <stdio.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLine.h>

class COIN_DLL_API SbCylinder {
public:
  SbCylinder(void);
  SbCylinder(const SbLine& a, const float r);
  void setValue(const SbLine& a, const float r);
  void setAxis(const SbLine& a);
  void setRadius(const float r);
  const SbLine& getAxis(void) const;
  float getRadius(void) const;
  SbBool intersect(const SbLine& l, SbVec3f& intersection) const;
  SbBool intersect(const SbLine& l, SbVec3f& enter, SbVec3f& exit) const;

  void print(FILE * file) const;

private:
  SbLine axis;
  float radius;
};

#endif // !COIN_SBCYLINDER_H
