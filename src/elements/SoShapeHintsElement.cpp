/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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


#include <assert.h>

SO_ELEMENT_SOURCE(SoShapeHintsElement);

/*!
  This static method initializes static data for the SoShapeHintsElement
  class.
*/

void
SoShapeHintsElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoShapeHintsElement, inherited);
}

/*!
  The destructor.
*/

SoShapeHintsElement::~SoShapeHintsElement(void)
{
}

//! FIXME: write doc.

void
SoShapeHintsElement::init(SoState * state)
{
  inherited::init(state);
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
                         SoNode * const /* node */,
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
SoShapeHintsElement::print(FILE * /* file */) const
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
                              const FaceType faceType)
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
