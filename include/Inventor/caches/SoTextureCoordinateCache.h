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

#ifndef COIN_SOTEXTURECOORDINATECACHE_H
#define COIN_SOTEXTURECOORDINATECACHE_H

#include <Inventor/caches/SoCache.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbVec2f.h>

class COIN_DLL_API SoTextureCoordinateCache : public SoCache {
  typedef SoCache inherited;
public:
  SoTextureCoordinateCache(SoState * const state);
  ~SoTextureCoordinateCache();

  // TODO: more ways to generate texture coordinates
  void generate(const SbBox3f &bbox, const SbVec3f *vertices,
                const int numvertices);

  const SbVec2f *get() const;
  int getNum() const;

private:
  SbList <SbVec2f> texCoords;
};

#endif // !COIN_SOTEXTURECOORDINATECACHE_H
