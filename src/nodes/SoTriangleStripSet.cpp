/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoTriangleStripSet SoTriangleStripSet.h Inventor/nodes/SoTriangleStripSet.h
  \brief The SoTriangleStripSet class is used to render and control non-indexed triangle strips.
  \ingroup nodes

  Triangle strips are specified using the numVertices
  field. Coordinates, normals, materials and texture coordinates are
  fetched in order from the current state or from the vertexProperty
  node if set. For example, if numVertices is set to [3, 4, 5, 3],
  this node would specify a triangle from coordinates 0, 1 and 2, a
  triangle strip from coordinates 3, 4, 5 and 6, a triangle strip from
  coordinates 7, 8, 9, 10 and 11 and finally a triangle from
  coordinates 12, 13, 14.

  Strips are converted into triangles the way OpenGL does it, of
  course.

  Binding PER_PART (per strip), PER_VERTEX, PER_FACE or OVERALL can be
  set for material, and normals. The default material binding is
  OVERALL. The default normal binding is PER_VERTEX.
*/

#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>

#include <Inventor/actions/SoGLRenderAction.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>

#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/misc/SoGL.h>

/*!
  \var SoMFInt32 SoTriangleStripSet::numVertices

  Specifies the number of vertices in each triangle strip. The
  vertices are fetched from the current coordinate node, or from the
  vertexProperty field if present.
*/

SO_NODE_SOURCE(SoTriangleStripSet);

/*!
  Constructor.
*/
SoTriangleStripSet::SoTriangleStripSet()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTriangleStripSet);

  SO_NODE_ADD_FIELD(numVertices, (-1));
}

/*!
  Destructor.
*/
SoTriangleStripSet::~SoTriangleStripSet()
{
}

// doc from parent
void
SoTriangleStripSet::computeBBox(SoAction * action,
                                SbBox3f & box, SbVec3f & center)
{
  int32_t numvertices = 0;
  for (int i=0; i < this->numVertices.getNum(); i++)
    numvertices += this->numVertices[i];

  inherited::computeCoordBBox(action, numvertices, box, center);
}

/*!
  \COININTERNAL
*/
SoTriangleStripSet::Binding
SoTriangleStripSet::findMaterialBinding(SoState * const state) const
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
    binding = PER_STRIP;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = OVERALL;
