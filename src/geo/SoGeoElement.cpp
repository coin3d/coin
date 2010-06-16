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
  \class SoGeoElement Inventor/elements/SoGeoElement.h
  \brief The SoGeoElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  \since Coin 2.5
*/

#include <Inventor/elements/SoGeoElement.h>
#include <Inventor/nodes/SoGeoOrigin.h>
#include <assert.h>


class SoGeoElementP {
public:
  SoGeoOrigin * origin;
};

#define PRIVATE(obj) obj->pimpl

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoGeoElement);

/*!
  This static method initializes static data for the
  SoGeoElement class.
*/

void
SoGeoElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGeoElement, inherited);
}


SoGeoElement::SoGeoElement(void)
{
  PRIVATE(this) = new SoGeoElementP;
  PRIVATE(this)->origin = NULL;

  this->setTypeId(SoGeoElement::classTypeId);
  this->setStackIndex(SoGeoElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGeoElement::~SoGeoElement()
{
  delete PRIVATE(this);
}

//! FIXME: write doc.

void
SoGeoElement::set(SoState * const state,
                  SoGeoOrigin * origin)
{
  SoGeoElement * element = (SoGeoElement *)
    SoReplacedElement::getElement(state, classStackIndex, origin);

  if (element) {
    element->setElt(origin);
  }
}

//! FIXME: write doc.

SoGeoOrigin *
SoGeoElement::get(SoState * const state)
{
  SoGeoElement * element = (SoGeoElement *)
    SoElement::getConstElement(state, classStackIndex);

  return PRIVATE(element)->origin;
}

//! FIXME: write doc.

void
SoGeoElement::init(SoState * state)
{
  inherited::init(state);
  PRIVATE(this)->origin = NULL;
}

//! FIXME: doc
void
SoGeoElement::setElt(SoGeoOrigin * origin)
{
  PRIVATE(this)->origin = origin;
}

#undef PRIVATE

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  BOOST_CHECK_MESSAGE(SoGeoElement::getClassStackIndex() != -1,
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
