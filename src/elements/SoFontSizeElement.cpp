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
  \class SoFontSizeElement Inventor/elements/SoFontSizeElement.h
  \brief The SoFontSizeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoFontSizeElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoFontSizeElement )

/*!
  \var SoFontSizeElement::classTypeId

  This is the static class type identifier for the
  SoFontSizeElement class.
*/

SoType SoFontSizeElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoFontSizeElement::getClassTypeId(void)
{
  return SoFontSizeElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoFontSizeElement class.
*/
void *
SoFontSizeElement::createInstance(void)
{
  return (void *) new SoFontSizeElement;
}

/*!
  \var SoFontSizeElement::classStackIndex

  This is the static state stack index for the
  SoFontSizeElement class.
*/
int SoFontSizeElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoFontSizeElement class.
*/
int
SoFontSizeElement::getClassStackIndex(void)
{
  return SoFontSizeElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoFontSizeElement
  class.
*/

void
SoFontSizeElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoFontSizeElement )
  assert(SoFontSizeElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoFontSizeElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoFontSizeElement",
    &SoFontSizeElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoFontSizeElement::classStackIndex =
      createStackIndex( SoFontSizeElement::classTypeId );
  } else {
    SoFontSizeElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoFontSizeElement class.
*/

void
SoFontSizeElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoFontSizeElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoFontSizeElement::createInstance( void )
*/

SoFontSizeElement::SoFontSizeElement( // protected
    void )
{
    setTypeId( SoFontSizeElement::classTypeId );
    setStackIndex( SoFontSizeElement::classStackIndex );
    this->data = getDefault();
} // SoFontSizeElement()

/*!
  The destructor.
*/

SoFontSizeElement::~SoFontSizeElement( // virtual protected
    void )
{
} // ~SoFontSizeElement()

//! FIXME: write doc.

void
SoFontSizeElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
  this->data = getDefault();
} // init()

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoFontSizeElement::set( SoState * const state, SoNode * const node,
			const float fontSize)
{ 
  SoFloatElement::set( classStackIndex, state, node, fontSize );
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoFontSizeElement::set(SoState * const state, const float fontSize)
{ 
  set( state, NULL, fontSize ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoFontSizeElement::get(SoState * const state)
{ 
  return SoFloatElement::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoFontSizeElement::getDefault()
{ 
  return 10.0f; 
}

