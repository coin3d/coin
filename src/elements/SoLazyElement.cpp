/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoLazyElement Inventor/elements/SoLazyElement.h
  \brief The SoLazyElement class is a very stupid class..
  \ingroup elements

*/

#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFColor.h>
#include "../tidbits.h"
#include <assert.h>
#include <string.h>


static SbColor * lazy_defaultdiffuse = NULL;
static float * lazy_defaulttransp = NULL;
static int32_t * lazy_defaultindex = NULL;
static uint32_t * lazy_defaultpacked = NULL;
static SbColor * lazy_unpacked = NULL;

static void
lazyelement_cleanup(void)
{
  delete lazy_defaultdiffuse;
  delete lazy_defaulttransp;
  delete lazy_defaultindex;
  delete lazy_defaultpacked;
  delete lazy_unpacked;
}


SO_ELEMENT_SOURCE(SoLazyElement);

/*!
  This static method initializes static data for the
  SoDiffuseColorElement class.
*/

void
SoLazyElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoLazyElement, inherited);

  if (lazy_defaultdiffuse == NULL) {
    lazy_defaultdiffuse = new SbColor;
    lazy_defaulttransp = new float;
    lazy_defaultindex = new int32_t;
    lazy_defaultpacked = new uint32_t;
    lazy_unpacked = new SbColor;

    *lazy_defaultdiffuse = getDefaultDiffuse();
    *lazy_defaulttransp = getDefaultTransparency();
    *lazy_defaultindex = getDefaultColorIndex();
    *lazy_defaultpacked = getDefaultPacked();

    coin_atexit((coin_atexit_f*) lazyelement_cleanup);
  }
}

// ! FIXME: write doc

SoLazyElement::~SoLazyElement()
{
}

// ! FIXME: write doc

void
SoLazyElement::init(SoState * state)
{
  this->coinstate.ambient = this->getDefaultAmbient();
  this->coinstate.specular = this->getDefaultSpecular();
  this->coinstate.emissive = this->getDefaultEmissive();
  this->coinstate.shininess = this->getDefaultShininess();
  this->coinstate.blending = FALSE;
  this->coinstate.lightmodel = PHONG;
  this->coinstate.packeddiffuse = FALSE;
  this->coinstate.numdiffuse = 1;
  this->coinstate.numtransp = 1;
  this->coinstate.diffusearray = lazy_defaultdiffuse;
  this->coinstate.packedarray = lazy_defaultpacked;
  this->coinstate.transparray = lazy_defaulttransp;
  this->coinstate.colorindexarray = lazy_defaultindex;
  this->coinstate.packedtransparency = FALSE;  
  this->coinstate.transptype= (int32_t) SoGLRenderAction::SCREEN_DOOR;
  this->coinstate.diffusenodeid = 0;
  this->coinstate.transpnodeid = 0;
  this->coinstate.stipplenum = 0;
  this->coinstate.vertexordering = CCW;
  this->coinstate.twoside = FALSE;
  this->coinstate.culling = FALSE;
  this->coinstate.flatshading = FALSE;
}

// ! FIXME: write doc

void
SoLazyElement::push(SoState *state)
{
  inherited::push(state);
  SoLazyElement * prev = (SoLazyElement*) this->getNextInStack();
  this->coinstate = prev->coinstate;
}


/*!
  Will always return TRUE in Coin.
*/
SbBool
SoLazyElement::matches(const SoElement * element) const
{
  assert(0 && "should never happen");
  return TRUE;
}

/*! 
  Just returns NULL in Coin.
*/
SoElement *
SoLazyElement::copyMatchInfo(void) const
{
  assert(0 && "should never happen");
  return NULL;
}

// ! FIXME: write doc

