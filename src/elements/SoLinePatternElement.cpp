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
  \class SoLinePatternElement Inventor/elements/SoLinePatternElement.h
  \brief The SoLinePatternElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLinePatternElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoLinePatternElement )

/*!
  \var SoLinePatternElement::classTypeId

  This is the static class type identifier for the
  SoLinePatternElement class.
*/

SoType SoLinePatternElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoLinePatternElement::getClassTypeId(void)
{
  return SoLinePatternElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoLinePatternElement class.
*/
void *
SoLinePatternElement::createInstance(void)
{
  return (void *) new SoLinePatternElement;
}

/*!
  \var SoLinePatternElement::classStackIndex

  This is the static state stack index for the
  SoLinePatternElement class.
*/
int SoLinePatternElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoLinePatternElement class.
*/
int
SoLinePatternElement::getClassStackIndex(void)
{
  return SoLinePatternElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoLinePatternElement class.
*/

void
SoLinePatternElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoLinePatternElement )
  assert(SoLinePatternElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLinePatternElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoLinePatternElement",
    &SoLinePatternElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoLinePatternElement::classStackIndex =
      createStackIndex( SoLinePatternElement::classTypeId );
  } else {
    SoLinePatternElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoLinePatternElement class.
*/

void
SoLinePatternElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoLinePatternElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Canæt be used directly.

  \sa void * SoLinePatternElement::createInstance( void )
*/

SoLinePatternElement::SoLinePatternElement(
    void )
{
    setTypeId( SoLinePatternElement::classTypeId );
    setStackIndex( SoLinePatternElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoLinePatternElement::~SoLinePatternElement(
    void )
{
}

//! FIXME: write doc.

void
SoLinePatternElement::set( // static
    SoState * const state,
    SoNode * const node,
    const int32_t pattern )
{
    SoInt32Element::set( classStackIndex, state, node, pattern );
}

//! FIXME: write doc.

void
SoLinePatternElement::init( // virtual
    SoState * state )
{
  inherited::init( state );

  this->data = SoLinePatternElement::getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoLinePatternElement::set(SoState * const state, const int32_t pattern)
{ 
  set(state, NULL, pattern); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoLinePatternElement::get(SoState * const state)
{ 
  return SoInt32Element::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoLinePatternElement::getDefault()
{ 
  return CONTINUOUS; 
}

