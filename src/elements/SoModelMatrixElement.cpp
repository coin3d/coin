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
  \class SoModelMatrixElement Inventor/elements/SoModelMatrixElement.h
  \brief The SoModelMatrixElement class is used to manage the current transformation.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>

// defines for the flags member
#define FLG_IDENTITY   0x1  // modelMatrix is identity
#define FLG_CULLMATRIX 0x2  // cullMatrix is set
#define FLG_COMBINED   0x4  // the combined matrix is calculated

/*!
  \fn SoModelMatrixElement::modelMatrix

  FIXME: write doc.
*/

/*!
  \fn SoModelMatrixElement::cullMatrix

  FIXME: write doc.
*/

/*!
  \fn SoModelMatrixElement::combinedMatrix

  FIXME: write doc.
*/


/*!
  \fn SoModelMatrixElement::flags

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoModelMatrixElement);

/*!
  This static method initializes static data for the
  SoModelMatrixElement class.
*/

void
SoModelMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoModelMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoModelMatrixElement::~SoModelMatrixElement(void)
{
}

// doc from parent
SbBool
SoModelMatrixElement::matches(const SoElement * element) const
{
  return inherited::matches(element);
}

/*!
  Sets the current model matrix to the identity matrix.
*/
void
SoModelMatrixElement::makeIdentity(SoState * const state,
                                   SoNode * const node)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->makeEltIdentity();
    if (node) elem->setNodeId(node);
  }
}

/*!
  Sets the current model matrix to \a matrix.
*/
void
SoModelMatrixElement::set(SoState * const state,
                          SoNode * const node,
                          const SbMatrix & matrix)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);

  if (elem) {
    elem->setElt(matrix);
    if (node) elem->setNodeId(node);
  }
}

/*!
  Sets the current cull matrix.
*/
void
SoModelMatrixElement::setCullMatrix(SoState * state, SoNode * node,
                                    const SbMatrix & matrix)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->cullMatrix = matrix;
    elem->flags |= FLG_CULLMATRIX;
    elem->flags &= ~FLG_COMBINED;
  }
}

/*!
  Multiplies \a matrix into the model matrix.
*/
void
SoModelMatrixElement::mult(SoState * const state,
                           SoNode * const node,
                           const SbMatrix &matrix)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->multElt(matrix);
    if (node) elem->addNodeId(node);
  }
}


/*!
  Appends \a translation to the model matrix.
*/
void
SoModelMatrixElement::translateBy(SoState * const state,
                                  SoNode * const node,
                                  const SbVec3f &translation)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->translateEltBy(translation);
    if (node) elem->addNodeId(node);
  }
}


/*!
  Appends \a rotation to the model matrix.
*/
void
SoModelMatrixElement::rotateBy(SoState * const state,
                               SoNode * const node,
                               const SbRotation & rotation)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->rotateEltBy(rotation);
    if (node) elem->addNodeId(node);
  }
}

/*!
  Appends \a scaleFactor to the model matrix.
*/
void
SoModelMatrixElement::scaleBy(SoState * const state,
                              SoNode * const node,
                              const SbVec3f & scaleFactor)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->scaleEltBy(scaleFactor);
    if (node) elem->addNodeId(node);
  }
}

/*!
  Used by SoTransformSeparator to store and restore model matrix.
  Don't use it for any other reason.
*/
SbMatrix
SoModelMatrixElement::pushMatrix(SoState * const state)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->pushMatrixElt();
}

/*!
  Used by SoTransformSeparator to store and restore model matrix.
  Don't use it for any other reason.
*/
void
SoModelMatrixElement::popMatrix(SoState * const state,
                                const SbMatrix & matrix)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  elem->popMatrixElt(matrix);
}

