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
  \class SoTextureMatrixElement Inventor/elements/SoTextureMatrixElement.h
  \brief The SoTextureMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureMatrixElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoTextureMatrixElement::textureMatrix

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoTextureMatrixElement )

/*!
  \var SoTextureMatrixElement::classTypeId

  This is the static class type identifier for the
  SoTextureMatrixElement class.
*/

SoType SoTextureMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoTextureMatrixElement::getClassTypeId(void)
{
  return SoTextureMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoTextureMatrixElement class.
*/
void *
SoTextureMatrixElement::createInstance(void)
{
  return (void *) new SoTextureMatrixElement;
}

/*!
  \var SoTextureMatrixElement::classStackIndex

  This is the static state stack index for the
  SoTextureMatrixElement class.
*/
int SoTextureMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoTextureMatrixElement class.
*/
int
SoTextureMatrixElement::getClassStackIndex(void)
{
  return SoTextureMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoTextureMatrixElement class.
*/

void
SoTextureMatrixElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoTextureMatrixElement )
  assert(SoTextureMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextureMatrixElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoTextureMatrixElement",
    &SoTextureMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoTextureMatrixElement::classStackIndex =
      createStackIndex( SoTextureMatrixElement::classTypeId );
  } else {
    SoTextureMatrixElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoTextureMatrixElement class.
*/

void
SoTextureMatrixElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoTextureMatrixElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoTextureMatrixElement::createInstance( void )
*/

SoTextureMatrixElement::SoTextureMatrixElement(
    void )
  : textureMatrix()
{
    setTypeId( SoTextureMatrixElement::classTypeId );
    setStackIndex( SoTextureMatrixElement::classStackIndex );
}

/*!
  The destructor.
*/

SoTextureMatrixElement::~SoTextureMatrixElement(
    void )
{
}

//! FIXME: write doc.

void
SoTextureMatrixElement::makeIdentity( // static
    SoState * const state,
    SoNode * const node )
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->makeEltIdentity();
  if (node) elem->setNodeId(node);
}

//! FIXME: write doc.

void
SoTextureMatrixElement::mult(SoState * const state,
			   SoNode * const node,
			   const SbMatrix &matrix)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->multElt(matrix);
  if (node) elem->addNodeId(node);
}

//! FIXME: write doc.

void
SoTextureMatrixElement::translateBy(SoState * const state,
				  SoNode * const node,
				  const SbVec3f &translation)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->translateEltBy(translation);
  if (node) elem->addNodeId(node);
  
}

//! FIXME: write doc.

void
SoTextureMatrixElement::rotateBy(SoState * const state,
			       SoNode * const node,
			       const SbRotation & rotation)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->rotateEltBy(rotation);
  if (node) elem->addNodeId(node);

}

//! FIXME: write doc.

void
SoTextureMatrixElement::scaleBy(SoState * const state,
			      SoNode * const node,
			      const SbVec3f & scaleFactor)
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->scaleEltBy(scaleFactor);
  if (node) elem->addNodeId(node);
}

//! FIXME: write doc.

const SbMatrix &
SoTextureMatrixElement::get( // static
    SoState * const state )
{
  SoTextureMatrixElement *elem = (SoTextureMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->getElt();
}

//! FIXME: write doc.

void
SoTextureMatrixElement::print( // virtual
    FILE * file ) const
{
    fprintf( file, "SoTextureMatrixElement[%p]\n", this );
}

//! FIXME: write doc.

void
SoTextureMatrixElement::makeEltIdentity(void)
{
  this->textureMatrix.makeIdentity();
}

//! FIXME: write doc.

void
SoTextureMatrixElement::multElt(const SbMatrix & matrix)
{
  this->textureMatrix.multRight(matrix);
}

//! FIXME: write doc.

void
SoTextureMatrixElement::translateEltBy(const SbVec3f & translation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setTranslate(translation);
  this->textureMatrix.multRight(matrix);
}

//! FIXME: write doc.

void
SoTextureMatrixElement::rotateEltBy(const SbRotation & rotation)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setRotate(rotation);
  this->textureMatrix.multRight(matrix);
}

//! FIXME: write doc.

void
SoTextureMatrixElement::scaleEltBy(const SbVec3f & scaleFactor)
{
  SbMatrix matrix = SbMatrix::identity();
  matrix.setScale(scaleFactor);
  this->textureMatrix.multRight(matrix);
}

//! FIXME: write doc.

const SbMatrix &
SoTextureMatrixElement::getElt(
    void ) const
{
  return this->textureMatrix;
}

//! FIXME: write doc.

void
SoTextureMatrixElement::init(
    SoState * state )
{
    inherited::init( state );
    this->textureMatrix.makeIdentity();
}

//! FIXME: write doc.

void
SoTextureMatrixElement::push(
    SoState * state )
{
    inherited::push( state );

    SoTextureMatrixElement * const element =
        (SoTextureMatrixElement *)( this->next );
    element->textureMatrix = this->textureMatrix;
}

//! FIXME: write doc.

void
SoTextureMatrixElement::pop(
    SoState * state,
    const SoElement * prevTopElement )
{
    inherited::pop( state, prevTopElement );
}

/*!
  FIXME: write doc.
*/

void
SoTextureMatrixElement::emptyMatrix(
  SoState * const state )
{
  assert(0 && "FIXME: not implemented");
}

