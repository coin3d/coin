#ifndef COIN_SBDPLINE_H
#define COIN_SBDPLINE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
