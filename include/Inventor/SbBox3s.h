#ifndef COIN_SBBOX3S_H
#define COIN_SBBOX3S_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3s.h>

class SbBox3s {
public:
  SbBox3s(void);
  SbBox3s(short xmin, short ymin, short zmin, short xmax, short ymax, short zmax);
  SbBox3s(const SbVec3s & minvec, const SbVec3s & maxvec);
  ~SbBox3s(void);

  const SbVec3s & getMin(void) const;
  const SbVec3s & getMax(void) const;
  void extendBy(const SbVec3s & point);
  void extendBy(const SbBox3s & box);
  SbBool intersect(const SbVec3s & point) const;
  SbBool intersect(const SbBox3s & box) const;
  void setBounds(short xmin, short ymin, short zmin, short xmax, short ymax, short zmax);
  void setBounds(const SbVec3s & minvec, const SbVec3s & maxvec);
  void getBounds(short & xmin, short & ymin, 
                 short & zmin, short & xmax, 
                 short & ymax, short & zmax) const;
  void getBounds(SbVec3s & minvec, SbVec3s & maxvec) const;
  void getOrigin(short & originX, short & originY, short & originZ) const;
  void getSize(short & sizeX, short & sizeY, short & sizeZ) const;
  void makeEmpty(void);
  friend int operator ==(const SbBox3s & b1, const SbBox3s & b2);
  friend int operator !=(const SbBox3s & b1, const SbBox3s & b2);

private:
  SbVec3s minpt, maxpt;

  short width(void) const;
  short height(void) const;
  short depth(void) const;
  SbBool hasArea(void) const;
};

int operator ==(const SbBox3s & b1, const SbBox3s & b2);
int operator !=(const SbBox3s & b1, const SbBox3s & b2);

#endif // !COIN_SBBOX3S_H
