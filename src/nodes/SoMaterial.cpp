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
  \class SoMaterial SoMaterial.h Inventor/nodes/SoMaterial.h
  \brief The SoMaterial class is a node type for setting up material values for scene geometry.
  \ingroup nodes

  Subsequent geometry-type nodes in the scene graph will use values
  from the material "pool" of the traversal state.

  Note that values from a material node will \e replace the previous
  values from in the traversal state.

  \sa SoMaterialBinding
*/

#include <Inventor/nodes/SoMaterial.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLAmbientColorElement.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoGLEmissiveColorElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#include <Inventor/elements/SoGLShininessElement.h>
#include <Inventor/elements/SoGLSpecularColorElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTransparencyElement.h>

/*!
  \var SoMFColor SoMaterial::ambientColor

  Ambient material part color values.

  The ambient part of the material is not influenced by the lights in
  the scene, but which is always added to the geometry surfaces.
*/
/*!
  \var SoMFColor SoMaterial::diffuseColor

  Diffuse material part color values.

  The diffuse part is combined with the light emitted from the scene's
  light sources.
*/
/*!
  \var SoMFColor SoMaterial::specularColor

  Specular material part color values.
*/
/*!
  \var SoMFColor SoMaterial::emissiveColor

  The color of the light "emitted" by the geometry surfaces.
*/
/*!
  \var SoMFFloat SoMaterial::shininess

  Shininess values. Decides how the light from light sources are
  distributed across the geometry surfaces. Valid range is from 0.0
  (which gives a dim appearance), to 1.0 (glossy-looking surfaces).
*/
/*!
  \var SoMFFloat SoMaterial::transparency

  Transparency values. Valid range is from 0.0 (completely opaque,
  which is the default) to 1.0 (completely transparent,
  i.e. invisible).
*/


// *************************************************************************

SO_NODE_SOURCE(SoMaterial);

/*!
  Constructor.
*/
SoMaterial::SoMaterial(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMaterial);

  SO_NODE_ADD_FIELD(ambientColor, (0.2f, 0.2f, 0.2f));
  SO_NODE_ADD_FIELD(diffuseColor, (0.8f, 0.8f, 0.8f));
  SO_NODE_ADD_FIELD(specularColor, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(emissiveColor, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(shininess, (0.2f));
  SO_NODE_ADD_FIELD(transparency, (0.0f));
}

/*!
  Destructor.
*/
SoMaterial::~SoMaterial()
{
}

// Doc from superclass.
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

// Doc from superclass.
void
SoMaterial::GLRender(SoGLRenderAction * action)
{
  SoMaterial::doAction(action);
}

// Doc from superclass.
void
SoMaterial::doAction(SoAction * action)
{
  SoState * state = action->getState();

  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!ambientColor.isIgnored() && !TEST_OVERRIDE(AMBIENT_COLOR)) {
    SoAmbientColorElement::set(state,
                               this,
                               ambientColor.getNum(),
                               ambientColor.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setAmbientColorOverride(state, this, TRUE);
    }
  }
  if (!diffuseColor.isIgnored() && !TEST_OVERRIDE(DIFFUSE_COLOR)) {
    SoDiffuseColorElement::set(action->getState(),
                               this,
                               diffuseColor.getNum(),
                               diffuseColor.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
  if (!emissiveColor.isIgnored() && !TEST_OVERRIDE(EMISSIVE_COLOR)) {
    SoEmissiveColorElement::set(action->getState(),
                                this,
                                emissiveColor.getNum(),
                                emissiveColor.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setEmissiveColorOverride(state, this, TRUE);
    }

  }
  if (!specularColor.isIgnored() && !TEST_OVERRIDE(SPECULAR_COLOR)) {
    SoSpecularColorElement::set(action->getState(),
                                this,
                                specularColor.getNum(),
                                specularColor.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setSpecularColorOverride(state, this, TRUE);
    }
  }
  if (!shininess.isIgnored() && !TEST_OVERRIDE(SHININESS)) {
    SoShininessElement::set(action->getState(),
                            this,
                            shininess.getNum(),
                            shininess.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setShininessOverride(state, this, TRUE);
    }
  }
  if (!transparency.isIgnored() && !TEST_OVERRIDE(TRANSPARENCY)) {
    SoTransparencyElement::set(action->getState(),
                               this,
                               transparency.getNum(),
                               transparency.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setTransparencyOverride(state, this, TRUE);
    }
  }
#undef TEST_OVERRIDE
}

// Doc from superclass.
void
SoMaterial::callback(SoCallbackAction * action)
{
  SoMaterial::doAction(action);
}
