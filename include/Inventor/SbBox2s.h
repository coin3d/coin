#ifndef COIN_SBBOX2S_H
#define COIN_SBBOX2S_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2s.h>

class COIN_DLL_API SbBox2s {
public:
  SbBox2s(void);
  SbBox2s(short xmin, short ymin, short xmax, short ymax);
  SbBox2s(const SbVec2s& _min, const SbVec2s& _max);
  ~SbBox2s(void);

  const SbVec2s& getMin(void) const;
  const SbVec2s& getMax(void) const;
  void extendBy(const SbVec2s& point);
  void extendBy(const SbBox2s& box);
  SbBool intersect(const SbVec2s& point) const;
  SbBool intersect(const SbBox2s& box) const;
  void setBounds(short xmin, short ymin, short xmax, short ymax);
  void setBounds(const SbVec2s& _min, const SbVec2s& _max);
  void getBounds(short& xmin, short& ymin, short& xmax, short& ymax) const;
  void getBounds(SbVec2s& _min, SbVec2s& _max) const;
  void getOrigin(short& originX, short& originY) const;
  void getSize(short& sizeX, short& sizeY) const;
  float getAspectRatio(void) const;
  void makeEmpty(void);
  friend COIN_DLL_API int operator ==(const SbBox2s& b1, const SbBox2s& b2);
  friend COIN_DLL_API int operator !=(const SbBox2s& b1, const SbBox2s& b2);

private:
  SbVec2s minpt, maxpt;

  short width(void) const;
  short height(void) const;
  SbBool hasArea(void) const;
};

COIN_DLL_API int operator ==(const SbBox2s& b1, const SbBox2s& b2);
COIN_DLL_API int operator !=(const SbBox2s& b1, const SbBox2s& b2);

#endif // !COIN_SBBOX2S_H
