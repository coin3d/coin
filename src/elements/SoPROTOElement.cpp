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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoPROTOElement Inventor/elements/SoPROTOElement.h
  \brief The SoPROTOElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoPROTOElement.h>

#include <coindefs.h> // COIN_STUB()


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
