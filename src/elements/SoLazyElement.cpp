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
  \class SoLazyElement Inventor/elements/SoLazyElement.h
  \brief The SoLazyElement class is a very stupid class..

  This is just a wrap-around implementation for compatibility. It should (hopefully)
  work in the same way as the Inventor class though.
*/

//
// FIXME: cache element pointers to make this class a bit more effective
//        pederb, 20000208
//

#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/elements/SoEmissiveColorElement.h>
#include <Inventor/elements/SoSpecularColorElement.h>
#include <Inventor/elements/SoShininessElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFColor.h>
#include <coindefs.h> // COIN_STUB()
#include <assert.h>

SO_ELEMENT_SOURCE(SoLazyElement);

/*!
  This static method initializes static data for the
  SoDiffuseColorElement class.
*/

void
SoLazyElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoLazyElement, inherited);
}

// ! FIXME: write doc

SoLazyElement::SoLazyElement()
{
  setTypeId(SoLazyElement::classTypeId);
  setStackIndex(SoLazyElement::classStackIndex);
}

// ! FIXME: write doc

SoLazyElement::~SoLazyElement()
{
}

// ! FIXME: write doc

void
SoLazyElement::init(SoState *state)
{
  this->colorMaterial = TRUE;
  this->blending = FALSE;
  this->state = state;
  this->shininess = this->getDefaultShininess();
  this->transparencyType = (int32_t) SoGLRenderAction::SCREEN_DOOR;
}

// ! FIXME: write doc

void
SoLazyElement::push(SoState *state)
{
  inherited::push(state);
  SoLazyElement *elem = (SoLazyElement*) this->next;
  elem->colorMaterial = this->colorMaterial;
  elem->blending = this->blending;
  elem->state = this->state;
  elem->shininess = this->shininess;
  elem->transparencyType = this->transparencyType;
}

// ! FIXME: write doc

SbBool
SoLazyElement::matches(const SoElement *element) const
{
  SoLazyElement *elem = (SoLazyElement*) element;

  // shininess is stored in element, don't check this value
  if (elem->colorMaterial != this->colorMaterial) return FALSE;
  if (elem->blending != this->blending) return FALSE;
  if (elem->transparencyType != this->transparencyType) return FALSE;

  if (!SoLightModelElement::getInstance(this->state)->matches(elem->lightModelMatchInfo))
    return FALSE;
  if (!SoDiffuseColorElement::getInstance(this->state)->matches(elem->diffuseMatchInfo))
    return FALSE;
  if (!SoAmbientColorElement::getInstance(this->state)->matches(elem->ambientMatchInfo))
    return FALSE;
  if (!SoEmissiveColorElement::getInstance(this->state)->matches(elem->emissiveMatchInfo))
    return FALSE;
  if (!SoSpecularColorElement::getInstance(this->state)->matches(elem->specularMatchInfo))
    return FALSE;
  if (!SoShininessElement::getInstance(this->state)->matches(elem->shininessMatchInfo))
    return FALSE;
  if (!SoTransparencyElement::getInstance(this->state)->matches(elem->transparencyMatchInfo))
    return FALSE;

  return TRUE;
}

// ! FIXME: write doc

SoElement *
SoLazyElement::copyMatchInfo(void) const
{
  SoLazyElement *element = (SoLazyElement*)this->getTypeId().createInstance();

  element->lightModelMatchInfo = SoLightModelElement::getInstance(this->state)->copyMatchInfo();
  element->diffuseMatchInfo = SoDiffuseColorElement::getInstance(this->state)->copyMatchInfo();
  element->ambientMatchInfo = SoAmbientColorElement::getInstance(this->state)->copyMatchInfo();
  element->emissiveMatchInfo = SoEmissiveColorElement::getInstance(this->state)->copyMatchInfo();
  element->specularMatchInfo = SoSpecularColorElement::getInstance(this->state)->copyMatchInfo();
  element->shininessMatchInfo = SoShininessElement::getInstance(this->state)->copyMatchInfo();
  element->transparencyMatchInfo = SoTransparencyElement::getInstance(this->state)->copyMatchInfo();
  element->colorMaterial = this->colorMaterial;
  element->blending = this->blending;
  element->shininess = this->shininess;
  element->transparencyType = this->transparencyType;

  return element;
}

// ! FIXME: write doc

void
SoLazyElement::print(FILE *fp) const
{
  COIN_STUB();
}

// ! FIXME: write doc

void
SoLazyElement::setDiffuse(SoState *state, SoNode *node, int32_t numcolors,
                          const SbColor *colors, SoColorPacker *)
{
  SoDiffuseColorElement::set(state, node, numcolors, colors);
}

// ! FIXME: write doc

