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
  \class SoShapeStyleElement Inventor/elements/SoShapeStyleElement.h
  \brief The SoShapeStyleElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoShapeStyleElement.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoLightModelElement.h>

#include <coindefs.h> // COIN_OBSOLETED()
#include <assert.h>

#define FLAG_TRANSPARENCYTYPE_MASK 0x000f
#define FLAG_LIGHTING              0x0010
#define FLAG_TEXENABLED            0x0020
#define FLAG_TEXFUNC               0x0040
#define FLAG_BBOXCMPLX             0x0080
#define FLAG_INVISIBLE             0x0100
#define FLAG_ABORTCB               0x0200
#define FLAG_SCREENDOOR            0x0400
#define FLAG_OVERRIDE              0x0800
#define FLAG_TEX3ENABLED           0x1000

#define FLAG_DELAYMASK (FLAG_BBOXCMPLX|FLAG_INVISIBLE|FLAG_ABORTCB)

SO_ELEMENT_SOURCE(SoShapeStyleElement);

/*!
  This static method initializes static data for the SoShapeStyleElement class.
*/

void
SoShapeStyleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoShapeStyleElement, inherited);
}

/*!
  The destructor.
*/

SoShapeStyleElement::~SoShapeStyleElement()
{
}

//! FIXME: write doc.

void
SoShapeStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->flags = FLAG_LIGHTING;
}

//! FIXME: write doc.

void
SoShapeStyleElement::push(SoState * state)
{
  inherited::push(state);
  SoShapeStyleElement * prev = (SoShapeStyleElement *) this->getNextInStack();
  this->flags = prev->flags;
}

//! FIXME: write doc.

void
SoShapeStyleElement::pop(SoState * state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoShapeStyleElement::matches(const SoElement * element) const
{
  const SoShapeStyleElement * elem =
    (const SoShapeStyleElement*) element;
  return this->flags == elem->flags;
}

//! FIXME: write doc.

SoElement *
SoShapeStyleElement::copyMatchInfo(void) const
{
  SoShapeStyleElement * elem =
    (SoShapeStyleElement*) this->getTypeId().createInstance();
  elem->flags = this->flags;
  return elem;
}

//! FIXME: write doc.

const SoShapeStyleElement *
SoShapeStyleElement::get(SoState * const state)
{
  return (const SoShapeStyleElement *)
    SoElement::getConstElement(state, classStackIndex);
}

//! FIXME: write doc.

void
SoShapeStyleElement::setDrawStyle(SoState * const state,
                                  const int32_t value)
{
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    if (value == (int32_t)SoDrawStyleElement::INVISIBLE) {
      elem->flags |= FLAG_INVISIBLE;
    }
    else {
      elem->flags &= ~FLAG_INVISIBLE;
    }
  }
}

//! FIXME: write doc.

void
SoShapeStyleElement::setComplexityType(SoState * const state,
                                       const int32_t value)
{
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    if (value == (int32_t) SoComplexityTypeElement::BOUNDING_BOX) {
      elem->flags |= FLAG_BBOXCMPLX;
    }
    else {
      elem->flags &= ~FLAG_BBOXCMPLX;
    }
  }
}

//! FIXME: write doc.

void
SoShapeStyleElement::setTransparencyType(SoState * const state,
                                         const int32_t value)
{
  assert(value >= 0 && value < FLAG_TRANSPARENCYTYPE_MASK);
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    elem->flags &= ~FLAG_TRANSPARENCYTYPE_MASK;
    elem->flags |= value;
  }
}

//! FIXME: write doc.

void
SoShapeStyleElement::setTextureEnabled(SoState * const state,
                                       const SbBool value)
{
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    if (value) {
      elem->flags |= FLAG_TEXENABLED;
    }
    else {
      elem->flags &= ~FLAG_TEXENABLED;
    }
  }
}

