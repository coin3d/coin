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

#ifndef __SBLINE_H__
#define __SBLINE_H__

#include <stdio.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>

class SbLine {
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

#endif // !__SBLINE_H__
