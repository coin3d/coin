/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoLineSet SoLineSet.h Inventor/nodes/SoLineSet.h
  \brief The SoLineSet class is used to render and organize non-indexed polylines.
  \ingroup nodes

  Polylines are specified using the numVertices field. Coordinates,
  normals, materials and texture coordinates are fetched in order from
  the current state or from the vertexProperty node if set. For
  example, if numVertices is set to [3, 4, 2], this node would specify
  a line through coordinates 0, 1 and 2, a line through coordinates 3, 4, 5
  and 6, and finally a single line segment between coordinates 7 and 8.

  Binding PER_VERTEX, PER_FACE, PER_PART or OVERALL can be set for
  material, and normals. The default material binding is OVERALL. The
  default normal binding is PER_VERTEX. If no normals are set, the
  line set will be rendered with lighting disabled.

  The width of the rendered lines can be controlled through the
  insertion of an SoDrawStyle node in front of SoLineSet node(s) in
  the scenegraph.

  \sa SoIndexedLineSet
*/

#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/caches/SoBoundingBoxCache.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/misc/SoGL.h>

/*!
  \var SoMFInt32 SoLineSet::numVertices
  Used to specify polylines. Each entry specifies the number of coordinates
  in a line. The coordinates are taken in order from the state or from
  the vertexProperty node.
*/

// *************************************************************************

SO_NODE_SOURCE(SoLineSet);

/*!
  Constructor.
*/
SoLineSet::SoLineSet()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLineSet);

  SO_NODE_ADD_FIELD(numVertices, (-1));
}

/*!
  Destructor.
*/
SoLineSet::~SoLineSet()
{
}

// doc from parent
void
SoLineSet::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  int32_t numvertices = 0;
  for (int i=0; i < this->numVertices.getNum(); i++)
    numvertices += this->numVertices[i];
  inherited::computeCoordBBox(action, numvertices, box, center);
}

//
// translates the current material binding to the internal Binding enum
//
SoLineSet::Binding
SoLineSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;
  SoMaterialBindingElement::Binding matbind =
    SoMaterialBindingElement::get(state);

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
    binding = PER_SEGMENT;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_LINE;
    break;
  default:
    binding = OVERALL;
