/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoMaterial SoMaterial.h Inventor/nodes/SoMaterial.h
  \brief The SoMaterial class is a node type for setting up material values for scene geometry.
  \ingroup nodes

  After traversing an SoMaterial node, subsequent shape nodes with
  geometry in the scene graph will use values from the material "pool"
  of the traversal state set up from nodes of this type.

  Note that values from a material node will \e replace the previous
  values from the traversal state, they will \e not accumulate.

  FIXME: usage example here. 20020118 mortene.

  FIXME: describe what happens if the number of values in the fields
  is not consistent. 20020119 mortene.

  FIXME: refer to the OpenGL color model. 20020119 mortene.



  Note that nodes imported as part of a VRML V1.0 file has a special
  case, where the fields SoMaterial::ambientColor,
  SoMaterial::diffuseColor and SoMaterial::specularColor contains zero
  values, and SoMaterial::emissiveColor contains one or more
  values. The values in SoMaterial::emissiveColor should then be
  treated as precalculated lighting, and the other fields should be
  ignored.

  You can detect this case by checking the values of the material
  elements when the scene graph is traversed using an
  SoCallbackAction. SoDiffuseColorElement, SoAmbientColorElement, and
  SoSpecularColorElement will contain one value with a completely
  black color (0.0f, 0.0f, 0.0f), SoShininessElement will contain one
  value of 0.0f, and SoEmissiveColorElement will contain one or more
  values. It is done like this to make rendering work correctly on
  systems that do not test for this specific case.

  You should only check for this case when you're traversing a VRML
  V1.0 file scene graph, of course. See SoNode::getNodeType() for
  information about how nodes can be tested for whether or not they
  have been imported or otherwise set up as of VRML1 type versus
  Inventor type.

  When the scene graph is rendered using an SoGLRenderAction, the
  elements will be set differently to optimize rendering.  The
  SoDiffuseColorElement will be set to the values in
  SoMaterial::emissiveColor, and the light model will be set to
  SoLightModel::BASE_COLOR.

  The SoMaterial::transparency values will always be treated normally.

  \sa SoMaterialBinding, SoBaseColor, SoPackedColor
*/

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoSpecularColorElement.h>
#include <Inventor/elements/SoEmissiveColorElement.h>
#include <Inventor/elements/SoShininessElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/C/tidbits.h> // coin_atexit()
#include <stdlib.h>

/*!
  \var SoMFColor SoMaterial::ambientColor

  Ambient material part color values. Will by default contain a single
  color value of [0.2, 0.2, 0.2] (ie dark gray).

  The ambient part of the material is not influenced by any
  lightsources, and should be thought of conceptually as the constant,
  but small contribution of light to a scene "seeping in" from
  everywhere.

  (Think of the ambient contribution in the context that there's
  always photons fizzing around everywhere -- even in a black,
  lightsource-less room, for instance).

  \sa SoEnvironment::ambientIntensity
*/
/*!
  \var SoMFColor SoMaterial::diffuseColor

  Diffuse material part color values. This field is by default
  initialized to contain a single color value of [0.8, 0.8, 0.8]
  (light gray).

  The diffuse part is combined with the light emitted from the scene's
  light sources.
*/
/*!
  \var SoMFColor SoMaterial::specularColor

  Specular material part color values. Defaults to a single color
  value of [0, 0, 0] (black).
*/
/*!
  \var SoMFColor SoMaterial::emissiveColor

  The color of the light "emitted" by the subsequent geometry,
  independent of lighting / shading.

  Defaults to contain a single color value of [0, 0, 0] (black, ie no
  contribution).
*/
/*!
  \var SoMFFloat SoMaterial::shininess

  Shininess values. Decides how the light from light sources are
  distributed across the geometry surfaces. Valid range is from 0.0
  (which gives a dim appearance), to 1.0 (glossy-looking surfaces).

  Defaults to contain a single value of 0.2.
*/
/*!
  \var SoMFFloat SoMaterial::transparency

  Transparency values. Valid range is from 0.0 (completely opaque,
  which is the default) to 1.0 (completely transparent,
  i.e. invisible).

  Defaults to contain a single value of 0.0.
*/

// defines for materialtype
#define TYPE_UNKNOWN            0
#define TYPE_NORMAL             1
#define TYPE_VRML1_ONLYEMISSIVE 2 // special case in vrml1

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

class SoMaterialP {
public:
  int materialtype;
  int transparencyflag;
  SoColorPacker colorpacker;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

SO_NODE_SOURCE(SoMaterial);

/*!
  Constructor.
*/
SoMaterial::SoMaterial(void)
{
  THIS = new SoMaterialP;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoMaterial);

  SO_NODE_ADD_FIELD(ambientColor, (0.2f, 0.2f, 0.2f));
  SO_NODE_ADD_FIELD(diffuseColor, (0.8f, 0.8f, 0.8f));
  SO_NODE_ADD_FIELD(specularColor, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(emissiveColor, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(shininess, (0.2f));
  SO_NODE_ADD_FIELD(transparency, (0.0f));
  
  THIS->materialtype = TYPE_NORMAL;
  THIS->transparencyflag = FALSE; // we know it's not transparent
}

/*!
  Destructor.
*/
SoMaterial::~SoMaterial()
{
  delete THIS;
}

// Doc from superclass.
void
SoMaterial::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoMaterial, SO_FROM_INVENTOR_1|SoNode::VRML1);

  SO_ENABLE(SoGLRenderAction, SoGLLazyElement);
  SO_ENABLE(SoCallbackAction, SoLazyElement);

