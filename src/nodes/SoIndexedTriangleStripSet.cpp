/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoIndexedTriangleStripSet SoIndexedTriangleStripSet.h Inventor/nodes/SoIndexedTriangleStripSet.h
  \brief The SoIndexedTriangleStripSet class keeps data for rendering and otherwise representing triangle strips.
  \ingroup nodes

  Use nodes of this type as an effective way of drawing triangles
  which are strung together.

  For more information, see the documentation of the \c
  GL_TRIANGLE_STRIP primitive rendering type in OpenGL.
*/

#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <coindefs.h> // COIN_STUB()
#include <assert.h>
#ifdef _WIN32 // FIXME: use configure detection. 20000519 mortene.
#include <windows.h> // Needed for the GL/gl.h include.
#endif // !_WIN32
#include <GL/gl.h>


/*!
  \enum SoIndexedTriangleStripSet::Binding

  Enumeration of possible bindings to use for materials and normals
  for triangle strip nodes.
*/

// *************************************************************************

SO_NODE_SOURCE(SoIndexedTriangleStripSet);

/*!
  Constructor. Default field values will equal those set in the
  SoIndexedShape parent class.
*/
SoIndexedTriangleStripSet::SoIndexedTriangleStripSet(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoIndexedTriangleStripSet);
}

/*!
  Destructor.
*/
SoIndexedTriangleStripSet::~SoIndexedTriangleStripSet()
{
}

// doc from superclass
void
SoIndexedTriangleStripSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoIndexedTriangleStripSet);
}

// private
SoIndexedTriangleStripSet::Binding
SoIndexedTriangleStripSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;

  SoMaterialBindingElement::Binding matbind =
    SoMaterialBindingElement::get(state);

  switch (matbind) {
  case SoMaterialBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoMaterialBindingElement::PER_VERTEX:
    binding = PER_VERTEX;
    break;
  case SoMaterialBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX_INDEXED;
    break;
  case SoMaterialBindingElement::PER_PART:
    binding = PER_STRIP;
    break;
  case SoMaterialBindingElement::PER_FACE:
    binding = PER_TRIANGLE;
    break;
  case SoMaterialBindingElement::PER_PART_INDEXED:
    binding = PER_STRIP_INDEXED;
    break;
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_TRIANGLE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedTriangleStripSet::findMaterialBinding",
                              "unknown material binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

// private
SoIndexedTriangleStripSet::Binding
SoIndexedTriangleStripSet::findNormalBinding(SoState * const state) const
{
  Binding binding = PER_VERTEX_INDEXED;
  SoNormalBindingElement::Binding normbind =
    SoNormalBindingElement::get(state);

  switch (normbind) {
  case SoNormalBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoNormalBindingElement::PER_VERTEX:
    binding = PER_VERTEX;
    break;
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX_INDEXED;
    break;
  case SoNormalBindingElement::PER_PART:
    binding = PER_STRIP;
    break;
  case SoNormalBindingElement::PER_FACE:
    binding = PER_TRIANGLE;
    break;
  case SoNormalBindingElement::PER_PART_INDEXED:
    binding = PER_STRIP_INDEXED;
    break;
  case SoNormalBindingElement::PER_FACE_INDEXED:
    binding = PER_TRIANGLE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedTriangleStripSet::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

// doc from superclass
void
SoIndexedTriangleStripSet::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->GLRender(action);
  }


  if (!this->shouldGLRender(action)) {
    if (this->vertexProperty.getValue()) {
      state->pop();
    }
    return;
  }

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool dotextures;
  SbBool normalcacheused;

  SbBool sendnormals =
    (SoLightModelElement::get(state) != SoLightModelElement::BASE_COLOR);

  this->getVertexData(state, coords, normals, cindices,
                      nindices, tindices, mindices, numindices,
                      sendnormals, normalcacheused);

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  dotextures = tb.needCoordinates();

  Binding tbind = PER_VERTEX_INDEXED; // most common
  if (dotextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
        SoTextureCoordinateBindingElement::PER_VERTEX) {
      tbind = PER_VERTEX;
      tindices = NULL;
    }
    else if (tindices == NULL) {
      tindices = cindices;
    }
  }

  if (!sendnormals) nbind = OVERALL;
  else if (nbind == OVERALL) {
    if (normals) glNormal3fv(normals[0].getValue());
    else glNormal3f(0.0f, 0.0f, 1.0f);
  }
  else if (normalcacheused && nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
  }

  SoMaterialBundle mb(action);

  mb.sendFirst(); // make sure we have the correct material

  sogl_render_tristrip((SoGLCoordinateElement *)coords,
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
                       dotextures?1:0);

  if (this->vertexProperty.getValue()) {
    state->pop();
  }
}

