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
#include <Inventor/actions/SoGLRenderAction.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/caches/SoConvexDataCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbRWMutex.h>
#endif // COIN_THREADSAFE

/*!
  \var SoMFInt32 SoFaceSet::numVertices
  Used to specify faces. Each entry specifies the number of coordinates
  in a face. The coordinates are taken in order from the state or from
  the vertexProperty node.
*/

// for concavestatus
#define STATUS_UNKNOWN 0
#define STATUS_CONVEX  1
#define STATUS_CONCAVE 2

#ifndef DOXYGEN_SKIP_THIS
class SoFaceSetP {
public:
  SoFaceSetP(void) 
#ifdef COIN_THREADSAFE
    : convexmutex(SbRWMutex::READ_PRECEDENCE)
#endif // COIN_THREADSAFE 
  { }
  SoConvexDataCache * convexCache;
  int concavestatus;
#ifdef COIN_THREADSAFE
  SbRWMutex convexmutex;
#endif // COIN_THREADSAFE

  void readLockConvexCache(void) {
#ifdef COIN_THREADSAFE
    this->convexmutex.readLock();
#endif // COIN_THREADSAFE
  }
  void readUnlockConvexCache(void) {
#ifdef COIN_THREADSAFE
    this->convexmutex.readUnlock();
#endif // COIN_THREADSAFE
  }
  void writeLockConvexCache(void) {
#ifdef COIN_THREADSAFE
    this->convexmutex.writeLock();
#endif // COIN_THREADSAFE
  }
  void writeUnlockConvexCache(void) {
#ifdef COIN_THREADSAFE
    this->convexmutex.writeUnlock();
#endif // COIN_THREADSAFE
  }
};
#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

SO_NODE_SOURCE(SoFaceSet);

/*!
  Constructor.
*/
SoFaceSet::SoFaceSet()
{
  THIS = new SoFaceSetP;
  THIS->convexCache = NULL;
  THIS->concavestatus = STATUS_UNKNOWN;
  
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFaceSet);

  SO_NODE_ADD_FIELD(numVertices, (-1));
}

/*!
  Destructor.
*/
SoFaceSet::~SoFaceSet()
{
  if (THIS->convexCache) THIS->convexCache->unref();
  delete THIS;
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
  case SoNormalBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoNormalBindingElement::PER_VERTEX:
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX;
    break;
  case SoNormalBindingElement::PER_PART:
  case SoNormalBindingElement::PER_PART_INDEXED:
  case SoNormalBindingElement::PER_FACE:
  case SoNormalBindingElement::PER_FACE_INDEXED:
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

static sogl_render_ni_faceset_func *sofaceset_ni_render_funcs[22];

#define OVERALL             0
#define PER_FACE            1
#define PER_VERTEX          2

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nifs_m0_n0_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nifs_m1_n0_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_nifs_m2_n0_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nifs_m0_n1_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nifs_m1_n1_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_nifs_m2_n1_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nifs_m0_n2_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nifs_m1_n2_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_nifs_m2_n2_t0
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nifs_m0_n0_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nifs_m1_n0_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_nifs_m2_n0_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nifs_m0_n1_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nifs_m1_n1_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_nifs_m2_n1_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nifs_m0_n2_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nifs_m1_n2_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_nifs_m2_n2_t1
#include "../misc/SoGLnonindexedFaceSetTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#undef OVERALL
#undef PER_FACE
#undef PER_VERTEX


// doc from parent
void
SoFaceSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoFaceSet, SO_FROM_INVENTOR_1);

  sofaceset_ni_render_funcs[ 0] = sogl_nifs_m0_n0_t0;
  sofaceset_ni_render_funcs[ 1] = sogl_nifs_m0_n0_t1;
  sofaceset_ni_render_funcs[ 2] = sogl_nifs_m0_n1_t0;
  sofaceset_ni_render_funcs[ 3] = sogl_nifs_m0_n1_t1;
  sofaceset_ni_render_funcs[ 4] = sogl_nifs_m0_n2_t0;
  sofaceset_ni_render_funcs[ 5] = sogl_nifs_m0_n2_t1;
  
  sofaceset_ni_render_funcs[ 8] = sogl_nifs_m1_n0_t0;
  sofaceset_ni_render_funcs[ 9] = sogl_nifs_m1_n0_t1;
  sofaceset_ni_render_funcs[10] = sogl_nifs_m1_n1_t0;
  sofaceset_ni_render_funcs[11] = sogl_nifs_m1_n1_t1;
  sofaceset_ni_render_funcs[12] = sogl_nifs_m1_n2_t0;
  sofaceset_ni_render_funcs[13] = sogl_nifs_m1_n2_t1;
    
  sofaceset_ni_render_funcs[16] = sogl_nifs_m2_n0_t0;
  sofaceset_ni_render_funcs[17] = sogl_nifs_m2_n0_t1;
  sofaceset_ni_render_funcs[18] = sogl_nifs_m2_n1_t0;
  sofaceset_ni_render_funcs[19] = sogl_nifs_m2_n1_t1;
  sofaceset_ni_render_funcs[20] = sogl_nifs_m2_n2_t0;
  sofaceset_ni_render_funcs[21] = sogl_nifs_m2_n2_t1;
}


