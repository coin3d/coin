/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoViewingMatrixElement Inventor/elements/SoViewingMatrixElement.h
  \brief The SoViewingMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoViewingMatrixElement.h>

#include <Inventor/SbName.h>
#ifdef COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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
  The constructor.  Can't be used directly.

  \sa void * SoViewingMatrixElement::createInstance(void)
*/

SoViewingMatrixElement::SoViewingMatrixElement(void)
  : viewingMatrix()
{
    setTypeId(SoViewingMatrixElement::classTypeId);
    setStackIndex(SoViewingMatrixElement::classStackIndex);
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
  element->setElt(viewingMatrix);
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
SoViewingMatrixElement::print(FILE * /* file */) const
{
  COIN_STUB();
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
