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
  \class SoDecimationTypeElement Inventor/elements/SoDecimationTypeElement.h
  \brief The SoDecimationTypeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDecimationTypeElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoDecimationTypeElement::Type

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoDecimationTypeElement )

/*!
  \var SoDecimationTypeElement::classTypeId

  This is the static class type identifier for the
  SoDecimationTypeElement class.
*/

SoType SoDecimationTypeElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoDecimationTypeElement::getClassTypeId(void)
{
  return SoDecimationTypeElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoDecimationTypeElement class.
*/
void *
SoDecimationTypeElement::createInstance(void)
{
  return (void *) new SoDecimationTypeElement;
}

/*!
  \var SoDecimationTypeElement::classStackIndex

  This is the static state stack index for the
  SoDecimationTypeElement class.
*/
int SoDecimationTypeElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoDecimationTypeElement class.
*/
int
SoDecimationTypeElement::getClassStackIndex(void)
{
  return SoDecimationTypeElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoDecimationTypeElement class.
*/

void
SoDecimationTypeElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoDecimationTypeElement )
  assert(SoDecimationTypeElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoDecimationTypeElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoDecimationTypeElement",
    &SoDecimationTypeElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoDecimationTypeElement::classStackIndex =
      createStackIndex( SoDecimationTypeElement::classTypeId );
  } else {
    SoDecimationTypeElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoDecimationTypeElement class.
*/

void
SoDecimationTypeElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoDecimationTypeElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoDecimationTypeElement::createInstance( void )
*/

SoDecimationTypeElement::SoDecimationTypeElement(
    void )
{
    setTypeId( SoDecimationTypeElement::classTypeId );
    setStackIndex( SoDecimationTypeElement::classStackIndex );
    this->data = getDefault();
} // SoDecimationTypeElement()

/*!
  The destructor.
*/

SoDecimationTypeElement::~SoDecimationTypeElement(
    void )
{
} // ~SoDecimationTypeElement()

//! FIXME: write doc.

void
SoDecimationTypeElement::set( // static
    SoState * const state,
    SoNode * const node,
    const Type type )
{
  assert((int)type >= (int)AUTOMATIC && (int)type <= (int)PERCENTAGE);
  SoInt32Element::set(classStackIndex, state, node, type);
} // set()

//! FIXME: write doc.

void
SoDecimationTypeElement::init( // virtual
    SoState * state )
{
    inherited::init( state );
} // init()

//! FIXME: write doc.

void
SoDecimationTypeElement::print(FILE *fp) const
{
  assert(0 && "FIXME: not implemented");
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoDecimationTypeElement::set(SoState * const state, const Type type)
{ 
  set(state, NULL, type); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDecimationTypeElement::Type
SoDecimationTypeElement::get(SoState * const state)
{ 
  return (Type) SoInt32Element::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDecimationTypeElement::Type 
SoDecimationTypeElement::getDefault()
{ 
  return AUTOMATIC; 
}

