/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoFaceSet SoFaceSet.h Inventor/nodes/SoFaceSet.h
  \brief The SoFaceSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SoPrimitiveVertex.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLCOORDINATEELEMENT)
#include <Inventor/elements/SoGLCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
#include <Inventor/elements/SoShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
#include <Inventor/elements/SoCreaseAngleElement.h>
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT
#if !defined(COIN_EXCLUDE_SOLOGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLOGHTMODELELEMENT

#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>

/*!
  \enum SoFaceSet::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoFaceSet::Binding SoFaceSet::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoFaceSet::Binding SoFaceSet::PER_FACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoFaceSet::Binding SoFaceSet::PER_VERTEX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFInt32 SoFaceSet::numVertices
  FIXME: write documentation for field
*/


// *************************************************************************

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

/*!
  Does initialization common for all objects of the
  SoFaceSet class. This includes setting up the
  type system, among other things.
*/
void
SoFaceSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoFaceSet);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoFaceSet::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  int32_t numvertices = 0;
  for (int i=0; i < this->numVertices.getNum(); i++)
    numvertices += this->numVertices[i];

  inherited::computeCoordBBox(action, numvertices, box, center);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  \internal
*/
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


/*!
  \internal
*/
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

/*!
  FIXME: write function documentation
*/
void
SoFaceSet::GLRender(SoGLRenderAction * action)
{
  // FIXME: check for concave polygons, and use the
  // SbTesselator to draw triangles...

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
  SbBool needNormals = TRUE;
  
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
#endif // !COIN_EXCLUDE_SOLOGHTMODELELEMENT

  SoVertexShape::getVertexData(action->getState(), tmp, normals,
			       needNormals);
  
  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;
  
  SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
  doTextures = tb.needCoordinates();
  
  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());
  
  if (!needNormals) nbind = OVERALL;

  if (needNormals && normals == NULL) {
    normals = getNormalCache()->getNormals();
  }

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = startIndex.getValue();
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  
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
  if (this->vertexProperty.getValue())
    state->pop();
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoFaceSet::generateDefaultNormals(SoState * state, SoNormalCache * nc)
{
  SbBool ccw = TRUE;
  if (SoShapeHintsElement::getVertexOrdering(state) ==
      SoShapeHintsElement::CLOCKWISE) ccw = FALSE;
  
  const SoCoordinateElement * coords = 
    SoCoordinateElement::getInstance(state);
  assert(coords);

  SbBool perVertex = TRUE;

#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
  SoNormalBindingElement::Binding normbind = 
    SoNormalBindingElement::get(state);
    
  switch (normbind) {
  case SoNormalBindingElement::PER_FACE:
  case SoNormalBindingElement::PER_FACE_INDEXED:
  case SoNormalBindingElement::PER_PART:
  case SoNormalBindingElement::PER_PART_INDEXED:
    perVertex = FALSE;
    break;
  default:
    break;
  }
#endif // COIN_EXCLUDE_SONORMALBINDINGELEMENT

  if (perVertex) {
    SoNormalGenerator * gen = 
      new SoNormalGenerator(ccw, this->numVertices.getNum() * 3,
			    TRUE);
    
    int32_t idx = startIndex.getValue();
    const int32_t * ptr = numVertices.getValues(0);
    const int32_t * end = ptr + numVertices.getNum();

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
    gen->generate(SoCreaseAngleElement::get(state));
    nc->set(gen);
  }
  else {
    assert(0);
  }
  return TRUE;
}

#endif // COIN_EXCLUDE_SOGLRENDERACTION



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
SbBool
SoFaceSet::generateDefaultNormals(SoState * /* state */,
				  SoNormalBundle * /* nb */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}
#endif

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoFaceSet::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
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
  
  SoVertexShape::getVertexData(action->getState(), coords, normals,
			       needNormals);
  
  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();
  
  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());
  
  if (needNormals && normals == NULL) {
    normals = getNormalCache()->getNormals();
  }

  int32_t idx = startIndex.getValue();
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  
  int matnr = 0;
  int texnr = 0;
  TriangleShape mode = POLYGON;
  TriangleShape newmode;
  int n;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;

  SoPrimitiveVertex vertex;

  while (ptr < end) {
    n = *ptr++;
    if (n == 3) newmode = TRIANGLES;
    else if (n == 4) newmode = QUADS;
    else newmode = POLYGON;
    if (newmode != mode) {
      if (mode != POLYGON) this->endShape();
      mode = newmode;
      this->beginShape(action, mode);
    }
    else if (mode == POLYGON) this->beginShape(action, mode);
    if (nbind != OVERALL) {
      currnormal = normals++;
      vertex.setNormal(*currnormal);
    }
    if (mbind != OVERALL) vertex.setMaterialIndex(matnr++);
    if (doTextures) {
      if (tb.isFunction()) 
	vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal));
      else
	vertex.setTextureCoords(tb.get(texnr++));
    }
    vertex.setPoint(coords->get3(idx++));
    this->shapeVertex(&vertex);
    while (--n) {
      if (nbind == PER_VERTEX) {
	currnormal = normals++;
	vertex.setNormal(*currnormal);
      }
      if (mbind == PER_VERTEX) vertex.setMaterialIndex(matnr++);
      if (doTextures) {
	if (tb.isFunction()) 
	  vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal));
	else
	  vertex.setTextureCoords(tb.get(texnr++));
      }
      vertex.setPoint(coords->get3(idx++));
      this->shapeVertex(&vertex);
    }
    if (mode == POLYGON) this->endShape();
  }
  if (mode != POLYGON) this->endShape();
  if (this->vertexProperty.getValue())
    state->pop();
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
SoDetail *
SoFaceSet::createTriangleDetail(SoRayPickAction * /* action */,
				const SoPrimitiveVertex * /* v1 */,
				const SoPrimitiveVertex * /* v2 */,
				const SoPrimitiveVertex * /* v3 */,
				SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
