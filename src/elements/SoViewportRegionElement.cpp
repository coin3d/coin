/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoViewportRegionElement Inventor/elements/SoViewportRegionElement.h
  \brief The SoViewportRegionElement class is yet to be documented.
  \ingroup elements

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
  if (elem) {
    elem->setElt(viewportRegion);
  }
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
