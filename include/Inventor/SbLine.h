/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBLINE_H
#define COIN_SBLINE_H

#include <stdio.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>

class COIN_DLL_EXPORT SbLine {
public:
  SbLine(void);
  SbLine(const SbVec3f& p0, const SbVec3f& p1);
  void setValue(const SbVec3f& p0, const SbVec3f& p1);
  SbBool getClosestPoints(const SbLine& line2,
                          SbVec3f& ptOnThis, SbVec3f& ptOnLine2) const;
  SbVec3f getClosestPoint(const SbVec3f& point) const;
  const SbVec3f& getPosition(void) const;
  const SbVec3f& getDirection(void) const;

  void print(FILE * file) const;

private:
  SbVec3f pos, dir;
};

#endif // !COIN_SBLINE_H
