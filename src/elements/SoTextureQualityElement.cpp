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
  \class SoTextureQualityElement Inventor/elements/SoTextureQualityElement.h
  \brief The SoTextureQualityElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureQualityElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoTextureQualityElement )

/*!
  \var SoTextureQualityElement::classTypeId

  This is the static class type identifier for the
  SoTextureQualityElement class.
*/

SoType SoTextureQualityElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoTextureQualityElement::getClassTypeId(void)
{
  return SoTextureQualityElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoTextureQualityElement class.
*/
void *
SoTextureQualityElement::createInstance(void)
{
  return (void *) new SoTextureQualityElement;
}

/*!
  \var SoTextureQualityElement::classStackIndex

  This is the static state stack index for the
  SoTextureQualityElement class.
*/
int SoTextureQualityElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoTextureQualityElement class.
*/
int
SoTextureQualityElement::getClassStackIndex(void)
{
  return SoTextureQualityElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoTextureQualityElement class.
*/

void
SoTextureQualityElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoTextureQualityElement )
  assert(SoTextureQualityElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextureQualityElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoTextureQualityElement",
    &SoTextureQualityElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoTextureQualityElement::classStackIndex =
      createStackIndex( SoTextureQualityElement::classTypeId );
  } else {
    SoTextureQualityElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoTextureQualityElement class.
*/

void
SoTextureQualityElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoTextureQualityElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoTextureQualityElement::createInstance( void )
*/

SoTextureQualityElement::SoTextureQualityElement( // protected
    void )
{
    setTypeId( SoTextureQualityElement::classTypeId );
    setStackIndex( SoTextureQualityElement::classStackIndex );
    this->data = getDefault();
} // SoTextureQualityElement()

/*!
  The destructor.
*/

SoTextureQualityElement::~SoTextureQualityElement( // virtual protected
    void )
{
} // ~SoTextureQualityElement()

//! FIXME: write doc.

void
SoTextureQualityElement::init( // virtual
    SoState * state )
{
  inherited::init( state );

  this->data = SoTextureQualityElement::getDefault();
} // init()

//! FIXME: write doc.

void
SoTextureQualityElement::push( // virtual
    SoState * state )
{
    inherited::push( state );
} // push()

//! FIXME: write doc.

void
SoTextureQualityElement::pop( // virtual
    SoState * state,
    const SoElement * prevTopElement )
{
    inherited::pop( state, prevTopElement );
} // pop()

/*!
  FIXME: write doc.
*/

void
SoTextureQualityElement::set(
  SoState * const state, SoNode * const node, const float quality )
{
  SoFloatElement::set( classStackIndex, state, node, quality );
}

/*!
  FIXME: write doc.
*/

void
SoTextureQualityElement::set(
  SoState * const state, const float quality )
{
  set( state, NULL, quality );
}


/*!
  FIXME: write doc.
*/

float
SoTextureQualityElement::get(
  SoState * const state )
{
  return SoFloatElement::get( classStackIndex, state );
}

/*!
  FIXME: write doc.
*/

float
SoTextureQualityElement::getDefault( void )
{
  return 0.5f;
}

