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
  \class SoTextureCoordinateElement Inventor/elements/SoTextureCoordinateElement.h
  \brief The SoTextureCoordinateElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureCoordinateElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

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
  \fn SoTextureCoordinateElement::coords4

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::convert2

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::convert4

  FIXME: write doc.
*/

/*!
  \fn SoTextureCoordinateElement::coordsAre2D

  FIXME: write doc.
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
  A constructor.  Can't be used directly.

  \sa void * SoTextureCoordinateElement::createInstance(void)
*/

SoTextureCoordinateElement::SoTextureCoordinateElement()
  : coordsAre2D(TRUE)
{
  setTypeId(SoTextureCoordinateElement::classTypeId);
  setStackIndex(SoTextureCoordinateElement::classStackIndex);
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
  SoTextureCoordinateElement * element = (SoTextureCoordinateElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->whatKind = DEFAULT;
  element->numCoords = 0;
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::setFunction(SoState * const state,
                                        SoNode * const node,
                                        SoTextureCoordinateFunctionCB * const func,
                                        void * const userdata)
{
  SoTextureCoordinateElement * element = (SoTextureCoordinateElement *)
   SoReplacedElement::getElement(state, classStackIndex, node);
  element->funcCB = func;
  element->funcCBData = userdata;
  element->whatKind = FUNCTION;
  element->coords2 = NULL;
  element->coords4 = NULL;
  element->numCoords = 0;
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::set2(SoState * const state,
                                 SoNode * const node,
                                 const int32_t numCoords,
                                 const SbVec2f * const coords)
{
  SoTextureCoordinateElement * element = (SoTextureCoordinateElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->coordsAre2D = TRUE;
  element->coords2 = coords;
  element->numCoords = numCoords;
  element->coords4 = NULL;
  element->whatKind = EXPLICIT;
}

//! FIXME: write doc.

void
SoTextureCoordinateElement::set4(SoState * const state,
                                 SoNode * const node,
                                 const int32_t numCoords,
                                 const SbVec4f * const coords)
{
  SoTextureCoordinateElement * element = (SoTextureCoordinateElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->coordsAre2D = FALSE;
  element->numCoords = numCoords;
  element->coords2 = NULL;
  element->coords4 = coords;
  element->whatKind = EXPLICIT;
}

//! FIXME: write doc.

const SoTextureCoordinateElement *
SoTextureCoordinateElement::getInstance(SoState * const state)
{
  return (const SoTextureCoordinateElement *)
    (getConstElement(state, classStackIndex));
}

/*!
  This method returns texture coordinate for the given point and normal.
  The coordinate is returned as a 4D vector, but the r and q coordinates
  will always be 0 and 1 respecively until 3D texture coordinates is
  supported in OpenGL.

  This method should only be used if the CoordType is FUNCTION.
*/

const SbVec4f &
SoTextureCoordinateElement::get(const SbVec3f & point,
                                const SbVec3f & normal) const
{
  assert(this->whatKind == FUNCTION && this->funcCB);
  return (*(this->funcCB))(this->funcCBData, point, normal);
}

//! FIXME: write doc.

const SbVec2f &
SoTextureCoordinateElement::get2(const int index) const
{
  assert(index >= 0 && index < this->numCoords);
  assert(this->whatKind == EXPLICIT);
  if (this->coordsAre2D)
    return this->coords2[index];
  else {
    ((SoTextureCoordinateElement*)
     this)->convert2.setValue(this->coords4[index][0],
                              this->coords4[index][1]);
    return this->convert2;
  }
}

//!  FIXME: write doc.

const SbVec4f &
SoTextureCoordinateElement::get4(const int index) const
{
  assert(index >= 0 && index < this->numCoords);
  assert(this->whatKind == EXPLICIT);
  if (!this->coordsAre2D)
    return this->coords4[index];
  else {
    ((SoTextureCoordinateElement*)
     this)->convert4.setValue(this->coords2[index][0],
                              this->coords2[index][1],
                              0.0f,
                              1.0f);
    return this->convert4;
  }
}

/*!
  This method is used by shapes.  Three return values are possible.

  DEFAULT means that the shapes should generate their own texture coordinates.

  EXPLICIT means that discrete texture coordinates are stored, and should be
  fetched with get2() or get4().

  FUNCTION means that get(point, normal) must be used to generate texture
  coordinates.
*/

SoTextureCoordinateElement::CoordType
SoTextureCoordinateElement::getType(SoState * const state)
{
  const SoTextureCoordinateElement * element =
    (const SoTextureCoordinateElement *)
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
  whatKind = EXPLICIT;
  funcCB = NULL;
  funcCBData = NULL;
  numCoords = 0;
  coords2 = NULL;
  coords4 = NULL;
  coordsAre2D = TRUE;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoTextureCoordinateElement::getNum() const
{
  return this->numCoords;
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoTextureCoordinateElement::is2D() const
{
  return this->coordsAre2D;
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
  Returns a pointer to the 4D texture coordinate array. This method is not
  part of the OIV API.
*/
const SbVec4f *
SoTextureCoordinateElement::getArrayPtr4() const
{
  return this->coords4;
}