/*!
  FIXME: write doc.

  \since 2001-11-26
*/
void
SoShapeStyleElement::setTexture3Enabled(SoState * const state,
                                       const SbBool value)
{
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    if (value) {
      elem->flags |= FLAG_TEX3ENABLED;
    }
    else {
      elem->flags &= ~FLAG_TEX3ENABLED;
    }
  }
}

//! FIXME: write doc.

void
SoShapeStyleElement::setTextureFunction(SoState * const state,
                                        const SbBool value)
{
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    if (value) {
      elem->flags |= FLAG_TEXFUNC;
    }
    else {
      elem->flags &= ~FLAG_TEXFUNC;
    }
  }
}

//! FIXME: write doc.

void
SoShapeStyleElement::setLightModel(SoState * const state,
                                   const int32_t value)
{
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    if (value != (int32_t) SoLightModelElement::BASE_COLOR) {
      elem->flags |= FLAG_LIGHTING;
    }
    else {
      elem->flags &= ~FLAG_LIGHTING;
    }
  }
}

//! FIXME: write doc.

void
SoShapeStyleElement::setOverrides(SoState * const state,
                                  const SbBool value)
{
  SoShapeStyleElement * elem = getElement(state);
  if (elem) {
    if (value) {
      elem->flags |= FLAG_OVERRIDE;
    }
    else {
      elem->flags &= ~FLAG_OVERRIDE;
    }
  }
}

//! FIXME: write doc.

SbBool
SoShapeStyleElement::isScreenDoor(SoState * const state)
{
  const SoShapeStyleElement * elem = getConstElement(state);
  int val = elem->flags & FLAG_TRANSPARENCYTYPE_MASK;
  return val == (int) SoGLRenderAction::SCREEN_DOOR;
}

/*!
  Returns the current transparency type.

  This method was not part of the Open Inventor 2.1 API, and is an
  extension specific to Coin.
  
  \since 2001-11-04 
*/
int 
SoShapeStyleElement::getTransparencyType(SoState * const state)
{
  const SoShapeStyleElement * elem = getConstElement(state);
  return (elem->flags & FLAG_TRANSPARENCYTYPE_MASK);
}

/*!
  FIXME: write doc.
*/

SbBool
SoShapeStyleElement::mightNotRender() const
{
  return (this->flags & FLAG_DELAYMASK) != 0;
}

/*!
  FIXME: write doc.
*/

SbBool
SoShapeStyleElement::needNormals() const
{
  return (this->flags & FLAG_LIGHTING) != 0;
}

/*!
  FIXME: write doc.
*/

SbBool
SoShapeStyleElement::needTexCoords() const
{
  return (this->flags&(FLAG_TEXENABLED|FLAG_TEX3ENABLED));
}

/*!
  Not implemented in Coin. It is used by SoVertexProperty in SGI OIV.
*/
int
SoShapeStyleElement::getRenderCaseMask(void) const
{
  COIN_OBSOLETED();
  return 0;
}

/*!
  Returns the current modifiable instance (might cause a push())
*/
SoShapeStyleElement *
SoShapeStyleElement::getElement(SoState * const state)
{
  return (SoShapeStyleElement *)
    SoElement::getElement(state, classStackIndex);
}
/*!
  Returns the current read-only instance.
*/
const SoShapeStyleElement *
SoShapeStyleElement::getConstElement(SoState * const state)
{
  return (const SoShapeStyleElement *)
    SoElement::getConstElement(state, classStackIndex);
}

#undef FLAG_LIGHTING
#undef FLAG_TEXENABLED
#undef FLAG_TEXFUNC
#undef FLAG_BBOXCMPLX
#undef FLAG_INVISIBLE
#undef FLAG_ABORTCB
#undef FLAG_SCREENDOOR
#undef FLAG_OVERRIDE
#undef FLAG_TEX3ENABLED
#undef FLAG_TEXMASK
#undef FLAG_DELAYMASK