void
SoLazyElement::setDiffuse(SoState * state, SoNode * node, int32_t numcolors,
                          const SbColor * colors, SoColorPacker * packer)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (numcolors && elem->coinstate.diffusenodeid != node->getNodeId()) {
    getWInstance(state)->setDiffuseElt(node, numcolors, colors, packer);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setTransparency(SoState *state, SoNode *node, int32_t numvalues,
                               const float * transparency, SoColorPacker * packer)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (numvalues && elem->coinstate.transpnodeid != node->getNodeId()) {
    getWInstance(state)->setTranspElt(node, numvalues, transparency, packer);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setPacked(SoState * state, SoNode * node,
                         int32_t numcolors, const uint32_t * colors,
                         const SbBool packedtransparency)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (numcolors && elem->coinstate.diffusenodeid != node->getNodeId()) {
    getWInstance(state)->setPackedElt(node, numcolors, colors, packedtransparency);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setColorIndices(SoState *state, SoNode *node,
                               int32_t numindices, const int32_t * indices)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (numindices && elem->coinstate.diffusenodeid != node->getNodeId()) {
    getWInstance(state)->setColorIndexElt(node, numindices, indices);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setAmbient(SoState *state, const SbColor* color)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.ambient != *color) {
    getWInstance(state)->setAmbientElt(color);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setEmissive(SoState *state, const SbColor* color)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.emissive != *color) {
    getWInstance(state)->setEmissiveElt(color);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setSpecular(SoState *state, const SbColor* color)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.specular != *color) {
    getWInstance(state)->setSpecularElt(color);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setShininess(SoState *state, float value)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (SbAbs(elem->coinstate.shininess - value) > SO_LAZY_SHINY_THRESHOLD) {
    getWInstance(state)->setShininessElt(value);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setColorMaterial(SoState *state, SbBool value)
{
}

// ! FIXME: write doc

void
SoLazyElement::setBlending(SoState *state,  SbBool value)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.blending != value) {
    getWInstance(state)->setBlendingElt(value);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setLightModel(SoState * state, const int32_t model)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.lightmodel != model) {
    getWInstance(state)->setLightModelElt(state, model);
  }
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getDiffuse(SoState * state, int index)
{
  SoLazyElement * elem = getInstance(state);
  if (elem->coinstate.packeddiffuse) {
    float dummy;
    return lazy_unpacked->setPackedValue(elem->coinstate.packedarray[index], dummy);
  }
  return elem->coinstate.diffusearray[index];
}

// ! FIXME: write doc

float
SoLazyElement::getTransparency(SoState *state, int index)
{
  SoLazyElement * elem = getInstance(state);
  if (elem->coinstate.packeddiffuse) {
    float transp;
    SbColor dummy;
    dummy.setPackedValue(elem->coinstate.packedarray[index], transp);
    return transp;
  }
  return elem->coinstate.transparray[index];
}

// ! FIXME: write doc

const uint32_t *
SoLazyElement::getPackedColors(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.packedarray;
}

// ! FIXME: write doc

const int32_t *
SoLazyElement::getColorIndices(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.colorindexarray;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getColorIndex(SoState * state, int num)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.colorindexarray[num];
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getAmbient(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.ambient;
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getEmissive(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.emissive;
}

// ! FIXME: write doc

const SbColor &
SoLazyElement::getSpecular(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.specular;
}

// ! FIXME: write doc

float
SoLazyElement::getShininess(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.shininess;
}

// ! FIXME: write doc

SbBool
SoLazyElement::getColorMaterial(SoState * state)
{
  return TRUE;
}

// ! FIXME: write doc

SbBool
SoLazyElement::getBlending(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.blending;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getLightModel(SoState * state)
{
  SoLazyElement * elem = getInstance(state);
  return elem->coinstate.lightmodel;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getNumDiffuse(void) const
{
  return this->coinstate.numdiffuse;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getNumTransparencies(void) const
{
  if (this->coinstate.packeddiffuse) {
    return this->coinstate.numdiffuse;
  }
  return this->coinstate.numtransp;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getNumColorIndices(void) const
{
  return this->coinstate.numdiffuse;
}

// ! FIXME: write doc

SbBool
SoLazyElement::isPacked(void) const
{
  return this->coinstate.packeddiffuse;
}

// ! FIXME: write doc

SbBool
SoLazyElement::isTransparent(void) const
{
  if (this->coinstate.packeddiffuse) {
    return this->coinstate.packedtransparency;
  }
  const float * t = this->coinstate.transparray;
  const int n = this->coinstate.numtransp;
  for (int i = 0; i < n; i++) {
    if (t[i] != 0.0f) {
      return TRUE;
    }
  }
  return FALSE;
}

// ! FIXME: write doc

SoLazyElement *
SoLazyElement::getInstance(SoState *state)
{
  return (SoLazyElement*) state->getElementNoPush(classStackIndex);
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
  return (int32_t) SoLazyElement::PHONG;
}

// ! FIXME: write doc

int32_t
SoLazyElement::getDefaultColorIndex(void)
{
  return 0;
}

// ! FIXME: write doc

void 
SoLazyElement::setMaterials(SoState * state, SoNode *node, uint32_t bitmask,
                            SoColorPacker * packer,
                            const SbColor * diffuse, 
                            const int numdiffuse,
                            const float * transp,
                            const int numtransp,
                            const SbColor & ambient,
                            const SbColor & emissive,
                            const SbColor & specular,
                            const float shininess)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);

  uint32_t eltbitmask = 0;
  if (bitmask & DIFFUSE_MASK) {
    if (elem->coinstate.diffusenodeid != node->getNodeId()) {
      eltbitmask |= DIFFUSE_MASK;
    }
  }
  if (bitmask & TRANSPARENCY_MASK) {
    if (elem->coinstate.transpnodeid != node->getNodeId()) {
      eltbitmask |= TRANSPARENCY_MASK;
    }
  }
  if (bitmask & AMBIENT_MASK) {
    if (elem->coinstate.ambient != ambient) {
      eltbitmask |= AMBIENT_MASK;
    }
  }
  if (bitmask & EMISSIVE_MASK) {
    if (elem->coinstate.emissive != emissive) {
      eltbitmask |= EMISSIVE_MASK;
    }
  }
  if (bitmask & SPECULAR_MASK) {
    if (elem->coinstate.specular != specular) {
      eltbitmask |= SPECULAR_MASK;
    }
  }
  if (bitmask & SHININESS_MASK) {
    if (SbAbs(elem->coinstate.shininess-shininess) > SO_LAZY_SHINY_THRESHOLD) {
      eltbitmask |= SHININESS_MASK;
    }
  }
  if (eltbitmask) {
    getWInstance(state)->setMaterialElt(node, eltbitmask, packer, diffuse,
                                        numdiffuse, transp, numtransp,
                                        ambient, emissive, specular, shininess);
  }
}

void 
SoLazyElement::setVertexOrdering(SoState * state, VertexOrdering ordering)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.vertexordering != ordering) {
    getWInstance(state)->setVertexOrderingElt(ordering);
  }
}

void 
SoLazyElement::setBackfaceCulling(SoState * state, SbBool onoff)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.culling != onoff) {
    getWInstance(state)->setBackfaceCullingElt(onoff);
  }
}

void 
SoLazyElement::setTwosideLighting(SoState * state, SbBool onoff)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.twoside != onoff) {
    getWInstance(state)->setTwosideLightingElt(onoff);
  }
}

void 
SoLazyElement::setShadeModel(SoState * state, SbBool flatshading)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  
  if (elem->coinstate.flatshading != flatshading) {
    getWInstance(state)->setShadeModelElt(flatshading);
  }
}

// ! FIXME: write doc

SoLazyElement *
SoLazyElement::getWInstance(SoState * state)
{
  // don't use SoElement::getConstElement() as this will cause
  // cache dependencies.
  return (SoLazyElement*) state->getElement(classStackIndex);
}

// ! FIXME: write doc

const uint32_t *
SoLazyElement::getPackedPointer(void) const
{
  return NULL;
}

// ! FIXME: write doc

const SbColor *
SoLazyElement::getDiffusePointer(void) const
{
  return NULL;
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
  return NULL;
}

// ! FIXME: write doc

void
SoLazyElement::setTransparencyType(SoState *state, int32_t type)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.transptype != type) {
    getWInstance(state)->setTranspTypeElt(type);
  }
}


