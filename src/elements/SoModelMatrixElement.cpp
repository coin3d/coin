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
  \brief The SoModelMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoModelMatrixElement.h>

#include <Inventor/SbName.h>

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
  A constructor.  Can't be used directly.

  \sa SoModelMatrixElement::createInstance(void)
*/

SoModelMatrixElement::SoModelMatrixElement(void)
  : modelMatrix(), cullMatrix(), combinedMatrix(), flags(0)
{
  setTypeId(SoModelMatrixElement::classTypeId);
  setStackIndex(SoModelMatrixElement::classStackIndex);
}

/*!
  The destructor.
*/

SoModelMatrixElement::~SoModelMatrixElement(void)
{
}

//! FIXME: write doc.

SbBool
SoModelMatrixElement::matches(const SoElement * element) const
{
  const SoModelMatrixElement *elem = (const SoModelMatrixElement*)element;

  if ((this->flags & FLG_CULLMATRIX) !=
      (elem->flags & FLG_CULLMATRIX)) return FALSE;
  if ((this->flags & FLG_CULLMATRIX) &&
      (this->cullMatrix != elem->cullMatrix)) return FALSE;
  return SoAccumulatedElement::matches(element);
}

//! FIXME: write doc.

void
SoModelMatrixElement::makeIdentity(SoState * const state,
                                   SoNode * const node)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->makeEltIdentity();
  if (node) elem->setNodeId(node);
}

//! FIXME: write doc.

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

//! FIXME: write doc.

void
SoModelMatrixElement::setCullMatrix(SoState * state, SoNode * node,
                                    const SbMatrix & matrix)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->setElt(matrix);
  if (node) elem->setNodeId(node);

  elem->cullMatrix = matrix;
  elem->flags |= FLG_CULLMATRIX;
  elem->flags &= ~FLG_COMBINED;
}

//! FIXME: write doc.

void
SoModelMatrixElement::mult(SoState * const state,
                           SoNode * const node,
                           const SbMatrix &matrix)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);

#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoModelMatrixElement::mult",
                         "modelview element depth (%p): %d\n",
                         elem,
                         elem->getDepth());
#endif // 0
  elem->multElt(matrix);
  if (node) elem->addNodeId(node);
}

//! FIXME: write doc.

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

//! FIXME: write doc.

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

//! FIXME: write doc.

void
SoModelMatrixElement::scaleBy(SoState * const state,
                              SoNode * const node,
                              const SbVec3f & scaleFactor)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->scaleEltBy(scaleFactor);
  if (node) elem->addNodeId(node);
}

//! FIXME: write doc.

SbMatrix
SoModelMatrixElement::pushMatrix(SoState * const state)
{
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->pushMatrixElt();
}

//! FIXME: write doc.

void
SoModelMatrixElement::popMatrix(SoState * const state,
                                const SbMatrix & matrix)
{
  // FIXME: or should I use getElement()? pederb
  SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  elem->popMatrixElt(matrix);
}

//! FIXME: write doc.

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

//! FIXME: write doc.

const SbMatrix &
SoModelMatrixElement::get(SoState * const state)
{
  const SoModelMatrixElement *elem = (SoModelMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->modelMatrix;
}

//! FIXME: write doc.

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

//! FIXME: write doc.

void
SoModelMatrixElement::print(FILE * file) const
{
  fprintf(file, "SoModelMatrixElement[%p]\n", this);
}

//! FIXME: write doc.

void
SoModelMatrixElement::makeEltIdentity(// virtual, protected
    void)
{
  this->modelMatrix.makeIdentity();
  this->flags = FLG_IDENTITY;
}

//! FIXME: write doc.

void
SoModelMatrixElement::setElt(// virtual, protected
    const SbMatrix & matrix)
{
  this->modelMatrix = matrix;
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

//! FIXME: write doc.

void
SoModelMatrixElement::multElt(// virtual, protected
    const SbMatrix & matrix)
{
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

//! FIXME: write doc.

void
SoModelMatrixElement::translateEltBy(// virtual, protected
    const SbVec3f & translation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setTranslate(translation);
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

//! FIXME: write doc.

void
SoModelMatrixElement::rotateEltBy(// virtual, protected
    const SbRotation & rotation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setRotate(rotation);
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

//! FIXME: write doc.

void
SoModelMatrixElement::scaleEltBy(// virtual, protected
    const SbVec3f & scaleFactor)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setScale(scaleFactor);
  this->modelMatrix.multLeft(matrix);
  this->flags &= ~(FLG_IDENTITY|FLG_COMBINED);
}

//! FIXME: write doc.

SbMatrix
SoModelMatrixElement::pushMatrixElt()
{
  return this->modelMatrix;
}

//! FIXME: write doc.

void
SoModelMatrixElement::popMatrixElt(const SbMatrix & matrix)
{
  this->modelMatrix = matrix;
}

//! FIXME: write doc.

void
SoModelMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->modelMatrix.makeIdentity();
  this->flags = FLG_IDENTITY;
}

//! FIXME: write doc.

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
}
