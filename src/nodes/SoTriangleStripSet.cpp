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
  \class SoTriangleStripSet SoTriangleStripSet.h Inventor/nodes/SoTriangleStripSet.h
  \brief The SoTriangleStripSet class is used to render and control non-indexed triangle strips.
  \ingroup nodes

  Triangle strips are specified using the numVertices field. Coordinates,
  normals, materials and texture coordinates are fetched in order from
  the current state or from the vertexProperty node if set. For
  example, if numVertices is set to [3, 4, 5, 3], this node would
  specify a tringle from coordinates 0, 1 and 2, a triangle strip from
  coordinates 3, 4, 5 and 6, a triangle strip from coordinates 7, 8, 9, 10
  and 11 and finally a triangle from coordinates 12, 13, 14.

  Strips are converted into triangles the way OpenGL does it, of course.

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
#ifdef HAVE_WINDOWS_H
#include <windows.h> // *sigh* needed for gl.h
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoGLShadeModelElement.h>
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
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>

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
SoTriangleStripSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTriangleStripSet);
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
  \internal
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
  \internal
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

// doc from parent
void
SoTriangleStripSet::GLRender(SoGLRenderAction * action)
{
  // TODO: make several optimized functions to render this Shape, pederb 20000809

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

  SoVertexShape::getVertexData(action->getState(), tmp, normals,
                               needNormals);

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = this->findMaterialBinding(action->getState());
  Binding nbind = this->findNormalBinding(action->getState());

  if (!needNormals) nbind = OVERALL;

  if (needNormals && normals == NULL) {
    normals = getNormalCache()->getNormals();
    assert(normals);
  }

  const SoGLShadeModelElement * sm = SoGLShadeModelElement::getInstance(state);
  if ((nbind == PER_FACE) || (mbind == PER_FACE))
    sm->forceSend(TRUE);
  else
    sm->evaluate();

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  int matnr = 0;
  int texnr = 0;
  int n;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  if (nbind == OVERALL && needNormals)
    glNormal3fv((const GLfloat *)currnormal);

  while (ptr < end) {
    n = *ptr++ - 2;
    assert(n > 0);

    glBegin(GL_TRIANGLE_STRIP);

    if ((nbind == PER_VERTEX) || (nbind == PER_STRIP)) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if ((mbind == PER_STRIP) || (mbind == PER_VERTEX)) mb.send(matnr++, TRUE);
    if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
    coords->send(idx++);

    if (nbind == PER_VERTEX) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if (mbind == PER_VERTEX) mb.send(matnr++, TRUE);
    if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
    coords->send(idx++);

    while (n--) {
      if (nbind >= PER_FACE) {
        currnormal = normals++;
        glNormal3fv((const GLfloat *)currnormal);
      }
      if (mbind >= PER_FACE) mb.send(matnr++, TRUE);
      if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
      coords->send(idx++);
    }
    glEnd();
  }
  if (this->vertexProperty.getValue())
    state->pop();
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

/*!
  Overloaded to set shade model to flat if normals or materials are
  bound PER_FACE.  Please note that binding normals or materials
  PER_FACE might lead to incorrect rendering for point or spot light
  sources (since all vertices will be evaluated to the same color).
*/
SbBool
SoTriangleStripSet::willSetShadeModel(void) const
{
  return TRUE;
}

/*!
  Overloaded to return FALSE. Normals are generated in normal cache,
*/
SbBool
SoTriangleStripSet::generateDefaultNormals(SoState * /* state */,
                                           SoNormalBundle * /* nb */)
{
  return FALSE;
}

// doc from parent
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
  if (this->vertexProperty.getValue())
    state->pop();
}
