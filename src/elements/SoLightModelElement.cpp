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
  \class SoLightModelElement Inventor/elements/SoLightModelElement.h
  \brief The SoLightModelElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLightModelElement.h>

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoLightModelElement::Model

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoLightModelElement )

/*!
  \var SoLightModelElement::classTypeId

  This is the static class type identifier for the
  SoLightModelElement class.
*/

SoType SoLightModelElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoLightModelElement::getClassTypeId(void)
{
  return SoLightModelElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoLightModelElement class.
*/
void *
SoLightModelElement::createInstance(void)
{
  return (void *) new SoLightModelElement;
}

/*!
  \var SoLightModelElement::classStackIndex

  This is the static state stack index for the
  SoLightModelElement class.
*/
int SoLightModelElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoLightModelElement class.
*/
int
SoLightModelElement::getClassStackIndex(void)
{
  return SoLightModelElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoLightModelElement class.
*/

void 
SoLightModelElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoLightModelElement )
  assert(SoLightModelElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLightModelElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoLightModelElement",
    &SoLightModelElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoLightModelElement::classStackIndex =
      createStackIndex( SoLightModelElement::classTypeId );
  } else {
    SoLightModelElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoLightModelElement
  class.
*/

void 
SoLightModelElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoLightModelElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoLightModelElement::createInstance( void )
*/

SoLightModelElement::SoLightModelElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex); 
}

/*!
  The destructor.
*/

SoLightModelElement::~SoLightModelElement()
{ 
}

//! FIXME: write doc.

void 
SoLightModelElement::init(SoState *state)
{
  data = PHONG;
}

//! FIXME: write doc.

void 
SoLightModelElement::set(SoState * const state, const Model model)
{
  SoInt32Element::set(classStackIndex, state, model);
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setLightModel(state, (int32_t)model);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT
}

//! FIXME: write doc.

void 
SoLightModelElement::set(SoState * const state, SoNode * const node,
			 const Model model)
{
  SoInt32Element::set(classStackIndex, state, node, model);
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setLightModel(state, (int32_t)model);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT
}

//! FIXME: write doc.

SoLightModelElement::Model 
SoLightModelElement::get(SoState * const state)
{
  return (Model) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

SoLightModelElement::Model 
SoLightModelElement::getDefault()
{
  return PHONG;
}

