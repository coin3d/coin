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
  \class SoComplexityTypeElement Inventor/elements/SoComplexityTypeElement.h
  \brief The SoComplexityTypeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoComplexityTypeElement.h>

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoComplexityTypeElement::Type

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoComplexityTypeElement )

/*!
  \var SoComplexityTypeElement::classTypeId

  This is the static class type identifier for the
  SoComplexityTypeElement class.
*/

SoType SoComplexityTypeElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoComplexityTypeElement::getClassTypeId(void)
{
  return SoComplexityTypeElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoComplexityTypeElement class.
*/
void *
SoComplexityTypeElement::createInstance(void)
{
  return (void *) new SoComplexityTypeElement;
}

/*!
  \var SoComplexityTypeElement::classStackIndex

  This is the static state stack index for the
  SoComplexityTypeElement class.
*/
int SoComplexityTypeElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoComplexityTypeElement class.
*/
int
SoComplexityTypeElement::getClassStackIndex(void)
{
  return SoComplexityTypeElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoComplexityTypeElement
  class.
*/

void
SoComplexityTypeElement::initClass(
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoComplexityTypeElement )
  assert(SoComplexityTypeElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoComplexityTypeElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoComplexityTypeElement",
    &SoComplexityTypeElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoComplexityTypeElement::classStackIndex =
      createStackIndex( SoComplexityTypeElement::classTypeId );
  } else {
    SoComplexityTypeElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoComplexityTypeElement
  class.
*/

void
SoComplexityTypeElement::cleanClass(
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoComplexityTypeElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoComplexityElement::createInstance( void )
*/

SoComplexityTypeElement::SoComplexityTypeElement(
    void )
{
    setTypeId( SoComplexityTypeElement::classTypeId );
    setStackIndex( SoComplexityTypeElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoComplexityTypeElement::~SoComplexityTypeElement(
    void )
{
}

//! FIXME: write doc.

void
SoComplexityTypeElement::set(
    SoState * const state,
    SoNode * const node,
    const Type type )
{
  assert(type == OBJECT_SPACE || type == SCREEN_SPACE || type == BOUNDING_BOX);

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setComplexityType(state, type);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT
  
  SoInt32Element::set(classStackIndex, state, node, type);
}

//! FIXME: write doc.

void
SoComplexityTypeElement::init(SoState * state)
{
  inherited::init(state);
  this->data = OBJECT_SPACE;
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoComplexityTypeElement::set(SoState * const state, const Type type)
{ 
  set(state, NULL, type); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoComplexityTypeElement::Type 
SoComplexityTypeElement::get(SoState * const state)
{ 
  return (Type)SoInt32Element::get(classStackIndex, state); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoComplexityTypeElement::Type 
SoComplexityTypeElement::getDefault()
{ 
  return OBJECT_SPACE; 
}

