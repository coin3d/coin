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
  \class SoGLRenderPassElement Inventor/elements/SoGLRenderPassElement.h
  \brief The SoGLRenderPassElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLRenderPassElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoGLRenderPassElement::passnr

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoGLRenderPassElement )

/*!
  \var SoGLRenderPassElement::classTypeId

  This is the static class type identifier for the
  SoGLRenderPassElement class.
*/

SoType SoGLRenderPassElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLRenderPassElement::getClassTypeId(void)
{
  return SoGLRenderPassElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLRenderPassElement class.
*/
void *
SoGLRenderPassElement::createInstance(void)
{
  return (void *) new SoGLRenderPassElement;
}

/*!
  \var SoGLRenderPassElement::classStackIndex

  This is the static state stack index for the
  SoGLRenderPassElement class.
*/
int SoGLRenderPassElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLRenderPassElement class.
*/
int
SoGLRenderPassElement::getClassStackIndex(void)
{
  return SoGLRenderPassElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLRenderPassElement class.
*/

void
SoGLRenderPassElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLRenderPassElement )
  assert(SoGLRenderPassElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLRenderPassElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLRenderPassElement",
    &SoGLRenderPassElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLRenderPassElement::classStackIndex =
      createStackIndex( SoGLRenderPassElement::classTypeId );
  } else {
    SoGLRenderPassElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoGLRenderPassElement class.
*/

void
SoGLRenderPassElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLRenderPassElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLRenderPassElement::createInstance( void )
*/

SoGLRenderPassElement::SoGLRenderPassElement( // protected
					     void )
  : passnr( 0 )
{
  setTypeId( SoGLRenderPassElement::classTypeId );
  setStackIndex( SoGLRenderPassElement::classStackIndex );
} // SoGLRenderPassElement()

/*!
  The destructor.
*/

SoGLRenderPassElement::~SoGLRenderPassElement( // virtual
					      void )
{
} // ~SoGLRenderPassElement()

//! FIXME: write doc.

void
SoGLRenderPassElement::init( // virtual
			    SoState * state )
{
  inherited::init( state );
  this->passnr = 0;
} // init()

//! FIXME: write doc.

void
SoGLRenderPassElement::push( // virtual
			    SoState * state )
{
  inherited::push( state );
} // push()

//! FIXME: write doc.

void
SoGLRenderPassElement::pop( // virtual
			   SoState * state,
			   const SoElement * prevTopElement )
{
  inherited::pop( state, prevTopElement );
} // pop()

//! FIXME: write doc.

SbBool
SoGLRenderPassElement::matches( // virtual
			       const SoElement * element ) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
} // matches()

//! FIXME: write doc.

SoElement *
SoGLRenderPassElement::copyMatchInfo( // virtual
				     void ) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
} // copyMatchInfo()

//! FIXME: write doc.

void
SoGLRenderPassElement::set( // static
			   SoState * const state,
			   const int pass )
{
  SoGLRenderPassElement * element = (SoGLRenderPassElement *)
    getElement(state, classStackIndex);
  element->passnr = pass;
} // set()

//! FIXME: write doc.

int
SoGLRenderPassElement::get( // static
			   SoState * const state )
{
  const SoGLRenderPassElement * element = (const SoGLRenderPassElement *)
    getConstElement(state, classStackIndex);
  return element->passnr;
} // get()

//! FIXME: write doc.

void
SoGLRenderPassElement::print( // virtual
			     FILE * file ) const
{
  assert(0 && "FIXME: not implemented");
} // print()