void
SoLazyElement::setTransparency(SoState *state, SoNode *node, int32_t numvalues,
                               const float *transparency, SoColorPacker *)
{
  SoTransparencyElement::set(state, node, numvalues, transparency);
}

// ! FIXME: write doc

void
SoLazyElement::setPacked(SoState *state, SoNode *node,
                         int32_t numcolors, const uint32_t *colors)
{
  SoDiffuseColorElement::set(state, node, numcolors, colors);
}

// ! FIXME: write doc

void
SoLazyElement::setColorIndices(SoState *state, SoNode *node,
                               int32_t numIndices, const int32_t *indices)
{
  assert(0 && "color index mode is not supported in Coin");
}

// ! FIXME: write doc

void
SoLazyElement::setAmbient(SoState *state, const SbColor* color)
{
  SoAmbientColorElement::set(state, NULL, 1, color);
}

// ! FIXME: write doc

void
SoLazyElement::setEmissive(SoState *state, const SbColor* color)
{
  SoEmissiveColorElement::set(state, NULL, 1, color);
}

// ! FIXME: write doc

void
SoLazyElement::setSpecular(SoState *state, const SbColor* color)
{
  SoSpecularColorElement::set(state, NULL, 1, color);
}

// ! FIXME: write doc

void
SoLazyElement::setShininess(SoState *state, float value)
{
  SoLazyElement *elem = (SoLazyElement*)
    SoElement::getElement(state, classStackIndex);
  elem->shininess = value;
  SoShininessElement::set(state, NULL, 1, &elem->shininess);
}

// ! FIXME: write doc

void
SoLazyElement::setColorMaterial(SoState *state, SbBool value)
{
  SoLazyElement *elem = (SoLazyElement*)
    SoElement::getElement(state, classStackIndex);
  elem->colorMaterial = value;
}

// ! FIXME: write doc

void
SoLazyElement::setBlending(SoState *state,  SbBool value)
{
  SoLazyElement *elem = (SoLazyElement*)
    SoElement::getElement(state, classStackIndex);
  elem->blending = value;
}

// ! FIXME: write doc

