/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLMaterial SoVRMLMaterial.h Inventor/VRMLnodes/SoVRMLMaterial.h
  \brief The SoVRMLMaterial class is used to assign a material to geometry.
*/

/*!
  \var SoSFColor SoVRMLMaterial:: diffuseColor
  The diffuse color component. Default value is (0.8, 0.8, 0.8).
*/

/*!
  \var SoSFFloat SoVRMLMaterial:: ambientIntensity
  The ambient intensity. Default value is 0.2.
*/

/*!
  \var SoSFColor SoVRMLMaterial:: specularColor
  The specular color component. Default value is (0, 0, 0).
*/

/*!
  \var SoSFColor SoVRMLMaterial:: emissiveColor
  The emissive color component.  Default value is (0, 0, 0).
*/

/*!
  \var SoSFFloat SoVRMLMaterial:: shininess
  The shininess value. A number between 0 and 1. Default value is 0.2.
*/

/*!
  \var SoSFFloat SoVRMLMaterial:: transparency
  The material transparency. Valid range is from 0.0 (completely opaque) to 1.0 (completely transparent).
  Default value is 0.0.
*/


#include <Inventor/VRMLnodes/SoVRMLMaterial.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

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
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <stdlib.h>

SO_NODE_SOURCE(SoVRMLMaterial);

// Doc in parent
void
SoVRMLMaterial::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLMaterial, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLMaterial::SoVRMLMaterial(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLMaterial);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(diffuseColor, (0.8f, 0.8f, 0.8f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(ambientIntensity, (0.2f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(specularColor, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(emissiveColor, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(shininess, (0.2f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(transparency, (0.0f));
}

/*!
  Destructor.
*/
SoVRMLMaterial::~SoVRMLMaterial()
{
}

// Doc in parent
void
SoVRMLMaterial::doAction(SoAction * action)
{
  SoState * state = action->getState();

  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!this->diffuseColor.isIgnored() &&
      !TEST_OVERRIDE(AMBIENT_COLOR)) {
    this->tmpambient = this->diffuseColor.getValue();
    if (!this->ambientIntensity.isIgnored())
      this->tmpambient *= this->ambientIntensity.getValue();

    SoAmbientColorElement::set(state,
                               this,
                               1,
                               &this->tmpambient);
    if (this->isOverride()) {
      SoOverrideElement::setAmbientColorOverride(state, this, TRUE);
    }
  }
  if (!this->diffuseColor.isIgnored() &&
      !TEST_OVERRIDE(DIFFUSE_COLOR)) {
    // Note: the override flag bit values for diffuseColor and
    // transparency are equal (done like that to match SGI/TGS
    // Inventor behavior), so overriding one will also override the
    // other.
    SoDiffuseColorElement::set(action->getState(),
                               this,
                               1,
                               &this->diffuseColor.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
  if (!this->emissiveColor.isIgnored() &&
      !TEST_OVERRIDE(EMISSIVE_COLOR)) {
    SoEmissiveColorElement::set(action->getState(),
                                this,
                                1,
                                &this->emissiveColor.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setEmissiveColorOverride(state, this, TRUE);
    }

  }
  if (!this->specularColor.isIgnored() &&
      !TEST_OVERRIDE(SPECULAR_COLOR)) {
    SoSpecularColorElement::set(action->getState(),
                                this,
                                1,
                                &this->specularColor.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setSpecularColorOverride(state, this, TRUE);
    }
  }
  if (!this->shininess.isIgnored() &&
      !TEST_OVERRIDE(SHININESS)) {
    this->tmpshininess = this->shininess.getValue();
    SoShininessElement::set(action->getState(),
                            this,
                            1,
                            &this->tmpshininess);
    if (this->isOverride()) {
      SoOverrideElement::setShininessOverride(state, this, TRUE);
    }
  }
  if (!this->transparency.isIgnored() &&
      !TEST_OVERRIDE(TRANSPARENCY)) {
    this->tmptransparency = this->transparency.getValue();
    // Note: the override flag bit values for diffuseColor and
    // transparency are equal (done like that to match SGI/TGS
    // Inventor behavior), so overriding one will also override the
    // other.
    SoTransparencyElement::set(action->getState(),
                               this,
                               1,
                               &this->tmptransparency);
    if (this->isOverride()) {
      SoOverrideElement::setTransparencyOverride(state, this, TRUE);
    }
  }
#undef TEST_OVERRIDE
}

// Doc in parent
void
SoVRMLMaterial::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  if (SoShapeStyleElement::isScreenDoor(state) &&
      ! this->transparency.isIgnored() &&
      ! SoOverrideElement::getTransparencyOverride(state)) {
    float t = this->transparency.getValue();
    SoGLPolygonStippleElement::setTransparency(state, t);
    SoGLPolygonStippleElement::set(state, t >= 1.0f/255.0f);
  }
  SoVRMLMaterial::doAction(action);
}

// Doc in parent
void
SoVRMLMaterial::callback(SoCallbackAction * action)
{
  SoVRMLMaterial::doAction(action);
}