void 
SoLazyElement::setDiffuseElt(SoNode * node,  int32_t numcolors, 
                             const SbColor * colors, SoColorPacker * packer)
{
  this->coinstate.diffusenodeid = node->getNodeId();
  this->coinstate.diffusearray = colors;
  this->coinstate.numdiffuse = numcolors;
  this->coinstate.packeddiffuse = FALSE;
}

void 
SoLazyElement::setPackedElt(SoNode * node, int32_t numcolors, 
                            const uint32_t * colors, const SbBool packedtransparency)
{
  this->coinstate.diffusenodeid = node->getNodeId();
  this->coinstate.transpnodeid = node->getNodeId();
  this->coinstate.numdiffuse = numcolors;
  this->coinstate.packedarray = colors;
  this->coinstate.packeddiffuse = TRUE;
  this->coinstate.packedtransparency = packedtransparency;

  int alpha = colors[0] & 0xff;
  float transp = float(255-alpha)/255.0f;
  this->coinstate.stipplenum = SbClamp((int) (transp * 64.0f), 0, 64);
}

void 
SoLazyElement::setColorIndexElt(SoNode * node, int32_t numindices, 
                                const int32_t * indices)
{
  this->coinstate.colorindexarray = indices;
  this->coinstate.numdiffuse = numindices;
  this->coinstate.packeddiffuse = FALSE;
}

