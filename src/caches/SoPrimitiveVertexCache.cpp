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
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/details/SoFaceDetail.h>
#include <stddef.h>
#include <Inventor/SbDict.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoGL.h>
#include <string.h> // memcmp()

#define MAX_UNITS 16

typedef struct  {
  GLuint vertex;
  GLuint normal;
  GLuint texcoord0;
  GLuint rgba;
  GLuint multitex[MAX_UNITS];
} SoPrimitiveVertexCache_vboidx;

// for debugging
static unsigned long total_vbo_memory = 0;

class SoPrimitiveVertexCacheP {
public:
  SoPrimitiveVertexCacheP(void) 
    : vertexlist(256), 
      normallist(256),
      texcoordlist(256),
      bumpcoordlist(256),
      rgbalist(256),
      indices(1024), 
      vhash(1024),
      vbodict(4) { }

  SbList <SoPrimitiveVertexCache::Vertex> vertices;

  SbList <SbVec3f> vertexlist;
  SbList <SbVec3f> normallist;
  SbList <SbVec4f> texcoordlist;
  SbList <SbVec2f> bumpcoordlist;
  SbList <uint8_t> rgbalist;

  SbList <int32_t> indices;
  SbList <int32_t> lineindices;
  SbList <int32_t> pointindices;
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
  SbDict vbodict;

  void addVertex(const SoPrimitiveVertexCache::Vertex & v);
  void enableArrays(const cc_glglue * glue,
                    const SbBool color, const SbBool normal,
                    const SbBool texture, const SbBool * enabled,
                    const int lastenabled);
  
  void disableArrays(const cc_glglue * glue,
                     const SbBool color, const SbBool normal,
                     const SbBool texture, const SbBool * enabled,
                     const int lastenabled);

  void enableVBOs(const cc_glglue * glue,
                  const unsigned long contextid,
                  const SbBool color, const SbBool normal,
                  const SbBool texture, const SbBool * enabled,
                  const int lastenabled);
  
  void disableVBOs(const cc_glglue * glue,
                   const SbBool color, const SbBool normal,
                   const SbBool texture, const SbBool * enabled,
                   const int lastenabled);
};

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
SoPrimitiveVertexCache::renderTriangles(SoState * state, const int arrays) const
{
  int lastenabled = -1;
  const int n = this->getNumIndices();
  if (n == 0) return;

  const SbBool * enabled = NULL;
  const SbBool normal = (arrays & NORMAL) != 0;
  const SbBool texture = (arrays & TEXCOORD) != 0;
  SbBool color = this->colorPerVertex() && ((arrays & COLOR) != 0);  
  if (texture) {
    enabled = SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
  }
  const cc_glglue * glue = sogl_glue_instance(state);
  
  // not enabled yet, needs more testing, pederb 2004-02-24
  // SbBool renderasvbo = cc_glglue_has_vertex_buffer_object(glue);
  SbBool renderasvbo = FALSE;

  if (renderasvbo) {
    unsigned long contextid = (unsigned long) SoGLCacheContextElement::get(state);    
    PRIVATE(this)->enableVBOs(glue, contextid, color, normal, texture, enabled, lastenabled);
    cc_glglue_glDrawElements(glue, GL_TRIANGLES, n, GL_UNSIGNED_INT,
                             (const GLvoid*) this->getIndices());
    PRIVATE(this)->disableVBOs(glue, color, normal, texture, enabled, lastenabled);
  }
  else {
    PRIVATE(this)->enableArrays(glue, color, normal, texture, enabled, lastenabled);
    cc_glglue_glDrawElements(glue, GL_TRIANGLES, n, GL_UNSIGNED_INT,
                             (const GLvoid*) this->getIndices());
    PRIVATE(this)->disableArrays(glue, color, normal, texture, enabled, lastenabled);
  }
}

void 
SoPrimitiveVertexCache::renderLines(SoState * state, const int arrays) const
{
  int lastenabled = -1;
  const int n = this->getNumLineIndices();
  if (n == 0) return;
  const SbBool * enabled = NULL;
  const SbBool normal = (arrays & NORMAL) != 0;
  const SbBool texture = (arrays & TEXCOORD) != 0;
  SbBool color = this->colorPerVertex() && ((arrays & COLOR) != 0);  
  if (texture) {
    enabled = SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
  }
  const cc_glglue * glue = sogl_glue_instance(state);

  PRIVATE(this)->enableArrays(glue, color, normal, texture, enabled, lastenabled);
  cc_glglue_glDrawElements(glue, GL_LINES, n, GL_UNSIGNED_INT,
                           (const GLvoid*) this->getLineIndices());
  PRIVATE(this)->disableArrays(glue, color, normal, texture, enabled, lastenabled);
}

