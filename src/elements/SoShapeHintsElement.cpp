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
  \class SoShapehintsElement Inventor/elements/SoShapeHintsElement.h
  \brief The SoShapeHintsElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoShapeHintsElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoShapeHintsElement )

/*!
  \var SoShapeHintsElement::classTypeId

  This is the static class type identifier for the
  SoShapeHintsElement class.
*/

SoType SoShapeHintsElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoShapeHintsElement::getClassTypeId(void)
{
  return SoShapeHintsElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoShapeHintsElement class.
*/
void *
SoShapeHintsElement::createInstance(void)
{
  return (void *) new SoShapeHintsElement;
}

/*!
  \var SoShapeHintsElement::classStackIndex

  This is the static state stack index for the
  SoShapeHintsElement class.
*/
int SoShapeHintsElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoShapeHintsElement class.
*/
int
SoShapeHintsElement::getClassStackIndex(void)
{
  return SoShapeHintsElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoShapeHintsElement
  class.
*/

void
SoShapeHintsElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoShapeHintsElement )
  assert(SoShapeHintsElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoShapeHintsElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoShapeHintsElement",
    &SoShapeHintsElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoShapeHintsElement::classStackIndex =
      createStackIndex( SoShapeHintsElement::classTypeId );
  } else {
    SoShapeHintsElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoShapeHintsElement class.
*/

void
SoShapeHintsElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoShapeHintsElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoShapeHintsElement::createInstance( void )
*/

SoShapeHintsElement::SoShapeHintsElement(
    void )
  : vertexOrdering( UNKNOWN_ORDERING ),
    shapeType( UNKNOWN_SHAPE_TYPE ), faceType( CONVEX )
{
    setTypeId( SoShapeHintsElement::classTypeId );
    setStackIndex( SoShapeHintsElement::classStackIndex );
}

/*!
  The destructor.
*/

SoShapeHintsElement::~SoShapeHintsElement(
    void )
{
}

//! FIXME: write doc.

void
SoShapeHintsElement::init( SoState * state ) // virtual
{
  inherited::init( state );
  this->vertexOrdering = getDefaultVertexOrdering();
  this->shapeType = getDefaultShapeType();
  this->faceType = getDefaultFaceType();
}

//! FIXME: write doc.

void 
SoShapeHintsElement::push(SoState * state)
{
  inherited::push(state);
  SoShapeHintsElement *elem = (SoShapeHintsElement*)this->next;
  elem->vertexOrdering = this->vertexOrdering;
  elem->shapeType = this->shapeType;
  elem->faceType = this->faceType;
}

//! FIXME: write doc.

SbBool
SoShapeHintsElement::matches(const SoElement * element) const
{
  SoShapeHintsElement *elem = (SoShapeHintsElement*)element;
  return (this->vertexOrdering == elem->vertexOrdering &&
	  this->shapeType == elem->shapeType &&
	  this->faceType == elem->faceType);
  
}

//! FIXME: write doc.

SoElement *
SoShapeHintsElement::copyMatchInfo() const
{
  SoShapeHintsElement *elem = (SoShapeHintsElement*)
    getTypeId().createInstance();
  elem->vertexOrdering = this->vertexOrdering;
  elem->shapeType = this->shapeType;
  elem->faceType = this->faceType;
  return elem;
}

//! FIXME: write doc.

void
SoShapeHintsElement::set(SoState * const state,
			 SoNode * const node,
			 const VertexOrdering vertexOrdering,
			 const ShapeType shapeType,
			 const FaceType faceType)
{
  SoShapeHintsElement *elem = (SoShapeHintsElement*)
    SoElement::getElement(state, classStackIndex);

  elem->setElt(vertexOrdering, shapeType, faceType);
}
 
//! FIXME: write doc.

void
SoShapeHintsElement::get(SoState * const state,
			 VertexOrdering & vertexOrdering,
			 ShapeType & shapeType,
			 FaceType & faceType)
{
  SoShapeHintsElement *elem = (SoShapeHintsElement*)
    SoElement::getConstElement(state, classStackIndex);
  vertexOrdering = elem->vertexOrdering;
  shapeType = elem->shapeType;
  faceType = elem->faceType;
}

//! FIXME: write doc.

SoShapeHintsElement::VertexOrdering 
SoShapeHintsElement::getVertexOrdering(SoState * const state)
{
  SoShapeHintsElement *elem = (SoShapeHintsElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->vertexOrdering;
}

//! FIXME: write doc.

SoShapeHintsElement::ShapeType 
SoShapeHintsElement::getShapeType(SoState * const state)
{
  SoShapeHintsElement *elem = (SoShapeHintsElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->shapeType;
}

//! FIXME: write doc.

SoShapeHintsElement::FaceType 
SoShapeHintsElement::getFaceType(SoState * const state)
{
  SoShapeHintsElement *elem = (SoShapeHintsElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->faceType;
}

//! FIXME: write doc.

void
SoShapeHintsElement::print(FILE * file) const
{
}
 
//! FIXME: write doc.

void
SoShapeHintsElement::setElt(VertexOrdering vertexOrdering,
			    ShapeType shapeType,
			    FaceType faceType)
{
  if (vertexOrdering != ORDERING_AS_IS) {
    this->vertexOrdering = vertexOrdering;
  }
  if (shapeType != SHAPE_TYPE_AS_IS) {
    this->shapeType = shapeType;
  }
  if (faceType != FACE_TYPE_AS_IS) {
    this->faceType = faceType;
  }
}

//! FIXME: write doc.

//$ EXPORT INLINE
void SoShapeHintsElement::set(SoState * const state,
			      const VertexOrdering vertexOrdering,
			      const ShapeType shapeType, 
			      const FaceType faceType )
{ 
  set(state, NULL, vertexOrdering, shapeType, faceType); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoShapeHintsElement::VertexOrdering 
SoShapeHintsElement::getDefaultVertexOrdering()
{ 
  return UNKNOWN_ORDERING; 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoShapeHintsElement::ShapeType 
SoShapeHintsElement::getDefaultShapeType()
{ 
  return UNKNOWN_SHAPE_TYPE; 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoShapeHintsElement::FaceType 
SoShapeHintsElement::getDefaultFaceType()
{ 
  return CONVEX; 
}

