/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SONORMALCACHE_H
#define COIN_SONORMALCACHE_H

#include <Inventor/SbVec3f.h>
#include <Inventor/caches/SoCache.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/system/inttypes.h>

class SoNormalGenerator;


class COIN_DLL_EXPORT SoNormalCache : public SoCache {
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
  int numNormals;
  union {
    const SbVec3f *normals;
    SoNormalGenerator *generator;
  } normalData;
  SbList <int32_t> indices;
  SbList <SbVec3f> normalArray;

  void clearGenerator(void);
};

#endif // !COIN_SONORMALCACHE_H
