/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoGLRenderPassElement Inventor/elements/SoGLRenderPassElement.h
  \brief The SoGLRenderPassElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLRenderPassElement.h>

#include <coindefs.h> // COIN_STUB()

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
