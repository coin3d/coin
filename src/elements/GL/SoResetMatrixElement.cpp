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

#include "coindefs.h"
#include "elements/GL/SoResetMatrixElement.h"

SO_ELEMENT_SOURCE(SoResetMatrixElement);

// doc in parent
void
SoResetMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoResetMatrixElement, inherited);
}

/*!
  The destructor.
*/
SoResetMatrixElement::~SoResetMatrixElement(void)
{
}

// doc in parent
void
SoResetMatrixElement::init(SoState * COIN_UNUSED_ARG(stateptr))
{
  this->matrix = SbMatrix::identity();
}

void
SoResetMatrixElement::set(SoState * stateptr, const SbMatrix & matrix)
{
  SoResetMatrixElement * elem = (SoResetMatrixElement*)
    SoElement::getElement(stateptr, SoResetMatrixElement::classStackIndex);
  elem->setElt(matrix);
}

const SbMatrix  &
SoResetMatrixElement::get(SoState * state)
{
  const SoResetMatrixElement * elem = (const SoResetMatrixElement*)
    SoElement::getConstElement(state, SoResetMatrixElement::classStackIndex);
  return elem->matrix;
}

void
SoResetMatrixElement::setElt(const SbMatrix & matrix_in)
{
  this->matrix = matrix_in;
}

SbBool
SoResetMatrixElement::matches(const SoElement * element) const
{
  const SoResetMatrixElement * other = (const SoResetMatrixElement*) element;
  return this->matrix == other->matrix;
}

SoElement *
SoResetMatrixElement::copyMatchInfo(void) const
{
  SoResetMatrixElement * element =
    (SoResetMatrixElement *)(getTypeId().createInstance());
  element->matrix = this->matrix;
  return element;
}
