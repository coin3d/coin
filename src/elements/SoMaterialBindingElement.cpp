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
  \class SoMaterialBindingElement Inventor/elements/SoMaterialBindingElement.h
  \brief The SoMaterialBindingElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoMaterialBindingElement.h>

#include <Inventor/SbName.h>
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif

#include <assert.h>

/*!
  \fn SoMaterialBindingElement::Binding

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoMaterialBindingElement )

/*!
  \var SoMaterialBindingElement::classTypeId

  This is the static class type identifier for the
  SoMaterialBindingElement class.
*/

SoType SoMaterialBindingElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoMaterialBindingElement::getClassTypeId(void)
{
  return SoMaterialBindingElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoMaterialBindingElement class.
*/
void *
SoMaterialBindingElement::createInstance(void)
{
  return (void *) new SoMaterialBindingElement;
}

/*!
  \var SoMaterialBindingElement::classStackIndex

  This is the static state stack index for the
  SoMaterialBindingElement class.
*/
int SoMaterialBindingElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoMaterialBindingElement class.
*/
int
SoMaterialBindingElement::getClassStackIndex(void)
{
  return SoMaterialBindingElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoMaterialBindingElement class.
*/

void
SoMaterialBindingElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoMaterialBindingElement )
  assert(SoMaterialBindingElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMaterialBindingElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoMaterialBindingElement",
    &SoMaterialBindingElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoMaterialBindingElement::classStackIndex =
      createStackIndex( SoMaterialBindingElement::classTypeId );
  } else {
    SoMaterialBindingElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoMaterialBindingElement class.
*/

void
SoMaterialBindingElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoMaterialBindingElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoMaterialBindingElement::createInstance( void )
*/

SoMaterialBindingElement::SoMaterialBindingElement(
    void )
{
    setTypeId( SoMaterialBindingElement::classTypeId );
    setStackIndex( SoMaterialBindingElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoMaterialBindingElement::~SoMaterialBindingElement(
    void )
{
}

//! FIXME: write doc.

void
SoMaterialBindingElement::set(SoState * const state,
			      SoNode * const node,
			      const Binding binding)
{
  assert((int)binding >= (int)OVERALL && 
	 (int)binding <= (int)PER_VERTEX_INDEXED);
  SoInt32Element::set(classStackIndex, state, node, binding);

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  SoGLShadeModelElement::setMaterial(state,
				     binding == PER_VERTEX ||
				     binding == PER_VERTEX_INDEXED);
#endif
}

//! FIXME: write doc.

void
SoMaterialBindingElement::init( // virtual
    SoState * state )
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoMaterialBindingElement::set(SoState * const state, const Binding binding)
{ 
  set(state, NULL, binding); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoMaterialBindingElement::Binding 
SoMaterialBindingElement::get(SoState * const state)
{ 
  return (Binding) SoInt32Element::get(classStackIndex, state); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoMaterialBindingElement::Binding 
SoMaterialBindingElement::getDefault()
{ 
  return DEFAULT; 
}