  SO_ENABLE(SoCallbackAction, SoAmbientColorElement);
  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
  SO_ENABLE(SoCallbackAction, SoEmissiveColorElement);
  SO_ENABLE(SoCallbackAction, SoSpecularColorElement);
  SO_ENABLE(SoCallbackAction, SoShininessElement);
  SO_ENABLE(SoCallbackAction, SoTransparencyElement);

  SO_ENABLE(SoGLRenderAction, SoAmbientColorElement);
  SO_ENABLE(SoGLRenderAction, SoDiffuseColorElement);
  SO_ENABLE(SoGLRenderAction, SoEmissiveColorElement);
  SO_ENABLE(SoGLRenderAction, SoSpecularColorElement);
  SO_ENABLE(SoGLRenderAction, SoShininessElement);
  SO_ENABLE(SoGLRenderAction, SoTransparencyElement);
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
  SbBool istransparent = FALSE;

  SoState * state = action->getState();

  uint32_t bitmask = 0;
  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!this->ambientColor.isIgnored() && this->ambientColor.getNum() &&
      !TEST_OVERRIDE(AMBIENT_COLOR)) {
    bitmask |= SoLazyElement::AMBIENT_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setAmbientColorOverride(state, this, TRUE);
    }
  }
  if (!this->diffuseColor.isIgnored() && this->diffuseColor.getNum() &&
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
  if (!this->emissiveColor.isIgnored() && this->emissiveColor.getNum() &&
      !TEST_OVERRIDE(EMISSIVE_COLOR)) {
    bitmask |= SoLazyElement::EMISSIVE_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setEmissiveColorOverride(state, this, TRUE);
    }
  }
  if (!this->specularColor.isIgnored() && this->specularColor.getNum() &&
      !TEST_OVERRIDE(SPECULAR_COLOR)) {
    bitmask |= SoLazyElement::SPECULAR_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setSpecularColorOverride(state, this, TRUE);
    }
  }
  if (!this->shininess.isIgnored() && this->shininess.getNum() &&
      !TEST_OVERRIDE(SHININESS)) {
    bitmask |= SoLazyElement::SHININESS_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setShininessOverride(state, this, TRUE);
    }
  }
  if (!this->transparency.isIgnored() && this->transparency.getNum() &&
      !TEST_OVERRIDE(TRANSPARENCY)) {
    // Note: the override flag bit values for diffuseColor and
    // transparency are equal (done like that to match SGI/TGS
    // Inventor behavior), so overriding one will also override the
    // other.
    bitmask |= SoLazyElement::TRANSPARENCY_MASK;
    if (this->isOverride()) {
      SoOverrideElement::setTransparencyOverride(state, this, TRUE);
    }
    // if we don't know if material is transparent, run through all
    // values and test
    if (THIS->transparencyflag < 0) {
      int i, n = this->transparency.getNum();
      const float * p = this->transparency.getValues(0);
      for (i = 0; i < n; i++) {
        if (p[i] > 0.0f) {
          istransparent = TRUE;
          break;
        }
      }
      // we now know whether material is transparent or not
      THIS->transparencyflag = (int) istransparent;
    }
    istransparent = (SbBool) THIS->transparencyflag;
  }
#undef TEST_OVERRIDE

  if (bitmask) {
    SbColor dummycolor(0.8f, 0.8f, 0.0f);
    float dummyval = 0.2f;
    SoLazyElement::setMaterials(state, this, bitmask, 
                                &THIS->colorpacker,
                                this->diffuseColor.getValues(0), this->diffuseColor.getNum(), 
                                this->transparency.getValues(0), this->transparency.getNum(),
                                bitmask & SoLazyElement::AMBIENT_MASK ? 
                                this->ambientColor[0] : dummycolor,
                                bitmask & SoLazyElement::EMISSIVE_MASK ?
                                this->emissiveColor[0] : dummycolor,
                                bitmask & SoLazyElement::SPECULAR_MASK ?
                                this->specularColor[0] : dummycolor,
                                bitmask & SoLazyElement::SHININESS_MASK ?
                                this->shininess[0] : dummyval,
                                istransparent);
  }
}

// Doc from superclass.
void
SoMaterial::callback(SoCallbackAction * action)
{
  SoMaterial::doAction(action);
}

void
SoMaterial::notify(SoNotList *list)
{
  SoField * f = list->getLastField();
  if (f) THIS->materialtype = TYPE_UNKNOWN;
  if (f == &this->transparency) {
    THIS->transparencyflag = -1; // unknown
  }
  inherited::notify(list);
}

//
// to test for special vrml1 case. It's not used right now,
// but it might be enabled again later. pederb, 2002-09-11
//
int 
SoMaterial::getMaterialType(void)
{
  if (this->getNodeType() != SoNode::VRML1) return TYPE_NORMAL;
  else {
    if (THIS->materialtype == TYPE_UNKNOWN) {
      if (!this->diffuseColor.isIgnored() && this->diffuseColor.getNum() == 0 &&
          !this->ambientColor.isIgnored() && this->ambientColor.getNum() == 0 &&
          !this->specularColor.isIgnored() && this->specularColor.getNum() == 0 &&
          !this->emissiveColor.isIgnored() && this->emissiveColor.getNum()) {
        THIS->materialtype = TYPE_VRML1_ONLYEMISSIVE;
      }
      else {
        THIS->materialtype = TYPE_NORMAL;
      }
    }
    return THIS->materialtype;
  }
}

#undef THIS
#undef TYPE_UNKNOWN
#undef TYPE_NORMAL
#undef TYPE_ONLYEMISSIVE
