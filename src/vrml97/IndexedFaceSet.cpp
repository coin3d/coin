/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLIndexedFaceSet SoVRMLIndexedFaceSet.h Inventor/VRMLnodes/SoVRMLIndexedFaceSet.h
  \brief The SoVRMLIndexedFaceSet class is used for representing a generic 3D shape.
*/

/*!
  \var SoSFBool SoVRMLIndexedFaceSet::ccw
  Specifies if vertex ordering is counterclockwise. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLIndexedFaceSet::solid
  Can be used to enable backface culling. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLIndexedFaceSet::convex
  Specifies if all polygons are convex. Default value is TRUE.
*/

/*!
  \var SoSFFloat SoVRMLIndexedFaceSet::creaseAngle
  Specifies the crease angle for the generated normals. Default value is 0.0.
*/

#include <Inventor/VRMLnodes/SoVRMLIndexedFaceSet.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/caches/SoConvexDataCache.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/details/SoFaceDetail.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// for concavestatus
#define STATUS_UNKNOWN 0
#define STATUS_CONVEX  1
#define STATUS_CONCAVE 2

SO_NODE_SOURCE(SoVRMLIndexedFaceSet);

// Doc in parent
void
SoVRMLIndexedFaceSet::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLIndexedFaceSet, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLIndexedFaceSet::SoVRMLIndexedFaceSet(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLIndexedFaceSet);

  SO_VRMLNODE_ADD_FIELD(ccw, (TRUE));
  SO_VRMLNODE_ADD_FIELD(solid, (TRUE));
  SO_VRMLNODE_ADD_FIELD(convex, (TRUE));
  SO_VRMLNODE_ADD_FIELD(creaseAngle, (0.0f));

  this->convexCache = NULL;
  this->concavestatus = STATUS_UNKNOWN;
}

/*!
  Destructor.
*/
SoVRMLIndexedFaceSet::~SoVRMLIndexedFaceSet() // virtual, protected
{
  if (this->convexCache) this->convexCache->unref();
}

//
// translates current material binding into the internal Binding enum.
//
SoVRMLIndexedFaceSet::Binding
SoVRMLIndexedFaceSet::findMaterialBinding(void) const
{
  Binding binding = OVERALL;
  if (this->color.getValue()) {
    if (this->colorPerVertex.getValue()) {
      binding = PER_VERTEX_INDEXED;
      if (!this->colorIndex.getNum()) binding = PER_VERTEX;
    }
    else {
      binding = PER_FACE;
      if (this->colorIndex.getNum()) binding = PER_FACE_INDEXED;
    }
  }
  return binding;
}


//
// translates current normal binding into the internal Binding enum.
//
SoVRMLIndexedFaceSet::Binding
SoVRMLIndexedFaceSet::findNormalBinding(void) const
{
  Binding binding = OVERALL;
  if (this->normalPerVertex.getValue()) {
    binding = PER_VERTEX_INDEXED;
    if (this->normal.getValue() && !this->normalIndex.getNum()) binding = PER_VERTEX;
  }
  else {
    binding = PER_FACE;
    if (this->normalIndex.getNum()) binding = PER_FACE_INDEXED;
  }
  return binding;
}


