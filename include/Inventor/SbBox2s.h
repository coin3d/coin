/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SBBOX2S_H__
#define __SBBOX2S_H__

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2s.h>

class SbBox2s {
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
  friend int operator ==(const SbBox2s& b1, const SbBox2s& b2);
  friend int operator !=(const SbBox2s& b1, const SbBox2s& b2);

private:
  SbVec2s minpt, maxpt;

  short width(void) const;
  short height(void) const;
  SbBool hasArea(void) const;
};

#endif // !__SBBOX2S_H__
