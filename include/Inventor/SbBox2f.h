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

#ifndef __SBBOX2F_H__
#define __SBBOX2F_H__

#include <float.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2f.h>

class SbBox2f {
public:
  SbBox2f(void);
  SbBox2f(float xmin, float ymin, float xmax, float ymax);
  SbBox2f(const SbVec2f& min, const SbVec2f& max);
  ~SbBox2f(void);

  const SbVec2f& getMin(void) const;
  const SbVec2f& getMax(void) const;
  SbVec2f getCenter(void) const;
  void extendBy(const SbVec2f& point);
  void extendBy(const SbBox2f& box);
  SbBool intersect(const SbVec2f& point) const;
  SbBool intersect(const SbBox2f& box) const;
  void setBounds(float xmin, float ymin, float xmax, float ymax);
  void setBounds(const SbVec2f& min, const SbVec2f& max);
  void getBounds(float& xmin, float& ymin, float& xmax, float& ymax) const;
  void getBounds(SbVec2f& min, SbVec2f& max) const;
  void getOrigin(float& x0, float& y0) const;
  void getSize(float& w, float& h) const;
  float getAspectRatio(void) const;
  void makeEmpty(void);
  SbBool isEmpty(void) const;
  SbBool hasArea(void) const;


private:
  SbVec2f minpt, maxpt;

  float width(void) const;
  float height(void) const;
};

#endif // !__SBBOX2F_H__
