/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoLocalBBoxMatrixElement Inventor/elements/SoLocalBBoxMatrixElement.h
  \brief The SoLocalBBoxMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLocalBBoxMatrixElement.h>

#include <coindefs.h> // COIN_STUB()

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
  SoLocalBBoxMatrixElement *elem =
    (SoLocalBBoxMatrixElement*) this->next;
  elem->localMatrix = this->localMatrix;
  elem->modelInverseMatrix = this->modelInverseMatrix;
}

//! FIXME: write doc.

SbBool
SoLocalBBoxMatrixElement::matches(const SoElement * /* element */) const
{
  COIN_STUB();
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoLocalBBoxMatrixElement::copyMatchInfo(void) const
{
  COIN_STUB();
  return NULL;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::makeIdentity(SoState * const state)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *)
    getElement(state, getClassStackIndex());
  if (element) {
    element->localMatrix.makeIdentity();
    element->modelInverseMatrix = SoModelMatrixElement::get(state).inverse();
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::set(SoState * const state,
                              const SbMatrix & matrix)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *)
    getElement(state, getClassStackIndex());
  if (element) {
    element->localMatrix = matrix;
    // FIXME: no idea whether this is correct or not
    // FIXME2: what the puck is this supposed to be good for?? 990404 mortene.
    element->localMatrix.multRight(element->modelInverseMatrix);
  }

#if COIN_DEBUG && 0 // debug
  if (element && element->localMatrix.det4() == 0.0f) {
    SoDebugError::postWarning("SoLocalBBoxMatrixElement::set",
                              "invalid matrix");
  }
#endif // debug
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::mult(SoState * const state,
                               const SbMatrix & matrix)
{
  SoLocalBBoxMatrixElement * element = (SoLocalBBoxMatrixElement *)
    getElement(state, getClassStackIndex());

  if (element) element->localMatrix.multLeft(matrix);

#if COIN_DEBUG && 0 // debug
  if (element && element->localMatrix.det4() == 0.0f) {
    SoDebugError::postWarning("SoLocalBBoxMatrixElement::mult",
                              "invalid matrix");
  }
#endif // debug
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::translateBy(SoState * const state,
                                      const SbVec3f & translation)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *)
    getElement(state, getClassStackIndex());
  if (element) {
    SbMatrix matrix;
    matrix.setTranslate(translation);
    element->localMatrix.multLeft(matrix);
  }

#if COIN_DEBUG && 0 // debug
  if (element && element->localMatrix.det4() == 0.0f) {
    SoDebugError::postWarning("SoLocalBBoxMatrixElement::translateBy",
                              "invalid matrix");
  }
#endif // debug
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::rotateBy(SoState * const state,
                                   const SbRotation & rotation)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *)
    getElement(state, getClassStackIndex());
  if (element) {
    SbMatrix matrix;
    matrix.setRotate(rotation);
    element->localMatrix.multLeft(matrix);
  }

#if COIN_DEBUG && 0 // debug
  if (element && element->localMatrix.det4() == 0.0f) {
    SoDebugError::postWarning("SoLocalBBoxMatrixElement::rotateBy",
                              "invalid matrix");
  }
#endif // debug
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::scaleBy(SoState * const state,
                                  const SbVec3f & scaleFactor)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *)
    getElement(state, getClassStackIndex());
  if (element) {
    SbMatrix matrix;
    matrix.setScale(scaleFactor);
    element->localMatrix.multLeft(matrix);
  }

#if COIN_DEBUG && 0 // debug
  if (element && element->localMatrix.det4() == 0.0f) {
    SoDebugError::postWarning("SoLocalBBoxMatrixElement::scaleBy",
                              "invalid matrix");
  }
#endif // debug
}

//! FIXME: write doc.

SbMatrix
SoLocalBBoxMatrixElement::pushMatrix(SoState * const state)
{
  SoLocalBBoxMatrixElement *elem = (SoLocalBBoxMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->localMatrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::popMatrix(SoState * const state,
                                    const SbMatrix & matrix)
{
  // FIXME: should I use getElement() instead of getConstElement()
  SoLocalBBoxMatrixElement *elem = (SoLocalBBoxMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  elem->localMatrix = matrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::resetAll(SoState * const state)
{
  // get const element to avoid push. Since this method is called only
  // from SoBBoxModelMatrixElement::reset(), it should be safe.
  SoLocalBBoxMatrixElement * element =
    (SoLocalBBoxMatrixElement *) SoElement::getConstElement(state, getClassStackIndex());

  while (element) {
    element->localMatrix.makeIdentity();
    element = (SoLocalBBoxMatrixElement*) element->prev;
  }
}

//! FIXME: write doc.

const SbMatrix &
SoLocalBBoxMatrixElement::get(SoState * const state)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *)
    getConstElement(state, getClassStackIndex());
  return element->localMatrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::print(FILE * /* file */) const
{
  COIN_STUB();
}
