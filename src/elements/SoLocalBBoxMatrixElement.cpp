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
  \class SoLocalBBoxMatrixElement Inventor/elements/SoLocalBBoxMatrixElement.h
  \brief The SoLocalBBoxMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLocalBBoxMatrixElement.h>

#include <Inventor/SbName.h>

/*!
  \fn SoLocalBBoxMatrixElement::localMatrix

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource(SoLocalBBoxMatrixElement)

/*!
  \var SoLocalBBoxMatrixElement::classTypeId

  This is the static class type identifier for the
  SoLocalBBoxMatrixElement class.
*/

SoType SoLocalBBoxMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoLocalBBoxMatrixElement::getClassTypeId(void)
{
  return SoLocalBBoxMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoLocalBBoxMatrixElement class.
*/
void *
SoLocalBBoxMatrixElement::createInstance(void)
{
  return (void *) new SoLocalBBoxMatrixElement;
}

/*!
  \var SoLocalBBoxMatrixElement::classStackIndex

  This is the static state stack index for the
  SoLocalBBoxMatrixElement class.
*/
int SoLocalBBoxMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoLocalBBoxMatrixElement class.
*/
int
SoLocalBBoxMatrixElement::getClassStackIndex(void)
{
  return SoLocalBBoxMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoLocalBBoxMatrixElement class.
*/

void
SoLocalBBoxMatrixElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoLocalBBoxMatrixElement)
  assert(SoLocalBBoxMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLocalBBoxMatrixElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoLocalBBoxMatrixElement",
                       &SoLocalBBoxMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoLocalBBoxMatrixElement::classStackIndex =
      createStackIndex(SoLocalBBoxMatrixElement::classTypeId);
  }
  else {
    SoLocalBBoxMatrixElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoLocalBBoxMatrixElement class.
*/

void
SoLocalBBoxMatrixElement::cleanClass(void)
{
//$ BEGIN TEMPLATE CleanElementSource(SoLocalBBoxMatrixElement)
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoLocalBBoxMatrixElement::createInstance(void)
*/

SoLocalBBoxMatrixElement::SoLocalBBoxMatrixElement()
  : localMatrix(), modelInverseMatrix()
{
  setTypeId(SoLocalBBoxMatrixElement::classTypeId);
  setStackIndex(SoLocalBBoxMatrixElement:: classStackIndex);
}

/*!
  The destructor.
*/

SoLocalBBoxMatrixElement::~SoLocalBBoxMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->localMatrix.makeIdentity();
  this->modelInverseMatrix.makeIdentity();
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::push(SoState * state)
{
  inherited::push(state);
  SoLocalBBoxMatrixElement *elem = 
    (SoLocalBBoxMatrixElement*) this->next;
  elem->localMatrix = this->localMatrix;
  elem->modelInverseMatrix = this->modelInverseMatrix;
}

//! FIXME: write doc.

SbBool
SoLocalBBoxMatrixElement::matches(const SoElement * /* element */) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoLocalBBoxMatrixElement::copyMatchInfo(void) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::makeIdentity(SoState * const state)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *) 
    getElement(state, getClassStackIndex());
  if (element) {
    element->localMatrix.makeIdentity();
    element->modelInverseMatrix = SoModelMatrixElement::get(state).inverse();
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::set(SoState * const state,
			      const SbMatrix & matrix)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *) 
    getElement(state, getClassStackIndex());
  if (element) {
    element->localMatrix = matrix;
    // FIXME: no idea whether this is correct or not
    // FIXME2: what the puck is this supposed to be good for?? 990404 mortene.
    element->localMatrix.multRight(element->modelInverseMatrix);
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::mult(SoState * const state,
			       const SbMatrix & matrix)
{
  SoLocalBBoxMatrixElement * element = (SoLocalBBoxMatrixElement *) 
    getElement(state, getClassStackIndex());

  if (element) element->localMatrix.multLeft(matrix);
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::translateBy(SoState * const state,
				      const SbVec3f & translation)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *) 
    getElement(state, getClassStackIndex());
  if (element) {
    SbMatrix matrix;
    matrix.setTranslate(translation);
    element->localMatrix.multLeft(matrix);
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::rotateBy(SoState * const state,
				   const SbRotation & rotation)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *) 
    getElement(state, getClassStackIndex());
  if (element) {
    SbMatrix matrix;
    matrix.setRotate(rotation);
    element->localMatrix.multLeft(matrix);
  }
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::scaleBy(SoState * const state,
				  const SbVec3f & scaleFactor)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *) 
    getElement(state, getClassStackIndex());
  if (element) {
    SbMatrix matrix;
    matrix.setScale(scaleFactor);
    element->localMatrix.multLeft(matrix);
  }
}

//! FIXME: write doc.

SbMatrix
SoLocalBBoxMatrixElement::pushMatrix(SoState * const state)
{
  SoLocalBBoxMatrixElement *elem = (SoLocalBBoxMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->localMatrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::popMatrix(SoState * const state,
				    const SbMatrix & matrix)
{
  // FIXME: should I use getElement() instead of getConstElement()
  SoLocalBBoxMatrixElement *elem = (SoLocalBBoxMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  elem->localMatrix = matrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::resetAll(SoState * const /* state */)
{
  assert(0 && "FIXME: not implemented");
}

//! FIXME: write doc.

const SbMatrix & 
SoLocalBBoxMatrixElement::get(SoState * const state)
{
  SoLocalBBoxMatrixElement * element;
  element = (SoLocalBBoxMatrixElement *) 
    getConstElement(state, getClassStackIndex());
  return element->localMatrix;
}

//! FIXME: write doc.

void
SoLocalBBoxMatrixElement::print(FILE * /* file */) const
{
  assert(0 && "FIXME: not implemented");
}

