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

#include "soshape_bumprender.h"
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoBumpMapElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/caches/SoPrimitiveVertexCache.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/C/glue/glp.h>
#include <assert.h>

soshape_bumprender::soshape_bumprender(void)
{
}

soshape_bumprender::~soshape_bumprender()
{
}

// to avoid warnings from SbVec3f::normalize()
inline void NORMALIZE(SbVec3f &v)
{
  float len = v.length();
  if (len) {
    len = 1.0f / len;
    v[0] *= len;
    v[1] *= len;
    v[2] *= len;
  }
}

void
soshape_bumprender::renderBump(SoState * state,
                               const SoPrimitiveVertexCache * cache,
                               SoLight * light, const SbMatrix & toobjectspace)
{
  this->initLight(light, toobjectspace);
  this->calcTSBCoords(cache, light);

  const cc_glglue * glue = sogl_glue_instance(state);
  int i, lastenabled = -1;
  const SbBool * enabled = SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
  
  // disable texture units 1-n
  for (i = 1; i <= lastenabled; i++) {
    if (enabled[i]) {
      cc_glglue_glActiveTexture(glue, GL_TEXTURE0+i);
      glDisable(GL_TEXTURE_2D);
    }
  }
  SoGLImage * bumpimage = SoBumpMapElement::get(state);
  assert(bumpimage);

  // set up textures
  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  bumpimage->getGLDisplayList(state)->call(state);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

  cc_glglue_glActiveTexture(glue, GL_TEXTURE1);
  coin_apply_normalization_cube_map(glue);
  glEnable(GL_TEXTURE_CUBE_MAP);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);

  const SoPrimitiveVertexCache::Vertex * vptr =
    cache->getVertices();
  const int n = cache->getNumIndices();
  const SbVec3f * cmptr = this->cubemaplist.getArrayPtr();

  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, sizeof(SoPrimitiveVertexCache::Vertex),
                            (GLvoid*) &vptr->vertex);
  cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);
  cc_glglue_glTexCoordPointer(glue, 2, GL_FLOAT, sizeof(SoPrimitiveVertexCache::Vertex),
                              (GLvoid*) &vptr->bumpcoord);
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE1);
  cc_glglue_glTexCoordPointer(glue, 3, GL_FLOAT, 0,
                              (GLvoid*) cmptr);
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glDrawElements(glue, GL_TRIANGLES, n, GL_UNSIGNED_INT,
                           (const GLvoid*) cache->getIndices());

  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0);
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);

  glDisable(GL_TEXTURE_CUBE_MAP); // unit 1
  
  // reenable texture units 1-n if enabled  
  for (i = 1; i <= lastenabled; i++) {
    if (enabled[i]) {
      cc_glglue_glActiveTexture(glue, GL_TEXTURE0+i);
      glEnable(GL_TEXTURE_2D);
    }
  }

  if (lastenabled >= 1 && enabled[1]) {
    // restore blend mode for texture unit 1
    SoGLMultiTextureImageElement::restore(state, 1);
  }
  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);
  // disable texturing for unit 0 if not enabled
  if (!SoGLTextureEnabledElement::get(state)) glDisable(GL_TEXTURE_2D);
}
  
void
soshape_bumprender::renderNormal(SoState * state, const SoPrimitiveVertexCache * cache)
{
  const cc_glglue * glue = sogl_glue_instance(state);

  const SoPrimitiveVertexCache::Vertex * vptr =
    cache->getVertices();
  const int n = cache->getNumIndices();

  int lastenabled = -1;
  const SbBool * enabled = 
    SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
  
  SbBool colorpervertex = cache->colorPerVertex();

  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, sizeof(SoPrimitiveVertexCache::Vertex),
                            (GLvoid*) &vptr->vertex);
  cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);

  cc_glglue_glNormalPointer(glue, GL_FLOAT, sizeof(SoPrimitiveVertexCache::Vertex),
                            (GLvoid*) &vptr->normal);
  cc_glglue_glEnableClientState(glue, GL_NORMAL_ARRAY);

  cc_glglue_glTexCoordPointer(glue, 4, GL_FLOAT, sizeof(SoPrimitiveVertexCache::Vertex),
                              (GLvoid*) &vptr->texcoord0);
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  if (colorpervertex) {
    cc_glglue_glColorPointer(glue, 4, GL_UNSIGNED_BYTE,
                             sizeof(SoPrimitiveVertexCache::Vertex),
                             (GLvoid*) &vptr->rgba);
    cc_glglue_glEnableClientState(glue, GL_COLOR_ARRAY);
  }

  for (int i = 1; i <= lastenabled; i++) {
    if (enabled[i]) {
      cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0 + i);
      cc_glglue_glTexCoordPointer(glue, 4, GL_FLOAT, 0,
                                  (GLvoid*) cache->getMultiTextureCoordinateArray(i));
      cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);
    }
  }
  
  cc_glglue_glDrawElements(glue, GL_TRIANGLES, n, GL_UNSIGNED_INT,
                           (const GLvoid*) cache->getIndices());
  
  for (int j = 1; j <= lastenabled; j++) {
    if (enabled[j]) {
      cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0 + j);
      cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
    }
  }
  if (lastenabled >= 1) {
    // reset to default
    cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0); 
  }

  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_NORMAL_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  if (colorpervertex) {
    cc_glglue_glDisableClientState(glue, GL_COLOR_ARRAY);
  }
}

