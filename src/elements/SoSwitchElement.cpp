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
  \class SoSwitchElement Inventor/elements/SoSwitchElement.h
  \brief The SoSwitchElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoSwitchElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoSwitchElement )

/*!
  \var SoSwitchElement::classTypeId

  This is the static class type identifier for the
  SoSwitchElement class.
*/

SoType SoSwitchElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoSwitchElement::getClassTypeId(void)
{
  return SoSwitchElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoSwitchElement class.
*/
void *
SoSwitchElement::createInstance(void)
{
  return (void *) new SoSwitchElement;
}

/*!
  \var SoSwitchElement::classStackIndex

  This is the static state stack index for the
  SoSwitchElement class.
*/
int SoSwitchElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoSwitchElement class.
*/
int
SoSwitchElement::getClassStackIndex(void)
{
  return SoSwitchElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoSwitchElement class.
*/

void
SoSwitchElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoSwitchElement )
  assert(SoSwitchElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSwitchElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoSwitchElement",
    &SoSwitchElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoSwitchElement::classStackIndex =
      createStackIndex( SoSwitchElement::classTypeId );
  } else {
    SoSwitchElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoSwitchElement class.
*/

void
SoSwitchElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoSwitchElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoSwitchElement::createInstance( void )
*/

SoSwitchElement::SoSwitchElement(
    void )
{
    setTypeId( SoSwitchElement::classTypeId );
    setStackIndex( SoSwitchElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoSwitchElement::~SoSwitchElement(
    void )
{
}

//! FIXME: write doc.

void
SoSwitchElement::set(SoState * const state,
		     SoNode * const node,
		     const int32_t index)
{
  SoInt32Element::set(classStackIndex, state, node, index);
}

//! FIXME: write doc.

void
SoSwitchElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoSwitchElement::set(SoState * const state, const int32_t index)
{ 
  set(state, NULL, index); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoSwitchElement::get(SoState * const state)
{ 
  return SoInt32Element::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoSwitchElement::getDefault()
{ 
  return -1; 
}

