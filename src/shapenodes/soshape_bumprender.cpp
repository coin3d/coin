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
#include <Inventor/SbMatrix.h>

soshape_bumprender::soshape_bumprender(void)
{
  this->state = NULL;
  this->glglue = NULL;
  this->bumpimage = NULL;
}

soshape_bumprender::~soshape_bumprender()
{
}

void
soshape_bumprender::init(SoState * state)
{
  this->state = state;
  this->glglue = sogl_glue_instance(state);
  this->vlist.truncate(0);
  this->bumpimage = SoBumpMapElement::get(state);
}

// FIXME: create an SoPrimitiveVertexCache to store the triangles to
// avoid calling generatePrimitives() every frame.
void
soshape_bumprender::addTriangle(const SoPrimitiveVertex * v0,
                                const SoPrimitiveVertex * v1,
                                const SoPrimitiveVertex * v2)
{
  const SoPrimitiveVertex *vp[3] = { v0,v1,v2 };

  for (int i = 0; i < 3; i++) {
    Vertex v;
    v.v = vp[i]->getPoint();
    v.n = vp[i]->getNormal();
    const SbVec4f & tmp = vp[i]->getTextureCoords();
    v.bc = SbVec2f(tmp[0], tmp[1]);
    v.tc = tmp;
    const SoDetail * d = vp[i]->getDetail();
    if (d && d->isOfType(SoPointDetail::getClassTypeId())) {
      // FIXME: fetch texture coordinates from
      // SoBumpMapCoordinateElement and update bc
    }

    this->vlist.append(v);
  }
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
soshape_bumprender::renderBump(SoLight * light, const SbMatrix & toobjectspace)
{
  this->initLight(light, toobjectspace);
  this->calcTSBCoords(light);
  const cc_glglue * glue = this->glglue;

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // set up textures
  glEnable(GL_TEXTURE_2D);
  this->bumpimage->getGLDisplayList(state)->call(state);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

  cc_glglue_glActiveTexture(glue, GL_TEXTURE1);
  soshape_apply_normalization_cube_map(glue, 32);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_CUBE_MAP);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);

  const Vertex * vptr = this->vlist.getArrayPtr();
  const SbVec3f * cmptr = this->cubemaplist.getArrayPtr();
  const int n = this->vlist.getLength();

  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, sizeof(Vertex),
                            (GLvoid*) &vptr->v);
  cc_glglue_glEnableClientState(this->glglue, GL_VERTEX_ARRAY);
  cc_glglue_glTexCoordPointer(glue, 2, GL_FLOAT, sizeof(Vertex),
                              (GLvoid*) &vptr->bc);
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE1);
  cc_glglue_glTexCoordPointer(glue, 3, GL_FLOAT, 0,
                              (GLvoid*) cmptr);
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glDrawArrays(glue, GL_TRIANGLES, 0, n);

  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0);
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);

  glDisable(GL_TEXTURE_CUBE_MAP);
  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
}

void
soshape_bumprender::renderNormal(void)
{
  const cc_glglue * glue = this->glglue;
  const Vertex * vptr = this->vlist.getArrayPtr();
  const int n = this->vlist.getLength();

  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, sizeof(Vertex),
                            (GLvoid*) &vptr->v);
  cc_glglue_glEnableClientState(this->glglue, GL_VERTEX_ARRAY);

  cc_glglue_glNormalPointer(glue, GL_FLOAT, sizeof(Vertex),
                            (GLvoid*) &vptr->n);
  cc_glglue_glEnableClientState(glue, GL_NORMAL_ARRAY);

  cc_glglue_glTexCoordPointer(glue, 4, GL_FLOAT, sizeof(Vertex),
                              (GLvoid*) &vptr->tc);
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);


  cc_glglue_glDrawArrays(glue, GL_TRIANGLES, 0, n);

  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_NORMAL_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
}

void
soshape_bumprender::calcTSBCoords(SoLight * light)
{
  Vertex * vptr = (Vertex*) this->vlist.getArrayPtr();
  const int n = this->vlist.getLength();

  this->cubemaplist.truncate(0);

  SbVec3f sTangent;
  SbVec3f tTangent;
  SbVec3f lightvec;
  SbVec3f tlightvec;

  for (int i = 0; i < n; i += 3) {
    const Vertex & v0 = vptr[i];
    const Vertex & v1 = vptr[i+1];
    const Vertex & v2 = vptr[i+2];

    SbVec3f side0 = v1.v - v0.v;
    SbVec3f side1 = v2.v - v0.v;
//    SbVec3f normal = side0.cross(side1);
//     normal = v0
//     NORMALIZE(normal);

    float deltaT0 = v1.bc[1] - v0.bc[1];
    float deltaT1 = v2.bc[1] - v0.bc[1];
    sTangent = deltaT1 * side0 - deltaT0 * side1;
    NORMALIZE(sTangent);

    float deltaS0 = v1.bc[0] - v0.bc[0];
    float deltaS1 = v2.bc[0] - v0.bc[0];
    tTangent = deltaS1 * side0 - deltaS0 * side1;
    NORMALIZE(tTangent);

    SbVec3f tcross = tTangent.cross(sTangent);

    lightvec = this->getLightVec(v0.v);
    tlightvec = lightvec;
    if (v0.n.dot(tcross) < 0.0f) {
      tlightvec = -tlightvec;
    }
    this->cubemaplist.append(SbVec3f(sTangent.dot(tlightvec),
                                 tTangent.dot(tlightvec),
                                 v0.n.dot(lightvec)));

    lightvec = this->getLightVec(v1.v);
    tlightvec = lightvec;
    if (v1.n.dot(tcross) < 0.0f) {
      tlightvec = -tlightvec;
    }
    this->cubemaplist.append(SbVec3f(sTangent.dot(tlightvec),
                                 tTangent.dot(tlightvec),
                                 v1.n.dot(lightvec)));

    lightvec = this->getLightVec(v2.v);
    tlightvec = lightvec;
    if (v2.n.dot(tcross) < 0.0f) {
      tlightvec = -tlightvec;
    }
    this->cubemaplist.append(SbVec3f(sTangent.dot(tlightvec),
                                 tTangent.dot(tlightvec),
                                 v2.n.dot(lightvec)));
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
  //  return SbVec3f(0.0f, 0.0f, 1.0f);
  if (this->ispointlight) {
    SbVec3f tmp = lightvec - v;
    NORMALIZE(tmp);
    return tmp;
  }
  else return this->lightvec;
}
