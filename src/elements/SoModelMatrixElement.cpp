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
  \class SoModelMatrixElement Inventor/elements/SoModelMatrixElement.h
  \brief The SoModelMatrixElement class is used to manage the current transformation.
  
  FIXME: write doc.
*/

#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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
  // FIXME: should invalidate cache on certain cull conditions.
  // we are using a different culling scheme, so I don't know 
  // if this will ever apply to us... pederb, 20000608
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
  elem->makeEltIdentity();
  if (node) elem->setNodeId(node);
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
  elem->setElt(matrix);
  if (node) elem->setNodeId(node);
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
  elem->cullMatrix = matrix;
  elem->flags |= FLG_CULLMATRIX;
  elem->flags &= ~FLG_COMBINED;
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
  elem->multElt(matrix);
  if (node) elem->addNodeId(node);
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
  elem->translateEltBy(translation);
  if (node) elem->addNodeId(node);
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
  elem->rotateEltBy(rotation);
  if (node) elem->addNodeId(node);

}

/*!
  Appends \a scaleFactor to the model matrix.
*/
void
SoModelMatrixElement::scaleBy(SoState * const state,
                              SoNode * const node,
                              const SbVec3f & scaleFactor)
{
#if COIN_DEBUG
  if (scaleFactor[0] <= 0.0f ||
      scaleFactor[1] <= 0.0f ||
      scaleFactor[2] <= 0.0f) {
    SoDebugError::postWarning("SoModelMatrixElement::scaleBy",
                              "invalid scale vector: <%f, %f, %f>",
                              scaleFactor[0], scaleFactor[1], scaleFactor[2]);
  }
#endif // COIN_DEBUG

  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->scaleEltBy(scaleFactor);
  if (node) elem->addNodeId(node);
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
  SoModelMatrixElement * const element =
    (SoModelMatrixElement *)this->next;
  element->modelMatrix = this->modelMatrix;
  element->flags = this->flags;
  // only copy when needed
  if (this->flags & FLG_CULLMATRIX)
    element->cullMatrix = this->cullMatrix;
  if (this->flags & FLG_COMBINED)
    element->combinedMatrix = this->combinedMatrix;
  
  // make sure node ids are accumulated properly
  element->copyNodeIds(this);
}


