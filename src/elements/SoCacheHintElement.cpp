/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoShapehintsElement Inventor/elements/SoCacheHintElement.h
  \brief The SoCacheHintElement class is yet to be documented.
  \ingroup elements

  Please note that this is an experimental class. The API might change
  a lot before/if it's included in any official Coin release.
*/

#include <Inventor/elements/SoCacheHintElement.h>
#include <Inventor/elements/SoLazyElement.h>

#include <assert.h>

class SoCacheHintElementP {
public:
  float value;
};

#undef PRIVATE
#define PRIVATE(obj) obj->pimpl

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoCacheHintElement);

/*!
  This static method initializes static data for the SoCacheHintElement
  class.
*/

void
SoCacheHintElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoCacheHintElement, inherited);
}


SoCacheHintElement::SoCacheHintElement(void)
{
  PRIVATE(this) = new SoCacheHintElementP;
  this->setTypeId(SoCacheHintElement::classTypeId);
  this->setStackIndex(SoCacheHintElement::classStackIndex);
}

/*!
  The destructor.
*/

SoCacheHintElement::~SoCacheHintElement(void)
{
  delete PRIVATE(this);
}

//! FIXME: write doc.

void
SoCacheHintElement::init(SoState * state)
{
  inherited::init(state);
//   this->vertexOrdering = getDefaultVertexOrdering();
//   this->shapeType = getDefaultShapeType();
//   this->faceType = getDefaultFaceType();
}

//! FIXME: write doc.

void
SoCacheHintElement::push(SoState * state)
{
  inherited::push(state);
  SoCacheHintElement * prev = (SoCacheHintElement*) this->getNextInStack();
  PRIVATE(this)->value = PRIVATE(prev)->value;
}

void
SoCacheHintElement::pop(SoState * state, const SoElement * prevtopelement)
{
  // nothing to do?
}

//! FIXME: write doc.

SbBool
SoCacheHintElement::matches(const SoElement * element) const
{
  SoCacheHintElement *elem = (SoCacheHintElement*)element;
  return PRIVATE(this)->value == PRIVATE(elem)->value;
}

//! FIXME: write doc.

SoElement *
SoCacheHintElement::copyMatchInfo() const
{
  SoCacheHintElement *elem = (SoCacheHintElement*)
    getTypeId().createInstance();
  PRIVATE(elem)->value = PRIVATE(this)->value;
  return elem;
}

//! FIXME: write doc.

void
SoCacheHintElement::set(SoState * state,
                        SoNode * node,
                        const float value)
{
  SoCacheHintElement *elem = (SoCacheHintElement*)
    SoElement::getElement(state, classStackIndex);

  PRIVATE(elem)->value = value;
}

//! FIXME: write doc.

void
SoCacheHintElement::get(SoState * const state,
                        float & value)
{
  SoCacheHintElement *elem = (SoCacheHintElement*)
    SoElement::getConstElement(state, classStackIndex);

  value = PRIVATE(elem)->value;
}

#undef PRIVATE