void
soshape_bumprender::calcTangentSpace(const SoPrimitiveVertexCache * cache)
{
  const SoPrimitiveVertexCache::Vertex * vptr =
    cache->getVertices();
  int i;

  const int numv = cache->getNumVertices();
  const int32_t * idxptr = cache->getIndices();

  this->tangentlist.truncate(0);

  for (i = 0; i < numv; i++) {
    this->tangentlist.append(SbVec3f(0.0f, 0.0f, 0.0f));
    this->tangentlist.append(SbVec3f(0.0f, 0.0f, 0.0f));
  }
  const int numi = cache->getNumIndices();

  SbVec3f sTangent;
  SbVec3f tTangent;

  int idx[3];

  for (i = 0; i < numi; i += 3) {
    idx[0] = idxptr[i];
    idx[1] = idxptr[i+1];
    idx[2] = idxptr[i+2];
    const SoPrimitiveVertexCache::Vertex & v0 = vptr[idx[0]];
    const SoPrimitiveVertexCache::Vertex & v1 = vptr[idx[1]];
    const SoPrimitiveVertexCache::Vertex & v2 = vptr[idx[2]];

    SbVec3f side0 = v1.vertex - v0.vertex;
    SbVec3f side1 = v2.vertex - v0.vertex;

    float deltaT0 = v1.bumpcoord[1] - v0.bumpcoord[1];
    float deltaT1 = v2.bumpcoord[1] - v0.bumpcoord[1];
    sTangent = deltaT1 * side0 - deltaT0 * side1;
    NORMALIZE(sTangent);

    float deltaS0 = v1.bumpcoord[0] - v0.bumpcoord[0];
    float deltaS1 = v2.bumpcoord[0] - v0.bumpcoord[0];
    tTangent = deltaS1 * side0 - deltaS0 * side1;
    NORMALIZE(tTangent);

    for (int j = 0; j < 3; j++) {
      this->tangentlist[idx[j]*2] += sTangent;
      this->tangentlist[idx[j]*2+1] += tTangent;
    }
  }
  for (i = 0; i < numv; i++) {
    this->tangentlist[i*2].normalize();
    this->tangentlist[i*2+1].normalize();
  }
}

void
soshape_bumprender::calcTSBCoords(const SoPrimitiveVertexCache * cache, SoLight * light)
{
  SbVec3f lightvec;
  SbVec3f tlightvec;

  const SoPrimitiveVertexCache::Vertex * vptr =
    cache->getVertices();
  const int numv = cache->getNumVertices();

  this->cubemaplist.truncate(0);
  for (int i = 0; i < numv; i++) {
    const SoPrimitiveVertexCache::Vertex & v = vptr[i];
    SbVec3f sTangent = this->tangentlist[i*2];
    SbVec3f tTangent = this->tangentlist[i*2+1];
    lightvec = this->getLightVec(v.vertex);
    tlightvec = lightvec;
#if 0 // FIXME: I don't think it's necessary to do this test. pederb, 2003-11-20
    SbVec3f tcross = tTangent.cross(sTangent);
    if (tcross.dot(v.normal) < 0.0f) {
      tlightvec = -tlightvec;
    }
#endif // disabled, probably not necessary
    this->cubemaplist.append(SbVec3f(sTangent.dot(tlightvec),
                                     tTangent.dot(tlightvec),
                                     v.normal.dot(lightvec)));

  }
}

void
soshape_bumprender::initLight(SoLight * light, const SbMatrix & m)
{
  if (light->isOfType(SoPointLight::getClassTypeId())) {
    SoPointLight * pl = (SoPointLight*) light;
    this->lightvec = pl->location.getValue();
    m.multVecMatrix(this->lightvec, this->lightvec);
    this->ispointlight = TRUE;
  }
  else if (light->isOfType(SoDirectionalLight::getClassTypeId())) {
    SoDirectionalLight * dir = (SoDirectionalLight*)light;
    m.multVecMatrix(-(dir->direction.getValue()), this->lightvec);
    this->ispointlight = FALSE;
    NORMALIZE(this->lightvec);
  }
  else if (light->isOfType(SoSpotLight::getClassTypeId())) {
    SoSpotLight * pl = (SoSpotLight*) light;
    this->lightvec = pl->location.getValue();
    m.multVecMatrix(this->lightvec, this->lightvec);
    this->ispointlight = TRUE;

  }
  else {
    this->lightvec = SbVec3f(0.0f, 0.0f, 1.0f);
    this->ispointlight = FALSE;

  }
}


SbVec3f
soshape_bumprender::getLightVec(const SbVec3f & v) const
{
  if (this->ispointlight) {
    SbVec3f tmp = lightvec - v;
    NORMALIZE(tmp);
    return tmp;
  }
  else return this->lightvec;
}