// doc in SoVertexShape
SbBool
SoIndexedTriangleStripSet::generateDefaultNormals(SoState * state,
                                                  SoNormalCache * nc)
{
  SbBool ccw = TRUE;
  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);

  if (SoShapeHintsElement::getVertexOrdering(state) == SoShapeHintsElement::CLOCKWISE)
    ccw = FALSE;

  SoVertexProperty * vp = (SoVertexProperty *) this->vertexProperty.getValue();
  assert(!vp ||
         vp->getTypeId().isDerivedFrom(SoVertexProperty::getClassTypeId()));
  SbBool vpvtx = vp && (vp->vertex.getNum() > 0);
  SbBool vpnorm = vp && (vp->normal.getNum() > 0);

  const SbVec3f * coords = vpvtx ?
    vp->vertex.getValues(0) :
    coordelem->getArrayPtr3();
  assert(coords);

  SoNormalBindingElement::Binding normbind = vpnorm ?
    (SoNormalBindingElement::Binding) vp->normalBinding.getValue() :
    SoNormalBindingElement::get(state);


  switch (normbind) {
  case SoNormalBindingElement::PER_VERTEX:
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    nc->generatePerVertex(coords,
                          coordIndex.getValues(0),
                          coordIndex.getNum(),
                          SoCreaseAngleElement::get(state),
                          NULL,
                          ccw,
                          TRUE);
    break;
  case SoNormalBindingElement::PER_FACE:
  case SoNormalBindingElement::PER_FACE_INDEXED:
    nc->generatePerFaceStrip(coords,
                             coordIndex.getValues(0),
                             coordIndex.getNum(),
                             ccw);
    break;

  case SoNormalBindingElement::PER_PART:
  case SoNormalBindingElement::PER_PART_INDEXED:
    nc->generatePerStrip(coords,
                         coordIndex.getValues(0),
                         coordIndex.getNum(),
                         ccw);
    break;
  default:
    break;
  }
  return TRUE;
}

// private
void
SoIndexedTriangleStripSet::countPrimitives(int & strips, int & tris)
{
  if (this->coordIndex.getNum() < 3) {
    strips = 0;
    tris = 0;
    return;
  }

  const int32_t * ptr = this->coordIndex.getValues(0);
  const int32_t * endptr = ptr + this->coordIndex.getNum();

  int tmpcnt = 0;
  while (ptr < endptr) {
    if (*ptr++ >= 0) tmpcnt++;
    else {
      strips++;
      tris += tmpcnt - 2;
      tmpcnt = 0;
    }
  }
}

// doc from superclass
void
SoIndexedTriangleStripSet::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  // Note: default coordIndex setting is [ 0 ] so this check is
  // absolutely necessary.
  int n = this->coordIndex.getNum();
  if (n < 3) return;

  if (action->canApproximateCount()) {
    action->addNumTriangles(n - 2); // assumes one strip
  }
  else {
    int tris, strips;
    this->countPrimitives(strips, tris);
    action->addNumTriangles(tris);
  }
}

/*!
  FIXME: write doc
*/
SbBool
SoIndexedTriangleStripSet::generateDefaultNormals(SoState * state,
                                                  SoNormalBundle * nb)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
