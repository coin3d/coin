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
  \class SoInt32Element Inventor/elements/SoInt32Element.h
  \brief The SoInt32Element class is the base class for all elements
  that only take care of a 32-bit integer.
*/

#include <Inventor/elements/SoInt32Element.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoInt32Element::data

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE AbstractElementSource( SoInt32Element )

/*!
  \var SoInt32Element::classTypeId

  This is the static class type identifier for
  SoInt32Element.
*/

SoType SoInt32Element::classTypeId = SoType::badType(); // static

/*!
  This static method returns the SoType type for SoInt32Element.
*/

SoType
SoInt32Element::getClassTypeId( // static
  void )
{
  return SoInt32Element::classTypeId;
} // getClassTypeId()

/*!
  \var SoInt32Element::classStackIndex

  This is the static state stack index for the
  SoInt32Element class.
*/

int SoInt32Element::classStackIndex; // static

/*!
  This static method returns the state stack index for the SoInt32Element class.
*/

int
SoInt32Element::getClassStackIndex( // static
  void )
{
  return SoInt32Element::classStackIndex;
} // getClassStackIndex()
//$ END TEMPLATE AbstractElementSource

/*!
  This static method initializes static data for the SoInt32Element class.
*/

void
SoInt32Element::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitAbstractElementSource( SoInt32Element )
  assert(SoInt32Element::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoInt32Element::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoInt32Element", NULL );

  if ( inherited::classStackIndex < 0 ) {
    SoInt32Element::classStackIndex =
      createStackIndex( SoInt32Element::classTypeId );
  } else {
    SoInt32Element::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitAbstractElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoInt32Element class.
*/

void
SoInt32Element::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanAbstractElementSource( SoInt32Element )
//$ END TEMPLATE CleanAbstractElementSource
} // cleanClass()

/*!
  The constructor initialized the int32_t to 0, and sets the class type
  identifier and stack index of the element.
*/

SoInt32Element::SoInt32Element(
    void )
  : data( 0 )
{
  setTypeId( SoInt32Element::classTypeId );
  setStackIndex( SoInt32Element::classStackIndex );
} // SoInt32Element()

/*!
  The destructor.
*/

SoInt32Element::~SoInt32Element(
    void )
{
} // ~SoInt32Element()

//! FIXME: write doc.

SbBool
SoInt32Element::matches( // virtual
    const SoElement * element ) const
{
    assert( element );
    if ( getTypeId() != element->getTypeId() )
        return FALSE;
    if ( ((const SoInt32Element *)element)->data != this->data )
        return FALSE;
    return TRUE;
} // matches()

//! FIXME: write doc.

SoElement *
SoInt32Element::copyMatchInfo( // virtual
    void ) const
{
    assert( getTypeId().canCreateInstance() );
    SoInt32Element * element =
        (SoInt32Element *)(getTypeId().createInstance());
    element->data = this->data;

    // DEPRECATED 980807 pederb. copyMatchInfo should only copy 
    // information needed in matches(). An exact copy is not needed.
    //    element->dataNode = this->dataNode;
    return (SoElement *)element;
} // copyMatchInfo()

//! FIXME: write doc.

void
SoInt32Element::print( // virtual
    FILE * file ) const
{
    fprintf( file, "%s[%p]: data = %d\n", 
        getTypeId().getName().getString(), this, this->data );
} // print()

//! FIXME: write doc.

void
SoInt32Element::set(const int index,
		    SoState * const state,
		    SoNode * const node,
		    const int32_t value)
{
  SoInt32Element * element;
  element = (SoInt32Element *) getElement( state, index );
  if (element)
    element->setElt(value);
} // set()

/*!
  FIXME: write doc.
*/

void
SoInt32Element::set(
  const int index, SoState * const state, const int32_t value )
{
  set( index, state, NULL, value );
}

//! FIXME: write doc.

int32_t
SoInt32Element::get(const int index,
		    SoState * const state)
{
  SoInt32Element * element;
  element = (SoInt32Element *) getConstElement( state, index ); //, NULL );
  if (element)
    return element->data;
  return 0;
} // get()

//! FIXME: write doc.

void
SoInt32Element::setElt(int32_t value)
{
  this->data = value;
} // setElt()

//! FIXME: write doc.

void
SoInt32Element::init( // virtual
    SoState * state )
{
  inherited::init( state );
  this->data = 0;
} // init()

