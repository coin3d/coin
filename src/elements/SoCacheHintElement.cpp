/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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
#include <Inventor/elements/SoShapeStyleElement.h>

#include <cassert>

#include "SbBasicP.h"
#include "coindefs.h"

// FIXME: make it possible to control this constant. pederb, 2005-01-10
#define VERTEX_ARRAY_LIMIT 0.51f

class SoCacheHintElementP {
public:
  float memvalue;
  float gfxvalue;
};

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
  PRIVATE(this)->memvalue = 0.5f;
  PRIVATE(this)->gfxvalue = 0.5f;
}

//! FIXME: write doc.

void
SoCacheHintElement::push(SoState * state)
{
  inherited::push(state);
  SoCacheHintElement * prev = coin_assert_cast<SoCacheHintElement * >
    (
     this->getNextInStack()
     );
  PRIVATE(this)->memvalue = PRIVATE(prev)->memvalue;
  PRIVATE(this)->gfxvalue = PRIVATE(prev)->gfxvalue;
}

void
SoCacheHintElement::pop(SoState * state, const SoElement * prevtopelement)
{
  inherited::pop(state, prevtopelement);
}

//! FIXME: write doc.

SbBool
SoCacheHintElement::matches(const SoElement * element) const
{
  const SoCacheHintElement * elem = coin_assert_cast<const SoCacheHintElement *>(element);
  return
    (PRIVATE(this)->memvalue == PRIVATE(elem)->memvalue) &&
    (PRIVATE(this)->gfxvalue == PRIVATE(elem)->gfxvalue);
}

//! FIXME: write doc.

SoElement *
SoCacheHintElement::copyMatchInfo() const
{
  SoCacheHintElement * elem = static_cast<SoCacheHintElement *>(
    getTypeId().createInstance()
    );
  PRIVATE(elem)->memvalue = PRIVATE(this)->memvalue;
  PRIVATE(elem)->gfxvalue = PRIVATE(this)->gfxvalue;
  return elem;
}

//! FIXME: write doc.

void
SoCacheHintElement::set(SoState * state,
                        SoNode * COIN_UNUSED_ARG(node),
                        const float memvalue,
                        const float gfxvalue)
{
  SoCacheHintElement * elem =
    coin_assert_cast<SoCacheHintElement * >
    (
     SoElement::getElement(state, classStackIndex)
     );

  PRIVATE(elem)->memvalue = memvalue;
  PRIVATE(elem)->gfxvalue = gfxvalue;

  SoShapeStyleElement::setVertexArrayRendering(state, memvalue >= VERTEX_ARRAY_LIMIT);
}

//! FIXME: write doc.

void
SoCacheHintElement::get(SoState * const state, float & memvalue, float & gfxvalue)
{
  const SoCacheHintElement * elem = coin_assert_cast<const SoCacheHintElement *>(
    SoElement::getConstElement(state, classStackIndex)
    );

  memvalue = PRIVATE(elem)->memvalue;
  gfxvalue = PRIVATE(elem)->gfxvalue;
}

#undef PRIVATE
#undef VERTEX_ARRAY_LIMIT
