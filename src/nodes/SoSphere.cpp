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
  \class SoSphere SoSphere.h Inventor/nodes/SoSphere.h
  \brief The SoSphere class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <math.h> // M_PI
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/misc/SoState.h>
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENY
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif // ! COIN_EXCLUDE_SOGLSHADEMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif // !COIN_EXCLUDE_SOGLNORMALIZEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
#include <Inventor/elements/SoComplexityTypeElement.h>
#endif

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
#include <Inventor/elements/SoComplexityElement.h>
#endif

/*!
  \var SoSFFloat SoSphere::radius
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoSphere)
SoType SoSphere::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoSphere node class.
*/
void *
SoSphere::createInstance(void)
{
  return new SoSphere;
}

/*!
  Returns the unique type identifier for the SoSphere class.
*/
SoType
SoSphere::getClassTypeId(void)
{
  return SoSphere::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoSphere::getTypeId(void) const
{
  return SoSphere::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoSphere::SoSphere()
{
//$ BEGIN TEMPLATE NodeConstructor(SoSphere)
  // Make sure the class has been initialized.
  assert(SoSphere::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(radius, (1.0f));
}

/*!
  Destructor.
*/
SoSphere::~SoSphere()
{
}

/*!
  Does initialization common for all objects of the
  SoSphere class. This includes setting up the
  type system, among other things.
*/
void
SoSphere::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Sphere)
  // Make sure we only initialize once.
  assert(SoSphere::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSphere::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Sphere",
                       &SoSphere::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSphere::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoSphere::GLRender(SoGLRenderAction * action)
{
  if (!shouldGLRender(action)) return;
 
  SoState *state = action->getState();

  SoMaterialBundle mb(action);
  mb.sendFirst();

#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
  SbBool doTextures = SoGLTextureEnabledElement::get(state);
#else // COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
  SbBool doTextures = FALSE;
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT

#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
#else // COIN_EXCLUDE_SOLIGHTMODELELEMENT
  SbBool sendNormals = FALSE;
#endif // COIN_EXCLUDE_SOLIGHTMODELELEMENT

#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
  const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
    state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
  sh->forceSend(TRUE, TRUE, FALSE);
#endif

  float complexity = 1.0f;

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)

  // FIXME: check for OBJECT_SPACE or SCREEN_SPACE
  if (SoComplexityTypeElement::get(state) !=
      SoComplexityTypeElement::BOUNDING_BOX) {
    complexity = SoComplexityElement::get(state);
  }

#endif
#endif

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  const SoGLShadeModelElement * sm = (SoGLShadeModelElement *)
    state->getConstElement(SoGLShadeModelElement::getClassStackIndex());
  sm->forceSend(FALSE);
#endif

#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
  if (sendNormals) {
    const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
      state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
    ne->forceSend(TRUE);
  }
#endif // !COIN_EXCLUDE_SOGLNORMALIZEELEMENT

  unsigned int flags = 0;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;

  sogl_render_sphere(this->radius.getValue(),
		     (int)(30.0f * complexity),
		     (int)(30.0f * complexity),
		     &mb,
		     flags);
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoSphere::willSetShadeModel(void) const
{
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoSphere::willSetShapeHints(void) const
{
  return TRUE;
}

//! FIXME: doc
SbBool 
SoSphere::willUpdateNormalizeElement(SoState *) const
{
  return TRUE;
}

#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoSphere::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  float r = this->radius.getValue();
  box.setBounds(SbVec3f(-r, -r, -r), SbVec3f(r, r, r));
  center.setValue(0.0f, 0.0f, 0.0f);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
void
SoSphere::rayPick(SoRayPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoSphere::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoSphere::generatePrimitives(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION
