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
  \class SoTextOutlineEnabledElement Inventor/elements/SoTextOutlineEnabledElement.h
  \brief The SoTextOutlineEnabledElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextOutlineEnabledElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource(SoTextOutlineEnabledElement)

/*!
  \var SoTextOutlineEnabledElement::classTypeId

  This is the static class type identifier for the
  SoTextOutlineEnabledElement class.
*/

SoType SoTextOutlineEnabledElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoTextOutlineEnabledElement::getClassTypeId(void)
{
  return SoTextOutlineEnabledElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoTextOutlineEnabledElement class.
*/
void *
SoTextOutlineEnabledElement::createInstance(void)
{
  return (void *) new SoTextOutlineEnabledElement;
}

/*!
  \var SoTextOutlineEnabledElement::classStackIndex

  This is the static state stack index for the
  SoTextOutlineEnabledElement class.
*/
int SoTextOutlineEnabledElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoTextOutlineEnabledElement class.
*/
int
SoTextOutlineEnabledElement::getClassStackIndex(void)
{
  return SoTextOutlineEnabledElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoTextOutlinedEnabledElement class.
*/

void
SoTextOutlineEnabledElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoTextOutlineEnabledElement)
  assert(SoTextOutlineEnabledElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextOutlineEnabledElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoTextOutlineEnabledElement",
                       &SoTextOutlineEnabledElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoTextOutlineEnabledElement::classStackIndex =
      createStackIndex(SoTextOutlineEnabledElement::classTypeId);
  }
  else {
    SoTextOutlineEnabledElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
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
