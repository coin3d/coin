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

#ifndef COIN_SBLINE_H
#define COIN_SBLINE_H

#include <stdio.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>

class COIN_DLL_API SbLine {
public:
  SbLine(void);
  SbLine(const SbVec3f & origin, const SbVec3f & point);
  void setValue(const SbVec3f & origin, const SbVec3f & point);
  SbBool getClosestPoints(const SbLine & line2,
                          SbVec3f & ptOnThis, SbVec3f & ptOnLine2) const;
  SbVec3f getClosestPoint(const SbVec3f & point) const;
  const SbVec3f & getPosition(void) const;
  const SbVec3f & getDirection(void) const;

  void print(FILE * file) const;

private:
  SbVec3f pos, dir;
};

#endif // !COIN_SBLINE_H
