/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoPolygonOffsetElement Inventor/elements/SoPolygonOffsetElement.h
  \brief The SoPolygonOffsetElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoPolygonOffsetElement.h>
#include <assert.h>

/*!
  \fn SoPolygonOffsetElement::Style

  FIXME: write doc.
*/

/*!
  \fn SoPolygonOffsetElement::style

  FIXME: write doc.
*/

/*!
  \fn SoPolygonOffsetElement::active

  FIXME: write doc.
*/

/*!
  \fn SoPolygonOffsetElement::offsetfactor

  FIXME: write doc.
*/

/*!
  \fn SoPolygonOffsetElement::offsetunits

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoPolygonOffsetElement);

/*!
  This static method initializes static data for the SoPolygonOffsetElement
  class.
*/

void
SoPolygonOffsetElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoPolygonOffsetElement, inherited);
}

/*!
  The destructor.
*/

SoPolygonOffsetElement::~SoPolygonOffsetElement(void)
{
}

//! FIXME: write doc.

void
SoPolygonOffsetElement::init(SoState * state)
{
  inherited::init(state);
  SoPolygonOffsetElement::getDefault(this->offsetfactor,
                                     this->offsetunits,
                                     this->style,
                                     this->active);
}

//! FIXME: write doc.

void
SoPolygonOffsetElement::set(SoState * state, SoNode * node,
                            float factor, float units, Style styles, SbBool on)
{
  SoPolygonOffsetElement *elem = (SoPolygonOffsetElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (elem) {
    elem->setElt(factor, units, styles, on);
  }
}

//! FIXME: write doc.

void
SoPolygonOffsetElement::get(SoState * state, float & factor, float & units,
                            Style & styles, SbBool & on)
{
  const SoPolygonOffsetElement *elem = (SoPolygonOffsetElement *)
    SoElement::getConstElement(state, classStackIndex);

  factor = elem->offsetfactor;
  units = elem->offsetunits;
  styles = elem->style;
  on = elem->active;
}

//! FIXME: write doc.

void
SoPolygonOffsetElement::setElt(float factor, float units,
                               Style styles, SbBool on)
{
  this->offsetfactor = factor;
  this->offsetunits = units;
  this->style = styles;
  this->active = on;
}

//! FIXME: write doc.

void
SoPolygonOffsetElement::getDefault(float & factor, float & units,
                                   Style & styles, SbBool & on)
{
  factor = 0.0f;
  units = 0.0f;
  styles = SoPolygonOffsetElement::FILLED;
  on = FALSE;
}
