/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
