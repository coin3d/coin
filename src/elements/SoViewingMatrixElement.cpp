/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoViewingMatrixElement Inventor/elements/SoViewingMatrixElement.h
  \brief The SoViewingMatrixElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoViewingMatrixElement.h>
#include <assert.h>

/*!
  \fn SoViewingMatrixElement::viewingMatrix

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoViewingMatrixElement);

/*!
  This static method initializes static data for the SoViewingMatrixElement
  class.
*/

void
SoViewingMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoViewingMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoViewingMatrixElement::~SoViewingMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoViewingMatrixElement::set(SoState * const state,
                            SoNode * const node,
                            const SbMatrix & viewingMatrix)
{
  SoViewingMatrixElement * element = (SoViewingMatrixElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (element) {
    element->setElt(viewingMatrix);
  }
}

//! FIXME: write doc.

const SbMatrix &
SoViewingMatrixElement::get(SoState * const state)
{
  SoViewingMatrixElement * element = (SoViewingMatrixElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->viewingMatrix;
}

//! FIXME: write doc.

void
SoViewingMatrixElement::init(SoState * state)
{
  inherited::init(state);
  viewingMatrix.makeIdentity();
#if 0 // debug
  SoDebugError::postInfo("SoViewingMatrixElement::init",
                         "matrix set to identity");
#endif // debug
}

//! FIXME: write doc.

void
SoViewingMatrixElement::setElt(const SbMatrix &matrix)
{
  this->viewingMatrix = matrix;
#if 0 // debug
  SoDebugError::postInfo("SoViewingMatrixElement::setElt",
                         "viewingmatrix:");
  matrix.print(stdout);
#endif // debug
}
