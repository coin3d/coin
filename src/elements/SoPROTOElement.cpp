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
  \class SoPROTOElement Inventor/elements/SoPROTOElement.h
  \brief The SoPROTOElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoPROTOElement.h>

#include <coindefs.h> // COIN_STUB()

#include <Inventor/SoType.h>

#include <assert.h>

/*!
  \fn SoPROTOElement::nodeptr

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoPROTOElement);

/*!
  This static method initializes static data for the SoPROTOElement class.
*/

void
SoPROTOElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoPROTOElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoPROTOElement::createInstance(void)
*/

SoPROTOElement::SoPROTOElement(void)
{
    setTypeId(SoPROTOElement::classTypeId);
    setStackIndex(SoPROTOElement::classStackIndex);
}

/*!
  The destructor.
*/

SoPROTOElement::~SoPROTOElement(void)
{
}

//! FIXME: write doc.

void
SoPROTOElement::init(SoState * state)
{
    inherited::init(state);
}

//! FIXME: write doc.

void
SoPROTOElement::push(SoState * state)
{
    inherited::push(state);
}

//! FIXME: write doc.

void
SoPROTOElement::pop(SoState * state, const SoElement * prevTopElement)
{
    inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoPROTOElement::matches(const SoElement * /* element */) const
{
  COIN_STUB();
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoPROTOElement::copyMatchInfo(void) const
{
  COIN_STUB();
  return FALSE;
}
