/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoNormalElement Inventor/elements/SoNormalElement.h
  \brief The SoNormalElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoNormalElement.h>
#include <assert.h>

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
  if (elem) {
    elem->normals = normals;
    elem->numNormals = numNormals;
    elem->unitLength = unitLength;
  }
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
SoNormalElement::getNum(void) const
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
SoNormalElement::getArrayPtr(void) const
{
  return this->normals;
}

//$ EXPORT INLINE
SbBool
SoNormalElement::areNormalsUnitLength(void) const
{
  return this->unitLength;
}
