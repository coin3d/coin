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
  \class SoFontNameElement Inventor/elements/SoFontNameElement.h
  \brief The SoFontNameElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoFontNameElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoFontNameElement::defaultFontName

  FIXME: write doc.
*/

const SbName SoFontNameElement::defaultFontName( "defaultFont" );

/*!
  \fn SoFontNameElement::fontName

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoFontNameElement )

/*!
  \var SoFontNameElement::classTypeId

  This is the static class type identifier for the
  SoFontNameElement class.
*/

SoType SoFontNameElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoFontNameElement::getClassTypeId(void)
{
  return SoFontNameElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoFontNameElement class.
*/
void *
SoFontNameElement::createInstance(void)
{
  return (void *) new SoFontNameElement;
}

/*!
  \var SoFontNameElement::classStackIndex

  This is the static state stack index for the
  SoFontNameElement class.
*/
int SoFontNameElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoFontNameElement class.
*/
int
SoFontNameElement::getClassStackIndex(void)
{
  return SoFontNameElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoFontNameElement
  class.
*/

void
SoFontNameElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoFontNameElement )
  assert(SoFontNameElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoFontNameElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoFontNameElement",
    &SoFontNameElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoFontNameElement::classStackIndex =
      createStackIndex( SoFontNameElement::classTypeId );
  } else {
    SoFontNameElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoFontNameElement class.
*/

void
SoFontNameElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoFontNameElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoFontNameElement::createInstance( void )
*/

SoFontNameElement::SoFontNameElement(
    void )
  : fontName( defaultFontName )
{
  setTypeId( SoFontNameElement::classTypeId );
  setStackIndex( SoFontNameElement::classStackIndex );
} // SoFontNameElement()

/*!
  The destructor.
*/

SoFontNameElement::~SoFontNameElement()
{
} // ~SoFontNameElement()

//! FIXME: write doc.

void
SoFontNameElement::set(SoState * const state,
		       SoNode * const node,
		       const SbName fontName)
{
  SoFontNameElement * element = (SoFontNameElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->fontName = fontName;
} // set()

//! FIXME: write doc.

const SbName
SoFontNameElement::get( // static
    SoState * const state )
{
  SoFontNameElement * element = (SoFontNameElement *)
    SoElement::getConstElement( state, classStackIndex);
  return element->fontName;
} // get()

//! FIXME: write doc.

SbBool
SoFontNameElement::matches( // virtual
    SoElement * element ) const
{
  if ( (SoElement *)this == element )
    return TRUE;
  if (element->getTypeId() != SoFontNameElement::getClassTypeId())
    return FALSE;
  if (this->fontName != ((SoFontNameElement *)element)->fontName)
    return FALSE;
  return TRUE;
} // matches()

//! FIXME: write doc.

SoElement *
SoFontNameElement::copyMatchInfo( // virtual
    void ) const
{
  SoFontNameElement * element = (SoFontNameElement *)
    (SoFontNameElement::getClassTypeId().createInstance());
  element->fontName = this->fontName;
  element->nodeId = this->nodeId;
  return element;
} // copyMatchInfo()

//! FIXME: write doc.

void
SoFontNameElement::print(FILE * file) const
{
  fprintf( file, "SoFontNameElement[%p]: font = %s\n", this,
	   this->fontName.getString() );
} // print()

//! FIXME: write doc.

void
SoFontNameElement::init( // virtual
    SoState * state )
{
  inherited::init(state);
  fontName = defaultFontName;
} // init()

//! FIXME: write doc.

const SbName 
SoFontNameElement::getDefault()
{ 
  return SoFontNameElement::defaultFontName; 
}

