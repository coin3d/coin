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

//$ BEGIN TEMPLATE ElementSource(SoViewingMatrixElement)

/*!
  \var SoViewingMatrixElement::classTypeId

  This is the static class type identifier for the
  SoViewingMatrixElement class.
*/

SoType SoViewingMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoViewingMatrixElement::getClassTypeId(void)
{
  return SoViewingMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoViewingMatrixElement class.
*/
void *
SoViewingMatrixElement::createInstance(void)
{
  return (void *) new SoViewingMatrixElement;
}

/*!
  \var SoViewingMatrixElement::classStackIndex

  This is the static state stack index for the
  SoViewingMatrixElement class.
*/
int SoViewingMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoViewingMatrixElement class.
*/
int
SoViewingMatrixElement::getClassStackIndex(void)
{
  return SoViewingMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoViewingMatrixElement
  class.
*/

void
SoViewingMatrixElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoViewingMatrixElement)
  assert(SoViewingMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoViewingMatrixElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoViewingMatrixElement",
                       &SoViewingMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoViewingMatrixElement::classStackIndex =
      createStackIndex(SoViewingMatrixElement::classTypeId);
  }
  else {
    SoViewingMatrixElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
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
  assert(0 && "FIXME: not implemented");
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
