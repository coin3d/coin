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

/*!
  \class SoPrimitiveVertexCache include/Inventor/SoPrimitiveVertexCache.h
  The SoPrimitiveVertexClass is used to cache generated triangles.
*/

#include <Inventor/caches/SoPrimitiveVertexCache.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/misc/SbHash.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/details/SoPointDetail.h>

#ifndef DOXYGEN_SKIP_THIS

class SoPrimitiveVertexCacheP {
public:
  SoPrimitiveVertexCacheP(void) : vertices(1024), indices(1024), vhash(1024) { }

  SbList <SoPrimitiveVertexCache::Vertex> vertices;
  SbList <int32_t> indices;
  SbHash <int32_t, SoPrimitiveVertexCache::Vertex> vhash;

  const SbBool * enabledunits;
  int maxenabled;
};

#endif // DOXYGEN_SKIP_THIS

#undef PRIVATE
#define PRIVATE(obj) obj->pimpl


/*!
  Constructor.
*/
SoPrimitiveVertexCache::SoPrimitiveVertexCache(SoState * state)
  : SoCache(state)
{
  PRIVATE(this) = new SoPrimitiveVertexCacheP;

  PRIVATE(this)->enabledunits = 
    SoMultiTextureEnabledElement::getEnabledUnits(state,
                                                  PRIVATE(this)->maxenabled);
}

/*!
  Destructor.
*/
SoPrimitiveVertexCache::~SoPrimitiveVertexCache()
{
  delete PRIVATE(this);
}

void 
SoPrimitiveVertexCache::addTriangle(const SoPrimitiveVertex * v0,
                                    const SoPrimitiveVertex * v1,
                                    const SoPrimitiveVertex * v2)
{
  const SoPrimitiveVertex *vp[3] = { v0,v1,v2 };

  for (int i = 0; i < 3; i++) {
    Vertex v;
    v.vertex = vp[i]->getPoint();
    v.normal = vp[i]->getNormal();
    const SbVec4f & tmp = vp[i]->getTextureCoords();
    v.bumpcoord = SbVec2f(tmp[0], tmp[1]);
    v.texcoord0 = tmp;
    v.texcoord1 = SbVec2f(tmp[0], tmp[1]);
    v.texcoordidx = -1;
    v.rgba[0] = 0xcc;
    v.rgba[1] = 0xcc;
    v.rgba[2] = 0xcc;
    v.rgba[3] = 0xff;

    const SoDetail * d = vp[i]->getDetail();
    if (d && d->isOfType(SoPointDetail::getClassTypeId())) {
      // FIXME: fetch texture coordinates from
      // SoBumpMapCoordinateElement and update bc
    }

    int32_t idx;
    if (!PRIVATE(this)->vhash.get(v, idx)) {
      idx = PRIVATE(this)->vertices.getLength();
      PRIVATE(this)->vhash.put(v, idx);
      PRIVATE(this)->vertices.append(v);
      PRIVATE(this)->indices.append(idx);
    }
    else {
      PRIVATE(this)->indices.append(idx);
    }
  }
}

int 
SoPrimitiveVertexCache::getNumVertices(void) const
{
  return PRIVATE(this)->vertices.getLength();
}

int 
SoPrimitiveVertexCache::getNumIndices(void) const
{
  return PRIVATE(this)->indices.getLength();
}

const SoPrimitiveVertexCache::Vertex * 
SoPrimitiveVertexCache::getVertices(void) const
{
  return PRIVATE(this)->vertices.getArrayPtr();
}

const int32_t * 
SoPrimitiveVertexCache::getIndices(void) const
{
  return PRIVATE(this)->indices.getArrayPtr();
}

const SoPrimitiveVertexCache::Vertex & 
SoPrimitiveVertexCache::getVertex(const int idx) const
{
  return PRIVATE(this)->vertices[idx];
}

int
SoPrimitiveVertexCache::getIndex(const int idx) const
{
  return PRIVATE(this)->indices[idx];
}

#undef PRIVATE

SoPrimitiveVertexCache::Vertex::operator unsigned long(void) const
{
  int size = sizeof(*this);
  unsigned long key = 0;
  const unsigned char * ptr = (const unsigned char *) this;
  for (int i = 0; i < size; i++) {
    int shift = (i%4) * 8;
    key ^= (ptr[i]<<shift);
  }
  return key;
}

int 
SoPrimitiveVertexCache::Vertex::operator==(const Vertex & v) const
{
  return memcmp(this, &v, sizeof(Vertex)) == 0;
}
