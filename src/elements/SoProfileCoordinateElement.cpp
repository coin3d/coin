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
  \class SoProfileCoordinateElement Innventor/elements/SoProfileCoordinateElement.h
  \brief The SoProfileCoordinateElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoProfileCoordinateElement.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/nodes/SoNode.h>
#include <assert.h>

SbVec2f * SoProfileCoordinateElement::initdefaultcoords = NULL;

/*!
  \var SoProfileCoordinateElement::numCoords

  FIXME: write doc.
*/

/*!
  \var SoProfileCoordinateElement::coords2

  FIXME: write doc.
*/

/*!
  \var SoProfileCoordinateElement::coords3

  FIXME: write doc.
*/

/*!
  \var SoProfileCoordinateElement::coordsAre2D

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoProfileCoordinateElement);

// doc from parent
void
SoProfileCoordinateElement::initClass(void)
{
#if COIN_DEBUG
  // Debugging for memory leaks will be easier if we can clean up the
  // resource usage.
  coin_atexit((coin_atexit_f *)SoProfileCoordinateElement::clean);
#endif // COIN_DEBUG

  SO_ELEMENT_INIT_CLASS(SoProfileCoordinateElement, inherited);

  SoProfileCoordinateElement::initdefaultcoords = new SbVec2f(0.0f, 0.0f);
}

void
SoProfileCoordinateElement::clean(void)
{
#if COIN_DEBUG
  delete SoProfileCoordinateElement::initdefaultcoords;
#endif // COIN_DEBUG
}

/*!
  The destructor.
*/
SoProfileCoordinateElement::~SoProfileCoordinateElement(void)
{
}

// doc from parent
void
SoProfileCoordinateElement::init(SoState * state)
{
  inherited::init(state);
  this->numCoords = 1;
  this->coords2 = SoProfileCoordinateElement::initdefaultcoords;
  this->coords3 = NULL;
  this->coordsAre2D = TRUE;
}


/*!
  Sets the profile coordinates.
*/
void
SoProfileCoordinateElement::set2(SoState * const state,
                                 SoNode * const node,
                                 const int32_t numCoords,
                                 const SbVec2f * const coords)
{
  assert(numCoords >= 0);
  SoProfileCoordinateElement * element =
    (SoProfileCoordinateElement *)
    (getElement(state, classStackIndex, NULL));
  if (element) {
    element->numCoords = numCoords;
    element->coords2 = coords;
    element->coords3 = NULL;
    element->coordsAre2D = TRUE;
    element->nodeId = node->getNodeId();
  }
}

/*!
  Sets the profile coordinates.
*/
void
SoProfileCoordinateElement::set3(SoState * const state,
                                 SoNode * const node,
                                 const int32_t numCoords,
                                 const SbVec3f * const coords)
{
  assert(numCoords >= 0);
  SoProfileCoordinateElement * element =
    (SoProfileCoordinateElement *)
    (getElement(state, classStackIndex, NULL));
  if (element) {
    element->numCoords = numCoords;
    element->coords2 = NULL;
    element->coords3 = coords;
    element->coordsAre2D = FALSE;
    element->nodeId = node->getNodeId();
  }
}

/*!
  Returns the current element.
*/
const SoProfileCoordinateElement *
SoProfileCoordinateElement::getInstance(SoState * const state)
{
  return (const SoProfileCoordinateElement *)
    (SoElement::getConstElement(state, classStackIndex));
}

/*!
  Returns the number of profile coordinates in this element.
*/
int32_t
SoProfileCoordinateElement::getNum(void) const
{
  return this->numCoords;
}

/*!
  Returns the \a index'th 2D coordinate.
  \sa is2D()
*/
const SbVec2f &
SoProfileCoordinateElement::get2(const int index) const
{
  assert(index >= 0 && index < this->numCoords);
  assert(this->coordsAre2D);
  return this->coords2[ index ];
}

/*!
  Returns the \a index'th 3D coordinate.
  \sa is2D()
*/
const SbVec3f &
SoProfileCoordinateElement::get3(const int index) const
{
  assert(index >= 0 && index < this->numCoords);
  assert(! this->coordsAre2D);
  return this->coords3[ index ];
}

/*!
  Returns if this element contains 2D coordinates.
*/
SbBool
SoProfileCoordinateElement::is2D(void) const
{
  return this->coordsAre2D;
}

/*!
  Returns the default 2D coordinate.
*/
SbVec2f
SoProfileCoordinateElement::getDefault2(void)
{
  return SbVec2f(0.0f, 0.0f);
}

/*!
  Returns the default 3D coordinate.
*/
SbVec3f
SoProfileCoordinateElement::getDefault3(void)
{
  return SbVec3f(0.0f, 0.0f, 1.0f);
}

/*!
  Returns a pointer to the 2D coordinates.
*/
const SbVec2f * 
SoProfileCoordinateElement::getArrayPtr2(void) const
{
  return this->coords2;
}

/*!
  Returns a pointer to the 3D coordinates.
*/
const SbVec3f * 
SoProfileCoordinateElement::getArrayPtr3(void) const
{
  return this->coords3;
}
