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
  \class SoLineSet SoLineSet.h Inventor/nodes/SoLineSet.h
  \brief The SoLineSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoLineSet.h>


#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/SoPrimitiveVertex.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT

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
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
#include <Inventor/elements/SoDrawStyleElement.h>
#endif
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT

#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>

/*!
  \enum SoLineSet::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoLineSet::Binding SoLineSet::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLineSet::Binding SoLineSet::PER_LINE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLineSet::Binding SoLineSet::PER_SEGMENT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLineSet::Binding SoLineSet::PER_VERTEX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFInt32 SoLineSet::numVertices
  FIXME: write documentation for field
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

/*!
  Does initialization common for all objects of the
  SoLineSet class. This includes setting up the
  type system, among other things.
*/
void
SoLineSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLineSet);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoLineSet::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  int32_t numvertices = 0;
  for (int i=0; i < this->numVertices.getNum(); i++)
    numvertices += this->numVertices[i];

  inherited::computeCoordBBox(action, numvertices, box, center);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

/*!
  \internal
*/
SoLineSet::Binding 
SoLineSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
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
#endif
  return binding;
}


/*!
  \internal
*/
SoLineSet::Binding 
SoLineSet::findNormalBinding(SoState * const state) const
{
  Binding binding = PER_VERTEX;

#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
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
#endif
  return binding;
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
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
  SbBool needNormals = TRUE;
  
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
#endif // !COIN_EXCLUDE_SOLOGHTMODELELEMENT

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
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
    const SoGLLightModelElement * lm = (SoGLLightModelElement *)
      state->getConstElement(SoGLLightModelElement::getClassStackIndex());
    lm->forceSend(SoLightModelElement::BASE_COLOR);
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT
  }

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  if (nbind == OVERALL && needNormals) 
    glNormal3fv((const GLfloat *)currnormal); 


  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = startIndex.getValue();
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();

  int matnr = 0;
  int texnr = 0;

  SbBool drawPoints = FALSE;
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
  drawPoints = SoDrawStyleElement::get(state) ==
    SoDrawStyleElement::POINTS;
#endif

  if (drawPoints) glBegin(GL_POINTS);

  if (nbind == PER_SEGMENT || mbind == PER_SEGMENT) {
    assert(0);
  }
  else {
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
  }
  if (drawPoints) glEnd();
  if (this->vertexProperty.getValue())
    state->pop();
}

#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write function documentation
*/
SbBool 
SoLineSet::generateDefaultNormals(SoState * , SoNormalCache * nc)
{
  // not possible to generate normals for LineSet
  nc->set(0, NULL);
  return TRUE;
}


#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
 */
void
SoLineSet::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  // FIXME: tell caches that geometry contains lines
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoLineSet::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
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

  int32_t idx = startIndex.getValue();
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();

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
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
SoDetail *
SoLineSet::createLineSegmentDetail(SoRayPickAction * /* action */,
				   const SoPrimitiveVertex * /* v1 */,
				   const SoPrimitiveVertex * /* v2 */,
				   SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
