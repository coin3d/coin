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
  \class SoCreaseAngleElement Inventor/elements/SoCreaseAngleElement.h
  \brief The SoCreaseAngleElement class stores the crease angle during a
  scene graph traversal.
*/

#include <Inventor/elements/SoCreaseAngleElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoCreaseAngleElement )

/*!
  \var SoCreaseAngleElement::classTypeId

  This is the static class type identifier for the
  SoCreaseAngleElement class.
*/

SoType SoCreaseAngleElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoCreaseAngleElement::getClassTypeId(void)
{
  return SoCreaseAngleElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoCreaseAngleElement class.
*/
void *
SoCreaseAngleElement::createInstance(void)
{
  return (void *) new SoCreaseAngleElement;
}

/*!
  \var SoCreaseAngleElement::classStackIndex

  This is the static state stack index for the
  SoCreaseAngleElement class.
*/
int SoCreaseAngleElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoCreaseAngleElement class.
*/
int
SoCreaseAngleElement::getClassStackIndex(void)
{
  return SoCreaseAngleElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoCreaseAngleElement class.
*/

void
SoCreaseAngleElement::initClass(
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoCreaseAngleElement )
  assert(SoCreaseAngleElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoCreaseAngleElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoCreaseAngleElement",
    &SoCreaseAngleElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoCreaseAngleElement::classStackIndex =
      createStackIndex( SoCreaseAngleElement::classTypeId );
  } else {
    SoCreaseAngleElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoCreaseAngleElement class.
*/

void
SoCreaseAngleElement::cleanClass(
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoCreaseAngleElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoCreaseAngleElement::createInstance( void )
*/

SoCreaseAngleElement::SoCreaseAngleElement(
    void )
{
    setTypeId( SoCreaseAngleElement::classTypeId );
    setStackIndex( SoCreaseAngleElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoCreaseAngleElement::~SoCreaseAngleElement( // virtual protected
    void )
{
}

//! FIXME: write doc.

void
SoCreaseAngleElement::init(
    SoState * state )
{
  inherited::init( state );
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoCreaseAngleElement::set(SoState * const state, SoNode * const node,
			  const float complexity)
{ 
  SoFloatElement::set( classStackIndex,state,node, complexity );
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoCreaseAngleElement::set(SoState * const state, const float complexity)
{ 
  set( state, NULL, complexity ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoCreaseAngleElement::get(SoState * const state)
{ 
  return SoFloatElement::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoCreaseAngleElement::getDefault()
{ 
  return 0.0f; 
}

