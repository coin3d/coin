/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoGLRenderPassElement Inventor/elements/SoGLRenderPassElement.h
  \brief The SoGLRenderPassElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLRenderPassElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoGLRenderPassElement::passnr

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoGLRenderPassElement);

/*!
  This static method initializes static data for the
  SoGLRenderPassElement class.
*/

void
SoGLRenderPassElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLRenderPassElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLRenderPassElement::createInstance(void)
*/

SoGLRenderPassElement::SoGLRenderPassElement(void)
  : passnr(0)
{
  setTypeId(SoGLRenderPassElement::classTypeId);
  setStackIndex(SoGLRenderPassElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLRenderPassElement::~SoGLRenderPassElement(void)
{
}

//! FIXME: write doc.

void
SoGLRenderPassElement::init(SoState * state)
{
  inherited::init(state);
  this->passnr = 0;
}

//! FIXME: write doc.

void
SoGLRenderPassElement::push(SoState * state)
{
  inherited::push(state);
}

//! FIXME: write doc.

void
SoGLRenderPassElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoGLRenderPassElement::matches(const SoElement * /* element */) const
{
  COIN_STUB();
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoGLRenderPassElement::copyMatchInfo(void) const
{
  COIN_STUB();
  return NULL;
}

//! FIXME: write doc.

void
SoGLRenderPassElement::set(SoState * const state,
                           const int pass)
{
  SoGLRenderPassElement * element = (SoGLRenderPassElement *)
    getElement(state, classStackIndex);
  element->passnr = pass;
}

//! FIXME: write doc.

int
SoGLRenderPassElement::get(SoState * const state)
{
  const SoGLRenderPassElement * element = (const SoGLRenderPassElement *)
    getConstElement(state, classStackIndex);
  return element->passnr;
}

//! FIXME: write doc.

void
SoGLRenderPassElement::print(FILE * /* file */) const
{
  COIN_STUB();
}
