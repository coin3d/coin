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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoBBoxModelMatrixElement Inventor/elements/SoBBoxModelMatrixElement.h
  \brief The SoBBoxModelMatrixElement class keeps track of the current model
  matrix during a scene graph traversal.  It is used by amongst others the
  SoGetBoundingBoxAction class.
*/

#include <Inventor/elements/SoBBoxModelMatrixElement.h>

#include <Inventor/SbName.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>

//$ BEGIN TEMPLATE ElementSource( SoBBoxModelMatrixElement )

/*!
  \var SoBBoxModelMatrixElement::classTypeId

  This is the static class type identifier for the
  SoBBoxModelMatrixElement class.
*/

SoType SoBBoxModelMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoBBoxModelMatrixElement::getClassTypeId(void)
{
  return SoBBoxModelMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoBBoxModelMatrixElement class.
*/
void *
SoBBoxModelMatrixElement::createInstance(void)
{
  return (void *) new SoBBoxModelMatrixElement;
}

/*!
  \var SoBBoxModelMatrixElement::classStackIndex

  This is the static state stack index for the
  SoBBoxModelMatrixElement class.
*/
int SoBBoxModelMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoBBoxModelMatrixElement class.
*/
int
SoBBoxModelMatrixElement::getClassStackIndex(void)
{
  return SoBBoxModelMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoBBoxModelMatrixElement class.
*/

void
SoBBoxModelMatrixElement::initClass(
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoBBoxModelMatrixElement )
  assert(SoBBoxModelMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoBBoxModelMatrixElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoBBoxModelMatrixElement",
    &SoBBoxModelMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoBBoxModelMatrixElement::classStackIndex =
      createStackIndex( SoBBoxModelMatrixElement::classTypeId );
  } else {
    SoBBoxModelMatrixElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoBBoxModelMatrixElement class.
*/

void
SoBBoxModelMatrixElement::cleanClass(
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoBBoxModelMatrixElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoBBoxModelMatrixElement::createInstance()
*/

SoBBoxModelMatrixElement::SoBBoxModelMatrixElement()
  : state(NULL)
{
  setTypeId( SoBBoxModelMatrixElement::classTypeId );
  setStackIndex( SoBBoxModelMatrixElement::classStackIndex );
} // SoBBoxModelMatrixElement()

/*!
  The destructor.
*/

SoBBoxModelMatrixElement::~SoBBoxModelMatrixElement()
{
} // ~SoBBoxModelMatrixElement()

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state;
} // init()

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::push(SoState * state)
{
  inherited::push(state);
  
  SoBBoxModelMatrixElement * const element =
    (SoBBoxModelMatrixElement *)( this->next );
  element->state = this->state;
} // push()

/*!
  This method is for the SoGetBoundingBoxAction class so it can reset the
  current model matrix and all local matrices to identity.
*/

void
SoBBoxModelMatrixElement::reset(SoState * const state,
				SoNode * const node)
{
  assert(0 && "FIXME: not implemented");
} // reset()

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
} // pushMatrix()

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
} // popMatrix()
 
//! FIXME: write doc.

void
SoBBoxModelMatrixElement::makeEltIdentity()
{
  inherited::makeEltIdentity();
  SoLocalBBoxMatrixElement::makeIdentity(state);
} // makeEltIdentity()

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::setElt(const SbMatrix &matrix)
{
  inherited::setElt(matrix);
  SoLocalBBoxMatrixElement::set(state, matrix);
} // setElt()

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::multElt(const SbMatrix & matrix)
{
  inherited::multElt(matrix);
  SoLocalBBoxMatrixElement::mult(this->state, matrix);
} // multElt()

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::translateEltBy(const SbVec3f & translation)
{
  inherited::translateEltBy(translation);
  SoLocalBBoxMatrixElement::translateBy(this->state, translation);
} // translateByElt()

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::rotateEltBy(const SbRotation &rotation)
{
  inherited::rotateEltBy(rotation);
  SoLocalBBoxMatrixElement::rotateBy(this->state, rotation);
} // rotateByElt()

//! FIXME: write doc.

void
SoBBoxModelMatrixElement::scaleEltBy(const SbVec3f &scaleFactor)
{
  inherited::scaleEltBy(scaleFactor);
  SoLocalBBoxMatrixElement::scaleBy(this->state, scaleFactor);
} // scaleByElt()
 
/*!
  This method is for debug use only.
*/

SbMatrix
SoBBoxModelMatrixElement::pushMatrixElt()
{
  assert(0 && "Should never be called!");
  return SbMatrix::identity();
} // pushMatrixElt()

/*!
  This method is for debug use only.
*/

void
SoBBoxModelMatrixElement::popMatrixElt(const SbMatrix &)
{
  assert(0 && "Should never be called!");
} // popMatrixElt()