// doc from parent
void
SoFaceSet::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);
  state->push(); // for convex cache

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->GLRender(action);
  }

  if (!this->shouldGLRender(action)) {
    if (this->vertexProperty.getValue()) {
      state->pop();
    }
    // for convex cache
    (void) SoCacheElement::setInvalid(storedinvalid);
    state->pop();
    return;
  }

  if (!this->useConvexCache(action)) {
    // render normally
    const SoCoordinateElement * tmp;
    const SbVec3f * normals;
    SbBool doTextures;

    SoMaterialBundle mb(action);

    SbBool needNormals = !mb.isColorOnly();

    SoVertexShape::getVertexData(state, tmp, normals,
                                 needNormals);

    const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

    SoTextureCoordinateBundle tb(action, TRUE, FALSE);
    doTextures = tb.needCoordinates();

    Binding mbind = this->findMaterialBinding(state);
    Binding nbind = this->findNormalBinding(state);

    if (!needNormals) nbind = OVERALL;

    SoNormalCache * nc = NULL;

    if (needNormals && normals == NULL) {
      nc = this->generateAndReadLockNormalCache(state);
      normals = nc->getNormals();
    }

    mb.sendFirst(); // make sure we have the correct material

    int32_t idx = this->startIndex.getValue();
    int32_t dummyarray[1];
    const int32_t *ptr = this->numVertices.getValues(0);
    const int32_t *end = ptr + this->numVertices.getNum();
    this->fixNumVerticesPointers(state, ptr, end, dummyarray);

    sofaceset_ni_render_funcs[ (mbind << 3) | (nbind << 1) | doTextures ]
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
  }

  if (this->vertexProperty.getValue())
    state->pop();

  // needed for convex cache
  (void) SoCacheElement::setInvalid(storedinvalid);
  state->pop();
  
  int numv = this->numVertices.getNum();
  // send approx number of triangles for autocache handling
  sogl_autocache_update(state, numv ? 
                        (this->numVertices[0]-2)*numv : 0);
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