#if COIN_DEBUG
    SoDebugError::postWarning("SoLineSet::findMaterialBinding",
                              "unknown material binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}


//
// translates the current normal binding to the internal Binding enum
//
SoLineSet::Binding
SoLineSet::findNormalBinding(SoState * const state) const
{
  Binding binding = PER_VERTEX;

  SoNormalBindingElement::Binding normbind =
    SoNormalBindingElement::get(state);

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
    binding = PER_SEGMENT;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_LINE;
    break;
  default:
    binding = PER_VERTEX;
#if COIN_DEBUG
    SoDebugError::postWarning("SoLineSet::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

typedef void sogl_render_lineset_func( const SoGLCoordinateElement * coords,
    const SbVec3f *normals,
    SoMaterialBundle * mb,
    const SoTextureCoordinateBundle * tb,
    int nbind,
    int mbind,
    int doTextures,
    int32_t idx,
    const int32_t *ptr,
    const int32_t *end,
    SbBool needNormals,
    SbBool drawPoints);

static sogl_render_lineset_func *lineset_render_funcs[ 32 ];

#define OVERALL     0
#define PER_LINE    1
#define PER_SEGMENT 2
#define PER_VERTEX  3

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ls_m0_n0_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ls_m1_n0_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ls_m2_n0_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ls_m3_n0_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_LINE
#define TEXTURES FALSE
static void sogl_ls_m0_n1_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING PER_LINE
#define TEXTURES FALSE
static void sogl_ls_m1_n1_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING PER_LINE
#define TEXTURES FALSE
static void sogl_ls_m2_n1_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_LINE
#define TEXTURES FALSE
static void sogl_ls_m3_n1_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_SEGMENT
#define TEXTURES FALSE
static void sogl_ls_m0_n2_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING PER_SEGMENT
#define TEXTURES FALSE
static void sogl_ls_m1_n2_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING PER_SEGMENT
#define TEXTURES FALSE
static void sogl_ls_m2_n2_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_SEGMENT
#define TEXTURES FALSE
static void sogl_ls_m3_n2_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ls_m0_n3_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ls_m1_n3_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ls_m2_n3_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ls_m3_n3_t0
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_ls_m0_n0_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_ls_m1_n0_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_ls_m2_n0_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_ls_m3_n0_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_LINE
#define TEXTURES TRUE
static void sogl_ls_m0_n1_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING PER_LINE
#define TEXTURES TRUE
static void sogl_ls_m1_n1_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING PER_LINE
#define TEXTURES TRUE
static void sogl_ls_m2_n1_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_LINE
#define TEXTURES TRUE
static void sogl_ls_m3_n1_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_SEGMENT
#define TEXTURES TRUE
static void sogl_ls_m0_n2_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING PER_SEGMENT
#define TEXTURES TRUE
static void sogl_ls_m1_n2_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING PER_SEGMENT
#define TEXTURES TRUE
static void sogl_ls_m2_n2_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_SEGMENT
#define TEXTURES TRUE
static void sogl_ls_m3_n2_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_ls_m0_n3_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_LINE
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_ls_m1_n3_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_SEGMENT
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_ls_m2_n3_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_ls_m3_n3_t1
#include "../misc/SoGLLineSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES


#undef OVERALL
#undef PER_LINE
#undef PER_SEGMENT
#undef PER_VERTEX

// doc from parent
void
SoLineSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLineSet, SO_FROM_INVENTOR_1);

  lineset_render_funcs[ 0] = sogl_ls_m0_n0_t0;
  lineset_render_funcs[ 1] = sogl_ls_m0_n0_t1;
  lineset_render_funcs[ 2] = sogl_ls_m0_n1_t0;
  lineset_render_funcs[ 3] = sogl_ls_m0_n1_t1;
  lineset_render_funcs[ 4] = sogl_ls_m0_n2_t0;
  lineset_render_funcs[ 5] = sogl_ls_m0_n2_t1;
  lineset_render_funcs[ 6] = sogl_ls_m0_n3_t0;
  lineset_render_funcs[ 7] = sogl_ls_m0_n3_t1;
  
  lineset_render_funcs[ 8] = sogl_ls_m1_n0_t0;
  lineset_render_funcs[ 9] = sogl_ls_m1_n0_t1;
  lineset_render_funcs[10] = sogl_ls_m1_n1_t0;
  lineset_render_funcs[11] = sogl_ls_m1_n1_t1;
  lineset_render_funcs[12] = sogl_ls_m1_n2_t0;
  lineset_render_funcs[13] = sogl_ls_m1_n2_t1;
  lineset_render_funcs[14] = sogl_ls_m1_n3_t0;
  lineset_render_funcs[15] = sogl_ls_m1_n3_t1;

  lineset_render_funcs[16] = sogl_ls_m2_n0_t0;
  lineset_render_funcs[17] = sogl_ls_m2_n0_t1;
  lineset_render_funcs[18] = sogl_ls_m2_n1_t0;
  lineset_render_funcs[19] = sogl_ls_m2_n1_t1;
  lineset_render_funcs[20] = sogl_ls_m2_n2_t0;
  lineset_render_funcs[21] = sogl_ls_m2_n2_t1;
  lineset_render_funcs[22] = sogl_ls_m2_n3_t0;
  lineset_render_funcs[23] = sogl_ls_m2_n3_t1;

  lineset_render_funcs[24] = sogl_ls_m3_n0_t0;
  lineset_render_funcs[25] = sogl_ls_m3_n0_t1;
  lineset_render_funcs[26] = sogl_ls_m3_n1_t0;
  lineset_render_funcs[27] = sogl_ls_m3_n1_t1;
  lineset_render_funcs[28] = sogl_ls_m3_n2_t0;
  lineset_render_funcs[29] = sogl_ls_m3_n2_t1;
  lineset_render_funcs[30] = sogl_ls_m3_n3_t0;
  lineset_render_funcs[31] = sogl_ls_m3_n3_t1;
}

// doc from parent
void
SoLineSet::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  SbBool didpush = FALSE;
  if (this->vertexProperty.getValue()) {
    state->push();
    didpush = TRUE;
    this->vertexProperty.getValue()->GLRender(action);
  }

  const SoCoordinateElement * tmp;
  const SbVec3f * normals;
  SbBool needNormals = SoLazyElement::getLightModel(state) != SoLazyElement::BASE_COLOR;
  SoVertexShape::getVertexData(state, tmp, normals,
                               needNormals);
  if (normals == NULL && needNormals) {
    needNormals = FALSE;
    if (!didpush) {
      state->push();
      didpush = TRUE;
    }
    SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
  }

  if (!this->shouldGLRender(action)) {
    if (didpush)
      state->pop();
    return;
  }

  SbBool doTextures;
  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = findMaterialBinding(action->getState());


  Binding nbind;
  if (!needNormals) nbind = OVERALL;
  else nbind = findNormalBinding(action->getState());

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  SbBool drawPoints =
    SoDrawStyleElement::get(state) == SoDrawStyleElement::POINTS;

  lineset_render_funcs[ (mbind << 3) | (nbind << 1) | doTextures ]
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
      needNormals,
      drawPoints);

  if (didpush)
    state->pop();

  int numv = this->numVertices.getNum();
  // send approx number of lines for autocache handling
  sogl_autocache_update(state, numv ? 
                        (this->numVertices[0]-1)*numv : 0);
}

