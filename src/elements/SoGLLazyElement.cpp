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
  \class SoGLLazyElement Inventor/elements/SoGLLazyElement.h
  \brief The SoGLLazyElement class is meant to optimize GL rendering.
  \ingroup elements

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
#include <Inventor/elements/SoGLColorIndexElement.h>
#include <Inventor/misc/SoState.h>

#include <Inventor/bundles/SoMaterialBundle.h>
#include <coindefs.h> // COIN_OBSOLETED
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
  stippleElt->lazyEvaluate();
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
  return SoGLColorIndexElement::isColorIndexMode(state);
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
  COIN_OBSOLETED();
  return TRUE;
}

//! FIXME: write doc

SoGLLazyElement *
SoGLLazyElement::copyLazyMatchInfo(SoState* /*state*/)
{
  COIN_OBSOLETED();
  return NULL;
}

//! FIXME: write doc

void
SoGLLazyElement::getCopyGL(SoGLLazyElement * /*cacheLazyElement*/,
                           SoGLLazyElement::GLLazyState & /*cacheGLState*/)
{
  COIN_OBSOLETED();
}

//! FIXME: write doc

void
SoGLLazyElement::copyBackGL(SoGLLazyElement * /*cacheLazyElement*/,
                            SoGLLazyElement::GLLazyState & /*cacheGLState*/)
{
  COIN_OBSOLETED();
}

//! FIXME: write doc

void
SoGLLazyElement::mergeCacheInfo(SoGLRenderCache * /*childCache*/,
                                SoGLRenderCache* /*parentCache*/,
                                uint32_t /*doSendFlag*/,
                                uint32_t /*checkIVFlag*/,
                                uint32_t /*checkGLFlag*/)
{
  COIN_OBSOLETED();
}
