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

#include "soshape_trianglesort.h"
#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/SbPlane.h>
#include <Inventor/C/tidbits.h>
#include <stdlib.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/system/gl.h>


soshape_trianglesort::soshape_trianglesort(void)
{
  this->pvlist = NULL;
  this->trianglelist = NULL;
}

soshape_trianglesort::~soshape_trianglesort()
{
  delete this->pvlist;
  delete this->trianglelist;
}

void
soshape_trianglesort::beginShape(SoState *)
{
  if (this->pvlist == NULL) {
    this->pvlist = new SbList <SoPrimitiveVertex>;
    this->trianglelist = new SbList <sorted_triangle>;
  }
  pvlist->truncate(0);
}

void
soshape_trianglesort::triangle(SoState *,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3)
{
  assert(this->pvlist);
  this->pvlist->append(*v1);
  this->pvlist->append(*v2);
  this->pvlist->append(*v3);
}

// qsort() callback.
//
// "extern C" wrapper is needed with the OSF1/cxx compiler (probably a
// bug in the compiler, but it doesn't seem to hurt to do this
// anyway).
extern "C" {
static int
compare_triangles(const void * ptr1, const void * ptr2)
{
  soshape_trianglesort::sorted_triangle * tri1 = (soshape_trianglesort::sorted_triangle*) ptr1;
  soshape_trianglesort::sorted_triangle * tri2 = (soshape_trianglesort::sorted_triangle*) ptr2;

  if (tri1->dist > tri2->dist) return -1;
  if (tri1->dist == tri2->dist) return tri2->backface - tri1->backface;
  return 1;
}
}

void
soshape_trianglesort::endShape(SoState * state, SoMaterialBundle & mb)
{
  int i, n = this->pvlist->getLength() / 3;
  if (n == 0) return;

  const SoPrimitiveVertex * varray = this->pvlist->getArrayPtr();

  this->trianglelist->truncate(0);
  sorted_triangle tri;

  const SoPrimitiveVertex * v;
  const SbMatrix & mm = SoModelMatrixElement::get(state);

  SoShapeHintsElement::VertexOrdering vo;
  SoShapeHintsElement::ShapeType st;
  SoShapeHintsElement::FaceType ft;
  SoShapeHintsElement::get(state, vo, st, ft);

  SbBool bfcull =
    (vo != SoShapeHintsElement::UNKNOWN_ORDERING) &&
    (st == SoShapeHintsElement::SOLID);

  if (bfcull || vo == SoShapeHintsElement::UNKNOWN_ORDERING) {
    SbPlane nearp = SoViewVolumeElement::get(state).getPlane(0.0f);
    nearp = SbPlane(-nearp.getNormal(), -nearp.getDistanceFromOrigin());
    // if back face culling is enabled, we can do less work
    SbVec3f center;
    for (i = 0; i < n; i++) {
      int idx = i*3;
      center.setValue(0.0f, 0.0f, 0.0f);
      tri.idx = idx;
      for (int j = 0; j < 3; j++) {
        tri.backface = 0;
        v = varray + idx + j;
        center += v->getPoint();
      }
      center /= 3.0f;
      mm.multVecMatrix(center, center);
      tri.dist = nearp.getDistance(center);
      trianglelist->append(tri);
    }
  }
  else {
    // project each point onto screen to find the vertex
    // ordering of the triangle. Sort on vertex closest
    // to the near plane.
    SbMatrix obj2vp =
      mm * SoViewingMatrixElement::get(state) *
      SoProjectionMatrixElement::get(state);

    int clockwise = (vo == SoShapeHintsElement::CLOCKWISE) ? 1 : 0;
    SbVec3f c[3];
    for (i = 0; i < n; i++) {
      int idx = i*3;
      tri.idx = idx;
      // projected coordinates are between -1 and 1
      float smalldist = 10.0f;
      for (int j = 0; j < 3; j++) {
        v = varray + idx + j;
        c[j] = v->getPoint();
        obj2vp.multVecMatrix(c[j], c[j]);
        float dist = c[j][2];
        if (dist < smalldist) smalldist = dist;
      }
      SbVec3f v0 = c[2]-c[0];
      SbVec3f v1 = c[1]-c[0];
      // we need only the z-component of the cross product
      // to determine if triangle is cw or ccw
      float cz = v0[0]*v1[1] - v0[1]*v1[0];
      tri.backface = clockwise;
      if (cz < 0.0f) tri.backface = 1 - clockwise;
      tri.dist = smalldist;
      this->trianglelist->append(tri);
    }
  }

  const sorted_triangle * tarray = this->trianglelist->getArrayPtr();
  qsort((void*)tarray, n, sizeof(sorted_triangle), compare_triangles);

  int idx;

  // this rendering loop can be optimized a lot, of course, but speed
  // is not so important here, since it's slow to generate, copy and
  // sort the triangles anyway.
  glBegin(GL_TRIANGLES);
  for (i = 0; i < n; i++) {
    idx = tarray[i].idx;
    v = varray + idx;
    glTexCoord4fv(v->getTextureCoords().getValue());
    glNormal3fv(v->getNormal().getValue());
    mb.send(v->getMaterialIndex(), TRUE);
    glVertex3fv(v->getPoint().getValue());

    v = varray + idx+1;
    glTexCoord4fv(v->getTextureCoords().getValue());
    glNormal3fv(v->getNormal().getValue());
    mb.send(v->getMaterialIndex(), TRUE);
    glVertex3fv(v->getPoint().getValue());

    v = varray + idx+2;
    glTexCoord4fv(v->getTextureCoords().getValue());
    glNormal3fv(v->getNormal().getValue());
    mb.send(v->getMaterialIndex(), TRUE);
    glVertex3fv(v->getPoint().getValue());
  }
  glEnd();
}
