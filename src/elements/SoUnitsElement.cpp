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
  \class SoUnitsElement Inventor/elements/SoUnitsElement.h
  \brief The SoUnitsElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoUnitsElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoUnitsElement::Units

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoUnitsElement )

/*!
  \var SoUnitsElement::classTypeId

  This is the static class type identifier for the
  SoUnitsElement class.
*/

SoType SoUnitsElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoUnitsElement::getClassTypeId(void)
{
  return SoUnitsElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoUnitsElement class.
*/
void *
SoUnitsElement::createInstance(void)
{
  return (void *) new SoUnitsElement;
}

/*!
  \var SoUnitsElement::classStackIndex

  This is the static state stack index for the
  SoUnitsElement class.
*/
int SoUnitsElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoUnitsElement class.
*/
int
SoUnitsElement::getClassStackIndex(void)
{
  return SoUnitsElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoUnitsElement class.
*/

void
SoUnitsElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoUnitsElement )
  assert(SoUnitsElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoUnitsElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoUnitsElement",
    &SoUnitsElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoUnitsElement::classStackIndex =
      createStackIndex( SoUnitsElement::classTypeId );
  } else {
    SoUnitsElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoUnitsElement class.
*/

void
SoUnitsElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoUnitsElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoUnitsElement::createInstance( void )
*/

SoUnitsElement::SoUnitsElement(
    void )
{
    setTypeId( SoUnitsElement::classTypeId );
    setStackIndex( SoUnitsElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoUnitsElement::~SoUnitsElement(
    void )
{
}

//! FIXME: write doc.

void
SoUnitsElement::set( // static
    SoState * const state,
    SoNode * const node,
    const Units unit )
{
  assert((int)unit >= (int)METERS && (int)unit <= (int)NAUTICAL_MILES);
  SoInt32Element::set(classStackIndex, state, node, unit);
}

//! FIXME: write doc.

void
SoUnitsElement::init( // virtual
    SoState * state )
{
    inherited::init( state );
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoUnitsElement::set(SoState * const state, const Units units)
{ 
  set(state, NULL, units); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoUnitsElement::Units 
SoUnitsElement::get(SoState * const state)
{ 
  return (Units) SoInt32Element::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoUnitsElement::Units 
SoUnitsElement::getDefault()
{ 
  return METERS; 
}