void 
SoLazyElement::setTranspElt(SoNode * node, int32_t numtransp, 
                            const float * transp, SoColorPacker * packer)
{
  this->coinstate.transpnodeid = node->getNodeId();
  this->coinstate.transparray = transp;
  this->coinstate.numtransp = numtransp;
  this->coinstate.stipplenum = SbClamp((int) (transp[0] * 64.0f), 0, 64);

  // check for common case
  if (numtransp == 1 && transp[0] == 0.0f) {
    this->coinstate.transpnodeid = 0;
  }
}


void 
SoLazyElement::setTranspTypeElt(int32_t type)
{
  this->coinstate.transptype = type;
}

void 
SoLazyElement::setAmbientElt(const SbColor* color)
{
  this->coinstate.ambient = *color;
}

void 
SoLazyElement::setEmissiveElt(const SbColor* color)
{ 
  this->coinstate.emissive = *color;
}

void 
SoLazyElement::setSpecularElt(const SbColor* color)
{ 
  this->coinstate.specular = *color;
}

void 
SoLazyElement::setShininessElt(float value)
{ 
  this->coinstate.shininess = value;
}

void 
SoLazyElement::setColorMaterialElt(SbBool value)
{ 
}

void 
SoLazyElement::setBlendingElt(SbBool value)
{
  this->coinstate.blending = value;
}

void 
SoLazyElement::setLightModelElt(SoState * state, int32_t model)
{
  SoShapeStyleElement::setLightModel(state, model);
  this->coinstate.lightmodel = model;
}

void 
SoLazyElement::setMaterialElt(SoNode * node, uint32_t bitmask, 
                              SoColorPacker * packer, 
                              const SbColor * diffuse, const int numdiffuse,
                              const float * transp, const int numtransp,
                              const SbColor & ambient,
                              const SbColor & emissive,
                              const SbColor & specular,
                              const float shininess)
{
  if (bitmask & DIFFUSE_MASK) {
    this->coinstate.diffusenodeid = node->getNodeId();
    this->coinstate.diffusearray = diffuse;
    this->coinstate.numdiffuse = numdiffuse;
    this->coinstate.packeddiffuse = FALSE;
  }
  if (bitmask & TRANSPARENCY_MASK) {
    this->coinstate.transpnodeid = node->getNodeId();
    this->coinstate.transparray = transp;
    this->coinstate.numtransp = numtransp;
    this->coinstate.stipplenum = SbClamp((int) (transp[0] * 64.0f), 0, 64);
    // check for common case
    if (numtransp == 1 && transp[0] == 0.0f) {
      this->coinstate.transpnodeid = 0;
    }
  }
  if (bitmask & AMBIENT_MASK) {
    this->coinstate.ambient = ambient;
  }
  if (bitmask & EMISSIVE_MASK) {
    this->coinstate.emissive = emissive;
  }
  if (bitmask & SPECULAR_MASK) {
    this->coinstate.specular = specular;
  }
  if (bitmask & SHININESS_MASK) {
    this->coinstate.shininess = shininess;
  }
}

void 
SoLazyElement::setVertexOrderingElt(VertexOrdering ordering)
{
  this->coinstate.vertexordering = ordering;
}

void 
SoLazyElement::setBackfaceCullingElt(SbBool onoff)
{
  this->coinstate.culling = onoff;
}

void 
SoLazyElement::setTwosideLightingElt(SbBool onoff)
{
  this->coinstate.twoside = onoff;
}

void 
SoLazyElement::setShadeModelElt(SbBool flatshading)
{
  this->coinstate.flatshading = flatshading;
}

// SoColorPacker class. FIXME: move to separate file and document, pederb, 2002-09-09

SoColorPacker::SoColorPacker(void)
{
  this->array = NULL;
  this->arraysize = 0;
  this->diffuseid = 0;
  this->transpid = 0;
}

SoColorPacker::~SoColorPacker()
{
  delete[] this->array;
}

void 
SoColorPacker::reallocate(const int32_t size)
{
  assert(size > this->arraysize);
  uint32_t * newarray = new uint32_t[size];
  delete[] this->array;
  this->array = newarray;
  this->arraysize = size;
}





