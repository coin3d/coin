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
  \class SoLocalBBoxMatrixElement Inventor/elements/SoLocalBBoxMatrixElement.h
  \brief The SoLocalBBoxMatrixElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include "SbBasicP.h"

#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#include <Inventor/misc/SoState.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \fn SoLocalBBoxMatrixElement::localMatrix

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoLocalBBoxMatrixElement);

/*!
  This static method initializes static data for the
  SoLocalBBoxMatrixElement class.
*/

void
SoLocalBBoxMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLocalBBoxMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoLocalBBoxMatrixElement::~SoLocalBBoxMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->localMatrix.makeIdentity();
  this->modelInverseMatrix.makeIdentity();
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::push(SoState * state)
{
  inherited::push(state);
  SoLocalBBoxMatrixElement * prev =
    coin_assert_cast<SoLocalBBoxMatrixElement*>(this->getNextInStack());
  this->localMatrix = prev->localMatrix;

  // avoid cache dependencies by using the state getElement method
  const SoModelMatrixElement * modelelem = coin_assert_cast<const SoModelMatrixElement*>
    (
     state->getConstElement(SoModelMatrixElement::getClassStackIndex())
     );
  // FIXME: is this really sensible caching? If push() is called more
  // often than set() (the only place where it's actually used), I
  // guess not. 20020905 mortene.
  this->modelInverseMatrix = modelelem->getModelMatrix().inverse();
}

//! FIXME: write doc.

SbBool
SoLocalBBoxMatrixElement::matches(const SoElement * /* element */) const
{
#if COIN_DEBUG
  SoDebugError::postInfo("SoLocalBBoxMatrixElement::matches",
                         "This method should never be called for this element.");
#endif // COIN_DEBUG
  return TRUE;
}

//! FIXME: write doc.

SoElement *
SoLocalBBoxMatrixElement::copyMatchInfo(void) const
{
#if COIN_DEBUG
  SoDebugError::postInfo("SoLocalBBoxMatrixElement::copyMatchInfo",
                         "This method should never be called for this element.");
#endif // COIN_DEBUG
  return NULL;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::makeIdentity(SoState * const state)
{
  SoLocalBBoxMatrixElement * element;
  element = coin_safe_cast<SoLocalBBoxMatrixElement *>
    (
     SoElement::getElement(state, getClassStackIndex())
     );
  if (element) {
    element->localMatrix.makeIdentity();
    // inverse model matrix is set in push(), no need to set it here
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::set(SoState * const state,
                              const SbMatrix & matrix)
{
  SoLocalBBoxMatrixElement * element = coin_safe_cast<SoLocalBBoxMatrixElement *>
    (
     SoElement::getElement(state, getClassStackIndex())
     );
  if (element) {
    element->localMatrix = matrix * element->modelInverseMatrix;
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::mult(SoState * const state,
                               const SbMatrix & matrix)
{
  SoLocalBBoxMatrixElement * element = coin_safe_cast<SoLocalBBoxMatrixElement *>
    (
     SoElement::getElement(state, getClassStackIndex())
     );

  if (element) {
    element->localMatrix.multLeft(matrix);
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::translateBy(SoState * const state,
                                      const SbVec3f & translation)
{
  SoLocalBBoxMatrixElement * element = coin_safe_cast<SoLocalBBoxMatrixElement *>
    (
     SoElement::getElement(state, getClassStackIndex())
     );

  if (element) {
    SbMatrix matrix;
    matrix.setTranslate(translation);
    element->localMatrix.multLeft(matrix);
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::rotateBy(SoState * const state,
                                   const SbRotation & rotation)
{
  SoLocalBBoxMatrixElement * element = coin_safe_cast<SoLocalBBoxMatrixElement *>
    (
     SoElement::getElement(state, getClassStackIndex())
     );
  if (element) {
    SbMatrix matrix;
    matrix.setRotate(rotation);
    element->localMatrix.multLeft(matrix);
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::scaleBy(SoState * const state,
                                  const SbVec3f & scaleFactor)
{
  SoLocalBBoxMatrixElement * element = coin_safe_cast<SoLocalBBoxMatrixElement *>
    (
     SoElement::getElement(state, getClassStackIndex())
     );

  if (element) {
    SbMatrix matrix;
    matrix.setScale(scaleFactor);
    element->localMatrix.multLeft(matrix);
  }
}

//! FIXME: write doc.

SbMatrix
SoLocalBBoxMatrixElement::pushMatrix(SoState * const state)
{
  // use getElementNoPush to avoid element push
  SoLocalBBoxMatrixElement * elem = coin_assert_cast<SoLocalBBoxMatrixElement *>
    (
     state->getElementNoPush(classStackIndex)
     );
  return elem->localMatrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::popMatrix(SoState * const state,
                                    const SbMatrix & matrix)
{
  // Important: use getElementNoPush to avoid a push on element
  SoLocalBBoxMatrixElement *elem = coin_assert_cast<SoLocalBBoxMatrixElement*>
    (
     state->getElementNoPush(classStackIndex)
    );
  elem->localMatrix = matrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::resetAll(SoState * const state)
{
  SoLocalBBoxMatrixElement * element =
    coin_safe_cast<SoLocalBBoxMatrixElement*>
    (
     state->getElement(getClassStackIndex())
     );
  while (element) {
    element->localMatrix.makeIdentity();
    element = coin_safe_cast<SoLocalBBoxMatrixElement*>(element->getNextInStack());
  }
}

//! FIXME: write doc.

const SbMatrix &
SoLocalBBoxMatrixElement::get(SoState * const state)
{
  const SoLocalBBoxMatrixElement * element =
    coin_assert_cast<const SoLocalBBoxMatrixElement *>
    (
     SoElement::getConstElement(state, getClassStackIndex())
     );
  return element->localMatrix;
}