// Doc in parent
void
SoVRMLIndexedFaceSet::GLRender(SoGLRenderAction * action)
{
  if (this->coordIndex.getNum() < 3 || this->coord.getValue() == NULL) return;
  SoState * state = action->getState();

  if (!this->shouldGLRender(action)) return;

  // update state with coordinates, normals and texture information
  SoVRMLVertexShape::GLRender(action);

  if (this->coordIndex.getNum() && this->coordIndex[this->coordIndex.getNum()-1] >= 0) {
    this->coordIndex.set1Value(coordIndex.getNum(), -1);
  }

  Binding mbind = this->findMaterialBinding();
  Binding nbind = this->findNormalBinding();

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool doTextures;
  SbBool normalCacheUsed;

  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  this->getVertexData(state, coords, normals, cindices,
                      nindices, tindices, mindices, numindices,
                      sendNormals, normalCacheUsed);

  if (!sendNormals) nbind = OVERALL;
  else if (nbind == OVERALL) {
    if (normals) glNormal3fv(normals[0].getValue());
    else glNormal3f(0.0f, 0.0f, 1.0f);
  }
  else if (normalCacheUsed && nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
  }
  else if (normalCacheUsed && nbind == PER_FACE_INDEXED) {
    nbind = PER_FACE;
  }

  if (mbind == PER_VERTEX) {
    mbind = PER_VERTEX_INDEXED;
    mindices = tindices;
  }
  if (nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
    tindices = cindices;
  }

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  Binding tbind = NONE;
  if (doTextures) {
    if (tb.isFunction()) {
      tbind = NONE;
      tindices = NULL;
    }
    else {
      tbind = PER_VERTEX_INDEXED;
      if (tindices == NULL) tindices = cindices;
    }
  }

  if (this->useConvexCache(action)) {
    cindices = this->convexCache->getCoordIndices();
    numindices = this->convexCache->getNumCoordIndices();
    mindices = this->convexCache->getMaterialIndices();
    nindices = this->convexCache->getNormalIndices();
    tindices = this->convexCache->getTexIndices();

    if (mbind == PER_VERTEX) mbind = PER_VERTEX_INDEXED;
    else if (mbind == PER_FACE) mbind = PER_FACE_INDEXED;
    if (nbind == PER_VERTEX) nbind = PER_VERTEX_INDEXED;
    else if (nbind == PER_FACE) nbind = PER_FACE_INDEXED;

    if (tbind != NONE) tbind = PER_VERTEX_INDEXED;
  }

  SoMaterialBundle mb(action);

  mb.sendFirst(); // make sure we have the correct material

  this->setupShapeHints(state, this->ccw.getValue(), this->solid.getValue());

  sogl_render_faceset((SoGLCoordinateElement *)coords,
                      cindices,
                      numindices,
                      normals,
                      nindices,
                      &mb,
                      mindices,
                      &tb,
                      tindices,
                      (int)nbind,
                      (int)mbind,
                      doTextures?1:0);
}

// Doc in parent
void
SoVRMLIndexedFaceSet::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  int n = this->coordIndex.getNum();
  if (n < 3) return;

  if (action->canApproximateCount()) {
    action->addNumTriangles(n/4);
  }
  else {
    const int32_t * ptr = this->coordIndex.getValues(0);
    const int32_t * endptr = ptr + n;
    int cnt = 0;
    int add = 0;
    while (ptr < endptr) {
      if (*ptr++ >= 0) cnt++;
      else {
        add += cnt-2;
        cnt = 0;
      }
    }
    // in case index array wasn't terminated with a -1
    if (cnt >= 3) add += cnt-2;
    action->addNumTriangles(add);
  }
}

// Doc in parent
SbBool
SoVRMLIndexedFaceSet::generateDefaultNormals(SoState * s,
                                             SoNormalBundle * nb)
{
  return FALSE;
}

  // this macro actually makes the code below more readable  :-)
#define DO_VERTEX(idx) \
  if (mbind == PER_VERTEX) {                  \
    pointDetail.setMaterialIndex(matnr);      \
    vertex.setMaterialIndex(matnr++);         \
  }                                           \
  else if (mbind == PER_VERTEX_INDEXED) {     \
    pointDetail.setMaterialIndex(*mindices); \
    vertex.setMaterialIndex(*mindices++); \
  }                                         \
  if (nbind == PER_VERTEX) {                \
    pointDetail.setNormalIndex(normnr);     \
    currnormal = &normals[normnr++];        \
    vertex.setNormal(*currnormal);          \
  }                                         \
  else if (nbind == PER_VERTEX_INDEXED) {   \
    pointDetail.setNormalIndex(*nindices);  \
    currnormal = &normals[*nindices++];     \
    vertex.setNormal(*currnormal);          \
  }                                         \
  if (tbind != NONE) {                      \
    pointDetail.setTextureCoordIndex(tindices ? *tindices : texidx); \
    vertex.setTextureCoords(tb.get(tindices ? *tindices++ : texidx++)); \
  }                                         \
  else if (tb.isFunction()) {               \
    vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal)); \
  }                                         \
  vertex.setPoint(coords->get3(idx));        \
  pointDetail.setCoordinateIndex(idx);      \
  this->shapeVertex(&vertex);

