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

#ifndef __SOBOUNDINGBOXCACHE_H__
#define __SOBOUNDINGBOXCACHE_H__

#include <Inventor/caches/SoCache.h>
#include <Inventor/SbXfBox3f.h>

class SoBoundingBoxCache : public SoCache {
  typedef SoCache inherited;
public: 
  SoBoundingBoxCache(SoState *state);
  ~SoBoundingBoxCache();
  
  void set(const SbXfBox3f &boundingBox,
	   SbBool centerSet,
	   const SbVec3f &centerPoint);
  
  const SbXfBox3f &getBox() const;
  const SbBox3f &getProjectedBox() const;
  
  SbBool isCenterSet() const;
  const SbVec3f &getCenter() const;
  
  static void setHasLinesOrPoints(SoState *state);
  SbBool hasLinesOrPoints() const;

private:
  SbXfBox3f bbox;
  SbBox3f localBBox;
  SbVec3f centerPoint;
  unsigned int centerSet : 1;
  unsigned int linesOrPoints : 1;
};

#endif // !__SOBOUNDINGBOXCACHE_H__
