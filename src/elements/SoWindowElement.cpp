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
  \class SoWindowElement Inventor/elements/SoWindowElement.h
  \brief The SoWindowElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoWindowElement.h>

#include <Inventor/SbName.h>

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
  A constructor.  Can't be used directly.

  \sa void * SoWindowElement::createInstance(void)
*/

SoWindowElement::SoWindowElement(void)
  : // window(), context(),
  display(NULL), glRenderAction( NULL )
{
    setTypeId(SoWindowElement::classTypeId);
    setStackIndex(SoWindowElement::classStackIndex);
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
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoWindowElement::copyMatchInfo(void) const
{
  assert(0 && "FIXME: not implemented");
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
    assert(0 && "FIXME: not implemented");
}

//! FIXME: write doc.

void
SoWindowElement::get(SoState * const /* state */,
                     Window & /* window */,
                     GLXContext & /* context */,
                     Display * & /* display */,
                     SoGLRenderAction * & /* action */)
{
    assert(0 && "FIXME: not implemented");
}
