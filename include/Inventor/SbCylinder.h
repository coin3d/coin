/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SBCYLINDER_H__
#define __SBCYLINDER_H__

#include <stdio.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLine.h>

class SbCylinder {
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

#endif // !__SBCYLINDER_H__
