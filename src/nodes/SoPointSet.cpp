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
  \class SoPointSet SoPointSet.h Inventor/nodes/SoPointSet.h
  \brief The SoPointSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>

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
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT

#include <Inventor/caches/SoNormalCache.h>

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

//$ BEGIN TEMPLATE NodeSource(SoPointSet)
SoType SoPointSet::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPointSet node class.
*/
void *
SoPointSet::createInstance(void)
{
  return new SoPointSet;
}

/*!
  Returns the unique type identifier for the SoPointSet class.
*/
SoType
SoPointSet::getClassTypeId(void)
{
  return SoPointSet::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPointSet::getTypeId(void) const
{
  return SoPointSet::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPointSet::SoPointSet()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPointSet)
  // Make sure the class has been initialized.
  assert(SoPointSet::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(PointSet)
  // Make sure we only initialize once.
  assert(SoPointSet::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPointSet::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "PointSet",
                       &SoPointSet::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPointSet::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoPointSet::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  inherited::computeCoordBBox(action, this->numPoints.getValue(), box, center);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

/*!
  \internal
*/
SoPointSet::Binding 
SoPointSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  if (SoMaterialBindingElement::get(state) !=
      SoMaterialBindingElement::OVERALL) binding = PER_VERTEX;
#endif
  return binding;
}

/*!
  \internal
*/
SoPointSet::Binding 
SoPointSet::findNormalBinding(SoState * const state) const

{
  Binding binding = PER_VERTEX;

#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  if (SoNormalBindingElement::get(state) ==
      SoNormalBindingElement::OVERALL) binding = OVERALL;
#endif
  return binding;
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
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
  SbBool needNormals;
  
  SoVertexShape::getGLData(action->getState(), tmp, normals,
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
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

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

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
 */
void
SoPointSet::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoPointSet::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPointSet::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
SoDetail *
SoPointSet::createPointDetail(SoRayPickAction * /* action */,
			      const SoPrimitiveVertex * /* v */,
			      SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
