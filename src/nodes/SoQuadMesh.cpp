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
  \class SoQuadMesh SoQuadMesh.h Inventor/nodes/SoQuadMesh.h
  \brief The SoQuadMesh class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/misc/SoState.h>

#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <GL/gl.h>

#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif

/*!
  \enum SoQuadMesh::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoQuadMesh::Binding SoQuadMesh::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoQuadMesh::Binding SoQuadMesh::PER_ROW
  FIXME: write documentation for enum definition
*/
/*!
  \var SoQuadMesh::Binding SoQuadMesh::PER_FACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoQuadMesh::Binding SoQuadMesh::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoQuadMesh::Binding SoQuadMesh::NONE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFInt32 SoQuadMesh::verticesPerColumn
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoQuadMesh::verticesPerRow
  FIXME: write documentation for field
*/

// *************************************************************************
SO_NODE_SOURCE(SoQuadMesh);

/*!
  Constructor.
*/
SoQuadMesh::SoQuadMesh()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoQuadMesh);

  SO_NODE_ADD_FIELD(verticesPerColumn, (1));
  SO_NODE_ADD_FIELD(verticesPerRow, (1));
}

/*!
  Destructor.
*/
SoQuadMesh::~SoQuadMesh()
{
}

/*!
  Does initialization common for all objects of the
  SoQuadMesh class. This includes setting up the
  type system, among other things.
*/
void
SoQuadMesh::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoQuadMesh);
}

/*!
  FIXME: write function documentation
*/
void
SoQuadMesh::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  inherited::computeCoordBBox(action,
                              this->verticesPerRow.getValue() *
                              this->verticesPerColumn.getValue(),
                              box, center);
}

/*!
  \internal
*/
SoQuadMesh::Binding
SoQuadMesh::findMaterialBinding(SoState * const state) const
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
    binding = PER_ROW;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = OVERALL;
