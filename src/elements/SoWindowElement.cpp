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
  \class SoWindowElement Inventor/elements/SoWindowElement.h
  \brief The SoWindowElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoWindowElement.h>

#include <coindefs.h> // COIN_STUB()

#include <assert.h>

/*!
  \fn SoWindowElement::window

  FIXME: write doc.
*/

/*!
  \fn SoWindowElement::context

  FIXME: write doc.
*/

/*!
  \fn SoWindowElement::display

  FIXME: write doc.
*/

/*!
  \fn SoWindowElement::glRenderAction

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoWindowElement);

/*!
  This static method initializes static data for the SoWindowElement class.
*/

void
SoWindowElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoWindowElement, inherited);
}

/*!
  The destructor.
*/

SoWindowElement::~SoWindowElement(void)
{
}

//! FIXME: write doc.

void
SoWindowElement::init(SoState * state)
{
    inherited::init(state);
}

//! FIXME: write doc.

void
SoWindowElement::push(SoState * state)
{
    inherited::push(state);
}

//! FIXME: write doc.

void
SoWindowElement::pop(SoState * state,
                     const SoElement * prevTopElement)
{
    inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoWindowElement::matches(const SoElement * /* element */) const
{
  COIN_STUB();
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoWindowElement::copyMatchInfo(void) const
{
  COIN_STUB();
  return NULL;
}

//! FIXME: write doc.

void
SoWindowElement::set(SoState * const /* state */,
                     const Window & /* window */,
                     const GLXContext & /* context */,
                     Display * const /* display */,
                     SoGLRenderAction * const /* action */)
{
  COIN_STUB();
}

//! FIXME: write doc.

void
SoWindowElement::get(SoState * const /* state */,
                     Window & /* window */,
                     GLXContext & /* context */,
                     Display * & /* display */,
                     SoGLRenderAction * & /* action */)
{
  COIN_STUB();
}
