#ifndef COIN_SBCYLINDER_H
#define COIN_SBCYLINDER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

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
