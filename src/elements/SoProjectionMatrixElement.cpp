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
  \class SoProjectionMatrix Inventor/elements/SoProjectionMatrixElement.h
  \brief The SoProjectionMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoProjectionMatrixElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource(SoProjectionMatrixElement)

/*!
  \var SoProjectionMatrixElement::classTypeId

  This is the static class type identifier for the
  SoProjectionMatrixElement class.
*/

SoType SoProjectionMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoProjectionMatrixElement::getClassTypeId(void)
{
  return SoProjectionMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoProjectionMatrixElement class.
*/
void *
SoProjectionMatrixElement::createInstance(void)
{
  return (void *) new SoProjectionMatrixElement;
}

/*!
  \var SoProjectionMatrixElement::classStackIndex

  This is the static state stack index for the
  SoProjectionMatrixElement class.
*/
int SoProjectionMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoProjectionMatrixElement class.
*/
int
SoProjectionMatrixElement::getClassStackIndex(void)
{
  return SoProjectionMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoProjectionMatrixElement
  class.
*/

void
SoProjectionMatrixElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoProjectionMatrixElement)
  assert(SoProjectionMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoProjectionMatrixElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoProjectionMatrixElement",
                       &SoProjectionMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoProjectionMatrixElement::classStackIndex =
      createStackIndex(SoProjectionMatrixElement::classTypeId);
  }
  else {
    SoProjectionMatrixElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoProjectionMatrixElement::createInstance(void)
*/

SoProjectionMatrixElement::SoProjectionMatrixElement(void)
  : projectionMatrix()
{
    setTypeId(SoProjectionMatrixElement::classTypeId);
    setStackIndex(SoProjectionMatrixElement::classStackIndex);
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
  element->setElt(projectionMatrix);
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
#if 0 // FIXME: there's still a bug here somewhere. 981029 mortene.
  // DEBUG
  this->projectionMatrix.print(stdout);
#endif // 0
}

//! FIXME: write doc.

void
SoProjectionMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->projectionMatrix.makeIdentity();
}