#if COIN_DEBUG
    SoDebugError::postWarning("SoQuadMesh::findMaterialBinding",
                              "unknown material binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}


/*!
  \internal
*/
SoQuadMesh::Binding
SoQuadMesh::findNormalBinding(SoState * const state) const
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
    binding = PER_ROW;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = PER_VERTEX;
#if COIN_DEBUG
    SoDebugError::postWarning("SoQuadMesh::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

/*!
  FIXME: write function documentation
*/
void
SoQuadMesh::GLRender(SoGLRenderAction * action)
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

  const int rowsize = this->verticesPerRow.getValue();
  const int colsize = this->verticesPerColumn.getValue();

  if (rowsize < 2 || colsize < 2) {
    if (this->vertexProperty.getValue()) state->pop();
    return; // nothing to draw
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

  SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
  doTextures = tb.needCoordinates();

  //
  // FIXME: should I test for texture coordinate binding other
  // than PER_VERTEX_INDEXED. The normal an material bindings
  // are equal for indexed and nonindex, this is probably the
  // case for texture coordinate binding too... (pederb, 990701)
  //

  int start = this->startIndex.getValue();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());

  if (!needNormals) nbind = OVERALL;

  if (needNormals && normals == NULL) {
    normals = getNormalCache()->getNormals();
  }

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;

  if (nbind == OVERALL && needNormals) {
    glNormal3fv((const GLfloat *)currnormal);
  }

  int curridx; // for optimization only

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

#define IDX(r,c) ((r)*rowsize+(c))

  int midx = 0;
  for (int i = 0; i < colsize-1; i++) {
    int j = 0;
    glBegin(GL_QUAD_STRIP);
    if (nbind == PER_ROW) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if (mbind == PER_ROW) mb.send(midx++,TRUE);

    for (j = 0; j < rowsize; j++) {
      curridx = IDX(i,j);
      if (nbind == PER_VERTEX) {
        currnormal = &normals[curridx];
        glNormal3fv((const GLfloat *)currnormal);
      }
      else if (nbind == PER_FACE) {
        currnormal = normals++;
        glNormal3fv((const GLfloat *)currnormal);
      }
      if (mbind == PER_VERTEX) mb.send(curridx, TRUE);
      else if (mbind == PER_FACE) mb.send(midx++, TRUE);

      if (doTextures) {
        tb.send(curridx, coords->get3(start + curridx),
                *currnormal);
      }
      coords->send(start + curridx);
      curridx = IDX(i+1,j);
      if (nbind == PER_VERTEX) {
        currnormal = &normals[curridx];
        glNormal3fv((const GLfloat *)currnormal);
      }
      if (mbind == PER_VERTEX) mb.send(curridx, TRUE);
      if (doTextures) {
        tb.send(curridx, coords->get3(start + curridx),
                *currnormal);
      }
      coords->send(start + curridx);
    }
    glEnd(); // end of strip/row
  }
#undef IDX

  if (this->vertexProperty.getValue())
    state->pop();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoQuadMesh::generateDefaultNormals(SoState * state, SoNormalCache * nc)
{
  if (verticesPerRow.getValue() < 2 || verticesPerColumn.getValue() < 2)
    return TRUE; // nothing to generate

  SbBool ccw = TRUE;
  if (SoShapeHintsElement::getVertexOrdering(state) ==
      SoShapeHintsElement::CLOCKWISE) ccw = FALSE;

  const SbVec3f * coords = SoCoordinateElement::getInstance(state)->getArrayPtr3();
  assert(coords);

  Binding binding = findNormalBinding(state);

  switch (binding) {
  case PER_VERTEX:
    nc->generatePerVertexQuad(coords + startIndex.getValue(),
                              verticesPerRow.getValue(),
                              verticesPerColumn.getValue(),
                              ccw);
    break;
  case PER_FACE:
    nc->generatePerFaceQuad(coords + startIndex.getValue(),
                            verticesPerRow.getValue(),
                            verticesPerColumn.getValue(),
                            ccw);
    break;
  case PER_ROW:
    nc->generatePerRowQuad(coords + startIndex.getValue(),
                           verticesPerRow.getValue(),
                           verticesPerColumn.getValue(),
                           ccw);
    break;
  case OVERALL:
    break;
  default:
    assert(0);
    return FALSE;
    break;
  }
  return TRUE;
}

/*!
  FIXME: write doc
 */
void
SoQuadMesh::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumTriangles(2 * this->verticesPerRow.getValue() *
                          this->verticesPerColumn.getValue());
}

/*!
  FIXME: write doc
 */
SbBool
SoQuadMesh::generateDefaultNormals(SoState * /* state */, SoNormalBundle * /* nb */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoQuadMesh::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  const int rowsize = this->verticesPerRow.getValue();
  const int colsize = this->verticesPerColumn.getValue();

  if (rowsize < 2 || colsize < 2) {
    if (vertexProperty.getValue()) state->pop();
    return;
  }
  const SoCoordinateElement *coords;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals = TRUE;

  SoVertexShape::getVertexData(action->getState(), coords, normals,
                               needNormals);

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  int start = this->startIndex.getValue();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());

  if (needNormals && normals == NULL) {
    normals = getNormalCache()->getNormals();
  }

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;

  SoPrimitiveVertex vertex;
  SoFaceDetail faceDetail;
  SoPointDetail pointDetail;
  vertex.setDetail(&pointDetail);

  if (nbind == OVERALL && needNormals) {
    vertex.setNormal(*currnormal);
  }

  int curridx; // for optimization only

#define IDX(r,c) ((r)*rowsize+(c))

  int normnr = 0;
  int midx = 0;
  for (int i = 0; i < colsize-1; i++) {
    int j = 0;
    this->beginShape(action, QUAD_STRIP, &faceDetail);
    if (nbind == PER_ROW) {
      pointDetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    if (mbind == PER_ROW) {
      pointDetail.setMaterialIndex(midx);
      vertex.setMaterialIndex(midx++);
    }
    SbBool first = TRUE;
    faceDetail.setFaceIndex(0);
    for (j = 0; j < rowsize; j++) {
      curridx = IDX(i,j);
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(curridx);
        currnormal = &normals[curridx];
        vertex.setNormal(*currnormal);
      }
      else if (nbind == PER_FACE) {
        pointDetail.setNormalIndex(normnr);
        currnormal = &normals[normnr++];
        vertex.setNormal(*currnormal);
      }
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(curridx);
        vertex.setMaterialIndex(curridx);
      }
      else if (mbind == PER_FACE) {
        pointDetail.setMaterialIndex(midx);
        vertex.setMaterialIndex(midx++);
      }
      if (doTextures) {
        if (tb.isFunction())
          vertex.setTextureCoords(tb.get(coords->get3(start+curridx), *currnormal));
        else {
          pointDetail.setTextureCoordIndex(curridx);
          vertex.setTextureCoords(tb.get(curridx));
        }
      }
      pointDetail.setCoordinateIndex(start + curridx);
      vertex.setPoint(coords->get3(start + curridx));
      this->shapeVertex(&vertex);

      curridx = IDX(i+1,j);
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(curridx);
        currnormal = &normals[curridx];
        vertex.setNormal(*currnormal);
      }
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(curridx);
        vertex.setMaterialIndex(curridx);
      }
      if (doTextures) {
        if (tb.isFunction())
          vertex.setTextureCoords(tb.get(coords->get3(start+curridx), *currnormal));
        else {
          pointDetail.setTextureCoordIndex(curridx);
          vertex.setTextureCoords(tb.get(curridx));
        }
      }
      pointDetail.setCoordinateIndex(start + curridx);
      vertex.setPoint(coords->get3(start + curridx));
      this->shapeVertex(&vertex);
      if (first) first = FALSE;
      else faceDetail.incFaceIndex();
    }
    this->endShape();
    faceDetail.incPartIndex();
  }
#undef IDX

  if (this->vertexProperty.getValue())
    state->pop();

}
