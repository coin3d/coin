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
  \class SoTextOutlineEnabledElement Inventor/elements/SoTextOutlineEnabledElement.h
  \brief The SoTextOutlineEnabledElement class is yet to be documented.

  FIXME: write doc.
*/


#include <Inventor/elements/SoTextOutlineEnabledElement.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoTextOutlineEnabledElement);

/*!
  This static method initializes static data for the
  SoTextOutlinedEnabledElement class.
*/

void
SoTextOutlineEnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextOutlineEnabledElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoTextOutlineEnabledElement::createInstance(void)
*/

SoTextOutlineEnabledElement::SoTextOutlineEnabledElement(void)
{
    setTypeId(SoTextOutlineEnabledElement::classTypeId);
    setStackIndex(SoTextOutlineEnabledElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoTextOutlineEnabledElement::~SoTextOutlineEnabledElement(void)
{
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::set(SoState * const state,
                                 SoNode * const node,
                                 const SbBool enabled)
{
    SoInt32Element::set(classStackIndex, state, node, enabled);
}

/*!
  FIXME: write doc.
*/

void
SoTextOutlineEnabledElement::set(SoState * const state, const SbBool enabled)
{
  set(state, NULL, enabled);
}

/*!
  FIXME: write doc.
*/

SbBool
SoTextOutlineEnabledElement::get(SoState * const state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}

/*!
  FIXME: write doc.
*/

SbBool
SoTextOutlineEnabledElement::getDefault(void)
{
  return FALSE;
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::init(SoState * state)
{
    inherited::init(state);
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::push(SoState * state)
{
    inherited::push(state);
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::pop(SoState * state,
                                 const SoElement * prevTopElement)
{
    inherited::pop(state, prevTopElement);
}