void 
SoPrimitiveVertexCache::renderPoints(SoState * state, const int arrays) const
{
  int lastenabled = -1;
  const int n = this->getNumPointIndices();
  if (n == 0) return;
  const SbBool * enabled = NULL;
  const SbBool normal = (arrays & NORMAL) != 0;
  const SbBool texture = (arrays & TEXCOORD) != 0;
  SbBool color = this->colorPerVertex() && ((arrays & COLOR) != 0);  
  if (texture) {
    enabled = SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
  }
  const cc_glglue * glue = sogl_glue_instance(state);

  PRIVATE(this)->enableArrays(glue, color, normal, texture, enabled, lastenabled);
  cc_glglue_glDrawElements(glue, GL_POINTS, n, GL_UNSIGNED_INT,
                           (const GLvoid*) this->getPointIndices());
  PRIVATE(this)->disableArrays(glue, color, normal, texture, enabled, lastenabled);
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
  const SoPrimitiveVertex *vp[3] = { v0, v1, v2 };

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
      idx = PRIVATE(this)->vertexlist.getLength();
      PRIVATE(this)->vhash.put(v, idx);
      PRIVATE(this)->addVertex(v);      
      PRIVATE(this)->indices.append(idx);
      
      // update texture coordinates for unit 1-n
      for (int j = 1; j <= PRIVATE(this)->lastenabled; j++) {
        if (v.texcoordidx >= 0 &&
            (PRIVATE(this)->multielem->getType(j) == SoTextureCoordinateElement::EXPLICIT)) {
          PRIVATE(this)->multitexcoords[j].append(PRIVATE(this)->multielem->get4(j, v.texcoordidx));
        }
        else if (PRIVATE(this)->multielem->getType(j) == SoTextureCoordinateElement::FUNCTION) {
          PRIVATE(this)->multitexcoords[j].append(PRIVATE(this)->multielem->get(j, v.vertex, v.normal));
        }
        else {
          PRIVATE(this)->multitexcoords[j].append(v.texcoord0);
        }
      }
    }
    else {
      PRIVATE(this)->indices.append(idx);
    }
  }
}

void 
SoPrimitiveVertexCache::addLine(const SoPrimitiveVertex * v0,
                                const SoPrimitiveVertex * v1)
{
  if (PRIVATE(this)->lastenabled >= 1 && PRIVATE(this)->multielem == NULL) {
    // fetch SoMultiTextureCoordinateElement the first time we get here
    PRIVATE(this)->multielem = SoMultiTextureCoordinateElement::getInstance(PRIVATE(this)->state);
  }
  const SoPrimitiveVertex *vp[2] = { v0,v1 };

  for (int i = 0; i < 2; i++) {
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

    if (d && d->isOfType(SoLineDetail::getClassTypeId())) {
      SoLineDetail * ld = (SoLineDetail*) d;
      const SoPointDetail * pd;
      if (i == 0) pd = ld->getPoint0();
      else pd = ld->getPoint1();
      
      int tidx  = v.texcoordidx = ((SoPointDetail*)pd)->getTextureCoordIndex();            
      if (PRIVATE(this)->numbumpcoords) {
        v.bumpcoord = PRIVATE(this)->bumpcoords[SbClamp(tidx, 0, PRIVATE(this)->numbumpcoords)];
      }
    }
    int32_t idx;
    if (!PRIVATE(this)->vhash.get(v, idx)) {
      idx = PRIVATE(this)->vertexlist.getLength();
      PRIVATE(this)->vhash.put(v, idx);
      PRIVATE(this)->addVertex(v);
      PRIVATE(this)->lineindices.append(idx);
      
      // update texture coordinates for unit 1-n
      for (int j = 1; j <= PRIVATE(this)->lastenabled; j++) {
        if (v.texcoordidx >= 0 &&
            (PRIVATE(this)->multielem->getType(j) == SoTextureCoordinateElement::EXPLICIT)) {
          PRIVATE(this)->multitexcoords[j].append(PRIVATE(this)->multielem->get4(j, v.texcoordidx));
        }
        else if (PRIVATE(this)->multielem->getType(j) == SoTextureCoordinateElement::FUNCTION) {
          PRIVATE(this)->multitexcoords[j].append(PRIVATE(this)->multielem->get(j, v.vertex, v.normal));
        }
        else {
          PRIVATE(this)->multitexcoords[j].append(v.texcoord0);
        }
      }
    }
    else {
      PRIVATE(this)->lineindices.append(idx);
    }
  }
}

