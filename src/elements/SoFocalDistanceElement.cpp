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
  \class SoFocalDistanceElement Inventor/elements/SoFocalDistanceElement.h
  \brief The SoFocalDistanceElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoFocalDistanceElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoFocalDistanceElement )

/*!
  \var SoFocalDistanceElement::classTypeId

  This is the static class type identifier for the
  SoFocalDistanceElement class.
*/

SoType SoFocalDistanceElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoFocalDistanceElement::getClassTypeId(void)
{
  return SoFocalDistanceElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoFocalDistanceElement class.
*/
void *
SoFocalDistanceElement::createInstance(void)
{
  return (void *) new SoFocalDistanceElement;
}

/*!
  \var SoFocalDistanceElement::classStackIndex

  This is the static state stack index for the
  SoFocalDistanceElement class.
*/
int SoFocalDistanceElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoFocalDistanceElement class.
*/
int
SoFocalDistanceElement::getClassStackIndex(void)
{
  return SoFocalDistanceElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoFocalDistanceElement class.
*/

void
SoFocalDistanceElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoFocalDistanceElement )
  assert(SoFocalDistanceElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoFocalDistanceElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoFocalDistanceElement",
    &SoFocalDistanceElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoFocalDistanceElement::classStackIndex =
      createStackIndex( SoFocalDistanceElement::classTypeId );
  } else {
    SoFocalDistanceElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoFocalDistanceElement class.
*/

void
SoFocalDistanceElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoFocalDistanceElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoFocalDistanceElement::createInstance( void )
*/

SoFocalDistanceElement::SoFocalDistanceElement( // protected
    void )
{
    setTypeId( SoFocalDistanceElement::classTypeId );
    setStackIndex( SoFocalDistanceElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoFocalDistanceElement::~SoFocalDistanceElement( // virtual protected
    void )
{
}

//! FIXME: write doc.

void
SoFocalDistanceElement::init( // virtual
    SoState * state )
{
    inherited::init( state );
    this->data = getDefault();
}

//! FIXME: write doc.

void 
SoFocalDistanceElement::set(SoState * const state, SoNode * const node,
			    const float distance)
{ 
  SoFloatElement::set( classStackIndex, state, node, distance );
}

//! FIXME: write doc.

void 
SoFocalDistanceElement::set(SoState * const state, const float distance)
{ 
  set( state, NULL, distance ); 
}

//! FIXME: write doc.

float 
SoFocalDistanceElement::get(SoState * const state)
{ 
  return SoFloatElement::get( classStackIndex, state ); 
}

//! FIXME: write doc.

float 
SoFocalDistanceElement::getDefault()
{ 
  return 5.0f; 
}

