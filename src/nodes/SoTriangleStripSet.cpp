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
  \class SoTriangleStripSet SoTriangleStripSet.h Inventor/nodes/SoTriangleStripSet.h
  \brief The SoTriangleStripSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#include <Inventor/misc/SoState.h>

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

#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>

/*!
  \enum SoTriangleStripSet::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoTriangleStripSet::Binding SoTriangleStripSet::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoTriangleStripSet::Binding SoTriangleStripSet::PER_STRIP
  FIXME: write documentation for enum definition
*/
/*!
  \var SoTriangleStripSet::Binding SoTriangleStripSet::PER_FACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoTriangleStripSet::Binding SoTriangleStripSet::PER_VERTEX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFInt32 SoTriangleStripSet::numVertices
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoTriangleStripSet)
SoType SoTriangleStripSet::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoTriangleStripSet node class.
*/
void *
SoTriangleStripSet::createInstance(void)
{
  return new SoTriangleStripSet;
}

/*!
  Returns the unique type identifier for the SoTriangleStripSet class.
*/
SoType
SoTriangleStripSet::getClassTypeId(void)
{
  return SoTriangleStripSet::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoTriangleStripSet::getTypeId(void) const
{
  return SoTriangleStripSet::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoTriangleStripSet::SoTriangleStripSet()
{
//$ BEGIN TEMPLATE NodeConstructor(SoTriangleStripSet)
  // Make sure the class has been initialized.
  assert(SoTriangleStripSet::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(numVertices, (-1));
}

/*!
  Destructor.
*/
SoTriangleStripSet::~SoTriangleStripSet()
{
}

/*!
  Does initialization common for all objects of the
  SoTriangleStripSet class. This includes setting up the
  type system, among other things.
*/
void
SoTriangleStripSet::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(TriangleStripSet)
  // Make sure we only initialize once.
  assert(SoTriangleStripSet::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTriangleStripSet::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "TriangleStripSet",
                       &SoTriangleStripSet::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoTriangleStripSet::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoTriangleStripSet::computeBBox(SoAction * action,
				SbBox3f & box, SbVec3f & center)
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

/*!
  FIXME: write function documentation
*/
void
SoTriangleStripSet::GLRender(SoGLRenderAction * action)
{
  // TODO: make several optimized functions to render this Shape.
  
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
  SbBool needNormals;
  
  SoVertexShape::getGLData(action->getState(), tmp, normals,
			   needNormals);
  
  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;
  
  SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
  doTextures = tb.needCoordinates();
  
  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());
  
  if (!needNormals) nbind = OVERALL;

  if (needNormals && normals == NULL) {
    normals = getNormalCache()->getNormals();
    if (normals == NULL) {
      // FIXME: temporary until normals can be generated per face
      //        and per strip
      nbind = OVERALL;
    }
  }

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = startIndex.getValue();
  const int32_t * ptr = numVertices.getValues(0);
  const int32_t * end = ptr + numVertices.getNum();
  
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

    if (nbind != OVERALL) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if (mbind != OVERALL) mb.send(matnr++, TRUE);
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

/*!
  FIXME: write function documentation
*/
SbBool 
SoTriangleStripSet::generateDefaultNormals(SoState * state, SoNormalCache * nc)
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
      new SoNormalGenerator(ccw, this->numVertices.getNum()*3,
			    TRUE);
    
    int32_t idx = startIndex.getValue();
    const int32_t * ptr = numVertices.getValues(0);
    const int32_t * end = ptr + numVertices.getNum();

    const SoCoordinateElement * coords = 
      SoCoordinateElement::getInstance(state);

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
    gen->generate(SoCreaseAngleElement::get(state), 
		  numVertices.getValues(0),
		  numVertices.getNum());
    nc->set(gen);
  }
  else {
    assert(0 && "Normals per face or per strip not implemented");
  }
  return TRUE;
}

#endif // COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoTriangleStripSet::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
SbBool
SoTriangleStripSet::generateDefaultNormals(SoState * /* state */,
					   SoNormalBundle * /* nb */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoTriangleStripSet::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
SoDetail*
SoTriangleStripSet::createTriangleDetail(SoRayPickAction * /* action */,
					 const SoPrimitiveVertex * /* v1 */,
					 const SoPrimitiveVertex * /* v2 */,
					 const SoPrimitiveVertex * /* v3 */,
					 SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