void
SoLazyElement::setLightModel(SoState *state, const int32_t model)
{
  SoLightModelElement::set(state, NULL, (SoLightModelElement::Model)model);
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getDiffuse(SoState* state, int index)
{
  return SoDiffuseColorElement::getInstance(state)->get(index);
}

// ! FIXME: write doc

float
SoLazyElement::getTransparency(SoState *state, int index)
{
  return SoTransparencyElement::getInstance(state)->get(index);
}

// ! FIXME: write doc

const uint32_t *
SoLazyElement::getPackedColors(SoState *state)
{
  return SoDiffuseColorElement::getInstance(state)->getPackedArrayPtr();
}

// ! FIXME: write doc

const int32_t *
SoLazyElement::getColorIndices(SoState*)
{
  assert(0 && "color index mode is not supported in Coin");
  return NULL;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getColorIndex(SoState*, int num)
{
  assert(0 && "color index mode is not supported in Coin");
  return 0;
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getAmbient(SoState *state)
{
  return SoAmbientColorElement::getInstance(state)->get(0);
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getEmissive(SoState *state)
{
  return SoEmissiveColorElement::getInstance(state)->get(0);
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getSpecular(SoState *state)
{
  return SoSpecularColorElement::getInstance(state)->get(0);
}

// ! FIXME: write doc

float
SoLazyElement::getShininess(SoState *state)
{
  return SoShininessElement::getInstance(state)->get(0);
}

// ! FIXME: write doc

SbBool
SoLazyElement::getColorMaterial(SoState *state)
{
  return SoLazyElement::getInstance(state)->colorMaterial;
}

// ! FIXME: write doc

SbBool
SoLazyElement::getBlending(SoState *state)
{
  return SoLazyElement::getInstance(state)->blending;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getLightModel(SoState *state)
{
  return (int32_t) SoLightModelElement::get(state);
}

// ! FIXME: write doc

int32_t
SoLazyElement::getNumDiffuse(void) const
{
  return SoDiffuseColorElement::getInstance(this->state)->getNum();
}

// ! FIXME: write doc

int32_t
SoLazyElement::getNumTransparencies(void) const
{
  return SoTransparencyElement::getInstance(this->state)->getNum();
}

// ! FIXME: write doc

int32_t
SoLazyElement::getNumColorIndices(void) const
{
  assert(0 && "color index mode is not supported in Coin");
  return 0;
}

// ! FIXME: write doc

SbBool
SoLazyElement::isPacked(void) const
{
  return SoDiffuseColorElement::getInstance(this->state)->isPacked();
}

// ! FIXME: write doc

SbBool
SoLazyElement::isTransparent(void) const
{
  if (this->isPacked()) {
    // FIXME: should probably check all values...
    uint32_t packed = SoLazyElement::getPackedColors(this->state)[0];
    return (packed & 0xff) != 255;
  }
  const SoTransparencyElement *elem = SoTransparencyElement::getInstance(this->state);
  if (elem->getNum() > 1) return TRUE;
  return elem->get(0) != 0.0f;
}

// ! FIXME: write doc

SoLazyElement *
SoLazyElement::getInstance(SoState *state)
{
  return (SoLazyElement*) SoElement::getConstElement(state, classStackIndex);
}

// ! FIXME: write doc

float
SoLazyElement::getDefaultAmbientIntensity(void)
{
  return 0.2f;
}

// ! FIXME: write doc
	
SbColor
SoLazyElement::getDefaultDiffuse(void)
{
  return SbColor(0.8f, 0.8f, 0.8f);
}

// ! FIXME: write doc

SbColor
SoLazyElement::getDefaultAmbient(void)
{
  return SbColor(0.2f, 0.2f, 0.2f);
}

// ! FIXME: write doc

SbColor
SoLazyElement::getDefaultSpecular(void)
{
  return SbColor(0.0f, 0.0f, 0.0f);
}

// ! FIXME: write doc

SbColor
SoLazyElement::getDefaultEmissive(void)
{
  return SbColor(0.0f, 0.0f, 0.0f);
}

// ! FIXME: write doc

float
SoLazyElement::getDefaultShininess(void)
{
  return 0.2f;
}

// ! FIXME: write doc

uint32_t
SoLazyElement::getDefaultPacked(void)
{
  return 0xccccccff;
}

// ! FIXME: write doc

float
SoLazyElement::getDefaultTransparency(void)
{
  return 0.0f;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getDefaultLightModel(void)
{
  return (int32_t) SoLightModelElement::PHONG;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getDefaultColorIndex(void)
{
  assert(0 && "color index mode is not supported in Coin");
  return 0;
}

// ! FIXME: write doc

void
SoLazyElement::setMaterials(SoState *state, SoNode *node, uint32_t bitmask,
                            SoColorPacker *cPacker,
                            const SoMFColor& diffuse, const SoMFFloat& transp,
                            const SoMFColor& ambient, const SoMFColor& emissive,
                            const SoMFColor& specular, const SoMFFloat& shininess)
{
  // FIXME: check bitmask, pederb 20000208
  SoDiffuseColorElement::set(state, node,  diffuse.getNum(), diffuse.getValues(0));
  SoAmbientColorElement::set(state, node,  ambient.getNum(), ambient.getValues(0));
  SoSpecularColorElement::set(state, node,  specular.getNum(), specular.getValues(0));
  SoEmissiveColorElement::set(state, node,  emissive.getNum(), emissive.getValues(0));
  SoTransparencyElement::set(state, node,  transp.getNum(), transp.getValues(0));
  SoShininessElement::set(state, node,  shininess.getNum(), shininess.getValues(0));
}

// ! FIXME: write doc

void
SoLazyElement::setMaterials(SoState *state, SoNode *node, uint32_t bitmask,
                            SoColorPacker *packer,
                            SoMFColor *diffuse, SoMFFloat *transp,
                            SoMFColor *ambient, SoMFColor *emissive,
                            SoMFColor *specular, SoMFFloat *shininess)
{
  SoLazyElement::setMaterials(state, node, bitmask, packer,
                              *diffuse, *transp, *ambient, *emissive, *specular, *shininess);
}

// ! FIXME: write doc

SoLazyElement *
SoLazyElement::getWInstance(SoState *state)
{
  return (SoLazyElement*) SoElement::getElement(state, classStackIndex);
}

// ! FIXME: write doc

const uint32_t *
SoLazyElement::getPackedPointer(void) const
{
  return SoDiffuseColorElement::getInstance(this->state)->getPackedArrayPtr();
}

// ! FIXME: write doc

const SbColor *
SoLazyElement::getDiffusePointer(void) const
{
  return SoDiffuseColorElement::getInstance(this->state)->getColorArrayPtr();
}

// ! FIXME: write doc

const int32_t *
SoLazyElement::getColorIndexPointer(void) const
{
  assert(0 && "color index mode is not supported in Coin");
  return NULL;
}

// ! FIXME: write doc

const float *
SoLazyElement::getTransparencyPointer(void) const
{
  return SoTransparencyElement::getInstance(this->state)->getArrayPtr();
}

// ! FIXME: write doc

void
SoLazyElement::setTransparencyType(SoState *state, int32_t type)
{
  SoLazyElement::getWInstance(state)->transparencyType = type;
}
