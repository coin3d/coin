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
  \class SoLazyElement Inventor/elements/SoLazyElement.h
  \brief The SoLazyElement class is a very stupid class..
  \ingroup elements

*/

#include <Inventor/elements/SoLazyElement.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>
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

// helper functions to handle default diffuse/transp values
static uint32_t 
get_diffuse_node_id(SoNode * node, const int numdiffuse,
                    const SbColor * color)
{
  if (numdiffuse == 1 && color[0] == SbColor(0.8f, 0.8f, 0.8f)) return 0;
  return node->getNodeId();
}

static uint32_t 
get_transp_node_id(SoNode * node, const int numtransp,
                   const float * transp)
{
  if (numtransp == 1 && transp[0] == 0.0f) return 0;
  return node->getNodeId();
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
  this->coinstate.istransparent = FALSE;  
  this->coinstate.transptype= (int32_t) SoGLRenderAction::SCREEN_DOOR;
  this->coinstate.diffusenodeid = 0;
  this->coinstate.transpnodeid = 0;
  this->coinstate.stipplenum = 0;
  this->coinstate.vertexordering = CCW;
  this->coinstate.twoside = FALSE;
  this->coinstate.culling = FALSE;
  this->coinstate.flatshading = FALSE;
  this->coinstate.glimageid = 0;
  this->coinstate.glimageusealphatest = FALSE;
  this->coinstate.alphatest = FALSE;
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
  if (numcolors && (elem->coinstate.diffusenodeid != 
                    get_diffuse_node_id(node, numcolors, colors))) {
    elem = getWInstance(state);
    elem->setDiffuseElt(node, numcolors, colors, packer);
    if (state->isCacheOpen()) elem->lazyDidSet(DIFFUSE_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(DIFFUSE_MASK);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setTransparency(SoState *state, SoNode *node, int32_t numvalues,
                               const float * transparency, SoColorPacker * packer)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (numvalues && (elem->coinstate.transpnodeid != 
                    get_transp_node_id(node, numvalues, transparency))) {
    elem = getWInstance(state);
    elem->setTranspElt(node, numvalues, transparency, packer);
    if (state->isCacheOpen()) elem->lazyDidSet(TRANSPARENCY_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(TRANSPARENCY_MASK);
  }
  SoLazyElement::setAlphaTest(state, 
                              !elem->coinstate.istransparent && 
                              elem->coinstate.glimageusealphatest);
}

// ! FIXME: write doc

void
SoLazyElement::setPacked(SoState * state, SoNode * node,
                         int32_t numcolors, const uint32_t * colors,
                         const SbBool packedtransparency)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (numcolors && elem->coinstate.diffusenodeid != node->getNodeId()) {
    elem = getWInstance(state);
    elem->setPackedElt(node, numcolors, colors, packedtransparency);
    if (state->isCacheOpen()) elem->lazyDidSet(TRANSPARENCY_MASK|DIFFUSE_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(TRANSPARENCY_MASK|DIFFUSE_MASK);
  }
  SoLazyElement::setAlphaTest(state, 
                              !elem->coinstate.istransparent && 
                              elem->coinstate.glimageusealphatest);
}

// ! FIXME: write doc

void
SoLazyElement::setColorIndices(SoState *state, SoNode *node,
                               int32_t numindices, const int32_t * indices)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (numindices && elem->coinstate.diffusenodeid != node->getNodeId()) {
    elem = getWInstance(state);
    elem->setColorIndexElt(node, numindices, indices);
    if (state->isCacheOpen()) elem->lazyDidSet(DIFFUSE_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(DIFFUSE_MASK);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setAmbient(SoState *state, const SbColor* color)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.ambient != *color) {
    elem = getWInstance(state);
    elem->setAmbientElt(color);
    if (state->isCacheOpen()) elem->lazyDidSet(AMBIENT_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(AMBIENT_MASK);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setEmissive(SoState *state, const SbColor* color)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.emissive != *color) {
    elem = getWInstance(state);
    elem->setEmissiveElt(color);
    if (state->isCacheOpen()) elem->lazyDidSet(EMISSIVE_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(EMISSIVE_MASK);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setSpecular(SoState *state, const SbColor* color)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.specular != *color) {
    elem = getWInstance(state);
    elem->setSpecularElt(color);
    if (state->isCacheOpen()) elem->lazyDidSet(SPECULAR_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(SPECULAR_MASK);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setShininess(SoState *state, float value)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (SbAbs(elem->coinstate.shininess - value) > SO_LAZY_SHINY_THRESHOLD) {
    elem = getWInstance(state);
    elem->setShininessElt(value);
    if (state->isCacheOpen()) elem->lazyDidSet(SHININESS_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(SHININESS_MASK);
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
    elem = getWInstance(state);
    elem->setBlendingElt(value);
    if (state->isCacheOpen()) elem->lazyDidSet(BLENDING_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(BLENDING_MASK);
  }
}

// ! FIXME: write doc

void
SoLazyElement::setLightModel(SoState * state, const int32_t model)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.lightmodel != model) {
    elem = getWInstance(state);
    elem->setLightModelElt(state, model);
    if (state->isCacheOpen()) elem->lazyDidSet(LIGHT_MODEL_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(LIGHT_MODEL_MASK);
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
  return this->coinstate.istransparent;
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
                            const float shininess,
                            const SbBool istransparent)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);

  uint32_t eltbitmask = 0;
  if (bitmask & DIFFUSE_MASK) {
    if (elem->coinstate.diffusenodeid != 
        get_diffuse_node_id(node, numdiffuse, diffuse)) {
      eltbitmask |= DIFFUSE_MASK;
    }
  }
  if (bitmask & TRANSPARENCY_MASK) {
    if (elem->coinstate.transpnodeid != get_transp_node_id(node, numtransp, transp)) {
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

  SoLazyElement * welem = NULL;

  if (eltbitmask) {
    welem = getWInstance(state);
    welem->setMaterialElt(node, eltbitmask, packer, diffuse,
                          numdiffuse, transp, numtransp,
                          ambient, emissive, specular, shininess,
                          istransparent);
    if (state->isCacheOpen()) welem->lazyDidSet(eltbitmask);
  }
  
  if ((eltbitmask != bitmask) && state->isCacheOpen()) {
    if (welem) elem = welem;
    elem->lazyDidntSet((~eltbitmask) & bitmask);
  }
  SoLazyElement::setAlphaTest(state, 
                              !elem->coinstate.istransparent && 
                              elem->coinstate.glimageusealphatest);
}

void 
SoLazyElement::setVertexOrdering(SoState * state, VertexOrdering ordering)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.vertexordering != ordering) {
    elem = getWInstance(state);
    elem->setVertexOrderingElt(ordering);
    if (state->isCacheOpen()) elem->lazyDidSet(VERTEXORDERING_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(VERTEXORDERING_MASK);
  }
}

void 
SoLazyElement::setBackfaceCulling(SoState * state, SbBool onoff)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.culling != onoff) {
    elem = getWInstance(state);
    elem->setBackfaceCullingElt(onoff);
    if (state->isCacheOpen()) elem->lazyDidSet(CULLING_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(CULLING_MASK);
  }
}

void 
SoLazyElement::setTwosideLighting(SoState * state, SbBool onoff)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.twoside != onoff) {
    elem = getWInstance(state);
    elem->setTwosideLightingElt(onoff);
    if (state->isCacheOpen()) elem->lazyDidSet(TWOSIDE_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(TWOSIDE_MASK);
  }
}

void 
SoLazyElement::setShadeModel(SoState * state, SbBool flatshading)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  
  if (elem->coinstate.flatshading != flatshading) {
    elem = getWInstance(state);
    elem->setShadeModelElt(flatshading);
    if (state->isCacheOpen()) elem->lazyDidSet(SHADE_MODEL_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(SHADE_MODEL_MASK);
  }
}

void
SoLazyElement::setGLImageId(SoState * state, uint32_t glimageid, SbBool alphatest)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.glimageid != glimageid) {
    elem = getWInstance(state);
    elem->setGLImageIdElt(glimageid, alphatest);
    if (state->isCacheOpen()) elem->lazyDidSet(GLIMAGE_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(GLIMAGE_MASK);
  }
  SoLazyElement::setAlphaTest(state, !elem->coinstate.istransparent && alphatest);   
}

void 
SoLazyElement::setAlphaTest(SoState * state, SbBool onoff)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->coinstate.alphatest != onoff) {
    elem = getWInstance(state);
    elem->setAlphaTestElt(onoff);
    if (state->isCacheOpen()) elem->lazyDidSet(ALPHATEST_MASK);
  }
  else if (state->isCacheOpen()) {
    elem->lazyDidntSet(ALPHATEST_MASK);
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
  this->coinstate.diffusenodeid = get_diffuse_node_id(node, numcolors, colors);
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
  this->coinstate.istransparent = packedtransparency;

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
  this->coinstate.transpnodeid = get_transp_node_id(node, numtransp, transp);
  this->coinstate.transparray = transp;
  this->coinstate.numtransp = numtransp;
  this->coinstate.stipplenum = SbClamp((int) (transp[0] * 64.0f), 0, 64);

  this->coinstate.istransparent = FALSE;
  for (int i = 0; i < numtransp; i++) {
    if (transp[i] > 0.0f) {
      this->coinstate.istransparent = TRUE;
      break;
    } 
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
                              const float shininess,
                              const SbBool istransparent)
{
  if (bitmask & DIFFUSE_MASK) {
    this->coinstate.diffusenodeid = get_diffuse_node_id(node, numdiffuse, diffuse);
    this->coinstate.diffusearray = diffuse;
    this->coinstate.numdiffuse = numdiffuse;
    this->coinstate.packeddiffuse = FALSE;
  }
  if (bitmask & TRANSPARENCY_MASK) {
    this->coinstate.transpnodeid = get_transp_node_id(node, numtransp, transp);
    this->coinstate.transparray = transp;
    this->coinstate.numtransp = numtransp;
    this->coinstate.stipplenum = SbClamp((int) (transp[0] * 64.0f), 0, 64);
    // check for common case
    if (numtransp == 1 && transp[0] == 0.0f) {
      this->coinstate.transpnodeid = 0;
      this->coinstate.istransparent = FALSE;
    }
    else {
      this->coinstate.istransparent = istransparent;
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

void
SoLazyElement::setGLImageIdElt(uint32_t glimageid, SbBool alphatest)
{
  this->coinstate.glimageid = glimageid;
  this->coinstate.glimageusealphatest = alphatest;
}

void 
SoLazyElement::setAlphaTestElt(const SbBool onoff)
{
  this->coinstate.alphatest = onoff;
}


// SoColorPacker class. FIXME: move to separate file and document, pederb, 2002-09-09

static uint32_t colorpacker_default = 0xccccccff;

SoColorPacker::SoColorPacker(void)
{
  this->array = &colorpacker_default;
  this->arraysize = 0;
  this->diffuseid = 0;
  this->transpid = 0;
}

SoColorPacker::~SoColorPacker()
{
  if (this->array != &colorpacker_default) {
    delete[] this->array;
  }
}

void 
SoColorPacker::reallocate(const int32_t size)
{
  assert(size > this->arraysize);
  uint32_t * newarray = new uint32_t[size];
  if (this->array != &colorpacker_default) {
    delete[] this->array;
  }
  this->array = newarray;
  this->arraysize = size;
}

void 
SoLazyElement::lazyDidSet(uint32_t mask)
{
}

void 
SoLazyElement::lazyDidntSet(uint32_t mask)
{
}





