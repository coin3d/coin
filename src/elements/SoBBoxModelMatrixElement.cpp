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
  \class SoBBoxModelMatrixElement Inventor/elements/SoBBoxModelMatrixElement.h
  \brief The SoBBoxModelMatrixElement class keeps track of the current model
  matrix during a scene graph traversal.  It is used by amongst others the
  SoGetBoundingBoxAction class.
*/

#include <Inventor/elements/SoBBoxModelMatrixElement.h>

#include <coindefs.h> // COIN_STUB()
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>

SO_ELEMENT_SOURCE(SoBBoxModelMatrixElement);

/*!
  This static method initializes static data for the
  SoBBoxModelMatrixElement class.
*/

void
SoBBoxModelMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoBBoxModelMatrixElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoBBoxModelMatrixElement::createInstance()
*/

SoBBoxModelMatrixElement::SoBBoxModelMatrixElement()
  : state(NULL)
{
  setTypeId(SoBBoxModelMatrixElement::classTypeId);
  setStackIndex(SoBBoxModelMatrixElement::classStackIndex);
}

/*!
  The destructor.
*/

SoBBoxModelMatrixElement::~SoBBoxModelMatrixElement()
{
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state;
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::push(SoState * state)
{
  inherited::push(state);

  SoBBoxModelMatrixElement * const element =
    (SoBBoxModelMatrixElement *)(this->next);
  element->state = this->state;
}

/*!
  This method is for the SoGetBoundingBoxAction class so it can reset the
  current model matrix and all local matrices to identity.
*/

void
SoBBoxModelMatrixElement::reset(SoState * const /* state */,
                                SoNode * const /* node */)
{
  COIN_STUB();
}

/*!
  This method keeps two matrices up-to-date as opposed to the method it
  replaces.
*/

void
SoBBoxModelMatrixElement::pushMatrix(SoState * const state,
                                     SbMatrix &matrix,
                                     SbMatrix &localmatrix)
{
  matrix = SoModelMatrixElement::get(state);
  localmatrix = SoLocalBBoxMatrixElement::get(state);
}

/*!
  This method keeps two matrices up-to-date as opposed to the method it
  replaces.
*/

void
SoBBoxModelMatrixElement::popMatrix(SoState * const state,
                                    const SbMatrix & matrix,
                                    const SbMatrix & localmatrix)
{
  SoModelMatrixElement::set(state, NULL, matrix);
  SoLocalBBoxMatrixElement::set(state, localmatrix);
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::makeEltIdentity()
{
  inherited::makeEltIdentity();
  SoLocalBBoxMatrixElement::makeIdentity(state);
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::setElt(const SbMatrix &matrix)
{
  inherited::setElt(matrix);
  SoLocalBBoxMatrixElement::set(state, matrix);
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::multElt(const SbMatrix & matrix)
{
  inherited::multElt(matrix);
  SoLocalBBoxMatrixElement::mult(this->state, matrix);
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::translateEltBy(const SbVec3f & translation)
{
  inherited::translateEltBy(translation);
  SoLocalBBoxMatrixElement::translateBy(this->state, translation);
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::rotateEltBy(const SbRotation &rotation)
{
  inherited::rotateEltBy(rotation);
  SoLocalBBoxMatrixElement::rotateBy(this->state, rotation);
}

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::scaleEltBy(const SbVec3f &scaleFactor)
{
  inherited::scaleEltBy(scaleFactor);
  SoLocalBBoxMatrixElement::scaleBy(this->state, scaleFactor);
}

/*!
  This method is for debug use only.
*/

SbMatrix
SoBBoxModelMatrixElement::pushMatrixElt()
{
  assert(0 && "Should never be called!");
  return SbMatrix::identity();
}

/*!
  This method is for debug use only.
*/

void
SoBBoxModelMatrixElement::popMatrixElt(const SbMatrix &)
{
  assert(0 && "Should never be called!");
}