#if COIN_DEBUG
    SoDebugError::postWarning("SoTriangleStripSet::findMaterialBinding",
                              "unknown material binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

/*!
  \COININTERNAL
*/
SoTriangleStripSet::Binding
SoTriangleStripSet::findNormalBinding(SoState * const state) const
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
    binding = PER_STRIP;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = PER_VERTEX;
#if COIN_DEBUG
    SoDebugError::postWarning("SoTriangleStripSet::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

typedef void sogl_render_ni_tristripset_func( const SoGLCoordinateElement * coords,
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

static sogl_render_ni_tristripset_func *sotristripset_ni_render_funcs[ 32 ];

#define OVERALL       0
#define PER_STRIP     1
#define PER_FACE      2
#define PER_VERTEX    3

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nits_m0_n0_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nits_m1_n0_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nits_m2_n0_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nits_m3_n0_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_nits_m0_n1_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_nits_m1_n1_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_nits_m2_n1_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_nits_m3_n1_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nits_m0_n2_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nits_m1_n2_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nits_m2_n2_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nits_m3_n2_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nits_m0_n3_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nits_m1_n3_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nits_m2_n3_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nits_m3_n3_t0
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nits_m0_n0_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nits_m1_n0_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nits_m2_n0_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nits_m3_n0_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_STRIP
#define TEXTURES TRUE
static void sogl_nits_m0_n1_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_STRIP
#define TEXTURES TRUE
static void sogl_nits_m1_n1_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_STRIP
#define TEXTURES TRUE
static void sogl_nits_m2_n1_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_STRIP
#define TEXTURES TRUE
static void sogl_nits_m3_n1_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nits_m0_n2_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nits_m1_n2_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nits_m2_n2_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nits_m3_n2_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nits_m0_n3_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nits_m1_n3_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nits_m2_n3_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nits_m3_n3_t1
#include "../misc/SoGLnonindexedTristripSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#undef OVERALL
#undef PER_STRIP
#undef PER_FACE
#undef PER_VERTEX

// doc from parent
void
SoTriangleStripSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTriangleStripSet, SO_FROM_INVENTOR_1);

  sotristripset_ni_render_funcs[ 0] = sogl_nits_m0_n0_t0;
  sotristripset_ni_render_funcs[ 1] = sogl_nits_m0_n0_t1;
  sotristripset_ni_render_funcs[ 2] = sogl_nits_m0_n1_t0;
  sotristripset_ni_render_funcs[ 3] = sogl_nits_m0_n1_t1;
  sotristripset_ni_render_funcs[ 4] = sogl_nits_m0_n2_t0;
  sotristripset_ni_render_funcs[ 5] = sogl_nits_m0_n2_t1;
  sotristripset_ni_render_funcs[ 6] = sogl_nits_m0_n3_t0;
  sotristripset_ni_render_funcs[ 7] = sogl_nits_m0_n3_t1;

  sotristripset_ni_render_funcs[ 8] = sogl_nits_m1_n0_t0;
  sotristripset_ni_render_funcs[ 9] = sogl_nits_m1_n0_t1;
  sotristripset_ni_render_funcs[10] = sogl_nits_m1_n1_t0;
  sotristripset_ni_render_funcs[11] = sogl_nits_m1_n1_t1;
  sotristripset_ni_render_funcs[12] = sogl_nits_m1_n2_t0;
  sotristripset_ni_render_funcs[13] = sogl_nits_m1_n2_t1;
  sotristripset_ni_render_funcs[14] = sogl_nits_m1_n3_t0;
  sotristripset_ni_render_funcs[15] = sogl_nits_m1_n3_t1;
  
  sotristripset_ni_render_funcs[16] = sogl_nits_m2_n0_t0;
  sotristripset_ni_render_funcs[17] = sogl_nits_m2_n0_t1;
  sotristripset_ni_render_funcs[18] = sogl_nits_m2_n1_t0;
  sotristripset_ni_render_funcs[19] = sogl_nits_m2_n1_t1;
  sotristripset_ni_render_funcs[20] = sogl_nits_m2_n2_t0;
  sotristripset_ni_render_funcs[21] = sogl_nits_m2_n2_t1;
  sotristripset_ni_render_funcs[22] = sogl_nits_m2_n3_t0;
  sotristripset_ni_render_funcs[23] = sogl_nits_m2_n3_t1;
  
  sotristripset_ni_render_funcs[24] = sogl_nits_m3_n0_t0;
  sotristripset_ni_render_funcs[25] = sogl_nits_m3_n0_t1;
  sotristripset_ni_render_funcs[26] = sogl_nits_m3_n1_t0;
  sotristripset_ni_render_funcs[27] = sogl_nits_m3_n1_t1;
  sotristripset_ni_render_funcs[28] = sogl_nits_m3_n2_t0;
  sotristripset_ni_render_funcs[29] = sogl_nits_m3_n2_t1;
  sotristripset_ni_render_funcs[30] = sogl_nits_m3_n3_t0;
  sotristripset_ni_render_funcs[31] = sogl_nits_m3_n3_t1;
}

// doc from parent
void
SoTriangleStripSet::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  SbBool didpush = FALSE;

  if (this->vertexProperty.getValue()) {
    state->push();
    didpush = TRUE;
    this->vertexProperty.getValue()->GLRender(action);
  }

  Binding mbind = this->findMaterialBinding(action->getState());
  Binding nbind = this->findNormalBinding(action->getState());

  if ((nbind == PER_FACE) || (mbind == PER_FACE)) {
    if (!didpush) {
      didpush = TRUE;
      state->push();
    }
    SoLazyElement::setShadeModel(state, TRUE);
  }

  if (!this->shouldGLRender(action)) {
    if (didpush)
      state->pop();
    return;
  }

  const SoCoordinateElement * tmp;
  const SbVec3f * normals;
  SbBool doTextures;
  SoMaterialBundle mb(action);
  SbBool needNormals = !mb.isColorOnly();

  SoVertexShape::getVertexData(action->getState(), tmp, normals,
                               needNormals);

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  if (!needNormals) nbind = OVERALL;


  SoNormalCache * nc = NULL;
  if (needNormals && normals == NULL) {
    nc = this->generateAndReadLockNormalCache(state);
    normals = nc->getNormals();
  }

  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = this->startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = this->numVertices.getValues(0);
  const int32_t * end = ptr + this->numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);
  
  sotristripset_ni_render_funcs[ (mbind << 3) | (nbind << 1) | doTextures ]
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
  
  if (nc) {
    this->readUnlockNormalCache();
  }
  
  if (didpush)
    state->pop();

  int numv = this->numVertices.getNum();
  // send approx number of triangles for autocache handling
  sogl_autocache_update(state, numv ? 
                        (this->numVertices[0]-2)*numv : 0);
}

