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
#include <Inventor/elements/SoBumpMapCoordinateElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoFaceDetail.h>
#include <stddef.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoGL.h>
#include <string.h> // memcmp()

#ifndef DOXYGEN_SKIP_THIS

class SoPrimitiveVertexCacheP {
public:
  SoPrimitiveVertexCacheP(void) : vertices(1024), indices(1024), vhash(1024) { }

  SbList <SoPrimitiveVertexCache::Vertex> vertices;
  SbList <int32_t> indices;
  SbHash <int32_t, SoPrimitiveVertexCache::Vertex> vhash;

  const SbVec2f * bumpcoords;
  int numbumpcoords;

  const uint32_t * packedptr;
  const SbColor * diffuseptr;
  const float * transpptr;

  const SoLazyElement * lazyelement;
  int numdiffuse;
  int numtransp;
  int prevfaceidx;
  SbBool colorpervertex;
  uint32_t firstcolor;

  const SbBool * enabledunits;
  int lastenabled;
  const SoMultiTextureCoordinateElement * multielem;
  SbList <SbVec4f> * multitexcoords;
  SoState * state;
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
  PRIVATE(this)->state = state;

  const SoBumpMapCoordinateElement * belem =
    SoBumpMapCoordinateElement::getInstance(state);

  PRIVATE(this)->numbumpcoords = belem->getNum();
  PRIVATE(this)->bumpcoords = belem->getArrayPtr();

  SoLazyElement * lelem = SoLazyElement::getInstance(state);

  PRIVATE(this)->numdiffuse = lelem->getNumDiffuse();
  PRIVATE(this)->numtransp = lelem->getNumTransparencies();
  if (lelem->isPacked()) {
    PRIVATE(this)->packedptr = lelem->getPackedPointer();
    PRIVATE(this)->diffuseptr = NULL;
    PRIVATE(this)->transpptr = NULL;
  }
  else {
    PRIVATE(this)->packedptr = NULL;
    PRIVATE(this)->diffuseptr = lelem->getDiffusePointer();
    PRIVATE(this)->transpptr = lelem->getTransparencyPointer();
  }

  // set up variables to test if we need to supply color per vertex
  PRIVATE(this)->colorpervertex = FALSE;
  
  // just store diffuse color with index 0
  uint32_t col;
  if (PRIVATE(this)->packedptr) {
    col = PRIVATE(this)->packedptr[0];
  }
  else {
    SbColor tmpc = PRIVATE(this)->diffuseptr[0];
    float tmpt = PRIVATE(this)->transpptr[0];
    col = tmpc.getPackedValue(tmpt);
  }
  PRIVATE(this)->firstcolor = col;   
  
  // set up for multi texturing
  PRIVATE(this)->lastenabled = -1;
  PRIVATE(this)->enabledunits = 
    SoMultiTextureEnabledElement::getEnabledUnits(state, PRIVATE(this)->lastenabled);
  PRIVATE(this)->multielem = NULL;
  PRIVATE(this)->multitexcoords = NULL;
  if (PRIVATE(this)->lastenabled >= 1) {
    PRIVATE(this)->multitexcoords = new SbList<SbVec4f>[PRIVATE(this)->lastenabled+1];    
    // delay fetching SoMultiTextureCoordinateElement until the first
    // triangle callback. SoTextureCoordinateBundle might push a new
    // element.
  }
}

/*!
  Destructor.
*/
SoPrimitiveVertexCache::~SoPrimitiveVertexCache()
{
  if (PRIVATE(this)->lastenabled >= 1) {
    delete[] PRIVATE(this)->multitexcoords;
  }
  delete PRIVATE(this);
}

