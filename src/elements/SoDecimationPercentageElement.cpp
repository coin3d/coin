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
  \class SoDecimationPercentageElement Inventor/elements/SoDecimationPercentageElement.h
  \brief The SoDecimationPercentageElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDecimationPercentageElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoDecimationPercentageElement )

/*!
  \var SoDecimationPercentageElement::classTypeId

  This is the static class type identifier for the
  SoDecimationPercentageElement class.
*/

SoType SoDecimationPercentageElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoDecimationPercentageElement::getClassTypeId(void)
{
  return SoDecimationPercentageElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoDecimationPercentageElement class.
*/
void *
SoDecimationPercentageElement::createInstance(void)
{
  return (void *) new SoDecimationPercentageElement;
}

/*!
  \var SoDecimationPercentageElement::classStackIndex

  This is the static state stack index for the
  SoDecimationPercentageElement class.
*/
int SoDecimationPercentageElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoDecimationPercentageElement class.
*/
int
SoDecimationPercentageElement::getClassStackIndex(void)
{
  return SoDecimationPercentageElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoDecimationPercentageElement class.
*/

void
SoDecimationPercentageElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoDecimationPercentageElement )
  assert(SoDecimationPercentageElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoDecimationPercentageElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoDecimationPercentageElement",
    &SoDecimationPercentageElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoDecimationPercentageElement::classStackIndex =
      createStackIndex( SoDecimationPercentageElement::classTypeId );
  } else {
    SoDecimationPercentageElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoDecimationPercentageElement class.
*/

void
SoDecimationPercentageElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoDecimationPercentageElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoDecimationPercentageElement::createInstance( void )
*/

SoDecimationPercentageElement::SoDecimationPercentageElement( // protected
    void )
{
    setTypeId( SoDecimationPercentageElement::classTypeId );
    setStackIndex( SoDecimationPercentageElement::classStackIndex );
    this->data = getDefault();
} // SoDecimationPercentageElement()

/*!
  The destructor.
*/

SoDecimationPercentageElement::~SoDecimationPercentageElement( // virtual protected
    void )
{
} // ~SoDecimationPercentageElement()

//! FIXME: write doc.

void
SoDecimationPercentageElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
  this->data = getDefault();
} // init()

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoDecimationPercentageElement::set(SoState * const state, SoNode * const node,
			  const float complexity)
{ 
  SoFloatElement::set( classStackIndex,state,node, complexity );
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoDecimationPercentageElement::set(SoState * const state, const float complexity)
{ 
  set( state, NULL, complexity ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoDecimationPercentageElement::get(SoState * const state)
{ 
  return SoFloatElement::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
float 
SoDecimationPercentageElement::getDefault()
{ 
  return 1.0f; 
}

