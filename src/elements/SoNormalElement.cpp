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
  \class SoNormalElement Inventor/elements/SoNormalElement.h
  \brief The SoNormalElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoNormalElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

static SbVec3f defaultNormal(0.0f, 0.0f, 1.0f);

/*!
  \fn SoNormalElement::numNormals

  FIXME: write doc.
*/

/*!
  \fn SoNormalElement::normals

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoNormalElement);

/*!
  This static method initializes static data for the SoNormalElement class.
*/

void
SoNormalElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoNormalElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoNormalElement::createInstance(void)
*/

SoNormalElement::SoNormalElement()
{
  setTypeId(SoNormalElement::classTypeId);
  setStackIndex(SoNormalElement::classStackIndex);
}

/*!
  The destructor.
*/

SoNormalElement::~SoNormalElement()
{
}

//! FIXME: write doc.

void
SoNormalElement::set(SoState * const state,
                     SoNode * const node,
                     const int32_t numNormals,
                     const SbVec3f * const normals,
                     const SbBool unitLength)
{
  SoNormalElement *elem = (SoNormalElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->normals = normals;
  elem->numNormals = numNormals;
  elem->unitLength = unitLength;
}

//! FIXME: write doc.

void
SoNormalElement::print(FILE * /* file */) const
{
  assert(0 && "FIXME: not implemented");
}

//! FIXME: write doc.

void
SoNormalElement::init(SoState *state)
{
  inherited::init(state);
  this->normals = NULL;
  this->numNormals = 0;
  this->unitLength = FALSE;
}

//! FIXME: write doc.

const SoNormalElement *
SoNormalElement::getInstance(SoState * const state)
{
  return (const SoNormalElement *)
    SoElement::getConstElement(state, classStackIndex);
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoNormalElement::getNum() const
{
  return this->numNormals;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SbVec3f &
SoNormalElement::get(const int index) const
{
  assert(index >= 0 && index < getNum());
  return this->normals[index];
}

/*!
  Returns a pointer to the normal array. This method is not part of the OIV API.
*/
const SbVec3f *
SoNormalElement::getArrayPtr() const
{
  return this->normals;
}

//$ EXPORT INLINE
SbBool
SoNormalElement::areNormalsUnitLength() const
{
  return unitLength;
}
