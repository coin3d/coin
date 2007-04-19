#ifndef COIN_SOPRIMITIVEVERTEXCACHE_H
#define COIN_SOPRIMITIVEVERTEXCACHE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/caches/SoCache.h>
#include <Inventor/system/gl.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec2f.h>

class SoPrimitiveVertexCacheP;
class SoPrimitiveVertex;
class SoPointDetail;
class SoState;

// FIXME: evaluate the design of this class, pederb 2003-11-17

class SoPrimitiveVertexCache : public SoCache {
  typedef SoCache inherited;
public:
  SoPrimitiveVertexCache(SoState * state);
  ~SoPrimitiveVertexCache();

  enum Arrays {
    NORMAL = 0x01,
    TEXCOORD = 0x02,
    COLOR = 0x04,
    ALL = (NORMAL|TEXCOORD|COLOR)
  };

  void renderTriangles(SoState * state, const int arrays = ALL) const;
  void renderLines(SoState * state, const int arrays = ALL) const;
  void renderPoints(SoState * state, const int array = ALL) const;

  void addTriangle(const SoPrimitiveVertex * v0,
                   const SoPrimitiveVertex * v1,
                   const SoPrimitiveVertex * v2,
                   const int * pointdetailidx = NULL);
  void addLine(const SoPrimitiveVertex * v0,
               const SoPrimitiveVertex * v1);
  void addPoint(const SoPrimitiveVertex * v);

  int getNumVertices(void) const;
  const SbVec3f * getVertexArray(void) const;
  const SbVec3f * getNormalArray(void) const;
  const SbVec4f * getTexCoordArray(void) const;
  const SbVec2f * getBumpCoordArray(void) const;
  const uint8_t * getColorArray(void) const;

  int getNumIndices(void) const;
  const GLint * getIndices(void) const;
  int32_t getIndex(const int idx) const;

  SbBool colorPerVertex(void) const;
  const SbVec4f * getMultiTextureCoordinateArray(const int unit) const;

  int getNumLineIndices(void) const;
  int getNumPointIndices(void) const;

  const GLint * getLineIndices(void) const;
  const GLint * getPointIndices(void) const;

  void fit(void);
  void depthSortTriangles(SoState * state);
private:

  friend class SoPrimitiveVertexCacheP;
  class Vertex {
  public:
    SbVec3f vertex;
    SbVec3f normal;
    SbVec4f texcoord0;
    SbVec2f bumpcoord;
    uint8_t rgba[4];
    int texcoordidx;

    // needed for SbHash
    operator unsigned long(void) const;
    
    // needed, since if we don't add this the unsigned long operator
    // will be used when comparing two vertices.
    int operator==(const Vertex & v);
  };

  SoPrimitiveVertexCacheP * pimpl;
};

#endif // COIN_SOPRIMITIVEVERTEXCACHE_H
