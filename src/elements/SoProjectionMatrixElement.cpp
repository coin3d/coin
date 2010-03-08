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
  \class SoProjectionMatrix Inventor/elements/SoProjectionMatrixElement.h
  \brief The SoProjectionMatrixElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoProjectionMatrixElement.h>

#include "SbBasicP.h"

#include <cassert>

SO_ELEMENT_SOURCE(SoProjectionMatrixElement);

/*!
  This static method initializes static data for the SoProjectionMatrixElement
  class.
*/

void
SoProjectionMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoProjectionMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoProjectionMatrixElement::~SoProjectionMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoProjectionMatrixElement::set(SoState * const state,
                               SoNode * const node,
                               const SbMatrix & projectionMatrix)
{
  SoProjectionMatrixElement * element = coin_safe_cast<SoProjectionMatrixElement *>
    (
     SoReplacedElement::getElement(state, classStackIndex, node)
     );

  if (element) {
    element->setElt(projectionMatrix);
  }
}

//! FIXME: write doc.

const SbMatrix &
SoProjectionMatrixElement::get(SoState * const state)
{
  const SoProjectionMatrixElement * element =
    coin_assert_cast<const SoProjectionMatrixElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return element->projectionMatrix;
}

//! FIXME: write doc.

void
SoProjectionMatrixElement::setElt(SbMatrix const & matrix)
{
  this->projectionMatrix = matrix;
}

//! FIXME: write doc.

void
SoProjectionMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->projectionMatrix.makeIdentity();
}
