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
  \class SoDrawStyleElement Inventor/elements/SoDrawStyleElement.h
  \brief The SoDrawStyleElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDrawStyleElement.h>

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPEHINTSELEMENT

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoDrawStyleElement::Style

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource(SoDrawStyleElement)

/*!
  \var SoDrawStyleElement::classTypeId

  This is the static class type identifier for the
  SoDrawStyleElement class.
*/

SoType SoDrawStyleElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoDrawStyleElement::getClassTypeId(void)
{
  return SoDrawStyleElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoDrawStyleElement class.
*/
void *
SoDrawStyleElement::createInstance(void)
{
  return (void *) new SoDrawStyleElement;
}

/*!
  \var SoDrawStyleElement::classStackIndex

  This is the static state stack index for the
  SoDrawStyleElement class.
*/
int SoDrawStyleElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoDrawStyleElement class.
*/
int
SoDrawStyleElement::getClassStackIndex(void)
{
  return SoDrawStyleElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoDrawStyleElement class.
*/

void
SoDrawStyleElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoDrawStyleElement)
  assert(SoDrawStyleElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoDrawStyleElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoDrawStyleElement",
                       &SoDrawStyleElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoDrawStyleElement::classStackIndex =
      createStackIndex(SoDrawStyleElement::classTypeId);
  }
  else {
    SoDrawStyleElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoDrawStyleElement::createInstance(void)
*/

SoDrawStyleElement::SoDrawStyleElement(void)
{
  setTypeId(SoDrawStyleElement::classTypeId);
  setStackIndex(SoDrawStyleElement::classStackIndex);
}

/*!
  The destructor.
*/

SoDrawStyleElement::~SoDrawStyleElement(void)
{
}

//! FIXME: write doc.

void
SoDrawStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoDrawStyleElement::set(SoState * const state,
                        SoNode * const node,
                        const Style style)
{
  SoInt32Element::set(classStackIndex, state, node, (int32_t)style);
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setDrawStyle(state, (int32_t)style);
#endif // !COIN_EXCLUDE_SOSHAPESTYLEELEMENT
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoDrawStyleElement::set(SoState * const state, const Style style)
{
  set(state, NULL, style);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDrawStyleElement::Style
SoDrawStyleElement::get(SoState * const state)
{
  return (Style) inherited::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDrawStyleElement::Style
SoDrawStyleElement::getDefault()
{
  return FILLED;
}
