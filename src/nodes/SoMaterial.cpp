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

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLAmbientColorElement.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoGLEmissiveColorElement.h>
#include <Inventor/elements/SoGLSpecularColorElement.h>
#include <Inventor/elements/SoGLShininessElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#include <Inventor/elements/SoOverrideElement.h>

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
#if 0 // OBSOLETED 1999-12-04, pederb
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
#endif // OBSOLETED

// *************************************************************************

SO_NODE_SOURCE(SoMaterial);

/*!
  Constructor.
*/
SoMaterial::SoMaterial()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMaterial);

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
  SO_NODE_INTERNAL_INIT_CLASS(SoMaterial);

  SO_ENABLE(SoGLRenderAction, SoGLAmbientColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLEmissiveColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLSpecularColorElement);
  SO_ENABLE(SoGLRenderAction, SoGLShininessElement);
  SO_ENABLE(SoGLRenderAction, SoTransparencyElement);
  SO_ENABLE(SoGLRenderAction, SoGLPolygonStippleElement);

  SO_ENABLE(SoPickAction, SoAmbientColorElement);
  SO_ENABLE(SoPickAction, SoDiffuseColorElement);
  SO_ENABLE(SoPickAction, SoEmissiveColorElement);
  SO_ENABLE(SoPickAction, SoSpecularColorElement);
  SO_ENABLE(SoPickAction, SoShininessElement);
  SO_ENABLE(SoPickAction, SoTransparencyElement);

  SO_ENABLE(SoCallbackAction, SoAmbientColorElement);
  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
  SO_ENABLE(SoCallbackAction, SoEmissiveColorElement);
  SO_ENABLE(SoCallbackAction, SoSpecularColorElement);
  SO_ENABLE(SoCallbackAction, SoShininessElement);
  SO_ENABLE(SoCallbackAction, SoTransparencyElement);
}

/*!
  FIXME: write function documentation
*/
void
SoMaterial::GLRender(SoGLRenderAction * action)
{
  SoMaterial::doAction(action);
}

//
// Compares materials
// FIXME: Better compare function

#if 0 // OBSOLETED 1999-12-06, pederb
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
#endif // OBSOLETED

/*!
  FIXME: write doc
 */
void
SoMaterial::doAction(SoAction *action)
{
  SoState *state = action->getState();

  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!ambientColor.isIgnored() && !TEST_OVERRIDE(AMBIENT_COLOR)) {
    SoAmbientColorElement::set(state,
                               this,
                               ambientColor.getNum(),
                               ambientColor.getValues(0));
  }
  if (!diffuseColor.isIgnored() && !TEST_OVERRIDE(DIFFUSE_COLOR)) {
    SoDiffuseColorElement::set(action->getState(),
                               this,
                               diffuseColor.getNum(),
                               diffuseColor.getValues(0));
  }
  if (!emissiveColor.isIgnored() && !TEST_OVERRIDE(EMISSIVE_COLOR)) {
    SoEmissiveColorElement::set(action->getState(),
                                this,
                                emissiveColor.getNum(),
                                emissiveColor.getValues(0));
  }
  if (!specularColor.isIgnored() && !TEST_OVERRIDE(SPECULAR_COLOR)) {
    SoSpecularColorElement::set(action->getState(),
                                this,
                                specularColor.getNum(),
                                specularColor.getValues(0));
  }
  if (!shininess.isIgnored() && !TEST_OVERRIDE(SHININESS)) {
    SoShininessElement::set(action->getState(),
                            this,
                            shininess.getNum(),
                            shininess.getValues(0));
  }
  if (!transparency.isIgnored() && !TEST_OVERRIDE(TRANSPARENCY)) {
    SoTransparencyElement::set(action->getState(),
                               this,
                               transparency.getNum(),
                               transparency.getValues(0));
  }
#undef TEST_OVERRIDE
}

/*!
  FIXME: write doc
 */
void
SoMaterial::callback(SoCallbackAction *action)
{
  SoMaterial::doAction(action);
}
