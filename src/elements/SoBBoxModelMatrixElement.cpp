/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoBBoxModelMatrixElement Inventor/elements/SoBBoxModelMatrixElement.h
  \brief The SoBBoxModelMatrixElement class keeps track of the current model
  matrix during a scene graph traversal.  It is used by amongst others the
  SoGetBoundingBoxAction class.
  \ingroup elements
*/

#include <Inventor/elements/SoBBoxModelMatrixElement.h>
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

  SoBBoxModelMatrixElement * const prev =
    (SoBBoxModelMatrixElement *)this->getNextInStack();
  this->state = prev->state;
}

/*!
  This method is for the SoGetBoundingBoxAction class so it can reset the
  current model matrix and all local matrices to identity.
*/

void
SoBBoxModelMatrixElement::reset(SoState * const state,
                                SoNode * const node)
{
  SoModelMatrixElement::makeIdentity(state, node);
  SoLocalBBoxMatrixElement::resetAll(state);
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
