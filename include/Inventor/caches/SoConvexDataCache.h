#ifndef COIN_SOCONVEXDATACACHE_H
#define COIN_SOCONVEXDATACACHE_H

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

#include <Inventor/caches/SoCache.h>
#include <Inventor/system/inttypes.h>

class SbVec3f;
class SoCoordinateElement;
class SbMatrix;
class SoConvexDataCacheP;

class COIN_DLL_API SoConvexDataCache : public SoCache {
  typedef SoCache inherited;
public:
  SoConvexDataCache(SoState * const state);
  ~SoConvexDataCache();

  enum Binding {
    // do not change these values. We rely on them matching
    // values in SoIndededFaceSet.h and SoGL.cpp...
    NONE = 0,
    PER_FACE,
    PER_FACE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  void generate(const SoCoordinateElement * const coords,
                const SbMatrix & matrix,
                const int32_t *coordindices,
                const int numcoordindices,
                const int32_t *matindices, const int32_t *normindices,
                const int32_t *texindices,
                const Binding matbinding, const Binding normbinding,
                const Binding texbinding);

  const int32_t *getCoordIndices(void) const;
  int getNumCoordIndices(void) const;
  const int32_t *getMaterialIndices(void) const;
  int getNumMaterialIndices(void) const;
  const int32_t *getNormalIndices(void) const;
  int getNumNormalIndices(void) const;
  const int32_t *getTexIndices(void) const;
  int getNumTexIndices(void) const;

private:
  SoConvexDataCacheP * pimpl;
};

#endif // !COIN_SOCONVEXDATACACHE_H

