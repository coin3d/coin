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
  \class SoComplexityElement Inventor/elements/SoComplexityElement.h
  \brief The SoComplexityElement class contains the current shape complexity
  for a graph traverser.
*/

#include <Inventor/elements/SoComplexityElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoComplexityElement )

/*!
  \var SoComplexityElement::classTypeId

  This is the static class type identifier for the
  SoComplexityElement class.
*/

SoType SoComplexityElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoComplexityElement::getClassTypeId(void)
{
  return SoComplexityElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoComplexityElement class.
*/
void *
SoComplexityElement::createInstance(void)
{
  return (void *) new SoComplexityElement;
}

/*!
  \var SoComplexityElement::classStackIndex

  This is the static state stack index for the
  SoComplexityElement class.
*/
int SoComplexityElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoComplexityElement class.
*/
int
SoComplexityElement::getClassStackIndex(void)
{
  return SoComplexityElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoComplexityElement
  class.
*/

void
SoComplexityElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoComplexityElement )
  assert(SoComplexityElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoComplexityElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoComplexityElement",
    &SoComplexityElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoComplexityElement::classStackIndex =
      createStackIndex( SoComplexityElement::classTypeId );
  } else {
    SoComplexityElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoComplexityElement class.
*/

void
SoComplexityElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoComplexityElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoComplexityElement::createInstance( void )
*/

SoComplexityElement::SoComplexityElement( // protected
    void )
{
    setTypeId( SoComplexityElement::classTypeId );
    setStackIndex( SoComplexityElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoComplexityElement::~SoComplexityElement( // virtual protected
    void )
{
}

//! FIXME: write doc.

void
SoComplexityElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoComplexityElement::set(SoState * const state, SoNode * const node,
			 const float complexity)
{ 
  SoFloatElement::set(classStackIndex, state, node, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoComplexityElement::set(SoState * const state, const float complexity)
{ 
  set(state, NULL, complexity); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoComplexityElement::get(SoState * const state)
{ 
  return SoFloatElement::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoComplexityElement::getDefault()
{ 
  return 0.5f; 
}

