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
  \class SoPointSizeElement Inventor/elements/SoPointSizeElement.h
  \brief The SoPointSizeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoPointSizeElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoPointSizeElement )

/*!
  \var SoPointSizeElement::classTypeId

  This is the static class type identifier for the
  SoPointSizeElement class.
*/

SoType SoPointSizeElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoPointSizeElement::getClassTypeId(void)
{
  return SoPointSizeElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoPointSizeElement class.
*/
void *
SoPointSizeElement::createInstance(void)
{
  return (void *) new SoPointSizeElement;
}

/*!
  \var SoPointSizeElement::classStackIndex

  This is the static state stack index for the
  SoPointSizeElement class.
*/
int SoPointSizeElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoPointSizeElement class.
*/
int
SoPointSizeElement::getClassStackIndex(void)
{
  return SoPointSizeElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoPointSizeElement class.
*/

void
SoPointSizeElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoPointSizeElement )
  assert(SoPointSizeElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPointSizeElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoPointSizeElement",
    &SoPointSizeElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoPointSizeElement::classStackIndex =
      createStackIndex( SoPointSizeElement::classTypeId );
  } else {
    SoPointSizeElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoPointSizeElement class.
*/

void
SoPointSizeElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoPointSizeElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa SoPointSizeElement::createInstance( void )
*/

SoPointSizeElement::SoPointSizeElement( // protected
    void )
{
    setTypeId( SoPointSizeElement::classTypeId );
    setStackIndex( SoPointSizeElement::classStackIndex );
    this->data = getDefault();
} // SoPointSizeElement()

/*!
  The destructor.
*/

SoPointSizeElement::~SoPointSizeElement( // virtual protected
    void )
{
} // ~SoPointSizeElement()

//! FIXME: write doc.

void
SoPointSizeElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
  this->data = getDefault();
} // init()

//! FIXME: write doc.

void 
SoPointSizeElement::set(SoState * const state, 
			SoNode * const node,
			const float pointSize )
{ 
  SoFloatElement::set(classStackIndex,state, node, pointSize);
}

//! FIXME: write doc.

void 
SoPointSizeElement::set( SoState * const state, const float pointSize)
{ 
  set(state, NULL, pointSize); 
}

//! FIXME: write doc.

float 
SoPointSizeElement::get( SoState * const state )
{ 
  return SoFloatElement::get( classStackIndex, state ); 
}

//! FIXME: write doc.

float 
SoPointSizeElement::getDefault()
{ 
  return 0.0f; 
}

