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
  \class SoAnnoText3FontSizeHintElement Inventor/elements/SoAnnoText3FontSizeHintElement.h
  \brief The SoAnnoText3FontSizeHintElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoAnnoText3FontSizeHintElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoAnnoText3FontSizeHintElement::FontSizeHint

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource(SoAnnoText3FontSizeHintElement)

/*!
  \var SoAnnoText3FontSizeHintElement::classTypeId

  This is the static class type identifier for the
  SoAnnoText3FontSizeHintElement class.
*/

SoType SoAnnoText3FontSizeHintElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoAnnoText3FontSizeHintElement::getClassTypeId(void)
{
  return SoAnnoText3FontSizeHintElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoAnnoText3FontSizeHintElement class.
*/
void *
SoAnnoText3FontSizeHintElement::createInstance(void)
{
  return (void *) new SoAnnoText3FontSizeHintElement;
}

/*!
  \var SoAnnoText3FontSizeHintElement::classStackIndex

  This is the static state stack index for the
  SoAnnoText3FontSizeHintElement class.
*/
int SoAnnoText3FontSizeHintElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoAnnoText3FontSizeHintElement class.
*/
int
SoAnnoText3FontSizeHintElement::getClassStackIndex(void)
{
  return SoAnnoText3FontSizeHintElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoAnnoText3FontSizeHintElement class.
*/

void
SoAnnoText3FontSizeHintElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoAnnoText3FontSizeHintElement)
  assert(SoAnnoText3FontSizeHintElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoAnnoText3FontSizeHintElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoAnnoText3FontSizeHintElement",
                       &SoAnnoText3FontSizeHintElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoAnnoText3FontSizeHintElement::classStackIndex =
      createStackIndex(SoAnnoText3FontSizeHintElement::classTypeId);
  }
  else {
    SoAnnoText3FontSizeHintElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoAnnoText3FontSizeHintElement::createInstance()
*/

SoAnnoText3FontSizeHintElement::SoAnnoText3FontSizeHintElement(void)
{
    setTypeId(SoAnnoText3FontSizeHintElement::classTypeId);
    setStackIndex(SoAnnoText3FontSizeHintElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoAnnoText3FontSizeHintElement::~SoAnnoText3FontSizeHintElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoAnnoText3FontSizeHintElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3FontSizeHintElement::set(SoState * const state, SoNode * const node,
                                const FontSizeHint hint)
{
  SoInt32Element::set(classStackIndex,state,node,hint);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3FontSizeHintElement::set(SoState * const state, const FontSizeHint hint)
{
  set(state, NULL, hint);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3FontSizeHintElement::FontSizeHint
SoAnnoText3FontSizeHintElement::get(SoState * const state)
{
  return (SoAnnoText3FontSizeHintElement::FontSizeHint)
    SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3FontSizeHintElement::FontSizeHint
SoAnnoText3FontSizeHintElement::getDefault()
{
  return FIT_TEXT_VECTOR;
}
