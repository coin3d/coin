/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoTextureMatrixElement Inventor/elements/SoTextureMatrixElement.h
  \brief The SoTextureMatrixElement class is used to manage the texture matrix stack.
  \ingroup elements

  The texture matrix is used to transform texture coordinates before
  being used to map textures onto polygons.
*/

#include <Inventor/elements/SoTextureMatrixElement.h>
#include <coindefs.h> // COIN_OBSOLETED()

/*!
  \fn SoTextureMatrixElement::textureMatrix

  The matrix.
*/

SO_ELEMENT_SOURCE(SoTextureMatrixElement);

// doc from parent
void
SoTextureMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureMatrixElement, inherited);
}

/*!
  The destructor.
*/
SoTextureMatrixElement::~SoTextureMatrixElement(void)
{
}

/*!
  Sets current texture matrix to identity.
*/
void
SoTextureMatrixElement::makeIdentity(SoState * const state,
                                     SoNode * const node)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
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
SoTextureMatrixElement::set(SoState * const state,
                            SoNode * const node,
                            const SbMatrix & matrix)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->setElt(matrix);
    if (node) elem->setNodeId(node);
  }
}

/*!
  Multiplies \a matrix into the current texture matrix.
*/
void
SoTextureMatrixElement::mult(SoState * const state,
                           SoNode * const node,
                           const SbMatrix & matrix)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->multElt(matrix);
    if (node) elem->addNodeId(node);
  }
}


/*!
  Appends \a translation to the current texture matrix.
*/
void
SoTextureMatrixElement::translateBy(SoState * const state,
                                  SoNode * const node,
                                  const SbVec3f & translation)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->translateEltBy(translation);
    if (node) elem->addNodeId(node);
  }
}

/*!
  Appends \a rotation to the current texture matrix.
*/
void
SoTextureMatrixElement::rotateBy(SoState * const state,
                               SoNode * const node,
                               const SbRotation & rotation)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->rotateEltBy(rotation);
    if (node) elem->addNodeId(node);
  }
}

/*!
  Appends \a scaleFactor to the current texture matrix.
*/
void
SoTextureMatrixElement::scaleBy(SoState * const state,
                              SoNode * const node,
                              const SbVec3f & scaleFactor)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->scaleEltBy(scaleFactor);
    if (node) elem->addNodeId(node);
  }
}


/*!
  Returns current texture matrix.
*/
const SbMatrix &
SoTextureMatrixElement::get(SoState * const state)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->getElt();
}

/*!
  virtual method which is called from makeIdentity().
  Sets element matrix to identity.
*/
void
SoTextureMatrixElement::makeEltIdentity(void)
{
  this->textureMatrix.makeIdentity();
}

/*!
  virtual method which is called from set(). Sets the texture matrix
  to \a matrix.

  This method is an extension versus the Open Inventor API.
*/
void
SoTextureMatrixElement::setElt(const SbMatrix & matrix)
{
  this->textureMatrix = matrix;
}

/*!
  virtual method which is called from mult(). Multiplies \a matrix
  into element matrix.
*/
void
SoTextureMatrixElement::multElt(const SbMatrix & matrix)
{
  this->textureMatrix.multLeft(matrix);
}

/*!
  virtual method which is called from translateBy().
  Appends \a translation to the element matrix.
*/
void
SoTextureMatrixElement::translateEltBy(const SbVec3f & translation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setTranslate(translation);
  this->textureMatrix.multLeft(matrix);
}

/*!
  virtual method which is called from rotateBy().
  Appends \a rotation to the element matrix.
*/
void
SoTextureMatrixElement::rotateEltBy(const SbRotation & rotation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setRotate(rotation);
  this->textureMatrix.multLeft(matrix);
}

/*!
  virtual method which is called from scaleBy().
  Append \a scaleFactor to the element matrix.
*/
void
SoTextureMatrixElement::scaleEltBy(const SbVec3f & scaleFactor)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setScale(scaleFactor);
  this->textureMatrix.multLeft(matrix);
}

/*!
  Returns element matrix. Called from get().
*/
const SbMatrix &
SoTextureMatrixElement::getElt(void) const
{
  return this->textureMatrix;
}

// doc from parent
void
SoTextureMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->textureMatrix.makeIdentity();
  this->clearNodeIds();
}

/*!
  Overloaded to copy current matrix and update accumulated node ids.
*/
void
SoTextureMatrixElement::push(SoState * state)
{
  inherited::push(state);

  SoTextureMatrixElement * prev =
    (SoTextureMatrixElement *) this->getNextInStack();
  this->textureMatrix = prev->textureMatrix;

  // make sure node ids are accumulated properly
  this->copyNodeIds(prev);
}

/*!
  Provided for API compatibility only. Does nothing for the moment.
*/
void
SoTextureMatrixElement::emptyMatrix(SoState * const /* state */)
{
  COIN_OBSOLETED();
}