void 
SoPrimitiveVertexCache::addPoint(const SoPrimitiveVertex * v0)
{
  if (PRIVATE(this)->lastenabled >= 1 && PRIVATE(this)->multielem == NULL) {
    // fetch SoMultiTextureCoordinateElement the first time we get here
    PRIVATE(this)->multielem = SoMultiTextureCoordinateElement::getInstance(PRIVATE(this)->state);
  }
  Vertex v;
  v.vertex = v0->getPoint();
  v.normal = v0->getNormal();
  const SbVec4f & tmp = v0->getTextureCoords();
  v.bumpcoord = SbVec2f(tmp[0], tmp[1]);
  v.texcoord0 = tmp;
  v.texcoordidx = -1;
    
  int midx = v0->getMaterialIndex();
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
    
  SoDetail * d = (SoDetail*) v0->getDetail();

  if (d && d->isOfType(SoPointDetail::getClassTypeId())) {
    SoPointDetail * pd = (SoPointDetail*) d;      
    int tidx  = v.texcoordidx = pd->getTextureCoordIndex();            
    if (PRIVATE(this)->numbumpcoords) {
      v.bumpcoord = PRIVATE(this)->bumpcoords[SbClamp(tidx, 0, PRIVATE(this)->numbumpcoords)];
    }
  }

  int32_t idx;
  if (!PRIVATE(this)->vhash.get(v, idx)) {
    idx = PRIVATE(this)->vertexlist.getLength();
    PRIVATE(this)->vhash.put(v, idx);
    PRIVATE(this)->addVertex(v);
    PRIVATE(this)->pointindices.append(idx);
      
    // update texture coordinates for unit 1-n
    for (int j = 1; j <= PRIVATE(this)->lastenabled; j++) {
      if (v.texcoordidx >= 0 &&
          (PRIVATE(this)->multielem->getType(j) == SoTextureCoordinateElement::EXPLICIT)) {
        PRIVATE(this)->multitexcoords[j].append(PRIVATE(this)->multielem->get4(j, v.texcoordidx));
      }
      else if (PRIVATE(this)->multielem->getType(j) == SoTextureCoordinateElement::FUNCTION) {
        PRIVATE(this)->multitexcoords[j].append(PRIVATE(this)->multielem->get(j, v.vertex, v.normal));
      }
      else {
        PRIVATE(this)->multitexcoords[j].append(v.texcoord0);
      }
    }
  }
  else {
    PRIVATE(this)->pointindices.append(idx);
  }
}

int
SoPrimitiveVertexCache::getNumVertices(void) const
{
  return PRIVATE(this)->vertexlist.getLength();
}

const SbVec3f * 
SoPrimitiveVertexCache::getVertexArray(void) const
{
  return PRIVATE(this)->vertexlist.getArrayPtr();
}

const SbVec3f * 
SoPrimitiveVertexCache::getNormalArray(void) const
{
  return PRIVATE(this)->normallist.getArrayPtr();
}

const SbVec4f * 
SoPrimitiveVertexCache::getTexCoordArray(void) const
{
  return PRIVATE(this)->texcoordlist.getArrayPtr();
}

const SbVec2f * 
SoPrimitiveVertexCache::getBumpCoordArray(void) const
{
  return PRIVATE(this)->bumpcoordlist.getArrayPtr();
}

const uint8_t * 
SoPrimitiveVertexCache::getColorArray(void) const
{
  return PRIVATE(this)->rgbalist.getArrayPtr();
}

int
SoPrimitiveVertexCache::getNumIndices(void) const
{
  return PRIVATE(this)->indices.getLength();
}