// Documented in superclass.
SbBool
SoLineSet::generateDefaultNormals(SoState * , SoNormalCache * nc)
{
  // not possible to generate normals for LineSet
  nc->set(0, NULL);
  return TRUE;
}

// Documented in superclass.
SbBool
SoLineSet::generateDefaultNormals(SoState *, SoNormalBundle *)
{
  return FALSE;
}

// doc from parent
void
SoLineSet::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  // notify open (if any) bbox caches about lines in this shape
  SoBoundingBoxCache::setHasLinesOrPoints(action->getState());
}

// doc from parent
void
SoLineSet::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  int32_t dummyarray[1];
  const int32_t *ptr = this->numVertices.getValues(0);
  const int32_t *end = ptr + this->numVertices.getNum();
  this->fixNumVerticesPointers(action->getState(), ptr, end, dummyarray);

  if (action->canApproximateCount()) {
    action->addNumLines(end-ptr);
  }
  else {
    int cnt = 0;
    while (ptr < end) {
      cnt += *ptr++ - 1;
    }
    action->addNumLines(cnt);
  }
}

// doc from parent
void
SoLineSet::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  const SoCoordinateElement *coords;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals = TRUE;

  SoVertexShape::getVertexData(action->getState(), coords, normals,
                               needNormals);

  if (normals == NULL) needNormals = FALSE;

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());

  if (!needNormals) nbind = OVERALL;

  SoPrimitiveVertex vertex;
  SoLineDetail lineDetail;
  SoPointDetail pointDetail;

  vertex.setDetail(&pointDetail);

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  if (nbind == OVERALL && needNormals) {
    vertex.setNormal(*currnormal);
  }

  int32_t idx = this->startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = this->numVertices.getValues(0);
  const int32_t * end = ptr + this->numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  int normnr = 0;
  int matnr = 0;
  int texnr = 0;

  if (nbind == PER_SEGMENT || mbind == PER_SEGMENT) {
    this->beginShape(action, SoShape::LINES, &lineDetail);

    while (ptr < end) {
      int n = *ptr++;
      if (n < 2) {
        idx += n;
        continue;
      }
      if (nbind == PER_LINE || nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(normnr);
        currnormal = &normals[normnr++];
        vertex.setNormal(*currnormal);
      }
      if (mbind == PER_LINE || mbind == PER_VERTEX) {
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
      while (--n) {
        if (nbind == PER_SEGMENT) {
          pointDetail.setNormalIndex(normnr);
          currnormal = &normals[normnr++];
          vertex.setNormal(*currnormal);
        }
        if (mbind == PER_SEGMENT) {
          pointDetail.setMaterialIndex(matnr);
          vertex.setMaterialIndex(matnr++);
        }
        pointDetail.setCoordinateIndex(idx);
        vertex.setPoint(coords->get3(idx++));
        this->shapeVertex(&vertex);
        
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
        vertex.setPoint(coords->get3(idx));
        this->shapeVertex(&vertex);
        lineDetail.incPartIndex();
      }
      lineDetail.incLineIndex();
      idx++; // next (poly)line should use the next index
    }
    this->endShape();
  }
  else {
    while (ptr < end) {
      lineDetail.setLineIndex(0);
      int n = *ptr++;
      if (n < 2) {
        idx += n;
        continue;
      }
      n -= 2;
      this->beginShape(action, SoShape::LINE_STRIP, &lineDetail);
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
      do {
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
        lineDetail.incPartIndex();
      } while (n--);
      this->endShape();
      lineDetail.incLineIndex();
    }
  }
  if (this->vertexProperty.getValue())
    state->pop();
}