// Documented in superclass.
SbBool
SoFaceSet::generateDefaultNormals(SoState * /* state */,
                                  SoNormalBundle * /* nb */)
{
  // Normals are genereted directly in normal cache for this shape.
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

  SbBool needNormals = TRUE;

  SoVertexShape::getVertexData(state, coords, normals,
                               needNormals);

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  SoNormalCache * nc = NULL;

  if (needNormals && normals == NULL) {
    nc = this->generateAndReadLockNormalCache(state);
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

  if (nc) {
    this->readUnlockNormalCache();
  }

  if (this->vertexProperty.getValue())
    state->pop();
}

// Documented in superclass.
void
SoFaceSet::notify(SoNotList * l)
{
  // Overridden to invalidate convex cache.
  if (THIS->convexCache) THIS->convexCache->invalidate();
  SoField *f = l->getLastField();
  if (f == &this->numVertices) THIS->concavestatus = STATUS_UNKNOWN;
  SoNode::notify(l);
}

//
// internal method which checks if convex cache needs to be
// used or (re)created. Renders the shape if convex cache needs to be used.
//
SbBool
SoFaceSet::useConvexCache(SoAction * action)
{
  SoState * state = action->getState();
  if (SoShapeHintsElement::getFaceType(state) == SoShapeHintsElement::CONVEX)
    return FALSE;

  int32_t idx = this->startIndex.getValue();
  const int32_t * ptr = this->numVertices.getValues(0);;
  const int32_t * end = ptr + this->numVertices.getNum();
  int32_t dummyarray[1];
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  if (THIS->concavestatus == STATUS_UNKNOWN) {
    const int32_t * tst = ptr;
    while (tst < end) {
      if (*tst > 3) break;
      tst++;
    }
    if (tst < end) THIS->concavestatus = STATUS_CONCAVE;
    else THIS->concavestatus = STATUS_CONVEX;
  }
  if (THIS->concavestatus == STATUS_CONVEX) {
    return FALSE;
  }

  THIS->readLockConvexCache();
  
  SbBool isvalid = THIS->convexCache && THIS->convexCache->isValid(state);

  SbMatrix modelmatrix;
  if (!isvalid) {
    THIS->readUnlockConvexCache();
    THIS->writeLockConvexCache();
    if (THIS->convexCache) THIS->convexCache->unref();

    // use nopush to avoid cache dependencies.
    SoModelMatrixElement * nopushelem = (SoModelMatrixElement*)
      state->getElementNoPush(SoModelMatrixElement::getClassStackIndex());

    // need to send matrix if we have some weird transformation
    modelmatrix = nopushelem->getModelMatrix();
    if (modelmatrix[3][0] == 0.0f &&
        modelmatrix[3][1] == 0.0f &&
        modelmatrix[3][2] == 0.0f &&
        modelmatrix[3][3] == 1.0f) modelmatrix = SbMatrix::identity();

    THIS->convexCache = new SoConvexDataCache(state);
    THIS->convexCache->ref();
    SoCacheElement::set(state, THIS->convexCache);
  }

  const SoCoordinateElement * tmp;
  const SbVec3f * normals;
  SbBool doTextures;

  SoMaterialBundle mb(action);

  SbBool needNormals = !mb.isColorOnly();

  SoVertexShape::getVertexData(state, tmp, normals,
                               needNormals);

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  SoConvexDataCache::Binding mbind;

  switch (this->findMaterialBinding(state)) {
  case OVERALL:
    mbind = SoConvexDataCache::NONE;
    break;
  case PER_VERTEX:
    mbind = SoConvexDataCache::PER_VERTEX;
  case PER_FACE:
    mbind = SoConvexDataCache::PER_FACE;
    break;
  default:
    mbind = SoConvexDataCache::NONE;
    break;
  }

  SoConvexDataCache::Binding nbind;
  switch (this->findNormalBinding(state)) {
  case OVERALL:
    nbind = SoConvexDataCache::NONE;
    break;
  case PER_VERTEX:
    nbind = SoConvexDataCache::PER_VERTEX;
  case PER_FACE:
    nbind = SoConvexDataCache::PER_FACE;
    break;
  default:
    nbind = SoConvexDataCache::NONE;
    break;
  }

  SoNormalCache * nc = NULL;

  if (needNormals && normals == NULL) {
    nc = this->generateAndReadLockNormalCache(state);
    normals = nc->getNormals();
  }
  else if (!needNormals) {
    nbind = SoConvexDataCache::NONE;
  }
  if (nbind == SoConvexDataCache::NONE && normals == NULL) {
    static SbVec3f dummynormal;
    dummynormal.setValue(0.0f, 0.0f, 1.0f);
    normals = &dummynormal;
  }

  SoConvexDataCache::Binding tbind = SoConvexDataCache::NONE;
  if (tb.needCoordinates()) tbind = SoConvexDataCache::PER_VERTEX;

  if (!isvalid) {
    SoCacheElement::set(state, NULL); // close cache
    // create an index table to be able to use convex cache.
    // should be fast compared to the tessellation
    SbList <int32_t> dummyidx((end-ptr)*4);
    const int32_t * tptr = ptr;
    while (tptr < end) {
      int num = *tptr++;
      while (num--) {
        dummyidx.append(idx++);
      }
      dummyidx.append(-1);
    }
    THIS->convexCache->generate(coords, modelmatrix,
                                dummyidx.getArrayPtr(), dummyidx.getLength(),
                                NULL, NULL, NULL,
                                mbind,
                                nbind,
                                tbind);

    THIS->writeUnlockConvexCache();
    THIS->readLockConvexCache();
  }

  mb.sendFirst(); // make sure we have the correct material

  // the convex data cache will change PER_VERTEX binding
  // to PER_VERTEX_INDEXED. We must do so also.
  int realmbind = (int) mbind;
  int realnbind = (int) nbind;

  // hack warning. We rely on PER_VERTEX_INDEXED == PER_VERTEX+1
  // and PER_FACE_INDEXED == PER_FACE+1 in SoGL.cpp
  if (mbind == SoConvexDataCache::PER_VERTEX ||
      mbind == SoConvexDataCache::PER_FACE) realmbind++;
  if (nbind == SoConvexDataCache::PER_VERTEX ||
      nbind == SoConvexDataCache::PER_FACE) realnbind++;

  // use the IndededFaceSet rendering method.
  sogl_render_faceset(coords,
                      THIS->convexCache->getCoordIndices(),
                      THIS->convexCache->getNumCoordIndices(),
                      normals,
                      THIS->convexCache->getNormalIndices(),
                      &mb,
                      THIS->convexCache->getMaterialIndices(),
                      &tb,
                      THIS->convexCache->getTexIndices(),
                      realnbind,
                      realmbind,
                      doTextures?1:0);

  if (nc) {
    this->readUnlockNormalCache();
  }

  THIS->readUnlockConvexCache();

  return TRUE;
}

#undef THIS
#undef STATUS_UNKNOWN
#undef STATUS_CONVEX
#undef STATUS_CONCAVE
