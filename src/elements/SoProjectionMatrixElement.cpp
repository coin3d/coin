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
  \class SoProjectionMatrix Inventor/elements/SoProjectionMatrixElement.h
  \brief The SoProjectionMatrixElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoProjectionMatrixElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoProjectionMatrixElement);

/*!
  This static method initializes static data for the SoProjectionMatrixElement
  class.
*/

void
SoProjectionMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoProjectionMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoProjectionMatrixElement::~SoProjectionMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoProjectionMatrixElement::set(SoState * const state,
                               SoNode * const node,
                               const SbMatrix & projectionMatrix)
{
  SoProjectionMatrixElement * element = (SoProjectionMatrixElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (element) {
    element->setElt(projectionMatrix);
  }
}

//! FIXME: write doc.

const SbMatrix &
SoProjectionMatrixElement::get(SoState * const state)
{
  SoProjectionMatrixElement * element = (SoProjectionMatrixElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->projectionMatrix;
}

//! FIXME: write doc.

void
SoProjectionMatrixElement::setElt(SbMatrix const & matrix)
{
  this->projectionMatrix = matrix;
}

//! FIXME: write doc.

void
SoProjectionMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->projectionMatrix.makeIdentity();
}
