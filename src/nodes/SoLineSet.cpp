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
  \class SoLineSet SoLineSet.h Inventor/nodes/SoLineSet.h
  \brief The SoLineSet class is used to render and organize non-indexed polylines.
  \ingroup nodes

  Polylines are specified using the numVertices field. Coordinates,
  normals, materials and texture coordinates are fetched in order from
  the current state or from the vertexProperty node if set. For
  example, if numVertices is set to [3, 4, 2], this node would specify
  a line through coordinates 0, 1 and 2, a line through coordinates 3, 4, 5
  and 6, and finally a single line segment between coordinates 7 and 8.

  Binding PER_VERTEX, PER_FACE or OVERALL can be set for material,
  and normals. The default material binding is OVERALL. The default
  normal binding is PER_VERTEX. If no normals are set, the line set
  will be rendered with lighting disabled.
*/

#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/SoPrimitiveVertex.h>

#include <Inventor/actions/SoGLRenderAction.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#ifdef HAVE_WINDOWS_H
#include <windows.h> // Needed for stuff that GL/gl.h depends on.
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/details/SoLineDetail.h>

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
SoLineSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLineSet);
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

// doc from parent
void
SoLineSet::GLRender(SoGLRenderAction * action)
{
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

  if (normals == NULL) needNormals = FALSE;

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());

  if (!needNormals) {
    nbind = OVERALL;
    const SoGLLightModelElement * lm = (SoGLLightModelElement *)
      state->getConstElement(SoGLLightModelElement::getClassStackIndex());
    lm->forceSend(SoLightModelElement::BASE_COLOR);
  }

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  if (nbind == OVERALL && needNormals)
    glNormal3fv((const GLfloat *)currnormal);


  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = startIndex.getValue();
  int32_t dummyarray[1];
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  this->fixNumVerticesPointers(state, ptr, end, dummyarray);

  int matnr = 0;
  int texnr = 0;

  SbBool drawPoints =
    SoDrawStyleElement::get(state) == SoDrawStyleElement::POINTS;

  if (nbind == PER_SEGMENT || mbind == PER_SEGMENT) {
    if (drawPoints) glBegin(GL_POINTS);
    else glBegin(GL_LINES);

    while (ptr < end) {
      int n = *ptr++;
      if (n < 2) {
        idx += n;
        continue;
      }
      if (mbind == PER_LINE) {
        mb.send(matnr++, TRUE);
      }
      if (nbind == PER_LINE) {
        currnormal = normals++;
      }
      while (--n) {
        if (mbind == PER_SEGMENT || mbind == PER_VERTEX) {
          mb.send(matnr++, TRUE);
        }
        if (nbind == PER_SEGMENT || nbind == PER_VERTEX) {
          currnormal = normals++;
        }
        glNormal3fv((const GLfloat*)currnormal);
        if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
        coords->send(idx++);

        if (nbind == PER_VERTEX) {
          currnormal = normals++;
          glNormal3fv((const GLfloat *)currnormal);
        }
        if (mbind == PER_VERTEX) mb.send(matnr++, TRUE);
        if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
        coords->send(idx);
      }
      idx++;
    }
    glEnd();
  }
  else {
    if (drawPoints) glBegin(GL_POINTS);
    while (ptr < end) {
      int n = *ptr++;
      if (n < 2) {
        idx += n; // FIXME: is this correct?
        continue;
      }
      n -= 2;
      if (!drawPoints) glBegin(GL_LINE_STRIP);
      if (nbind != OVERALL) {
        currnormal = normals++;
        glNormal3fv((const GLfloat *)currnormal);
      }
      if (mbind != OVERALL) mb.send(matnr++, TRUE);
      if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
      coords->send(idx++);
      do {
        if (nbind == PER_VERTEX) {
          currnormal = normals++;
          glNormal3fv((const GLfloat *)currnormal);
        }
        if (mbind == PER_VERTEX) mb.send(matnr++, TRUE);
        if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
        coords->send(idx++);
      } while (n--);
      if (!drawPoints) glEnd();
    }
    if (drawPoints) glEnd();
  }
  if (this->vertexProperty.getValue())
    state->pop();
}

/*!
  Overloaded to clear normal cache.
*/
SbBool
SoLineSet::generateDefaultNormals(SoState * , SoNormalCache * nc)
{
  // not possible to generate normals for LineSet
  nc->set(0, NULL);
  return TRUE;
}

// doc from parent
void
SoLineSet::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  // FIXME: tell caches that geometry contains lines
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
    assert(0);
  }
  else {
    while (ptr < end) {
      lineDetail.setLineIndex(0);
      int n = *ptr++;
      if (n < 2) {
        idx += n; // FIXME: is this correct?
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
        lineDetail.incLineIndex();
      } while (n--);
      this->endShape();
      lineDetail.incPartIndex();
    }
  }
  if (this->vertexProperty.getValue())
    state->pop();
}
