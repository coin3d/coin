#ifndef COIN_SOPRIMITIVEVERTEXCACHE_H
#define COIN_SOPRIMITIVEVERTEXCACHE_H

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
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec2f.h>

class SoPrimitiveVertexCacheP;
class SoPrimitiveVertex;

// FIXME: evaluate the design of this class, pederb 2003-11-17

class COIN_DLL_API SoPrimitiveVertexCache : public SoCache {
  typedef SoCache inherited;
public:
  SoPrimitiveVertexCache(SoState * state);
  ~SoPrimitiveVertexCache();

  class Vertex {
  public:
    SbVec3f vertex;
    SbVec3f normal;
    SbVec4f texcoord0;
    SbVec2f texcoord1;
    SbVec2f bumpcoord;
    uint8_t rgba[4];
    int texcoordidx;

    // needed for SbHash
    operator unsigned long(void) const;
    int operator==(const Vertex & v) const;
  };

  void addTriangle(const SoPrimitiveVertex * v0,
                   const SoPrimitiveVertex * v1,
                   const SoPrimitiveVertex * v2);

  int getNumVertices(void) const;
  int getNumIndices(void) const;
  const Vertex * getVertices(void) const;
  const int32_t * getIndices(void) const;
  const Vertex & getVertex(const int idx) const;
  int32_t getIndex(const int idx) const;

private:
  SoPrimitiveVertexCacheP * pimpl;
};

#endif COIN_SOPRIMITIVEVERTEXCACHE_H