// Doc in parent
void
SoVRMLIndexedFaceSet::generatePrimitives(SoAction * action)
{
  if (this->coordIndex.getNum() < 3) return;

  SoState * state = action->getState();

  state->push();
  SoVRMLVertexShape::doAction(action);

  if (coordIndex.getNum() && coordIndex[coordIndex.getNum()-1] >= 0) {
    coordIndex.set1Value(coordIndex.getNum(), -1);
  }

  Binding mbind = this->findMaterialBinding();
  Binding nbind = this->findNormalBinding();

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool doTextures;
  SbBool sendNormals;
  SbBool normalCacheUsed;

  sendNormals = TRUE; // always generate normals

  this->getVertexData(state, coords, normals, cindices,
                      nindices, tindices, mindices, numindices,
                      sendNormals, normalCacheUsed);

  if (!sendNormals) nbind = OVERALL;
  else if (normalCacheUsed && nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
  }
  else if (normalCacheUsed && nbind == PER_FACE_INDEXED) {
    nbind = PER_FACE;
  }

  if (mbind == PER_VERTEX) {
    mbind = PER_VERTEX_INDEXED;
    mindices = cindices;
  }
  if (nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
    nindices = cindices;
  }

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding tbind = NONE;
  if (doTextures) {
    if (tb.isFunction()) {
      tbind = NONE;
      tindices = NULL;
    }
    else {
      tbind = PER_VERTEX_INDEXED;
      if (tindices == NULL) tindices = cindices;
    }
  }

  if (this->useConvexCache(action)) {
    cindices = this->convexCache->getCoordIndices();
    numindices = this->convexCache->getNumCoordIndices();
    mindices = this->convexCache->getMaterialIndices();
    nindices = this->convexCache->getNormalIndices();
    tindices = this->convexCache->getTexIndices();

    if (mbind == PER_VERTEX) mbind = PER_VERTEX_INDEXED;
    else if (mbind == PER_FACE) mbind = PER_FACE_INDEXED;
    if (nbind == PER_VERTEX) nbind = PER_VERTEX_INDEXED;
    else if (nbind == PER_FACE) nbind = PER_FACE_INDEXED;

    if (tbind != NONE) tbind = PER_VERTEX_INDEXED;
  }

  int texidx = 0;
  TriangleShape mode = POLYGON;
  TriangleShape newmode;
  const int32_t *viptr = cindices;
  const int32_t *viendptr = viptr + numindices;
  int32_t v1, v2, v3, v4, v5 = 0; // v5 init unnecessary, but kills a compiler warning.

  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  SoFaceDetail faceDetail;

  vertex.setDetail(&pointDetail);

  SbVec3f dummynormal(0,0,1);
  const SbVec3f *currnormal = &dummynormal;
  if (normals) currnormal = normals;
  vertex.setNormal(*currnormal);

  int matnr = 0;
  int normnr = 0;

  while (viptr < viendptr) {
    v1 = *viptr++;
    v2 = *viptr++;
    v3 = *viptr++;
    assert(v1 >= 0 && v2 >= 0 && v3 >= 0);
    v4 = *viptr++;
    if (v4  < 0) newmode = TRIANGLES;
    else {
      v5 = *viptr++;
      if (v5 < 0) newmode = QUADS;
      else newmode = POLYGON;
    }
    if (newmode != mode) {
      if (mode != POLYGON) this->endShape();
      mode = newmode;
      this->beginShape(action, mode, &faceDetail);
    }
    else if (mode == POLYGON) this->beginShape(action, POLYGON, &faceDetail);

    // vertex 1 can't use DO_VERTEX
    if (mbind == PER_VERTEX || mbind == PER_FACE) {
      pointDetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    else if (mbind == PER_VERTEX_INDEXED || mbind == PER_FACE_INDEXED) {
      pointDetail.setMaterialIndex(*mindices);
      vertex.setMaterialIndex(*mindices++);
    }
    if (nbind == PER_VERTEX || nbind == PER_FACE) {
      pointDetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    else if (nbind == PER_FACE_INDEXED || nbind == PER_VERTEX_INDEXED) {
      pointDetail.setNormalIndex(*nindices);
      currnormal = &normals[*nindices++];
      vertex.setNormal(*currnormal);
    }

    if (tbind != NONE) {
      pointDetail.setTextureCoordIndex(tindices ? *tindices : texidx);
      vertex.setTextureCoords(tb.get(tindices ? *tindices++ : texidx++));
    }
    else if (tb.isFunction()) {
      vertex.setTextureCoords(tb.get(coords->get3(v1), *currnormal));
    }
    pointDetail.setCoordinateIndex(v1);
    vertex.setPoint(coords->get3(v1));
    this->shapeVertex(&vertex);

    DO_VERTEX(v2);
    DO_VERTEX(v3);

    if (mode != TRIANGLES) {
      DO_VERTEX(v4);
      if (mode == POLYGON) {
        DO_VERTEX(v5);
        v1 = *viptr++;
        while (v1 >= 0) {
          DO_VERTEX(v1);
          v1 = *viptr++;
        }
        this->endShape();
      }
    }
    faceDetail.incFaceIndex();
    if (mbind == PER_VERTEX_INDEXED) {
      mindices++;
    }
    if (nbind == PER_VERTEX_INDEXED) {
      nindices++;
    }
    if (tindices) tindices++;
  }
  if (mode != POLYGON) this->endShape();
  state->pop();
}

#undef DO_VERTEX

// Doc in parent
SbBool
SoVRMLIndexedFaceSet::generateDefaultNormals(SoState * state,
                                         SoNormalCache * nc)
{
  SoVRMLCoordinate * node = (SoVRMLCoordinate*) this->coord.getValue();
  if (node == NULL) return TRUE; // ok, empty ifs

  const SbVec3f * coords = node->point.getValues(0);


  switch (this->findNormalBinding()) {
  case PER_VERTEX:
  case PER_VERTEX_INDEXED:
    nc->generatePerVertex(coords,
                          coordIndex.getValues(0),
                          coordIndex.getNum(),
                          this->creaseAngle.getValue(),
                          NULL,
                          this->ccw.getValue());
    break;
  case PER_FACE:
  case PER_FACE_INDEXED:
    nc->generatePerFace(coords,
                        coordIndex.getValues(0),
                        coordIndex.getNum(),
                        this->ccw.getValue());
    break;
  default:
    break;
  }
  return TRUE;
}

// Doc in parent
void
SoVRMLIndexedFaceSet::notify(SoNotList * list)
{
  if (this->convexCache) this->convexCache->invalidate();
  SoField *f = list->getLastField();
  if (f == &this->coordIndex) this->concavestatus = STATUS_UNKNOWN;
  inherited::notify(list);
}

//
// internal method which checks if convex cache needs to be
// used or (re)created. Returns TRUE if convex cache must be
// used. this->convexCache is then guaranteed to be != NULL.
//
SbBool
SoVRMLIndexedFaceSet::useConvexCache(SoAction * action)
{
  SoState * state = action->getState();
  if (this->convex.getValue())
    return FALSE;

  if (this->concavestatus == STATUS_UNKNOWN) {
    const int32_t * ptr = this->coordIndex.getValues(0);
    const int32_t * endptr = ptr + this->coordIndex.getNum();
    int cnt = 0;
    this->concavestatus = STATUS_CONVEX;
    while (ptr < endptr) {
      if (*ptr++ >= 0) cnt++;
      else {
        if (cnt > 3) { this->concavestatus = STATUS_CONCAVE; break; }
        cnt = 0;
      }
    }
  }
  if (this->concavestatus == STATUS_CONVEX) return FALSE;

  if (this->convexCache && this->convexCache->isValid(state))
    return TRUE;


  if (this->convexCache) this->convexCache->unref();
  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);

  // need to send matrix if we have some weird transformation
  SbMatrix modelmatrix = SoModelMatrixElement::get(state);
  if (modelmatrix[3][0] == 0.0f &&
      modelmatrix[3][1] == 0.0f &&
      modelmatrix[3][2] == 0.0f &&
      modelmatrix[3][3] == 1.0f) modelmatrix = SbMatrix::identity();

  // push to create cache dependencies
  state->push();
  this->convexCache = new SoConvexDataCache(state);
  this->convexCache->ref();
  SoCacheElement::set(state, this->convexCache);

  SoVRMLVertexShape::doAction(action);

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool normalCacheUsed;
  SbBool sendNormals = TRUE;

  this->getVertexData(state, coords, normals, cindices,
                      nindices, tindices, mindices, numindices,
                      sendNormals, normalCacheUsed);

  Binding mbind = this->findMaterialBinding();
  Binding nbind = this->findNormalBinding();

  if (normalCacheUsed && nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
  }

  if (mbind == PER_VERTEX) {
    mbind = PER_VERTEX_INDEXED;
    mindices = tindices;
  }
  if (nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
    tindices = cindices;
  }

  Binding tbind = PER_VERTEX_INDEXED;
  if (tindices == NULL) tindices = cindices;

  if (nbind == PER_VERTEX_INDEXED && nindices == NULL) {
    nindices = cindices;
  }
  if (mbind == PER_VERTEX_INDEXED && mindices == NULL) {
    mindices = cindices;
  }
  this->convexCache->generate(coords, modelmatrix,
                              cindices, numindices,
                              mindices, nindices, tindices,
                              (SoConvexDataCache::Binding)mbind,
                              (SoConvexDataCache::Binding)nbind,
                              (SoConvexDataCache::Binding)tbind);
  state->pop();
  SoCacheElement::setInvalid(storedinvalid);
  return TRUE;
}
