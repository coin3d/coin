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
  \class SoPointSet SoPointSet.h Inventor/nodes/SoPointSet.h
  \brief The SoPointSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/SoPrimitiveVertex.h>

#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/details/SoPointDetail.h>

/*!
  \enum SoPointSet::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoPointSet::Binding SoPointSet::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPointSet::Binding SoPointSet::PER_VERTEX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFInt32 SoPointSet::numPoints
  FIXME: write documentation for field
*/

// *************************************************************************
SO_NODE_SOURCE(SoPointSet);

/*!
  Constructor.
*/
SoPointSet::SoPointSet()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPointSet);

  SO_NODE_ADD_FIELD(numPoints, (-1));
}

/*!
  Destructor.
*/
SoPointSet::~SoPointSet()
{
}

/*!
  Does initialization common for all objects of the
  SoPointSet class. This includes setting up the
  type system, among other things.
*/
void
SoPointSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPointSet);
}

/*!
  FIXME: write function documentation
*/
void
SoPointSet::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  inherited::computeCoordBBox(action, this->numPoints.getValue(), box, center);
}

/*!
  \internal
*/
SoPointSet::Binding
SoPointSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;
  if (SoMaterialBindingElement::get(state) !=
      SoMaterialBindingElement::OVERALL) binding = PER_VERTEX;
  return binding;
}

/*!
  \internal
*/
SoPointSet::Binding
SoPointSet::findNormalBinding(SoState * const state) const

{
  Binding binding = PER_VERTEX;

  if (SoNormalBindingElement::get(state) ==
      SoNormalBindingElement::OVERALL) binding = OVERALL;
  return binding;
}

/*!
  FIXME: write function documentation
*/
void
SoPointSet::GLRender(SoGLRenderAction * action)
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

  SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
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

  int32_t idx = this->startIndex.getValue();
  int32_t numpts = this->numPoints.getValue();

  int matnr = 0;
  int texnr = 0;

  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    if (nbind == PER_VERTEX) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if (mbind == PER_VERTEX) mb.send(matnr++, TRUE);
    if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
    coords->send(idx++);
  }
  glEnd();

  if (this->vertexProperty.getValue())
    state->pop();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoPointSet::generateDefaultNormals(SoState *, SoNormalCache * nc)
{
  // not possible to generate normals for PointSet
  nc->set(0, NULL);
  return TRUE;
}

/*!
  FIXME: write doc
 */
void
SoPointSet::getBoundingBox(SoGetBoundingBoxAction *action)
{
  inherited::getBoundingBox(action);
}

/*!
  FIXME: write doc
*/
void
SoPointSet::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumPoints(this->numPoints.getValue());
}

/*!
  FIXME: write doc
 */
void
SoPointSet::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  const SoCoordinateElement *coords;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  SoVertexShape::getVertexData(action->getState(), coords, normals,
                               needNormals);

  if (normals == NULL) needNormals = FALSE;

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());

  if (!needNormals) nbind = OVERALL;

  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  vertex.setDetail(&pointDetail);

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  if (nbind == OVERALL && needNormals)
    vertex.setNormal(*currnormal);

  int32_t idx = this->startIndex.getValue();
  int32_t numpts = this->numPoints.getValue();

  int matnr = 0;
  int texnr = 0;
  int normnr = 0;

  this->beginShape(action, SoShape::POINTS);
  for (int i = 0; i < numpts; i++) {
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
      if (tb.isFunction()) {
        vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal));
      }
      else {
        pointDetail.setTextureCoordIndex(texnr);
        vertex.setTextureCoords(tb.get(texnr++));
      }
    }
    pointDetail.setCoordinateIndex(idx);
    vertex.setPoint(coords->get3(idx++));
    this->shapeVertex(&vertex);
  }
  this->endShape();

  if (this->vertexProperty.getValue())
    state->pop();

}
