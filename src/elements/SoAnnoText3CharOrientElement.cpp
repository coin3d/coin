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
  \class SoAnnoText3CharOrientElement Inventor/elements/SoAnnoText3CharOrientElement.h
  \brief The SoAnnoText3CharOrientElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoAnnoText3CharOrientElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoAnnoText3CharOrientElement )

/*!
  \var SoAnnoText3CharOrientElement::classTypeId

  This is the static class type identifier for the
  SoAnnoText3CharOrientElement class.
*/

SoType SoAnnoText3CharOrientElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoAnnoText3CharOrientElement::getClassTypeId(void)
{
  return SoAnnoText3CharOrientElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoAnnoText3CharOrientElement class.
*/
void *
SoAnnoText3CharOrientElement::createInstance(void)
{
  return (void *) new SoAnnoText3CharOrientElement;
}

/*!
  \var SoAnnoText3CharOrientElement::classStackIndex

  This is the static state stack index for the
  SoAnnoText3CharOrientElement class.
*/
int SoAnnoText3CharOrientElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoAnnoText3CharOrientElement class.
*/
int
SoAnnoText3CharOrientElement::getClassStackIndex(void)
{
  return SoAnnoText3CharOrientElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoAnnoText3CharOrientElement class.
*/

void
SoAnnoText3CharOrientElement::initClass(
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoAnnoText3CharOrientElement )
  assert(SoAnnoText3CharOrientElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoAnnoText3CharOrientElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoAnnoText3CharOrientElement",
    &SoAnnoText3CharOrientElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoAnnoText3CharOrientElement::classStackIndex =
      createStackIndex( SoAnnoText3CharOrientElement::classTypeId );
  } else {
    SoAnnoText3CharOrientElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoAnnoText3CharOrientElement class.
*/

void
SoAnnoText3CharOrientElement::cleanClass(
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoAnnoText3CharOrientElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoAnnoText3CharOrientElement::createInstance()
*/

SoAnnoText3CharOrientElement::SoAnnoText3CharOrientElement(
    void )
{
    setTypeId( SoAnnoText3CharOrientElement::classTypeId );
    setStackIndex( SoAnnoText3CharOrientElement::classStackIndex );
}

/*!
  The destructor.
*/

SoAnnoText3CharOrientElement::~SoAnnoText3CharOrientElement( // virtual protected
    void )
{
}

//! FIXME: write doc.

void
SoAnnoText3CharOrientElement::init(
    SoState * state )
{
  inherited::init( state );
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoAnnoText3CharOrientElement::set(SoState * const state, SbBool isOriented)
{ 
  inherited::set( classStackIndex, state, isOriented ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoAnnoText3CharOrientElement::get(SoState * state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoAnnoText3CharOrientElement::getDefault(void)
{
  return TRUE;
}

