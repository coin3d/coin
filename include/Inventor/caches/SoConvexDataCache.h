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

#ifndef _SO_CONVEX_DATA_CACHE_H_
#define _SO_CONVEX_DATA_CACHE_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOCONVEXDATACACHE)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOCONVEXDATACACHE

#include <Inventor/caches/SoCache.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/lists/SbList.h>

class SbVec3f;
class SoCoordinateElement;

class SoConvexDataCache : public SoCache {
  typedef SoCache inherited;
public:
  SoConvexDataCache(SoState * const state);
  ~SoConvexDataCache();

  enum Binding {
    NONE = 0,
    PER_FACE,         // not for texbinding
    PER_FACE_INDEXED,  //
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };
  
  void generate(const SoCoordinateElement* const coords, 
		const int32_t *coordindices,
		const int numcoordindices,
		const int32_t *matindices, const int32_t *normindices,
		const int32_t *texindices,
		const Binding matbinding, const Binding normbinding,
		const Binding texbinding);

  const int32_t *getCoordIndices() const;
  int getNumCoordIndices() const;
  const int32_t *getMaterialIndices() const;
  int getNumMaterialIndices() const;
  const int32_t *getNormalIndices() const;
  int getNumNormalIndices() const;
  const int32_t *getTexIndices() const;
  int getNumTexIndices() const;
  
private:
  SbList <int32_t> coordIndices;
  SbList <int32_t> normalIndices;
  SbList <int32_t> materialIndices;
  SbList <int32_t> texIndices; 
}; // class SoConvexDataCache

#endif // ! _SO_CONVEX_DATA_CACHE_H_