/*!
  Returns the combined cull and model matrix. This matrix
  is cached.
*/
const SbMatrix &
SoModelMatrixElement::getCombinedCullMatrix(SoState * const state)
{
  const SoModelMatrixElement *elem = (const SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  if (!(elem->flags & FLG_COMBINED)) {
    // Need to change this element, so cast away the const (_don't_
    // use the getElement() method, as it may invoke a
    // push()). --mortene
    SoModelMatrixElement * e = (SoModelMatrixElement *)elem;
    e->combinedMatrix = e->modelMatrix;
    if (e->flags & FLG_CULLMATRIX)
      e->combinedMatrix.multRight(e->cullMatrix);
    e->flags |= FLG_COMBINED;
  }
  return elem->combinedMatrix;
}

/*!
  Returns the current model matrix.
*/
const SbMatrix &
SoModelMatrixElement::get(SoState * const state)
{
  const SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->modelMatrix;
}

/*!
  Returns the current model matrix. Sets \a isIdentity to
  TRUE if the model matrix is known to be an identity matrix.
*/
const SbMatrix &
SoModelMatrixElement::get(SoState * const state,
                          SbBool & isIdentity)
{
  const SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  if (elem->flags & FLG_IDENTITY) isIdentity = TRUE;
  else isIdentity = FALSE;
  return elem->modelMatrix;
}

/*!
  virtual method which is called from the static method
  makeIdentity(). Sets element model matrix to identity.
*/
void
SoModelMatrixElement::makeEltIdentity(void)
{
  this->modelMatrix.makeIdentity();
  this->flags = FLG_IDENTITY;
}

/*!
  virtual method which is called from the static method
  set(). Sets element model matrix to \a matrix.
*/
void
SoModelMatrixElement::setElt(const SbMatrix & matrix)
{
  this->modelMatrix = matrix;
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

/*!
  virtual method which is called from the static method
  mult(). Multiplies \a matrix into element model matrix.
*/
void
SoModelMatrixElement::multElt(const SbMatrix & matrix)
{
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

/*!
  virtual method which is called from the static method
  translateBy(). Appends \a translation to element model matrix.
*/
void
SoModelMatrixElement::translateEltBy(const SbVec3f & translation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setTranslate(translation);
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

/*!
  virtual method which is called from the static method
  rotateBy(). Appends \a rotation to element model matrix.
*/
void
SoModelMatrixElement::rotateEltBy(const SbRotation & rotation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setRotate(rotation);
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

/*!
  virtual method which is called from the static method
  scaleBy(). Appends \a scaleFactor to element model matrix.
*/
void
SoModelMatrixElement::scaleEltBy(const SbVec3f & scaleFactor)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setScale(scaleFactor);
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

/*!
  virtual method which is called from the static method
  pushMatrix(). Returns current model matrix.
*/
SbMatrix
SoModelMatrixElement::pushMatrixElt()
{
  return this->modelMatrix;
}

/*!
  virtual method which is called from the static method
  popMatrix(). Retores model matrix to the matrix returned from
  pushMatrix().
*/
void
SoModelMatrixElement::popMatrixElt(const SbMatrix & matrix)
{
  this->modelMatrix = matrix;
}

// doc from parent
void
SoModelMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->modelMatrix.makeIdentity();
  this->flags = FLG_IDENTITY;
  this->clearNodeIds();
}

/*!
  Overloaded to copy matrices and accumulate node ids.
*/
void
SoModelMatrixElement::push(SoState * state)
{
  inherited::push(state);
  SoModelMatrixElement * prev =
    (SoModelMatrixElement *) this->getNextInStack();

  this->modelMatrix = prev->modelMatrix;
  this->flags = prev->flags;
  // only copy when needed
  if (prev->flags & FLG_CULLMATRIX)
    this->cullMatrix = prev->cullMatrix;
  if (prev->flags & FLG_COMBINED)
    this->combinedMatrix = prev->combinedMatrix;

  // make sure node ids are accumulated properly
  this->copyNodeIds(prev);
}

const SbMatrix & 
SoModelMatrixElement::getModelMatrix(void) const
{
  return this->modelMatrix;
}
