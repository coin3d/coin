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
  \class SoQuadMesh SoQuadMesh.h Inventor/nodes/SoQuadMesh.h
  \brief The SoQuadMesh class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoQuadMesh.h>


#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/misc/SoState.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
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
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <GL/gl.h>

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
  SO_NODE_CONSTRUCTOR(SoQuadMesh);

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

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
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
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
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

  if (rowsize < 2 || colsize < 2) return; // nothing to draw

  const SoCoordinateElement * tmp;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals = TRUE;

#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
  
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
  
  const SbVec3f * coords = SoCoordinateElement::getArrayPtr3(state);
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
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoQuadMesh::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
SbBool
SoQuadMesh::generateDefaultNormals(SoState * /* state */, SoNormalBundle * /* nb */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoQuadMesh::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
SoDetail *
SoQuadMesh::createTriangleDetail(SoRayPickAction * /* action */,
				 const SoPrimitiveVertex * /* v1 */,
				 const SoPrimitiveVertex * /* v2 */,
				 const SoPrimitiveVertex * /* v3 */,
				 SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
