/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoMultiTextureMatrixElement Inventor/elements/SoMultiTextureMatrixElement.h
  \brief The SoMultiTextureMatrixElement class is used to manage the texture matrix stack for texture units > 0.
  \ingroup elements

  The texture matrix is used to transform texture coordinates before
  being used to map textures onto polygons.

  \since Coin 2.2
*/

#include <Inventor/elements/SoMultiTextureMatrixElement.h>

#define MAX_UNITS 16
#define PRIVATE(obj) obj->pimpl

class SoMultiTextureMatrixElementP {
public:
  SoMultiTextureMatrixElement::UnitData unitdata[MAX_UNITS];
};

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoMultiTextureMatrixElement);

// doc from parent
void
SoMultiTextureMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoMultiTextureMatrixElement, inherited);
}

/*!
  The constructor.
 */
SoMultiTextureMatrixElement::SoMultiTextureMatrixElement(void)
{
  PRIVATE(this) = new SoMultiTextureMatrixElementP;

  this->setTypeId(SoMultiTextureMatrixElement::classTypeId);
  this->setStackIndex(SoMultiTextureMatrixElement::classStackIndex);
}

/*!
  The destructor.
*/
SoMultiTextureMatrixElement::~SoMultiTextureMatrixElement(void)
{
  delete PRIVATE(this);
}


/*!
  Multiplies \a matrix into the current texture matrix.
*/
void
SoMultiTextureMatrixElement::mult(SoState * const state,
                                  SoNode * const node,
                                  const int unit,
                                  const SbMatrix & matrix)
{
  SoMultiTextureMatrixElement *elem = (SoMultiTextureMatrixElement*)
    SoElement::getElement(state, classStackIndex);
  elem->multElt(unit, matrix);
  if (node) elem->addNodeId(node);
}

/*!
  Returns current texture matrix.
*/
const SbMatrix &
SoMultiTextureMatrixElement::get(SoState * const state, const int unit)
{
  SoMultiTextureMatrixElement *elem = (SoMultiTextureMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->getElt(unit);
}

SoMultiTextureMatrixElement::UnitData & 
SoMultiTextureMatrixElement::getUnitData(const int unit)
{
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(this)->unitdata[unit];
}

const SoMultiTextureMatrixElement::UnitData & 
SoMultiTextureMatrixElement::getUnitData(const int unit) const
{
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(this)->unitdata[unit];
}


/*!
  virtual method which is called from mult(). Multiplies \a matrix
  into element matrix.
*/
void
SoMultiTextureMatrixElement::multElt(const int unit, const SbMatrix & matrix)
{
  assert(unit >= 0 && unit < MAX_UNITS);
  PRIVATE(this)->unitdata[unit].textureMatrix.multLeft(matrix);
}

/*!
  Returns element matrix. Called from get().
*/
const SbMatrix &
SoMultiTextureMatrixElement::getElt(const int unit) const
{
  return PRIVATE(this)->unitdata[unit].textureMatrix;
}

// doc from parent
void
SoMultiTextureMatrixElement::init(SoState * state)
{
  inherited::init(state);
  for (int i = 0; i < MAX_UNITS; i++) {
    PRIVATE(this)->unitdata[i].textureMatrix.makeIdentity();
  }
  this->clearNodeIds();
}

// Documented in superclass. Overridden to copy current matrix and
// update accumulated node ids.
void
SoMultiTextureMatrixElement::push(SoState * state)
{
  inherited::push(state);

  SoMultiTextureMatrixElement * prev =
    (SoMultiTextureMatrixElement *) this->getNextInStack();
  for (int i = 0; i < MAX_UNITS; i++) {
    PRIVATE(this)->unitdata[i] = PRIVATE(prev)->unitdata[i];;
  }

  // make sure node ids are accumulated properly
  this->copyNodeIds(prev);
}

#undef PRIVATE
#undef MAX_UNITS
