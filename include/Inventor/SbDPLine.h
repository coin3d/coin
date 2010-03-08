#ifndef COIN_SBDPLINE_H
#define COIN_SBDPLINE_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3d.h>

class COIN_DLL_API SbDPLine {
public:
  SbDPLine(void);
  SbDPLine(const SbVec3d& p0, const SbVec3d& p1);
  void setValue(const SbVec3d& p0, const SbVec3d& p1);
  SbBool getClosestPoints(const SbDPLine& line2,
                          SbVec3d& ptOnThis, SbVec3d& ptOnLine2) const;
  SbVec3d getClosestPoint(const SbVec3d& point) const;
  const SbVec3d& getPosition(void) const;
  const SbVec3d& getDirection(void) const;

  void print(FILE * file) const;

private:
  SbVec3d pos, dir;
};

#endif // !COIN_SBDPLINE_H
