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
  \class SoGLLazyElement Inventor/elements/SoGLLazyElement.h
  \brief The SoGLLazyElement class is meant to optimize GL rendering.

  This is just a wrap-around implementation for compatibility. It should
  (hopefully) work in the same way as the Inventor class though.
*/

#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoGLAmbientColorElement.h>
#include <Inventor/elements/SoGLEmissiveColorElement.h>
#include <Inventor/elements/SoGLSpecularColorElement.h>
#include <Inventor/elements/SoGLShininessElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/misc/SoState.h>

#include <Inventor/bundles/SoMaterialBundle.h>
#include <coindefs.h> // COIN_STUB
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLLazyElement);

/*!
  This static method initializes static data for the
  SoDiffuseColorElement class.
*/

void
SoGLLazyElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLLazyElement, inherited);
}

// ! FIXME: write doc

SoGLLazyElement::~SoGLLazyElement()
{
}

//! FIXME: write doc

void
SoGLLazyElement::sendAllMaterial(SoState *state)
{
  // code adapted from SoMaterialBundle::reallySend

  SoGLDiffuseColorElement * diffuseElt = 
    (SoGLDiffuseColorElement*) SoDiffuseColorElement::getInstance(state);
  if (SoShapeStyleElement::isScreenDoor(state)) {
    diffuseElt->send(0);
  }
  else {
    SoTransparencyElement *transparencyElt = (SoTransparencyElement*) 
      SoTransparencyElement::getInstance(state);
    if (!diffuseElt->isPacked()) {
      float trans = transparencyElt->get(0);
      diffuseElt->send(0, 1.0f - trans);
    }
    else {
      diffuseElt->send(0);
    }
  }
  
  if (SoLightModelElement::get(state) !=
      SoLightModelElement::BASE_COLOR) {

    SoGLAmbientColorElement *ambientElt = (SoGLAmbientColorElement*)
      SoAmbientColorElement::getInstance(state);
    ambientElt->send(0);

    SoGLSpecularColorElement *specularElt = (SoGLSpecularColorElement*)
      SoSpecularColorElement::getInstance(state);
    specularElt->send(0);

    SoGLEmissiveColorElement *emissiveElt = (SoGLEmissiveColorElement*)
      SoEmissiveColorElement::getInstance(state);
    emissiveElt->send(0);

    SoGLShininessElement *shininessElt = (SoGLShininessElement*)
      SoShininessElement::getInstance(state);
    shininessElt->send(0);
  }
  
  // just to make sure polygon stipple is up-to-date
  const SoGLPolygonStippleElement *stippleElt = (const SoGLPolygonStippleElement*) 
    SoElement::getConstElement(state, SoGLPolygonStippleElement::getClassStackIndex());
  stippleElt->evaluate();
}

//! FIXME: write doc

void
SoGLLazyElement::sendNoMaterial(SoState *state)
{
  // elements should be up-to-date. Do nothing.
}

//! FIXME: write doc

void
SoGLLazyElement::sendOnlyDiffuseColor(SoState * state)
{
  SoGLDiffuseColorElement * diffuseElt = 
    (SoGLDiffuseColorElement*) SoDiffuseColorElement::getInstance(state);
  diffuseElt->send(0);
}

//! FIXME: write doc

void
SoGLLazyElement::sendDiffuseByIndex(int index) const
{
  SoGLDiffuseColorElement * elem = (SoGLDiffuseColorElement*)
    SoDiffuseColorElement::getInstance(this->state);
  elem->send(index);
}

//! FIXME: write doc

SbBool
SoGLLazyElement::isColorIndex(SoState *state)
{
  COIN_STUB();
  return FALSE;
}

//! FIXME: write doc

SoGLLazyElement *
SoGLLazyElement::getInstance(const SoState *state)
{
  // don't use SoElement::getConstElement() as this will cause
  // cache dependencies.
  return (SoGLLazyElement*) state->getConstElement(classStackIndex);
}

//! FIXME: write doc

void
SoGLLazyElement::send(const SoState * state, uint32_t mask) const
{
  // only test material flags, as other elements should be
  // updated in SoShaphe::shouldGLRender()
  if (mask == DIFFUSE_MASK) {
    SoGLLazyElement::sendOnlyDiffuseColor((SoState*)state);
  }
  else if (mask & (DIFFUSE_MASK |
                   AMBIENT_MASK |
                   EMISSIVE_MASK |
                   SPECULAR_MASK |
                   SHININESS_MASK | 
                   TRANSPARENCY_MASK)) {
    SoGLLazyElement::sendAllMaterial((SoState*)state);
  }
}

//! FIXME: write doc

void
SoGLLazyElement::sendVPPacked(SoState* state, const unsigned char* pcolor)
{
  SoGLDiffuseColorElement * elem = (SoGLDiffuseColorElement*)
    SoDiffuseColorElement::getInstance(state);
  const uint32_t rgba =
    (pcolor[0] << 24) | (pcolor[1] << 16) | (pcolor[2] << 8) | pcolor[3];
  elem->sendOnePacked(rgba);
}

//! FIXME: write doc

void
SoGLLazyElement::reset(SoState * /*state*/,  uint32_t /*mask*/) const
{
}

//! FIXME: write doc

SbBool
SoGLLazyElement::lazyMatches(uint32_t /*checkGLFlag*/, uint32_t /*checkIVFlag*/,
                             const SoGLLazyElement * /*eltInState*/)
{
  COIN_STUB();
  return TRUE;
}

//! FIXME: write doc

SoGLLazyElement *
SoGLLazyElement::copyLazyMatchInfo(SoState* /*state*/)
{
  COIN_STUB();
  return NULL;
}

//! FIXME: write doc

void
SoGLLazyElement::getCopyGL(SoGLLazyElement * /*cacheLazyElement*/,
                           SoGLLazyElement::GLLazyState & /*cacheGLState*/)
{
  COIN_STUB();
}

//! FIXME: write doc

void
SoGLLazyElement::copyBackGL(SoGLLazyElement * /*cacheLazyElement*/,
                            SoGLLazyElement::GLLazyState & /*cacheGLState*/)
{
  COIN_STUB();
}

//! FIXME: write doc

void
SoGLLazyElement::mergeCacheInfo(SoGLRenderCache * /*childCache*/,
                                SoGLRenderCache* /*parentCache*/,
                                uint32_t /*doSendFlag*/,
                                uint32_t /*checkIVFlag*/,
                                uint32_t /*checkGLFlag*/)
{
  COIN_STUB();
}
