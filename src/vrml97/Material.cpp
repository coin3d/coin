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
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim
  Material {
    exposedField SFFloat ambientIntensity  0.2         # [0,1]
    exposedField SFColor diffuseColor      0.8 0.8 0.8 # [0,1]
    exposedField SFColor emissiveColor     0 0 0       # [0,1]
    exposedField SFFloat shininess         0.2         # [0,1]
    exposedField SFColor specularColor     0 0 0       # [0,1]
    exposedField SFFloat transparency      0           # [0,1]
  }
  \endverbatim

  The Material node specifies surface material properties for
  associated geometry nodes and is used by the VRML lighting equations
  during rendering.  Subclause 4.14, Lighting model
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14>),
  contains a detailed description of the VRML lighting model
  equations.  All of the fields in the Material node range from 0.0 to
  1.0.  The fields in the Material node determine how light reflects
  off an object to create colour:

  - The ambientIntensity field specifies how much ambient light from
    light sources this surface shall reflect. Ambient light is
    omnidirectional and depends only on the number of light sources, not
    their positions with respect to the surface. Ambient colour is
    calculated as ambientIntensity × diffuseColor.
 
  - The diffuseColor field reflects all VRML light sources depending
    on the angle of the surface with respect to the light source. The more
    directly the surface faces the light, the more diffuse light reflects.
 
  - The emissiveColor field models "glowing" objects.
    This can be useful for displaying pre-lit models (where the light energy
    of the room is computed explicitly), or for displaying scientific data.
 
  - The specularColor and shininess fields determine the
    specular highlights (e.g., the shiny spots on an apple). When the
    angle from the light to the surface is close to the angle from the surface
    to the viewer, the specularColor is added to the diffuse and ambient
    colour calculations. Lower shininess values produce soft glows, while higher
    values result in sharper, smaller highlights.
 
  - The transparency field specifies how "clear" an object
    is, with 1.0 being completely transparent, and 0.0 completely opaque.  

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
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLLazyElement.h>

#include <stdlib.h>

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLMaterialP {
public:
  SbColor tmpambient;
  float tmptransparency;
  SoColorPacker colorpacker;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

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
  THIS = new SoVRMLMaterialP;

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
  delete THIS;
}

// Doc in parent
void
SoVRMLMaterial::doAction(SoAction * action)
{
  SoState * state = action->getState();

  uint32_t bitmask = 0;
  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!this->diffuseColor.isIgnored() &&
      !TEST_OVERRIDE(AMBIENT_COLOR)) {
    THIS->tmpambient = this->diffuseColor.getValue();
    if (!this->ambientIntensity.isIgnored())
      THIS->tmpambient *= this->ambientIntensity.getValue();
    bitmask |= SoLazyElement::AMBIENT_MASK;
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
    bitmask |= SoLazyElement::DIFFUSE_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
  if (!this->emissiveColor.isIgnored() &&
      !TEST_OVERRIDE(EMISSIVE_COLOR)) {

    bitmask |= SoLazyElement::EMISSIVE_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setEmissiveColorOverride(state, this, TRUE);
    }

  }
  if (!this->specularColor.isIgnored() &&
      !TEST_OVERRIDE(SPECULAR_COLOR)) {
    bitmask |= SoLazyElement::SPECULAR_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setSpecularColorOverride(state, this, TRUE);
    }
  }
  if (!this->shininess.isIgnored() &&
      !TEST_OVERRIDE(SHININESS)) {
    bitmask |= SoLazyElement::SHININESS_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setShininessOverride(state, this, TRUE);
    }
  }
  if (!this->transparency.isIgnored() &&
      !TEST_OVERRIDE(TRANSPARENCY)) {
    THIS->tmptransparency = this->transparency.getValue();
    bitmask |= SoLazyElement::TRANSPARENCY_MASK;
    // Note: the override flag bit values for diffuseColor and
    // transparency are equal (done like that to match SGI/TGS
    // Inventor behavior), so overriding one will also override the
    // other.
    if (this->isOverride()) {
      SoOverrideElement::setTransparencyOverride(state, this, TRUE);
    }
  }
#undef TEST_OVERRIDE

  if (bitmask) {
    SoLazyElement::setMaterials(state, this, bitmask,
                                &THIS->colorpacker,
                                &this->diffuseColor.getValue(), 1,
                                &THIS->tmptransparency, 1,
                                THIS->tmpambient,
                                this->emissiveColor.getValue(),
                                this->specularColor.getValue(),
                                this->shininess.getValue());
    
  }
}

// Doc in parent
void
SoVRMLMaterial::GLRender(SoGLRenderAction * action)
{
  SoVRMLMaterial::doAction(action);
}

// Doc in parent
void
SoVRMLMaterial::callback(SoCallbackAction * action)
{
  SoVRMLMaterial::doAction(action);
}

#undef THIS
