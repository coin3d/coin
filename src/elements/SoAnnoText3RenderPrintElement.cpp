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
  \class SoAnnoText3RenderPrintElement Inventor/elements/SoAnnoText3RenderPrintElement.h
  \brief The SoAnnoText3RenderPrintElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoAnnoText3RenderPrintElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoAnnoText3RenderPrintElement::RenderPrintType

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoAnnoText3RenderPrintElement )

/*!
  \var SoAnnoText3RenderPrintElement::classTypeId

  This is the static class type identifier for the
  SoAnnoText3RenderPrintElement class.
*/

SoType SoAnnoText3RenderPrintElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoAnnoText3RenderPrintElement::getClassTypeId(void)
{
  return SoAnnoText3RenderPrintElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoAnnoText3RenderPrintElement class.
*/
void *
SoAnnoText3RenderPrintElement::createInstance(void)
{
  return (void *) new SoAnnoText3RenderPrintElement;
}

/*!
  \var SoAnnoText3RenderPrintElement::classStackIndex

  This is the static state stack index for the
  SoAnnoText3RenderPrintElement class.
*/
int SoAnnoText3RenderPrintElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoAnnoText3RenderPrintElement class.
*/
int
SoAnnoText3RenderPrintElement::getClassStackIndex(void)
{
  return SoAnnoText3RenderPrintElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoAnnoText3RenderPrintElement class.
*/

void
SoAnnoText3RenderPrintElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoAnnoText3RenderPrintElement )
  assert(SoAnnoText3RenderPrintElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoAnnoText3RenderPrintElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoAnnoText3RenderPrintElement",
    &SoAnnoText3RenderPrintElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoAnnoText3RenderPrintElement::classStackIndex =
      createStackIndex( SoAnnoText3RenderPrintElement::classTypeId );
  } else {
    SoAnnoText3RenderPrintElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoAnnoText3RenderPrintElement class.
*/

void
SoAnnoText3RenderPrintElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoAnnoText3RenderPrintElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoAnnoText3RenderPrintElement::createInstance()
*/

SoAnnoText3RenderPrintElement::SoAnnoText3RenderPrintElement( // protected
    void )
{
    setTypeId( SoAnnoText3RenderPrintElement::classTypeId );
    setStackIndex( SoAnnoText3RenderPrintElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoAnnoText3RenderPrintElement::~SoAnnoText3RenderPrintElement( // virtual protected
    void )
{
}

//! FIXME: write doc.

void
SoAnnoText3RenderPrintElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoAnnoText3RenderPrintElement::set(SoState * const state, SoNode * const node,
				const RenderPrintType type)
{ 
  SoInt32Element::set( classStackIndex,state,node,type );
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoAnnoText3RenderPrintElement::set(SoState * const state,
				   const RenderPrintType type)
{ 
  set( state, NULL, type ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3RenderPrintElement::RenderPrintType
SoAnnoText3RenderPrintElement::get(SoState * const state)
{ 
  return (SoAnnoText3RenderPrintElement::RenderPrintType)
    SoInt32Element::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3RenderPrintElement::RenderPrintType
SoAnnoText3RenderPrintElement::getDefault()
{ 
  return RENDER3D_PRINT_VECTOR; 
}

