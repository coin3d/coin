#ifndef COIN_SOBOUNDINGBOXCACHE_H
#define COIN_SOBOUNDINGBOXCACHE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/caches/SoCache.h>
#include <Inventor/SbXfBox3f.h>

class SoBoundingBoxCacheP;

class COIN_DLL_API SoBoundingBoxCache : public SoCache {
  typedef SoCache inherited;
public:
  SoBoundingBoxCache(SoState *state);
  ~SoBoundingBoxCache();

  void set(const SbXfBox3f & boundingbox,
           SbBool centerset,
           const SbVec3f & centerpoint);

  const SbXfBox3f & getBox() const;
  const SbBox3f & getProjectedBox() const;

  SbBool isCenterSet() const;
  const SbVec3f & getCenter() const;

  static void setHasLinesOrPoints(SoState *state);
  SbBool hasLinesOrPoints(void) const;

private:
  SoBoundingBoxCacheP * pimpl;
};

#endif // !COIN_SOBOUNDINGBOXCACHE_H
