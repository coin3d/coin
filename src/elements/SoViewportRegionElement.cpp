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
  \class SoViewportRegionElement Inventor/elements/SoViewportRegionElement.h
  \brief The SoViewportRegionElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoViewportRegionElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

// static variables
SbViewportRegion * SoViewportRegionElement::emptyViewportRegion;

/*!
  \fn SoViewportRegionElement::viewportRegion

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoViewportRegionElement )

/*!
  \var SoViewportRegionElement::classTypeId

  This is the static class type identifier for the
  SoViewportRegionElement class.
*/

SoType SoViewportRegionElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoViewportRegionElement::getClassTypeId(void)
{
  return SoViewportRegionElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoViewportRegionElement class.
*/
void *
SoViewportRegionElement::createInstance(void)
{
  return (void *) new SoViewportRegionElement;
}

/*!
  \var SoViewportRegionElement::classStackIndex

  This is the static state stack index for the
  SoViewportRegionElement class.
*/
int SoViewportRegionElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoViewportRegionElement class.
*/
int
SoViewportRegionElement::getClassStackIndex(void)
{
  return SoViewportRegionElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoViewportregionElement
  class.
*/

void
SoViewportRegionElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoViewportRegionElement )
  assert(SoViewportRegionElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoViewportRegionElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoViewportRegionElement",
    &SoViewportRegionElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoViewportRegionElement::classStackIndex =
      createStackIndex( SoViewportRegionElement::classTypeId );
  } else {
    SoViewportRegionElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoViewportRegionElement
  class.
*/

void
SoViewportRegionElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoViewportRegionElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoViewportRegionElement::createInstance( void )
*/

SoViewportRegionElement::SoViewportRegionElement(
    void )
  : viewportRegion()
{
    setTypeId( SoViewportRegionElement::classTypeId );
    setStackIndex( SoViewportRegionElement::classStackIndex );
}

/*!
  The destructor.
*/

SoViewportRegionElement::~SoViewportRegionElement( // virtual
    void )
{
}

//! FIXME: write doc.

void
SoViewportRegionElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

SbBool
SoViewportRegionElement::matches(const SoElement * element) const
{
  return this->viewportRegion == 
    ((SoViewportRegionElement*) element)->viewportRegion;
}

//! FIXME: write doc.

SoElement *
SoViewportRegionElement::copyMatchInfo() const
{
  SoViewportRegionElement * elem = (SoViewportRegionElement*) 
    getTypeId().createInstance();
  elem->viewportRegion = this->viewportRegion;
  return elem;
}
 
//! FIXME: write doc.

void
SoViewportRegionElement::set(SoState * const state,
			     const SbViewportRegion &viewportRegion)
{
  SoViewportRegionElement *elem = (SoViewportRegionElement*)
    SoElement::getElement(state, classStackIndex);
  elem->setElt(viewportRegion);
}

//! FIXME: write doc.

const SbViewportRegion &
SoViewportRegionElement::get(SoState * const state)
{
  SoViewportRegionElement *elem = (SoViewportRegionElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->viewportRegion;
}
 
//! FIXME: write doc.

void
SoViewportRegionElement::setElt(const SbViewportRegion & viewportRegion)
{
  this->viewportRegion = viewportRegion;
}

//! FIXME: write doc.

void
SoViewportRegionElement::print( // virtual
    FILE * file ) const
{
  fprintf( file, "SoViewportRegionElement[%p]\n", this );
}

