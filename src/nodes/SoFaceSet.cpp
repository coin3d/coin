/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

/*!
  \class SoFaceSet SoFaceSet.h Inventor/nodes/SoFaceSet.h
  \brief The SoFaceSet class is used to render and organize non-indexed polygonal face data.
  \ingroup nodes

  Faces are specified using the numVertices field. Coordinates,
  normals, materials and texture coordinates are fetched in order from
  the current state or from the vertexProperty node if set. For
  example, if numVertices is set to [3, 4, 5, 3], this node would
  specify a tringle from coordinates 0, 1 and 2, a quad from
  coordinates 3, 4, 5 and 6, a polygon from coordinates 7, 8, 9, 10
  and 11 and finally a triangle from coordinates 12, 13, 14.

  Binding PER_VERTEX, PER_FACE or OVERALL can be set for material,
  and normals. The default material binding is OVERALL. The default
  normal binding is PER_VERTEX.
*/

#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/actions/SoGLRenderAction.h>
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

/*!
  \var SoMFInt32 SoFaceSet::numVertices
  Used to specify faces. Each entry specifies the number of coordinates
  in a face. The coordinates are taken in order from the state or from
  the vertexProperty node.
*/

SO_NODE_SOURCE(SoFaceSet);

/*!
  Constructor.
*/
SoFaceSet::SoFaceSet()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFaceSet);

  SO_NODE_ADD_FIELD(numVertices, (-1));
}

/*!
  Destructor.
*/
SoFaceSet::~SoFaceSet()
{
}

// doc from parent
void
SoFaceSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoFaceSet);
}

// doc from parent
void
SoFaceSet::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  int32_t numvertices = 0;
  for (int i=0; i < this->numVertices.getNum(); i++)
    numvertices += this->numVertices[i];

  inherited::computeCoordBBox(action, numvertices, box, center);
}

