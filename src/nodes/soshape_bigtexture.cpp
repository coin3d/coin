/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#include "soshape_bigtexture.h"
#include <Inventor/SbClip.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#if HAVE_WINDOWS_H
#include <windows.h>
#endif
#include <GL/gl.h>

static SbClip * bigtexture_clipper;
static SbPlane * bigtexture_planes;
static SoPrimitiveVertex * bigtexture_primv;
static SbVec2f bigtexture_start;
static SbVec2f bigtexture_end;
static SbVec2f bigtexture_tcmul;
static SoGLBigImage * bigtexture_image;
static SbBool bigtexture_didsubapply;
static float bigtexture_quality;
static int bigtexture_primvcount;
static int bigtexture_subidx;

static void *
bigtexture_clipcb(const SbVec3f & v0, void * vdata0,
                  const SbVec3f & v1, void * vdata1,
                  const SbVec3f & newvertex,
                  void * userdata)
{
  SoPrimitiveVertex * pv0 = (SoPrimitiveVertex*) vdata0;
  SoPrimitiveVertex * pv1 = (SoPrimitiveVertex*) vdata1;

  float dist = (v1-v0).length();
  float newdist = (newvertex-v0).length();
  if (dist == 0.0f) newdist = 0.0f;
  else newdist /= dist;

  SoPrimitiveVertex npv;
  npv.setPoint(pv0->getPoint() + (pv1->getPoint()-pv0->getPoint()) * newdist);
  npv.setTextureCoords(SbVec2f(newvertex[0], newvertex[1]));
  npv.setNormal(pv0->getNormal() + (pv1->getNormal()-pv0->getNormal()) * newdist);
  npv.setMaterialIndex(pv0->getMaterialIndex());

  bigtexture_primv[bigtexture_primvcount] = npv;
  return &bigtexture_primv[bigtexture_primvcount++];
}

static void
cleanup_bigtexture(void)
{
  delete bigtexture_clipper;
  delete[] bigtexture_planes;
  delete[] bigtexture_primv;
}

int
bigtexture_init(SoState * state, 
                SoGLBigImage * image,
                const float quality)
{
  if (bigtexture_clipper == NULL) {
    bigtexture_clipper = new SbClip(bigtexture_clipcb);
    bigtexture_planes = new SbPlane[4];
    bigtexture_primv = new SoPrimitiveVertex[256];
    atexit(cleanup_bigtexture);
  }
  bigtexture_image = image;
  bigtexture_quality = quality;

  return image->initSubImages(state, SbVec2s(512, 512));
}

void 
bigtexture_subinit(SoState * state, const int idx)
{
  bigtexture_subidx = idx;
  bigtexture_didsubapply = FALSE;
  bigtexture_image->handleSubImage(idx,
                                   bigtexture_start,
                                   bigtexture_end,
                                   bigtexture_tcmul);

#ifdef BIGTEXTURE_DEBUG
  fprintf(stderr,"big start: %g %g\n",
          bigtexture_start[0], bigtexture_start[1]);
  fprintf(stderr,"big end: %g %g\n",
          bigtexture_end[0], bigtexture_end[1]);
#endif
  
  bigtexture_planes[0] = SbPlane(SbVec3f(1.0f, 0.0f, 0.0f), bigtexture_start[0]);
  bigtexture_planes[1] = SbPlane(SbVec3f(0.0f, 1.0f, 0.0f), bigtexture_start[1]);
  bigtexture_planes[2] = SbPlane(SbVec3f(-1.0f, 0.0f, 0.0f), -bigtexture_end[0]);
  bigtexture_planes[3] = SbPlane(SbVec3f(0.0f, -1.0f, 0.0f), -bigtexture_end[1]);
}

void 
bigtexture_triangle(SoState * state,
                    SoMaterialBundle & mb,
                    const SoPrimitiveVertex * v1,
                    const SoPrimitiveVertex * v2,
                    const SoPrimitiveVertex * v3)
{
  bigtexture_primv[0] = *v1;
  bigtexture_primv[1] = *v2;
  bigtexture_primv[2] = *v3;
  bigtexture_primvcount = 3;
  bigtexture_clipper->reset();
  bigtexture_clipper->addVertex(SbVec3f(v1->getTextureCoords()[0],
                                        v1->getTextureCoords()[1],
                                        0.0f),
                                &bigtexture_primv[0]);
  bigtexture_clipper->addVertex(SbVec3f(v2->getTextureCoords()[0],
                                        v2->getTextureCoords()[1],
                                        0.0f),
                                &bigtexture_primv[1]);
  bigtexture_clipper->addVertex(SbVec3f(v3->getTextureCoords()[0],
                                        v3->getTextureCoords()[1],
                                        0.0f),
                                &bigtexture_primv[2]);
  bigtexture_clipper->clip(bigtexture_planes[0]);
  bigtexture_clipper->clip(bigtexture_planes[1]);
  bigtexture_clipper->clip(bigtexture_planes[2]);
  bigtexture_clipper->clip(bigtexture_planes[3]);
  
  assert(bigtexture_primvcount <= 256);
  
  const int numv = bigtexture_clipper->getNumVertices();
#ifdef BIGTEXTURE_DEBUG
  fprintf(stderr,"bigtexture numv: %d\n", numv);
#endif
  if (numv >= 3) {
    int i;
    SbBox3f bbox;
    for (i = 0; i < numv; i++) {
      SoPrimitiveVertex * v = (SoPrimitiveVertex*) bigtexture_clipper->getVertexData(i);
      bbox.extendBy(v->getPoint());
    }
    if (SoCullElement::cullTest(state, bbox)) return;
    
    if (!bigtexture_didsubapply) {
      bigtexture_didsubapply = TRUE;
      bigtexture_image->applySubImage(state,
                                      bigtexture_subidx,
                                      bigtexture_quality,
                                      SbVec2s(1,1));
    }
    glBegin(GL_TRIANGLE_FAN);
    
    for (i = 0; i < numv; i++) {
      SoPrimitiveVertex * v = (SoPrimitiveVertex*) bigtexture_clipper->getVertexData(i);
#ifdef BIGTEXTURE_DEBUG
      fprintf(stderr,"prividx: %p, %d\n", v, int(v-bigtexture_primv));
#endif
      SbVec4f tc = v->getTextureCoords();
      tc[0] -= bigtexture_start[0];
      tc[1] -= bigtexture_start[1];
      tc[0] /= (bigtexture_end[0]-bigtexture_start[0]);
      tc[1] /= (bigtexture_end[1]-bigtexture_start[1]);
      glTexCoord4fv(tc.getValue());
      glNormal3fv(v->getNormal().getValue());
      mb.send(v->getMaterialIndex(), TRUE);
      glVertex3fv(v->getPoint().getValue());
    }
    glEnd();
  }
}
