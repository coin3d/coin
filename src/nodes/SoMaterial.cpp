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
  \class SoMaterial SoMaterial.h Inventor/nodes/SoMaterial.h
  \brief The SoMaterial class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
#include <Inventor/elements/SoGLAmbientColorElement.h>
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
#include <Inventor/elements/SoGLEmissiveColorElement.h>
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
#include <Inventor/elements/SoGLSpecularColorElement.h>
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
#include <Inventor/elements/SoGLShininessElement.h>
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
#include <Inventor/elements/SoTransparencyElement.h>
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLE

#include <math.h>

/*!
  \var SoMFColor SoMaterial::ambientColor
  FIXME: write documentation for field
*/
/*!
  \var SoMFColor SoMaterial::diffuseColor
  FIXME: write documentation for field
*/
/*!
  \var SoMFColor SoMaterial::specularColor
  FIXME: write documentation for field
*/
/*!
  \var SoMFColor SoMaterial::emissiveColor
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoMaterial::shininess
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoMaterial::transparency
  FIXME: write documentation for field
*/


// *************************************************************************

// FIXME: old crust, find a good place to store this code. 19980913 mortene.
static float
compareAppearance(SoMFColor * firstColor, int firstIdx,
		   SoMFColor * otherColor, int otherIdx)
{
  if (firstColor->isDefault() && otherColor->isDefault()) return 0.0f;
  return 0.299 * pow(firstColor->getValues(0)[firstIdx][0]-otherColor->getValues(0)[otherIdx][0],2)
+
    0.587 * pow(firstColor->getValues(0)[firstIdx][1]-otherColor->getValues(0)[otherIdx][1],2)+
    0.114 * pow(firstColor->getValues(0)[firstIdx][2]-otherColor->getValues(0)[otherIdx][2],2);
}


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoMaterial)
SoType SoMaterial::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoMaterial node class.
*/
void *
SoMaterial::createInstance(void)
{
  return new SoMaterial;
}

/*!
  Returns the unique type identifier for the SoMaterial class.
*/
SoType
SoMaterial::getClassTypeId(void)
{
  return SoMaterial::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoMaterial::getTypeId(void) const
{
  return SoMaterial::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoMaterial::SoMaterial()
{
//$ BEGIN TEMPLATE NodeConstructor(SoMaterial)
  // Make sure the class has been initialized.
  assert(SoMaterial::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(ambientColor,(0.2f, 0.2f, 0.2f));
  SO_NODE_ADD_FIELD(diffuseColor,(0.8f, 0.8f, 0.8f));
  SO_NODE_ADD_FIELD(specularColor,(0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(emissiveColor,(0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(shininess,(0.2f));
  SO_NODE_ADD_FIELD(transparency,(0.0f));
}

/*!
  Destructor.
*/
SoMaterial::~SoMaterial()
{
}

/*!
  Does initialization common for all objects of the
  SoMaterial class. This includes setting up the
  type system, among other things.
*/
void
SoMaterial::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Material)
  // Make sure we only initialize once.
  assert(SoMaterial::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMaterial::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Material",
                       &SoMaterial::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLAmbientColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLEmissiveColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLSpecularColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLShininessElement);
  SO_ENABLE(SoGLRenderAction, SoTransparencyElement);
  SO_ENABLE(SoGLRenderAction, SoGLPolygonStippleElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMaterial::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoMaterial::GLRender(SoGLRenderAction * action)
{
  // FIXME: check for isIgnored() for all fields
  // FIXME: check light model element
  SoState * state = action->getState();
  if (!ambientColor.isIgnored()) {
    SoAmbientColorElement::set(state,
			       this,
			       ambientColor.getNum(),
			       ambientColor.getValues(0));
  }
  if (!diffuseColor.isIgnored()) {
    SoDiffuseColorElement::set(action->getState(),
			       this,
			       diffuseColor.getNum(),
			       diffuseColor.getValues(0));
  }
  if (!emissiveColor.isIgnored()) {
    SoEmissiveColorElement::set(action->getState(),
				this,
				emissiveColor.getNum(),
				emissiveColor.getValues(0));
  }
  if (!specularColor.isIgnored()) {
    SoSpecularColorElement::set(action->getState(),
				this,
				specularColor.getNum(),
				specularColor.getValues(0));
  }
  if (!shininess.isIgnored()) {
    SoShininessElement::set(action->getState(),
			    this,
			    shininess.getNum(),
			    shininess.getValues(0));
  }
  if (!transparency.isIgnored()) {
    SoTransparencyElement::set(action->getState(),
			       this,
			       transparency.getNum(),
			       transparency.getValues(0));
  }
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


//
// Compares materials
// FIXME: Better compare function
float
SoMaterial::compareAppearance(int thisIdx,SoMaterial * otherMaterial,int otherIdx)
{
  return 0.25 *(::compareAppearance(&(this->ambientColor), thisIdx,&(otherMaterial->ambientColor),otherIdx)+
	       ::compareAppearance(&(this->diffuseColor), thisIdx,&(otherMaterial->diffuseColor),otherIdx)+
	       ::compareAppearance(&(this->specularColor), thisIdx,&(otherMaterial->specularColor),otherIdx)+
	       ::compareAppearance(&(this->emissiveColor), thisIdx,&(otherMaterial->emissiveColor),otherIdx));
}

//
// Compares materials
// FIXME: Better compare function
float
SoMaterial::compareAppearanceVRML2(SoMaterial * otherMaterial)
{
  return 0.25 *(::compareAppearance(&(this->diffuseColor), 0,&(otherMaterial->diffuseColor),0)+
	       ::compareAppearance(&(this->specularColor), 0,&(otherMaterial->specularColor),0)+
	       ::compareAppearance(&(this->emissiveColor), 0,&(otherMaterial->emissiveColor),0));
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoMaterial::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoMaterial::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
