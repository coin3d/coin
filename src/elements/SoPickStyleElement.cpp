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
  \class SoPickStyleElement Inventor/elements/SoPickStyleElement.h
  \brief The SoPickStyleElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoPickStyleElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoPickStyleElement::Style

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource(SoPickStyleElement)

/*!
  \var SoPickStyleElement::classTypeId

  This is the static class type identifier for the
  SoPickStyleElement class.
*/

SoType SoPickStyleElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoPickStyleElement::getClassTypeId(void)
{
  return SoPickStyleElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoPickStyleElement class.
*/
void *
SoPickStyleElement::createInstance(void)
{
  return (void *) new SoPickStyleElement;
}

/*!
  \var SoPickStyleElement::classStackIndex

  This is the static state stack index for the
  SoPickStyleElement class.
*/
int SoPickStyleElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoPickStyleElement class.
*/
int
SoPickStyleElement::getClassStackIndex(void)
{
  return SoPickStyleElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoPickStyleElement class.
*/

void
SoPickStyleElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoPickStyleElement)
  assert(SoPickStyleElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPickStyleElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoPickStyleElement",
                       &SoPickStyleElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoPickStyleElement::classStackIndex =
      createStackIndex(SoPickStyleElement::classTypeId);
  }
  else {
    SoPickStyleElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoPickStyleElement::createInstance(void)
*/

SoPickStyleElement::SoPickStyleElement(void)
{
    setTypeId(SoPickStyleElement::classTypeId);
    setStackIndex(SoPickStyleElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoPickStyleElement::~SoPickStyleElement(void)
{
}

//! FIXME: write doc.

void
SoPickStyleElement::set(SoState * const state,
			SoNode * const node,
			const int32_t style)
{
  assert(style >= SHAPE && style <= UNPICKABLE);
  SoInt32Element::set(classStackIndex, state, node, style);
}

//! FIXME: write doc.

void
SoPickStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoPickStyleElement::set(SoState * const state, const Style style)
{ 
  set(state, NULL, style); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoPickStyleElement::Style 
SoPickStyleElement::get(SoState * const state)
{ 
  return (Style) SoInt32Element::get(classStackIndex, state); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoPickStyleElement::Style 
SoPickStyleElement::getDefault()
{ 
  return SHAPE; 
}