void
SoPrimitiveVertexCache::addTriangle(const SoPrimitiveVertex * v0,
                                    const SoPrimitiveVertex * v1,
                                    const SoPrimitiveVertex * v2,
                                    const int * pointdetailidx)
{
  if (PRIVATE(this)->lastenabled >= 1 && PRIVATE(this)->multielem == NULL) {
    // fetch SoMultiTextureCoordinateElement the first time we get here
    PRIVATE(this)->multielem = SoMultiTextureCoordinateElement::getInstance(PRIVATE(this)->state);
  }
  const SoPrimitiveVertex *vp[3] = { v0,v1,v2 };

  for (int i = 0; i < 3; i++) {
    Vertex v;
    v.vertex = vp[i]->getPoint();
    v.normal = vp[i]->getNormal();
    const SbVec4f & tmp = vp[i]->getTextureCoords();
    v.bumpcoord = SbVec2f(tmp[0], tmp[1]);
    v.texcoord0 = tmp;
    v.texcoordidx = -1;
    
    int midx = vp[i]->getMaterialIndex();
    uint32_t col;
    if (PRIVATE(this)->packedptr) {
      col = PRIVATE(this)->packedptr[SbClamp(midx, 0, PRIVATE(this)->numdiffuse)];
    }
    else {
      SbColor tmpc = PRIVATE(this)->diffuseptr[SbClamp(midx,0,PRIVATE(this)->numdiffuse)];
      float tmpt = PRIVATE(this)->transpptr[SbClamp(midx,0,PRIVATE(this)->numtransp)];
      col = tmpc.getPackedValue(tmpt);
    }
    if (col != PRIVATE(this)->firstcolor) PRIVATE(this)->colorpervertex = TRUE;
    
    v.rgba[0] = col>>24;
    v.rgba[1] = (col>>16)&0xff;
    v.rgba[2] = (col>>8)&0xff;
    v.rgba[3] = col&0xff;
    
    SoDetail * d = (SoDetail*) vp[i]->getDetail();

    if (d && d->isOfType(SoFaceDetail::getClassTypeId())) {
      SoFaceDetail * fd = (SoFaceDetail*) d;
      assert(pointdetailidx != NULL);
      assert(pointdetailidx[i] < fd->getNumPoints());
      
      SoPointDetail * pd = (SoPointDetail*) 
        fd->getPoint(pointdetailidx[i]);
      
      int tidx  = v.texcoordidx = pd->getTextureCoordIndex();            
      if (PRIVATE(this)->numbumpcoords) {
        v.bumpcoord = PRIVATE(this)->bumpcoords[SbClamp(tidx, 0, PRIVATE(this)->numbumpcoords)];
      }
    }
    int32_t idx;
    if (!PRIVATE(this)->vhash.get(v, idx)) {
      idx = PRIVATE(this)->vertices.getLength();
      PRIVATE(this)->vhash.put(v, idx);
      PRIVATE(this)->vertices.append(v);
      PRIVATE(this)->indices.append(idx);
      
      // update texture coordinates for unit 1-n
      for (int i = 1; i <= PRIVATE(this)->lastenabled; i++) {
        if (v.texcoordidx >= 0 &&
            (PRIVATE(this)->multielem->getType(i) == SoTextureCoordinateElement::EXPLICIT)) {
          PRIVATE(this)->multitexcoords[i].append(PRIVATE(this)->multielem->get4(i, v.texcoordidx));
        }
        else if (PRIVATE(this)->multielem->getType(i) == SoTextureCoordinateElement::FUNCTION) {
          PRIVATE(this)->multitexcoords[i].append(PRIVATE(this)->multielem->get(i, v.vertex, v.normal));
        }
        else {
          PRIVATE(this)->multitexcoords[i].append(v.texcoord0);
        }
      }
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

SbBool 
SoPrimitiveVertexCache::colorPerVertex(void) const
{
  return PRIVATE(this)->colorpervertex;
}

const SbVec4f * 
SoPrimitiveVertexCache::getMultiTextureCoordinateArray(const int unit) const
{
  assert(unit <= PRIVATE(this)->lastenabled);
  return PRIVATE(this)->multitexcoords[unit].getArrayPtr();
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

