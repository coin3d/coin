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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef _SO_TEXTURE_COORDINATE_CACHE_H_
#define _SO_TEXTURE_COORDINATE_CACHE_H_

#include <Inventor/caches/SoCache.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbVec2f.h>

class SoTextureCoordinateCache : public SoCache {
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
}; // class SoTextureCoordinateCache

#endif // ! _SO_TEXTURE_COORDINATE_CACHE_H_