*/
void
SoIndexedTriangleStripSet::generatePrimitives(SoAction * action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool dotextures;
  SbBool sendnormals = TRUE;
  SbBool normalcacheused;

  this->getVertexData(state, coords, normals, cindices,
                      nindices, tindices, mindices, numindices,
                      sendnormals, normalcacheused);

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  dotextures = tb.needCoordinates();

  Binding tbind = PER_VERTEX_INDEXED; // most common
  if (dotextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
        SoTextureCoordinateBindingElement::PER_VERTEX) {
      tbind = PER_VERTEX;
      tindices = NULL;
    }
    else if (tindices == NULL) {
      tindices = cindices;
    }
  }

  if (nbind == PER_VERTEX_INDEXED && nindices == NULL) {
    nindices = cindices;
  }
  if (mbind == PER_VERTEX_INDEXED && mindices == NULL) {
    mindices = cindices;
  }

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (!sendnormals) nbind = OVERALL;
  else if (nbind == OVERALL) {
    if (normals) currnormal = normals;
  }
  else if (normalcacheused && nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
  }

  int texidx = 0;
  int matnr = 0;
  int normnr = 0;

  const int32_t * viptr = cindices;
  const int32_t * viendptr = viptr + numindices;
  int32_t v1, v2, v3;

  SoPrimitiveVertex vertex;
  SoPointDetail pointdetail;
  SoFaceDetail facedetail;

  vertex.setNormal(*currnormal);
  vertex.setDetail(&pointdetail);

  while (viptr < viendptr) {
    facedetail.setFaceIndex(0);

    v1 = *viptr++;
    v2 = *viptr++;
    v3 = *viptr++;
    assert(v1 >= 0 && v2 >= 0 && v3 >= 0);

    this->beginShape(action, TRIANGLE_STRIP, &facedetail);

    // vertex 1
    // FIXME: optimize bindings test below
    if (mbind == PER_VERTEX || mbind == PER_TRIANGLE || mbind == PER_STRIP) {
      pointdetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    else if (mbind == PER_VERTEX_INDEXED || mbind == PER_STRIP_INDEXED ||
             mbind == PER_TRIANGLE_INDEXED) {
      pointdetail.setMaterialIndex(*mindices);
      vertex.setMaterialIndex(*mindices++);
    }
    if (nbind == PER_VERTEX || nbind == PER_TRIANGLE || nbind == PER_STRIP) {
      pointdetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    else if (nbind == PER_VERTEX || nbind == PER_TRIANGLE || nbind == PER_STRIP) {
      pointdetail.setNormalIndex(*nindices++);
      currnormal = &normals[*nindices++];
      vertex.setNormal(*currnormal);
    }
    if (dotextures) {
      if (tb.isFunction()) {
        vertex.setTextureCoords(tb.get(coords->get3(v1), *currnormal));
      }
      else {
        pointdetail.setTextureCoordIndex(tindices ? *tindices : texidx);
        vertex.setTextureCoords(tb.get(tindices ? *tindices++ : texidx++));
      }
    }
    pointdetail.setCoordinateIndex(v1);
    vertex.setPoint(coords->get3(v1));
    this->shapeVertex(&vertex);

    /* vertex 2 *********************************************************/
    if (mbind == PER_VERTEX) {
      pointdetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    else if (mbind == PER_VERTEX_INDEXED) {
      pointdetail.setMaterialIndex(*mindices);
      vertex.setMaterialIndex(*mindices++);
    }
    if (nbind == PER_VERTEX) {
      pointdetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    else if (nbind == PER_VERTEX_INDEXED) {
      pointdetail.setNormalIndex(*nindices);
      currnormal = &normals[*nindices++];
      vertex.setNormal(*currnormal);
    }

    if (dotextures) {
      if (tb.isFunction()) {
        vertex.setTextureCoords(tb.get(coords->get3(v2), *currnormal));
      }
      else {
        pointdetail.setTextureCoordIndex(tindices?*tindices:texidx);
        vertex.setTextureCoords(tb.get(tindices ? *tindices++ : texidx++));
      }
    }
    pointdetail.setCoordinateIndex(v2);
    vertex.setPoint(coords->get3(v2));
    this->shapeVertex(&vertex);

    // vertex 3
    if (mbind == PER_VERTEX) {
      pointdetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    else if (mbind == PER_VERTEX_INDEXED) {
      pointdetail.setMaterialIndex(*mindices);
      vertex.setMaterialIndex(*mindices++);
    }
    if (nbind == PER_VERTEX) {
      pointdetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    else if (nbind == PER_VERTEX_INDEXED) {
      pointdetail.setNormalIndex(*nindices);
      currnormal = &normals[*nindices++];
      vertex.setNormal(*currnormal);
    }

    if (dotextures) {
      if (tb.isFunction()) {
        vertex.setTextureCoords(tb.get(coords->get3(v3), *currnormal));
      }
      else {
        pointdetail.setTextureCoordIndex(tindices?*tindices:texidx);
        vertex.setTextureCoords(tb.get(tindices ? *tindices++ : texidx++));
      }
    }
    pointdetail.setCoordinateIndex(v3);
    vertex.setPoint(coords->get3(v3));
    this->shapeVertex(&vertex);
    facedetail.incFaceIndex();

    v1 = *viptr++;
    while (v1 >= 0) {
      if (mbind == PER_VERTEX || mbind == PER_TRIANGLE) {
        pointdetail.setMaterialIndex(matnr);
        vertex.setMaterialIndex(matnr++);
      }
      else if (mbind == PER_VERTEX_INDEXED || mbind == PER_TRIANGLE_INDEXED) {
        pointdetail.setMaterialIndex(*mindices);
        vertex.setMaterialIndex(*mindices++);
      }
      if (nbind == PER_VERTEX || nbind == PER_TRIANGLE) {
        pointdetail.setNormalIndex(normnr);
        currnormal = &normals[normnr++];
        vertex.setNormal(*currnormal);
      }
      else if (nbind == PER_VERTEX_INDEXED || nbind == PER_TRIANGLE_INDEXED) {
        pointdetail.setNormalIndex(*nindices);
        currnormal = &normals[*nindices++];
        vertex.setNormal(*currnormal);
      }

      if (dotextures) {
        if (tb.isFunction()) {
          vertex.setTextureCoords(tb.get(coords->get3(v1), *currnormal));
        }
        else {
          pointdetail.setTextureCoordIndex(tindices?*tindices:texidx);
          vertex.setTextureCoords(tb.get(tindices ? *tindices++ : texidx++));
        }
      }
      pointdetail.setCoordinateIndex(v1);
      vertex.setPoint(coords->get3(v1));
      this->shapeVertex(&vertex);
      facedetail.incFaceIndex();

      v1 = *viptr++;
    }
    this->endShape();
    facedetail.incPartIndex();

    //
    // PER_TRIANGLE binding for strips is really stupid, so I'm not quite
    // sure what to do here. I guess nobody uses PER_TRIANGLE anyway.
    //

    if (mbind == PER_VERTEX_INDEXED || mbind == PER_TRIANGLE_INDEXED)
      mindices++;
    if (nbind == PER_VERTEX_INDEXED || nbind == PER_TRIANGLE_INDEXED)
      nindices++;
    if (tindices) tindices++;
  }
  if (this->vertexProperty.getValue()) {
    state->pop();
  }
}
