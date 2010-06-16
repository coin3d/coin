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
  \class SoBumpMapMatrixElement Inventor/elements/SoBumpMapMatrixElement.h
  \brief The SoBumpMapMatrixElement class is used to manage the bump map matrix stack.
  \ingroup elements

  The bump map matrix is used to transform bump map coordinates before
  being used to map bump maps onto polygons.
*/

#include <Inventor/elements/SoBumpMapMatrixElement.h>
#include "coindefs.h" // COIN_OBSOLETED()
#include "SbBasicP.h"

/*!
  \fn SoBumpMapMatrixElement::bumpMapMatrix

  The matrix.
*/

SO_ELEMENT_SOURCE(SoBumpMapMatrixElement);

// doc from parent
void
SoBumpMapMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoBumpMapMatrixElement, inherited);
}

/*!
  The destructor.
*/
SoBumpMapMatrixElement::~SoBumpMapMatrixElement(void)
{
}

/*!
  Sets current texture matrix to identity.
*/
void
SoBumpMapMatrixElement::makeIdentity(SoState * const state,
                                     SoNode * const node)
{
  SoBumpMapMatrixElement * elem = coin_assert_cast<SoBumpMapMatrixElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->makeEltIdentity();
    if (node) elem->setNodeId(node);
  }
}

/*!
  Sets the current texture matrix to \a matrix.

  This method is an extension versus the Open Inventor API.
*/
void
SoBumpMapMatrixElement::set(SoState * const state,
                            SoNode * const node,
                            const SbMatrix & matrix)
{
  SoBumpMapMatrixElement * elem =
       coin_assert_cast<SoBumpMapMatrixElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->setElt(matrix);
    if (node) elem->setNodeId(node);
  }
}

/*!
  Multiplies \a matrix into the current texture matrix.
*/
void
SoBumpMapMatrixElement::mult(SoState * const state,
                             SoNode * const node,
                             const SbMatrix & matrix)
{
  SoBumpMapMatrixElement * elem = coin_assert_cast< SoBumpMapMatrixElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->multElt(matrix);
    if (node) elem->addNodeId(node);
  }
}


/*!
  Appends \a translation to the current texture matrix.
*/
void
SoBumpMapMatrixElement::translateBy(SoState * const state,
                                    SoNode * const node,
                                    const SbVec3f & translation)
{
  SoBumpMapMatrixElement * elem = coin_assert_cast< SoBumpMapMatrixElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->translateEltBy(translation);
    if (node) elem->addNodeId(node);
  }
}

/*!
  Appends \a rotation to the current texture matrix.
*/
void
SoBumpMapMatrixElement::rotateBy(SoState * const state,
                                 SoNode * const node,
                                 const SbRotation & rotation)
{
  SoBumpMapMatrixElement * elem = coin_assert_cast<SoBumpMapMatrixElement*>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->rotateEltBy(rotation);
    if (node) elem->addNodeId(node);
  }
}

/*!
  Appends \a scaleFactor to the current texture matrix.
*/
void
SoBumpMapMatrixElement::scaleBy(SoState * const state,
                                SoNode * const node,
                                const SbVec3f & scaleFactor)
{
  SoBumpMapMatrixElement * elem = coin_assert_cast<SoBumpMapMatrixElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->scaleEltBy(scaleFactor);
    if (node) elem->addNodeId(node);
  }
}


/*!
  Returns current texture matrix.
*/
const SbMatrix &
SoBumpMapMatrixElement::get(SoState * const state)
{
  const SoBumpMapMatrixElement * elem = coin_assert_cast<const SoBumpMapMatrixElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return elem->getElt();
}

/*!
  virtual method which is called from makeIdentity().
  Sets element matrix to identity.
*/
void
SoBumpMapMatrixElement::makeEltIdentity(void)
{
  this->bumpMapMatrix.makeIdentity();
}

/*!
  virtual method which is called from set(). Sets the texture matrix
  to \a matrix.

  This method is an extension versus the Open Inventor API.
*/
void
SoBumpMapMatrixElement::setElt(const SbMatrix & matrix)
{
  this->bumpMapMatrix = matrix;
}

/*!
  virtual method which is called from mult(). Multiplies \a matrix
  into element matrix.
*/
void
SoBumpMapMatrixElement::multElt(const SbMatrix & matrix)
{
  this->bumpMapMatrix.multLeft(matrix);
}

/*!
  virtual method which is called from translateBy().
  Appends \a translation to the element matrix.
*/
void
SoBumpMapMatrixElement::translateEltBy(const SbVec3f & translation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setTranslate(translation);
  this->bumpMapMatrix.multLeft(matrix);
}

/*!
  virtual method which is called from rotateBy().
  Appends \a rotation to the element matrix.
*/
void
SoBumpMapMatrixElement::rotateEltBy(const SbRotation & rotation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setRotate(rotation);
  this->bumpMapMatrix.multLeft(matrix);
}

/*!
  virtual method which is called from scaleBy().
  Append \a scaleFactor to the element matrix.
*/
void
SoBumpMapMatrixElement::scaleEltBy(const SbVec3f & scaleFactor)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setScale(scaleFactor);
  this->bumpMapMatrix.multLeft(matrix);
}

/*!
  Returns element matrix. Called from get().
*/
const SbMatrix &
SoBumpMapMatrixElement::getElt(void) const
{
  return this->bumpMapMatrix;
}

// doc from parent
void
SoBumpMapMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->bumpMapMatrix.makeIdentity();
  this->clearNodeIds();
}

// Documented in superclass. Overridden to copy current matrix and
// update accumulated node ids.
void
SoBumpMapMatrixElement::push(SoState * state)
{
  inherited::push(state);

  const SoBumpMapMatrixElement * prev =
    coin_assert_cast<const SoBumpMapMatrixElement *>(this->getNextInStack());
  this->bumpMapMatrix = prev->bumpMapMatrix;

  // make sure node ids are accumulated properly
  this->copyNodeIds(prev);
}