//
// translates current material binding to the internal enum
//
SoFaceSet::Binding
SoFaceSet::findMaterialBinding(SoState * const state) const
{
  SoMaterialBindingElement::Binding matbind =
    SoMaterialBindingElement::get(state);

  Binding binding;
  switch (matbind) {
  case SoMaterialBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoMaterialBindingElement::PER_VERTEX:
  case SoMaterialBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX;
    break;
  case SoMaterialBindingElement::PER_PART:
  case SoMaterialBindingElement::PER_PART_INDEXED:
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = OVERALL;
#if COIN_DEBUG
    SoDebugError::postWarning("SoFaceSet::findMaterialBinding",
                              "unknown material binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}


//
// translates current normal binding to the internal enum
//
SoFaceSet::Binding
SoFaceSet::findNormalBinding(SoState * const state) const
{
  SoNormalBindingElement::Binding normbind =
    SoNormalBindingElement::get(state);

  Binding binding;
  switch (normbind) {
  case SoMaterialBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoMaterialBindingElement::PER_VERTEX:
  case SoMaterialBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX;
    break;
  case SoMaterialBindingElement::PER_PART:
  case SoMaterialBindingElement::PER_PART_INDEXED:
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = PER_VERTEX;
#if COIN_DEBUG
    SoDebugError::postWarning("SoFaceSet::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

typedef void sogl_render_ni_faceset_func( const SoGLCoordinateElement * coords,
    const SbVec3f *normals,
    SoMaterialBundle * mb,
    const SoTextureCoordinateBundle * tb,
    int nbind,
    int mbind,
    int doTextures,
    int32_t idx,
    const int32_t *ptr,
    const int32_t *end,
    SbBool needNormals);

static sogl_render_ni_faceset_func *ni_render_funcs[22];

#define OVERALL             0
#define PER_FACE            1
#define PER_VERTEX          2

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nifs_m0_n0_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nifs_m1_n0_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nifs_m2_n0_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nifs_m0_n1_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nifs_m1_n1_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nifs_m2_n1_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nifs_m0_n2_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nifs_m1_n2_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nifs_m2_n2_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nifs_m0_n0_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nifs_m1_n0_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nifs_m2_n0_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nifs_m0_n1_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nifs_m1_n1_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nifs_m2_n1_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nifs_m0_n2_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nifs_m1_n2_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nifs_m2_n2_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#undef OVERALL
#undef PER_FACE
#undef PER_VERTEX

// doc from parent
void
SoFaceSet::GLRender(SoGLRenderAction * action)
{
  // FIXME:
  // 1) check for concave polygons, and use SbTesselator to draw triangles.
  //    Or use SoConvexDataCache (much faster).

  static int first = 1;
  if (first) {
    first = 0;
    ni_render_funcs[ 0] = sogl_nifs_m0_n0_t0;
    ni_render_funcs[ 1] = sogl_nifs_m0_n0_t1;
    ni_render_funcs[ 2] = sogl_nifs_m0_n1_t0;
    ni_render_funcs[ 3] = sogl_nifs_m0_n1_t1;
    ni_render_funcs[ 4] = sogl_nifs_m0_n2_t0;
    ni_render_funcs[ 5] = sogl_nifs_m0_n2_t1;

    ni_render_funcs[ 8] = sogl_nifs_m1_n0_t0;
    ni_render_funcs[ 9] = sogl_nifs_m1_n0_t1;
    ni_render_funcs[10] = sogl_nifs_m1_n1_t0;
    ni_render_funcs[11] = sogl_nifs_m1_n1_t1;
    ni_render_funcs[12] = sogl_nifs_m1_n2_t0;
    ni_render_funcs[13] = sogl_nifs_m1_n2_t1;

    ni_render_funcs[16] = sogl_nifs_m2_n0_t0;
    ni_render_funcs[17] = sogl_nifs_m2_n0_t1;
    ni_render_funcs[18] = sogl_nifs_m2_n1_t0;
    ni_render_funcs[19] = sogl_nifs_m2_n1_t1;
    ni_render_funcs[20] = sogl_nifs_m2_n2_t0;
    ni_render_funcs[21] = sogl_nifs_m2_n2_t1;
  }

  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->GLRender(action);
  }

  if (!this->shouldGLRender(action)) {
    if (this->vertexProperty.getValue())
      state->pop();
    return;
  }

  const SoCoordinateElement * tmp;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  SoVertexShape::getVertexData(state, tmp, normals,
                               needNormals);

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
  doTextures = tb.needCoordinates();

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  if (!needNormals) nbind = OVERALL;

  if (needNormals && normals == NULL) {
    normals = this->getNormalCache()->getNormals();
  }

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t *ptr = this->numVertices.getValues(0);
  const int32_t *end = ptr + this->numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  ni_render_funcs[ (mbind << 3) | (nbind << 1) | doTextures ]
    ( coords,
      normals,
      &mb,
      &tb,
      nbind,
      mbind,
      doTextures,
      idx,
      ptr,
      end,
      needNormals);

#if 0 // obsoleted 2000-12-18, skei

  int matnr = 0;
  int texnr = 0;
  int mode = GL_POLYGON;
  int newmode;
  int n;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  if (nbind == OVERALL && needNormals)
    glNormal3fv((const GLfloat *)currnormal);

  while (ptr < end) {
    n = *ptr++;
    if (n == 3) newmode = GL_TRIANGLES;
    else if (n == 4) newmode = GL_QUADS;
    else newmode = GL_POLYGON;
    if (newmode != mode) {
      if (mode != GL_POLYGON) glEnd();
      mode = newmode;
      glBegin((GLenum) mode);
    }
    else if (mode == GL_POLYGON) glBegin(GL_POLYGON);
    if (nbind != OVERALL) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if (mbind != OVERALL) mb.send(matnr++, TRUE);
    if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
    coords->send(idx++);
    while (--n) {
      if (nbind == PER_VERTEX) {
        currnormal = normals++;
        glNormal3fv((const GLfloat *)currnormal);
      }
      if (mbind == PER_VERTEX) mb.send(matnr++, TRUE);
      if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
      coords->send(idx++);
    }
    if (mode == GL_POLYGON) glEnd();
  }
  if (mode != GL_POLYGON) glEnd();

#endif // obsoleted

  if (this->vertexProperty.getValue())
    state->pop();
}

// doc from parent
SbBool
SoFaceSet::generateDefaultNormals(SoState * state, SoNormalCache * nc)
{
  SbBool ccw = TRUE;
  if (SoShapeHintsElement::getVertexOrdering(state) ==
      SoShapeHintsElement::CLOCKWISE) ccw = FALSE;

  SoNormalGenerator * gen =
    new SoNormalGenerator(ccw, this->numVertices.getNum() * 3);

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t *ptr = this->numVertices.getValues(0);
  const int32_t *end = ptr + this->numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  const SoCoordinateElement * coords =
    SoCoordinateElement::getInstance(state);

  while (ptr < end) {
    int num = *ptr++;
    assert(num >= 3);
    gen->beginPolygon();
    while (num--) {
      gen->polygonVertex(coords->get3(idx++));
    }
    gen->endPolygon();
  }

  switch (this->findNormalBinding(state)) {
  case PER_VERTEX:
    gen->generate(SoCreaseAngleElement::get(state));
    break;
  case PER_FACE:
    gen->generatePerFace();
    break;
  case OVERALL:
    gen->generateOverall();
    break;
  }
  nc->set(gen);
  return TRUE;
}

/*!
  Overloaded to return FALSE. Normals are genereted directly in normal
  cache for this shape.
*/
SbBool
SoFaceSet::generateDefaultNormals(SoState * /* state */,
                                  SoNormalBundle * /* nb */)
{
  return FALSE;
}

// doc from parent
void
SoFaceSet::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  int32_t dummyarray[1];
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  this->fixNumVerticesPointers(action->getState(), ptr, end, dummyarray);

  if (action->canApproximateCount()) {
    action->addNumTriangles((end-ptr)*3);
  }
  else {
    int cnt = 0;
    while (ptr < end) {
      cnt += *ptr++ - 2;
    }
    action->addNumTriangles(cnt);
  }
}

// doc from parent
void
SoFaceSet::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  const SoCoordinateElement *coords;
  const SbVec3f * normals;
  SbBool doTextures;
  // FIXME: maybe SoLightModelElement should be enabled for
  // SoPickAction. This would make it possible to test the
  // element here, instead of always generating normals.
  // pederb, 20000927
  SbBool needNormals = TRUE;

  SoVertexShape::getVertexData(state, coords, normals,
                               needNormals);

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  if (needNormals && normals == NULL) {
    SoNormalCache * nc = this->getNormalCache();
    if (nc == NULL || !nc->isValid(state)) {
      this->generateNormals(state);
      nc = this->getNormalCache();
    }
    normals = nc->getNormals();
  }

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = this->numVertices.getValues(0);
  const int32_t * end = ptr + this->numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  int matnr = 0;
  int texnr = 0;
  int normnr = 0;
  TriangleShape mode = POLYGON;
  TriangleShape newmode;
  int n;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;

  SoPrimitiveVertex vertex;
  SoFaceDetail faceDetail;
  SoPointDetail pointDetail;

  vertex.setDetail(&pointDetail);

  while (ptr < end) {
    n = *ptr++;
    if (n == 3) newmode = TRIANGLES;
    else if (n == 4) newmode = QUADS;
    else newmode = POLYGON;
    if (newmode != mode) {
      if (mode != POLYGON) this->endShape();
      mode = newmode;
      this->beginShape(action, mode, &faceDetail);
    }
    else if (mode == POLYGON) this->beginShape(action, mode, &faceDetail);
    if (nbind != OVERALL) {
      pointDetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    if (mbind != OVERALL) {
      pointDetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    if (doTextures) {
      if (tb.isFunction())
        vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal));
      else {
        pointDetail.setTextureCoordIndex(texnr);
        vertex.setTextureCoords(tb.get(texnr++));
      }
    }
    pointDetail.setCoordinateIndex(idx);
    vertex.setPoint(coords->get3(idx++));
    this->shapeVertex(&vertex);

    while (--n) {
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(normnr);
        currnormal = &normals[normnr++];
        vertex.setNormal(*currnormal);
      }
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(matnr);
        vertex.setMaterialIndex(matnr++);
      }
      if (doTextures) {
        if (tb.isFunction())
          vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal));
        else {
          pointDetail.setTextureCoordIndex(texnr);
          vertex.setTextureCoords(tb.get(texnr++));
        }
      }
      pointDetail.setCoordinateIndex(idx);
      vertex.setPoint(coords->get3(idx++));
      this->shapeVertex(&vertex);
    }
    if (mode == POLYGON) this->endShape();
    faceDetail.incFaceIndex();
  }
  if (mode != POLYGON) this->endShape();
  if (this->vertexProperty.getValue())
    state->pop();
}
