/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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


#include <assert.h>

// static variables
SbViewportRegion * SoViewportRegionElement::emptyViewportRegion;

/*!
  \fn SoViewportRegionElement::viewportRegion

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoViewportRegionElement);

/*!
  This static method initializes static data for the SoViewportregionElement
  class.
*/

void
SoViewportRegionElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoViewportRegionElement, inherited);
}

/*!
  The destructor.
*/

SoViewportRegionElement::~SoViewportRegionElement(void)
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
SoViewportRegionElement::print(FILE * file) const
{
  fprintf(file, "SoViewportRegionElement[%p]\n", this);
}
