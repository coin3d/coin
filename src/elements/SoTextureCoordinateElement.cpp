/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoTextureCoordinateElement Inventor/elements/SoTextureCoordinateElement.h
  \brief The SoTextureCoordinateElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureCoordinateElement.h>

#include "SbBasicP.h"

#include <Inventor/elements/SoGLVBOElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <cassert>

/*!
  \fn SoTextureCoordinateElement::CoordType

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::whatKind

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::funcCB

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::funcCBData

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::numCoords

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::coords2

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::coords3

  FIXME: write doc.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/

/*!
  \fn SoTextureCoordinateElement::coords4

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::convert2

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::convert3

  FIXME: write doc.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/

/*!
  \fn SoTextureCoordinateElement::convert4

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::coordsDimension

  FIXME: write doc.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/


SO_ELEMENT_SOURCE(SoTextureCoordinateElement);

/*!
  This static method initializes static data for the
  SoTextureCoordinateElement class.
*/

void
SoTextureCoordinateElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoTextureCoordinateElement, inherited);
}

/*!
  The destructor.
*/

SoTextureCoordinateElement::~SoTextureCoordinateElement()
{
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::setDefault(SoState * const state,
                                       SoNode * const node)
{
  if (state->isElementEnabled(SoGLVBOElement::getClassStackIndex())) {
    SoGLVBOElement::setTexCoordVBO(state, 0, NULL);
  }
  SoTextureCoordinateElement * element = coin_safe_cast<SoTextureCoordinateElement *>
    (
     SoReplacedElement::getElement(state, classStackIndex, node)
     );
  if (element) {
    element->whatKind = DEFAULT;
    element->numCoords = 0;
  }
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::setFunction(SoState * const state,
                                        SoNode * const node,
                                        SoTextureCoordinateFunctionCB * const func,
                                        void * const userdata)
{
  if (state->isElementEnabled(SoGLVBOElement::getClassStackIndex())) {
    SoGLVBOElement::setTexCoordVBO(state, 0, NULL);
  }
  SoTextureCoordinateElement * element = coin_safe_cast<SoTextureCoordinateElement *>
    (
     SoReplacedElement::getElement(state, classStackIndex, node)
     );
  if (element) {
    element->funcCB = func;
    element->funcCBData = userdata;
    element->whatKind = FUNCTION;
    element->coords2 = NULL;
    element->coords3 = NULL;
    element->coords4 = NULL;
    element->numCoords = 0;
  }
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::set2(SoState * const state,
                                 SoNode * const node,
                                 const int32_t numCoords,
                                 const SbVec2f * const coords)
{
  if (state->isElementEnabled(SoGLVBOElement::getClassStackIndex())) {
    SoGLVBOElement::setTexCoordVBO(state, 0, NULL);
  }
  SoTextureCoordinateElement * element = coin_assert_cast<SoTextureCoordinateElement *>
    (
     SoReplacedElement::getElement(state, classStackIndex, node)
     );
  if (element) {
    element->coordsDimension = 2;
    element->numCoords = numCoords;
    element->coords2 = coords;
    element->coords3 = NULL;
    element->coords4 = NULL;
    element->whatKind = EXPLICIT;
  }
}

/*!
  FIXME: write doc.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
void
SoTextureCoordinateElement::set3(SoState * const state,
                                 SoNode * const node,
                                 const int32_t numCoords,
                                 const SbVec3f * const coords)
{
  if (state->isElementEnabled(SoGLVBOElement::getClassStackIndex())) {
    SoGLVBOElement::setTexCoordVBO(state, 0, NULL);
  }
  SoTextureCoordinateElement * element =
    coin_assert_cast<SoTextureCoordinateElement *>
    (
     SoReplacedElement::getElement(state, classStackIndex, node)
     );
  if (element) {
    element->coordsDimension = 3;
    element->numCoords = numCoords;
    element->coords2 = NULL;
    element->coords3 = coords;
    element->coords4 = NULL;
    element->whatKind = EXPLICIT;
  }
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::set4(SoState * const state,
                                 SoNode * const node,
                                 const int32_t numCoords,
                                 const SbVec4f * const coords)
{
  if (state->isElementEnabled(SoGLVBOElement::getClassStackIndex())) {
    SoGLVBOElement::setTexCoordVBO(state, 0, NULL);
  }
  SoTextureCoordinateElement * element =
    coin_assert_cast<SoTextureCoordinateElement *>
    (
     SoReplacedElement::getElement(state, classStackIndex, node)
     );
  if (element) {
    element->coordsDimension = 4;
    element->numCoords = numCoords;
    element->coords2 = NULL;
    element->coords3 = NULL;
    element->coords4 = coords;
    element->whatKind = EXPLICIT;
  }
}

//! FIXME: write doc.

const SoTextureCoordinateElement *
SoTextureCoordinateElement::getInstance(SoState * const state)
{
  return coin_assert_cast<const SoTextureCoordinateElement *>
    (getConstElement(state, classStackIndex));
}

/*!
  This method returns texture coordinate for the given point and normal.
  The coordinate is returned as a 4D vector where the r and q coordinates
  may be set to 0 and 1 respecively depending on what texture coordinate
  dimension we're using.

  This method should only be used if the CoordType is FUNCTION.
*/

const SbVec4f &
SoTextureCoordinateElement::get(const SbVec3f & point,
                                const SbVec3f & normal) const
{
  assert((this->whatKind == FUNCTION || this->whatKind == TEXGEN) && this->funcCB);
  return (*(this->funcCB))(this->funcCBData, point, normal);
}

//! FIXME: write doc.

const SbVec2f &
SoTextureCoordinateElement::get2(const int index) const
{
  assert(index >= 0 && index < this->numCoords);
  assert(this->whatKind == EXPLICIT);
  if (this->coordsDimension==2) {
    return this->coords2[index];
  }
  else {
    // need an instance we can write to
    SoTextureCoordinateElement * elem = const_cast<SoTextureCoordinateElement *>(this);

    float tmp = this->coords4[index][3];
    float to2D = tmp == 0.0f ? 1.0f : 1.0f / tmp;

    elem->convert2.setValue(this->coords4[index][0] * to2D,
                            this->coords4[index][1] * to2D);
    return this->convert2;
  }
}

/*!
  FIXME: write doc.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
const SbVec3f &
SoTextureCoordinateElement::get3(const int index) const
{
  assert(index >= 0 && index < this->numCoords);
  assert(this->whatKind == EXPLICIT);
  if (this->coordsDimension==3) {
    return this->coords3[index];
  }
  else {
    // need an instance we can write to
    SoTextureCoordinateElement * elem = const_cast<SoTextureCoordinateElement *>(this);

    if (this->coordsDimension==2) {
      elem->convert3.setValue(this->coords2[index][0],
                              this->coords2[index][1],
                              0.0f);
    }
    else { // this->coordsDimension==4
      this->coords4[index].getReal(elem->convert3);
    }
    return this->convert3;
  }
}

//!  FIXME: write doc.

const SbVec4f &
SoTextureCoordinateElement::get4(const int index) const
{
  assert(this->whatKind == EXPLICIT);
  if (index >= this->numCoords) {
    static SbBool first = TRUE;
    if (first) {
      first = FALSE;
      SoDebugError::post("SoTextureCoordinateElement::get4",
                         "Index value %d into texture coordinate array "
                         "of size %d is out of bounds. This is usually an "
                         "indication that too few texture coordinates "
                         "were supplied in the scene graph.",
                         index, this->numCoords);

    }
    // need an instance we can write to
    SoTextureCoordinateElement * elem = const_cast<SoTextureCoordinateElement *>(this);
    elem->convert4 = SbVec4f(0.0f, 0.0f, 0.0f, 1.0f);
    return this->convert4;
  }
  if (this->coordsDimension==4) {
    return this->coords4[index];
  }
  else {
    // need an instance we can write to
    SoTextureCoordinateElement * elem = const_cast<SoTextureCoordinateElement *>(this);

    if (this->coordsDimension==2) {
      elem->convert4.setValue(this->coords2[index][0],
                              this->coords2[index][1],
                              0.0f,
                              1.0f);
    }
    else { // this->coordsDimension==3
      elem->convert4.setValue(this->coords3[index][0],
                              this->coords3[index][1],
                              this->coords3[index][2],
                              1.0f);
    }
    return this->convert4;
  }
}

/*!
  This method is used by shapes.  Three return values are possible.

  DEFAULT means that the shapes should generate their own texture coordinates.

  EXPLICIT means that discrete texture coordinates are stored, and should be
  fetched with get2(), get3() or get4().

  FUNCTION means that get(point, normal) must be used to generate texture
  coordinates.
*/

SoTextureCoordinateElement::CoordType
SoTextureCoordinateElement::getType(SoState * const state)
{
  const SoTextureCoordinateElement * element =
    coin_assert_cast<const SoTextureCoordinateElement *>
    (getConstElement(state, classStackIndex));
  return element->getType();
}

//! FIXME: write doc.

SoTextureCoordinateElement::CoordType
SoTextureCoordinateElement::getType() const
{
  return this->whatKind;
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::init(SoState * state)
{
  inherited::init(state);
  whatKind = DEFAULT;
  funcCB = NULL;
  funcCBData = NULL;
  numCoords = 0;
  coords2 = NULL;
  coords3 = NULL;
  coords4 = NULL;
  coordsDimension = 2; //Initialize to 2D as before
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoTextureCoordinateElement::getNum() const
{
  return this->numCoords;
}

//! FIXME: write doc. (for backwards compability. Use getDimension() instead).

//$ EXPORT INLINE
SbBool
SoTextureCoordinateElement::is2D() const
{
  return (this->coordsDimension==2);
}

/*! FIXME: write doc.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
//$ EXPORT INLINE
int32_t
SoTextureCoordinateElement::getDimension() const
{
  return this->coordsDimension;
}

/*!
  Returns a pointer to the 2D texture coordinate array. This method is not
  part of the OIV API.
*/
const SbVec2f *
SoTextureCoordinateElement::getArrayPtr2() const
{
  return this->coords2;
}

/*!
  Returns a pointer to the 3D texture coordinate array.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
const SbVec3f *
SoTextureCoordinateElement::getArrayPtr3() const
{
  return this->coords3;
}

/*!
  Returns a pointer to the 4D texture coordinate array. This method is not
  part of the OIV API.
*/
const SbVec4f *
SoTextureCoordinateElement::getArrayPtr4() const
{
  return this->coords4;
}