const int32_t *
SoPrimitiveVertexCache::getIndices(void) const
{
  return PRIVATE(this)->indices.getArrayPtr();
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

int 
SoPrimitiveVertexCache::getNumLineIndices(void) const
{
  return PRIVATE(this)->lineindices.getLength();
}

int 
SoPrimitiveVertexCache::getNumPointIndices(void) const
{
  return PRIVATE(this)->pointindices.getLength();
}

  
const int32_t * 
SoPrimitiveVertexCache::getLineIndices(void) const
{
  return PRIVATE(this)->lineindices.getArrayPtr();
}

const int32_t * 
SoPrimitiveVertexCache::getPointIndices(void) const
{
  return PRIVATE(this)->pointindices.getArrayPtr();
}

void 
SoPrimitiveVertexCache::fit(void)
{
  PRIVATE(this)->vertexlist.fit();
  PRIVATE(this)->normallist.fit();
  PRIVATE(this)->texcoordlist.fit();
  PRIVATE(this)->bumpcoordlist.fit();
  PRIVATE(this)->rgbalist.fit();
  PRIVATE(this)->indices.fit();
  PRIVATE(this)->lineindices.fit();
  PRIVATE(this)->pointindices.fit();
  PRIVATE(this)->vhash.clear();
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

void 
SoPrimitiveVertexCacheP::addVertex(const SoPrimitiveVertexCache::Vertex & v) 
{
  this->vertexlist.append(v.vertex);
  this->normallist.append(v.normal);
  this->texcoordlist.append(v.texcoord0);
  this->bumpcoordlist.append(v.bumpcoord);
  for (int c = 0; c < 4; c++) {
    this->rgbalist.append(v.rgba[c]);
  }
}

void 
SoPrimitiveVertexCacheP::enableArrays(const cc_glglue * glue,
                                      const SbBool color, const SbBool normal,
                                      const SbBool texture, const SbBool * enabled,
                                      const int lastenabled)
{
  int i;
  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, 0,
                            (GLvoid*) this->vertexlist.getArrayPtr());
  cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);
  
  if (color) {
    cc_glglue_glColorPointer(glue, 4, GL_UNSIGNED_BYTE, 0,
                             (GLvoid*) this->rgbalist.getArrayPtr());
    cc_glglue_glEnableClientState(glue, GL_COLOR_ARRAY);
  }

  if (texture) {
    cc_glglue_glTexCoordPointer(glue, 4, GL_FLOAT, 0,
                                (GLvoid*) this->texcoordlist.getArrayPtr());
    cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);
    
    for (i = 1; i <= lastenabled; i++) {
      if (enabled[i]) {
        cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0 + i);
        cc_glglue_glTexCoordPointer(glue, 4, GL_FLOAT, 0,
                                    (GLvoid*) this->multitexcoords[i].getArrayPtr());
        cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);
      }
    }
  }
  if (normal) {
    cc_glglue_glNormalPointer(glue, GL_FLOAT, 0,
                              (GLvoid*) this->normallist.getArrayPtr());
    cc_glglue_glEnableClientState(glue, GL_NORMAL_ARRAY);
  }
}

  
void 
SoPrimitiveVertexCacheP::disableArrays(const cc_glglue * glue,
                                       const SbBool color, const SbBool normal,
                                       const SbBool texture, const SbBool * enabled,
                                       const int lastenabled)
{
  int i;
  if (normal) {
    cc_glglue_glDisableClientState(glue, GL_NORMAL_ARRAY);
  }
  if (texture) {
    for (i = 1; i <= lastenabled; i++) {
      if (enabled[i]) {
        cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0 + i);
        cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
      }
    }
    if (lastenabled >= 1) {
      // reset to default
      cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0);
    }
    cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  }
  if (color) {
    cc_glglue_glDisableClientState(glue, GL_COLOR_ARRAY);
  }
  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);
}

