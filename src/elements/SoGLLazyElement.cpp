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
SoGLLazyElement::sendAllMaterial(SoState */*state*/)
{
  // elements should be up-to-date. Do nothing.
}

//! FIXME: write doc

void
SoGLLazyElement::sendNoMaterial(SoState *state)
{
  // elements should be up-to-date. Do nothing.
}

//! FIXME: write doc

void
SoGLLazyElement::sendOnlyDiffuseColor(SoState *state)
{
  // SoGLDiffuseColorElement should be up-to-date. Do nothing.
}

//! FIXME: write doc

void
SoGLLazyElement::sendDiffuseByIndex(int index) const
{
  SoGLDiffuseColorElement * elem = (SoGLDiffuseColorElement*)
    SoElement::getConstElement(this->state, classStackIndex);
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
  return (SoGLLazyElement*)SoElement::getConstElement((SoState*)state,
                                                      classStackIndex);
}

//! FIXME: write doc

void
SoGLLazyElement::send(const SoState *state, uint32_t mask) const
{
  // elements should be up=to-date. Do nothing.
}

//! FIXME: write doc

void
SoGLLazyElement::sendVPPacked(SoState* state, const unsigned char* pcolor)
{
  SoGLDiffuseColorElement * elem = (SoGLDiffuseColorElement*)
    SoElement::getConstElement(state, classStackIndex);
  const uint32_t rgba =
    (pcolor[0] << 24) | (pcolor[1] << 16) | (pcolor[2] << 8) | pcolor[3];
  elem->sendOnePacked(rgba);

  // FIXME: I couldn't be bothered to support polygon stipples for
  // transparency here. Let me know if anybody needs it, and I'll implement it.
  // I don't think polygons stipples will be used for transparency much longer,
  // as it looks darn ugly, and often is slower than blending on PCs
  // with a 3D accelerator. pederb, 20000214
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
