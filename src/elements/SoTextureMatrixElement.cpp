/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoTextureMatrixElement Inventor/elements/SoTextureMatrixElement.h
  \brief The SoTextureMatrixElement class is used to manage the texture matrix stack.

  The texture matrix is used to transform texture coordinates before 
  being used to map textures onto polygons.
*/

#include <Inventor/elements/SoTextureMatrixElement.h>
#include <coindefs.h> // COIN_STUB()

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
  elem->makeEltIdentity();
  if (node) elem->setNodeId(node);
}

/*!
  Multiplies \a matrix into the current texture matrix.
*/
void
SoTextureMatrixElement::mult(SoState * const state,
                           SoNode * const node,
                           const SbMatrix &matrix)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->multElt(matrix);
  if (node) elem->addNodeId(node);
}


/*!
  Appends \a translation to the current texture matrix.
*/
void
SoTextureMatrixElement::translateBy(SoState * const state,
                                  SoNode * const node,
                                  const SbVec3f &translation)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->translateEltBy(translation);
  if (node) elem->addNodeId(node);

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
  elem->rotateEltBy(rotation);
  if (node) elem->addNodeId(node);

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
  elem->scaleEltBy(scaleFactor);
  if (node) elem->addNodeId(node);
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
  virtual method which is called from mult(). Multiplies \a matrix
  into element matrix.
*/
void
SoTextureMatrixElement::multElt(const SbMatrix & matrix)
{
  this->textureMatrix.multRight(matrix);
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
  this->textureMatrix.multRight(matrix);
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
  this->textureMatrix.multRight(matrix);
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
  this->textureMatrix.multRight(matrix);
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
  
  SoTextureMatrixElement * const element =
    (SoTextureMatrixElement *)(this->next);
  element->textureMatrix = this->textureMatrix;

  // make sure node ids are accumulated properly
  element->copyNodeIds(this);
}

/*!
  Provided for API compatibility only. Does nothing for the moment.
*/
void
SoTextureMatrixElement::emptyMatrix(SoState * const /* state */)
{
  COIN_STUB();
}
