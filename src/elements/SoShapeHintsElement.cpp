/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoShapehintsElement Inventor/elements/SoShapeHintsElement.h
  \brief The SoShapeHintsElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoLazyElement.h>

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
  SoShapeHintsElement * prev = (SoShapeHintsElement*) this->getNextInStack();
  this->vertexOrdering = prev->vertexOrdering;
  this->shapeType = prev->shapeType;
  this->faceType = prev->faceType;
}

void 
SoShapeHintsElement::pop(SoState * state, const SoElement * prevtopelement)
{
  this->updateLazyElement(state);
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
  if (elem) {
    elem->setElt(vertexOrdering, shapeType, faceType);
    elem->updateLazyElement(state);
  }
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

void 
SoShapeHintsElement::updateLazyElement(SoState * state)
{
  if (state->isElementEnabled(SoLazyElement::getClassStackIndex())) {
    SoLazyElement::setVertexOrdering(state, this->vertexOrdering == CLOCKWISE ?
                                     SoLazyElement::CW : SoLazyElement::CCW);
    SoLazyElement::setTwosideLighting(state, this->vertexOrdering != UNKNOWN_ORDERING &&
                                      this->shapeType == UNKNOWN_SHAPE_TYPE);
    SoLazyElement::setBackfaceCulling(state, this->vertexOrdering != UNKNOWN_ORDERING &&
                                      this->shapeType == SOLID);
  }
}
