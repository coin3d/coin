#ifndef COIN_SONORMALCACHE_H
#define COIN_SONORMALCACHE_H

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

#include <Inventor/SbVec3f.h>
#include <Inventor/caches/SoCache.h>
#include <Inventor/system/inttypes.h>

class SoNormalGenerator;
class SoNormalCacheP;

class COIN_DLL_API SoNormalCache : public SoCache {
  typedef SoCache inherited;

public:
  SoNormalCache(SoState * const state);
  ~SoNormalCache();

  void set(const int num, const SbVec3f * const normals);
  void set(SoNormalGenerator *generator);

  void generatePerVertex(const SbVec3f * const coords,
                         const int32_t *coordindices,
                         const int numcoordindices,
                         const float crease_angle,
                         const SbVec3f *facenormals = NULL,
                         const SbBool ccw = TRUE,
                         const SbBool tristrip = FALSE);


  void generatePerFace(const SbVec3f * const coords,
                       const int32_t *coordindices,
                       const int numcoorindices,
                       const SbBool ccw);

  void generatePerFaceStrip(const SbVec3f * const coords,
                            const int32_t *coordindices,
                            const int numcoorindices,
                            const SbBool ccw);

  void generatePerStrip(const SbVec3f * const coords,
                        const int32_t *coordindices,
                        const int numcoorindices,
                        const SbBool ccw);

  void generatePerVertexQuad(const SbVec3f * const coords,
                             const int vPerRow,
                             const int vPerColumn,
                             const SbBool ccw);

  void generatePerFaceQuad(const SbVec3f * const coords,
                           const int vPerRow,
                           const int vPerColumn,
                           const SbBool ccw);

  void generatePerRowQuad(const SbVec3f * const coords,
                          const int vPerRow,
                          const int vPerColumn,
                          const SbBool ccw);

  int getNum(void) const;
  const SbVec3f *getNormals(void) const;

  int getNumIndices(void) const;
  const int32_t *getIndices(void) const;

private:
  SoNormalCacheP * pimpl;
  void clearGenerator(void);
};

#endif // !COIN_SONORMALCACHE_H