// doc from parent
SbBool
SoTriangleStripSet::generateDefaultNormals(SoState * state, SoNormalCache * nc)
{
  SbBool ccw = TRUE;
  if (SoShapeHintsElement::getVertexOrdering(state) ==
      SoShapeHintsElement::CLOCKWISE) ccw = FALSE;

  const SoCoordinateElement * coords =
    SoCoordinateElement::getInstance(state);
  assert(coords);

  SoNormalGenerator * gen =
    new SoNormalGenerator(ccw, this->numVertices.getNum()*3);

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * start = ptr;
  const int32_t * end = ptr + numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  while (ptr < end) {
    int num = *ptr++ - 3;
    assert(num >= 0);
    SbVec3f striptri[3];
    striptri[0] = coords->get3(idx++);
    striptri[1] = coords->get3(idx++);
    striptri[2] = coords->get3(idx++);
    gen->triangle(striptri[0], striptri[1], striptri[2]);
    SbBool flag = FALSE;
    while (num--) {
      if (flag) striptri[1] = striptri[2];
      else striptri[0] = striptri[2];
      flag = !flag;
      striptri[2] = coords->get3(idx++);
      gen->triangle(striptri[0], striptri[1], striptri[2]);
    }
  }

  switch (this->findNormalBinding(state)) {
  case OVERALL:
    gen->generateOverall();
    break;
  case PER_STRIP:
    gen->generatePerStrip(start, end-start);
    break;
  case PER_FACE:
    gen->generatePerFace();
    break;
  case PER_VERTEX:
    gen->generate(SoCreaseAngleElement::get(state), start, end-start);
    break;
  }
  nc->set(gen);
  return TRUE;
}

// doc from parent
void
SoTriangleStripSet::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  int32_t dummyarray[1];
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  this->fixNumVerticesPointers(action->getState(), ptr, end, dummyarray);

  if (action->canApproximateCount()) {
    // this is a wild guess, disable? pederb, 20000131
    action->addNumTriangles((end-ptr)*8);
  }
  else {
    int cnt = 0;
    while (ptr < end) {
      cnt += *ptr++ - 2;
    }
    action->addNumTriangles(cnt);
  }
}

// Documented in superclass.
SbBool
SoTriangleStripSet::generateDefaultNormals(SoState * /* state */,
                                           SoNormalBundle * /* nb */)
{
  // Normals are generated in normal cache.
  return FALSE;
}

// Documented in superclass.
void
SoTriangleStripSet::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals = TRUE;

  SoVertexShape::getVertexData(action->getState(), coords, normals,
                               needNormals);

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = this->findMaterialBinding(action->getState());
  Binding nbind = this->findNormalBinding(action->getState());

  SoNormalCache * nc = NULL;

  if (needNormals && normals == NULL) {
    nc = this->generateAndReadLockNormalCache(state);
    normals = nc->getNormals();
  }

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  this->fixNumVerticesPointers(action->getState(), ptr, end, dummyarray);

  int matnr = 0;
  int texnr = 0;
  int normnr = 0;
  int n;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;

  SoPrimitiveVertex vertex;
  SoFaceDetail faceDetail;
  SoPointDetail pointDetail;

  vertex.setNormal(*currnormal);
  vertex.setDetail(&pointDetail);

  while (ptr < end) {
    n = *ptr++ - 2;
    assert(n > 0);

    faceDetail.setFaceIndex(0);
    this->beginShape(action, TRIANGLE_STRIP, &faceDetail);

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

    while (n--) {
      if (nbind >= PER_FACE) {
        pointDetail.setNormalIndex(normnr);
        currnormal = &normals[normnr++];
        vertex.setNormal(*currnormal);
      }
      if (mbind >= PER_FACE) {
        pointDetail.setMaterialIndex(matnr);
        vertex.setMaterialIndex(matnr++);
      }
      if (doTextures) {
        if (tb.isFunction())
          vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal));
        else {
          pointDetail.setCoordinateIndex(texnr);
          vertex.setTextureCoords(tb.get(texnr++));
        }
      }
      pointDetail.setCoordinateIndex(idx);
      vertex.setPoint(coords->get3(idx++));
      this->shapeVertex(&vertex);
      faceDetail.incFaceIndex();
    }
    this->endShape();
    faceDetail.incPartIndex();
  }

  if (nc) {
    this->readUnlockNormalCache();
  }

  if (this->vertexProperty.getValue())
    state->pop();
}