void 
SoPrimitiveVertexCacheP::enableVBOs(const cc_glglue * glue,
                                    unsigned long contextid,
                                    const SbBool color, const SbBool normal,
                                    const SbBool texture, const SbBool * enabled,
                                    const int lastenabled)
{
  void * tmp;
  SoPrimitiveVertexCache_vboidx * vbo;
  if (!this->vbodict.find(contextid, tmp)) {
    vbo = new SoPrimitiveVertexCache_vboidx;
    memset(vbo, 0, sizeof(SoPrimitiveVertexCache_vboidx));
    (void) this->vbodict.enter(contextid, (void*) vbo);
  } else {
    vbo = (SoPrimitiveVertexCache_vboidx *) tmp;
  }

  int i;
  if (vbo->vertex == 0) {
    cc_glglue_glGenBuffers(glue, 1, &vbo->vertex);
    cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->vertex);
    cc_glglue_glBufferData(glue, GL_ARRAY_BUFFER, 
                           this->vertexlist.getLength()*3*sizeof(float),
                           this->vertexlist.getArrayPtr(),
                           GL_STATIC_DRAW);
    total_vbo_memory +=  this->vertexlist.getLength()*3*sizeof(float);
  }
  else {
    cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->vertex);    
  }
  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, 0, NULL);  
  cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);
  
  if (color) {
    if (vbo->rgba == 0) {
      cc_glglue_glGenBuffers(glue, 1, &vbo->rgba);
      cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->rgba);
      cc_glglue_glBufferData(glue, GL_ARRAY_BUFFER, 
                             this->rgbalist.getLength() * sizeof(uint8_t),
                             this->rgbalist.getArrayPtr(),
                             GL_STATIC_DRAW);
      total_vbo_memory += this->rgbalist.getLength() * sizeof(uint8_t);
    }
    else {
      cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->rgba);
    }
    cc_glglue_glColorPointer(glue, 4, GL_UNSIGNED_BYTE, 0, NULL);
    cc_glglue_glEnableClientState(glue, GL_COLOR_ARRAY);
  }
  if (texture) {
    if (vbo->texcoord0 == 0) {
      cc_glglue_glGenBuffers(glue, 1, &vbo->texcoord0);
      cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->texcoord0);
      cc_glglue_glBufferData(glue, GL_ARRAY_BUFFER,
                             this->texcoordlist.getLength()*4*sizeof(float),
                             this->texcoordlist.getArrayPtr(),
                             GL_STATIC_DRAW);
      total_vbo_memory += this->texcoordlist.getLength()*4*sizeof(float);
    }
    else {
      cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->texcoord0);    
    }
    
    cc_glglue_glTexCoordPointer(glue, 4, GL_FLOAT, 0, NULL);
    cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);
    
    for (i = 1; i <= lastenabled; i++) {
      if (enabled[i]) {
        if (vbo->multitex[i] == 0) {
          cc_glglue_glGenBuffers(glue, 1, &vbo->multitex[i]);
          cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->multitex[i]);
          cc_glglue_glBufferData(glue, GL_ARRAY_BUFFER,
                                 this->multitexcoords[i].getLength()*4*sizeof(float),
                                 this->multitexcoords[i].getArrayPtr(),
                                 GL_STATIC_DRAW);
          total_vbo_memory += this->multitexcoords[i].getLength()*4*sizeof(float);
        }
        else {
          cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->multitex[i]);    
        }

        cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0 + i);
        cc_glglue_glTexCoordPointer(glue, 4, GL_FLOAT, 0, NULL);
        cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);
      }
    }
  }
  if (normal) {
    if (vbo->normal == 0) {
      cc_glglue_glGenBuffers(glue, 1, &vbo->normal);
      cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->normal);
      cc_glglue_glBufferData(glue, GL_ARRAY_BUFFER, 
                             this->normallist.getLength()*3*sizeof(float),
                             this->normallist.getArrayPtr(),
                             GL_STATIC_DRAW);
      total_vbo_memory += this->normallist.getLength()*3*sizeof(float);
    }
    else {
      cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, vbo->normal);    
    }
    
    cc_glglue_glNormalPointer(glue, GL_FLOAT, 0, NULL);
    cc_glglue_glEnableClientState(glue, GL_NORMAL_ARRAY);
  }
}

  
void 
SoPrimitiveVertexCacheP::disableVBOs(const cc_glglue * glue,
                                     const SbBool color, const SbBool normal,
                                     const SbBool texture, const SbBool * enabled,
                                     const int lastenabled)
{
  this->disableArrays(glue, color, normal, texture, enabled, lastenabled);
  cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0); // Reset VBO binding  
}

#undef MAX_UNITS



