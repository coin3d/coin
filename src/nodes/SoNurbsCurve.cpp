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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoNurbsCurve SoNurbsCurve.h Inventor/nodes/SoNurbsCurve.h
  \brief The SoNurbsCurve class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <GL/gl.h>
#include <GL/glu.h>
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
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
#include <Inventor/elements/SoCreaseAngleElement.h>
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT

/*!
  \var SoSFInt32 SoNurbsCurve::numControlPoints
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoNurbsCurve::knotVector
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoNurbsCurve)
SoType SoNurbsCurve::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoNurbsCurve node class.
*/
void *
SoNurbsCurve::createInstance(void)
{
  return new SoNurbsCurve;
}

/*!
  Returns the unique type identifier for the SoNurbsCurve class.
*/
SoType
SoNurbsCurve::getClassTypeId(void)
{
  return SoNurbsCurve::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoNurbsCurve::getTypeId(void) const
{
  return SoNurbsCurve::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoNurbsCurve::SoNurbsCurve()
{
//$ BEGIN TEMPLATE NodeConstructor(SoNurbsCurve)
  // Make sure the class has been initialized.
  assert(SoNurbsCurve::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(numControlPoints, (0));
  SO_NODE_ADD_FIELD(knotVector, (0));

  this->nurbsRenderer = NULL;
}

/*!
  Destructor.
*/
SoNurbsCurve::~SoNurbsCurve()
{
  if (this->nurbsRenderer) {
    gluDeleteNurbsRenderer(this->nurbsRenderer);
  }
}

/*!
  Does initialization common for all objects of the
  SoNurbsCurve class. This includes setting up the
  type system, among other things.
*/
void
SoNurbsCurve::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(NurbsCurve)
  // Make sure we only initialize once.
  assert(SoNurbsCurve::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoNurbsCurve::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "NurbsCurve",
                       &SoNurbsCurve::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoNurbsCurve::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoNurbsCurve::GLRender(SoGLRenderAction * action)
{
  //
  // this is just a quick hack. See comment in SoNurbsSurface::GLRender
  //


  SoState * state = action->getState();
  
  if (!this->shouldGLRender(action)) {
    return;
  }

  if (this->nurbsRenderer == NULL) {
    this->nurbsRenderer = gluNewNurbsRenderer();
  }

  const SoCoordinateElement * coords = 
    SoCoordinateElement::getInstance(state);
    
//    SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
//    doTextures = tb.needCoordinates();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  gluNurbsProperty(nurbsRenderer, (GLenum) GLU_SAMPLING_TOLERANCE, 50.0);
  gluNurbsProperty(nurbsRenderer, (GLenum) GLU_DISPLAY_MODE, GLU_FILL);

  glEnable(GL_AUTO_NORMAL);

  int dim = coords->is3D() ? 3 : 4;

  GLfloat * ptr = coords->is3D() ? 
    (GLfloat *)SoCoordinateElement::getArrayPtr3(state) :
    (GLfloat *)SoCoordinateElement::getArrayPtr4(state);

  gluNurbsCurve(this->nurbsRenderer, knotVector.getNum(),
		(GLfloat *)knotVector.getValues(0),
		dim, ptr, 
		knotVector.getNum()-numControlPoints.getValue() ,
		(GLenum)GLU_FILL);

  gluEndSurface(this->nurbsRenderer);
  glDisable(GL_AUTO_NORMAL);
}
#endif


#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoNurbsCurve::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  // FIXME: this is just a quick approximation
  SoState * state = action->getState();
    
  int numCoords = SoCoordinateElement::getInstance(state)->getNum();
  int num = this->numControlPoints.getValue();

  assert(num <= numCoords);

  if (SoCoordinateElement::getInstance(state)->is3D()) {
    const SbVec3f * coords = SoCoordinateElement::getArrayPtr3(state);
    assert(coords);
    box.makeEmpty();
    for (int i = 0; i < num; i++) box.extendBy(coords[i]);
  }
  else {
    const SbVec4f * coords = SoCoordinateElement::getArrayPtr4(state);
    assert(coords);
    for (int i = 0; i< num; i++) {
      SbVec4f tmp = coords[i];
      box.extendBy(SbVec3f(tmp[0], tmp[1], tmp[2]));
    }
  }
  center = box.getCenter();
}

#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
void
SoNurbsCurve::rayPick(SoRayPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoNurbsCurve::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
 */
void
SoNurbsCurve::getBoundingBox(SoGetBoundingBoxAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoNurbsCurve::sendPrimitive(SoAction *,  SoPrimitiveVertex *)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoNurbsCurve::generatePrimitives(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION
