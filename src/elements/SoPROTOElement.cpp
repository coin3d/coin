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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoPROTOElement Inventor/elements/SoPROTOElement.h
  \brief The SoPROTOElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoPROTOElement.h>

#include <Inventor/SbName.h>

#include <Inventor/SoType.h>

#include <assert.h>

/*!
  \fn SoPROTOElement::nodeptr

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoPROTOElement )

/*!
  \var SoPROTOElement::classTypeId

  This is the static class type identifier for the
  SoPROTOElement class.
*/

SoType SoPROTOElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoPROTOElement::getClassTypeId(void)
{
  return SoPROTOElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoPROTOElement class.
*/
void *
SoPROTOElement::createInstance(void)
{
  return (void *) new SoPROTOElement;
}

/*!
  \var SoPROTOElement::classStackIndex

  This is the static state stack index for the
  SoPROTOElement class.
*/
int SoPROTOElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoPROTOElement class.
*/
int
SoPROTOElement::getClassStackIndex(void)
{
  return SoPROTOElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoPROTOElement class.
*/

void
SoPROTOElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoPROTOElement )
  assert(SoPROTOElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPROTOElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoPROTOElement",
    &SoPROTOElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoPROTOElement::classStackIndex =
      createStackIndex( SoPROTOElement::classTypeId );
  } else {
    SoPROTOElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoPROTOElement class.
*/

void
SoPROTOElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoPROTOElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoPROTOElement::createInstance( void )
*/

SoPROTOElement::SoPROTOElement(
    void )
{
    setTypeId( SoPROTOElement::classTypeId );
    setStackIndex( SoPROTOElement::classStackIndex );
} // SoPROTOElement()

/*!
  The destructor.
*/

SoPROTOElement::~SoPROTOElement(
    void )
{
} // ~SoPROTOElement()

//! FIXME: write doc.

void
SoPROTOElement::init( // virtual
    SoState * state )
{
    inherited::init( state );
} // init()

//! FIXME: write doc.

void
SoPROTOElement::push( // virtual
    SoState * state )
{
    inherited::push( state );
} // push()

//! FIXME: write doc.

void
SoPROTOElement::pop( // virtual
    SoState * state,
    const SoElement * prevTopElement )
{
    inherited::pop( state, prevTopElement );
} // pop()

//! FIXME: write doc.

SbBool 
SoPROTOElement::matches( const SoElement * element ) const
{
  // FIXME
  assert(0 && "FIXME: not implemented");
  element;
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoPROTOElement::copyMatchInfo( void ) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

