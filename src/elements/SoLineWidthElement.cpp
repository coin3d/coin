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
  \class SoLineWidthElement Inventor/elements/SoLineWidthElement.h
  \brief The SoLineWidthElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLineWidthElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoLineWidthElement )

/*!
  \var SoLineWidthElement::classTypeId

  This is the static class type identifier for the
  SoLineWidthElement class.
*/

SoType SoLineWidthElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoLineWidthElement::getClassTypeId(void)
{
  return SoLineWidthElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoLineWidthElement class.
*/
void *
SoLineWidthElement::createInstance(void)
{
  return (void *) new SoLineWidthElement;
}

/*!
  \var SoLineWidthElement::classStackIndex

  This is the static state stack index for the
  SoLineWidthElement class.
*/
int SoLineWidthElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoLineWidthElement class.
*/
int
SoLineWidthElement::getClassStackIndex(void)
{
  return SoLineWidthElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoLineWidthElement class.
*/

void
SoLineWidthElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoLineWidthElement )
  assert(SoLineWidthElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLineWidthElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoLineWidthElement",
    &SoLineWidthElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoLineWidthElement::classStackIndex =
      createStackIndex( SoLineWidthElement::classTypeId );
  } else {
    SoLineWidthElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoLineWidthElement class.
*/

void
SoLineWidthElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoLineWidthElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoLineWidthElement::createInstance( void )
*/

SoLineWidthElement::SoLineWidthElement( // protected
    void )
{
    setTypeId( SoLineWidthElement::classTypeId );
    setStackIndex( SoLineWidthElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoLineWidthElement::~SoLineWidthElement( // virtual protected
    void )
{
}

//! FIXME: write doc.

void
SoLineWidthElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
  this->data = SoLineWidthElement::getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoLineWidthElement::set(SoState * const state, SoNode * const node,
			const float lineWidth)
{ 
  SoFloatElement::set(classStackIndex,state, node, lineWidth);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoLineWidthElement::set(SoState * const state, const float lineWidth)
{ 
  set( state, NULL, lineWidth ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoLineWidthElement::get(SoState * const state)
{ 
  return SoFloatElement::get(classStackIndex, state); 
}
 
//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoLineWidthElement::getDefault()
{ 
  return 0.0f; 
}

