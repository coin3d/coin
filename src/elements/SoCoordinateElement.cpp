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
  \class SoCoordinateElement Inventor/elements/SoCoordinateElement.h
  \brief The SoCoordinateElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoCoordinateElement.h>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>

#include <assert.h>

// static variables
SbVec3f * SoCoordinateElement::initialdefaultcoords = NULL;

/*!
  \fn SoCoordinateElement::numCoords

  FIXME: write doc.
*/

/*!
  \fn SoCoordinateElement::coords3D

  FIXME: write doc.
*/

/*!
  \fn SoCoordinateElement::coords4D

  FIXME: write doc.
*/

/*!
  \fn SoCoordinateElement::areCoords3D

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoCoordinateElement);

/*!
  This static method initializes static data for the
  SoCoordinateElement class.
*/

void
SoCoordinateElement::initClass(void)
{
#if COIN_DEBUG
  // Debugging for memory leaks will be easier if we can clean up the
  // resource usage.
  (void)atexit(SoCoordinateElement::clean);
#endif // COIN_DEBUG

  SO_ELEMENT_INIT_CLASS(SoCoordinateElement, inherited);

  SoCoordinateElement::initialdefaultcoords = new SbVec3f(0.0f, 0.0f, 0.0f);
}

// Clean up internal resource usage.
void
SoCoordinateElement::clean(void)
{
#if COIN_DEBUG
  delete SoCoordinateElement::initialdefaultcoords;
#endif // COIN_DEBUG
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoCoordinateElement::createInstance(void)
*/

SoCoordinateElement::SoCoordinateElement(void)
  : numCoords(1),
    coords3D(SoCoordinateElement::initialdefaultcoords),
    coords4D(NULL),
    areCoords3D(TRUE)
{
  setTypeId(SoCoordinateElement::classTypeId);
  setStackIndex(SoCoordinateElement::classStackIndex);
}

/*!
  The destructor.
*/

SoCoordinateElement::~SoCoordinateElement(void)
{
}

//! FIXME: write doc.

void
SoCoordinateElement::init(SoState * state)
{
  inherited::init(state);
  this->numCoords = 1;
  this->coords3D = SoCoordinateElement::initialdefaultcoords;
  this->coords4D = NULL;
  this->areCoords3D = TRUE;
}

//! FIXME: write doc.

void
SoCoordinateElement::set3(SoState * const state,
                          SoNode * const node,
                          const int32_t numCoords,
                          const SbVec3f * const coords)
{
  SoCoordinateElement *elem =
    (SoCoordinateElement*) SoElement::getElement(state, classStackIndex);
  elem->areCoords3D = TRUE;
  elem->coords3D = coords;
  elem->numCoords = numCoords;
  elem->nodeId = node->getNodeId();
}

//! FIXME: write doc.

void
SoCoordinateElement::set4(SoState * const state,
                          SoNode * const node,
                          const int32_t numCoords,
                          const SbVec4f * const coords)
{
  SoCoordinateElement *elem =
    (SoCoordinateElement*) SoElement::getElement(state, classStackIndex);
  elem->areCoords3D = FALSE;
  elem->coords4D = coords;
  elem->numCoords = numCoords;

  elem->nodeId = node->getNodeId();
}

//! FIXME: write doc.

const SbVec3f &
SoCoordinateElement::get3(const int index) const
{
  assert(index >= 0 && index < this->numCoords);

  if (areCoords3D) return this->coords3D[index];
  else {
    // hack around const
    SoCoordinateElement *elem = (SoCoordinateElement*)this;
    const SbVec4f &vec = this->coords4D[index];
    elem->dummy3D[0] = vec[0];
    elem->dummy3D[1] = vec[1];
    elem->dummy3D[2] = vec[2];
    return this->dummy3D;
  }
}

//! FIXME: write doc.

const SbVec4f &
SoCoordinateElement::get4(const int index) const
{
  assert(index >= 0 && index < this->numCoords);

  if (!areCoords3D) return this->coords4D[index];
  else {
    // hack around const
    SoCoordinateElement *elem = (SoCoordinateElement*)this;
    const SbVec3f &vec = this->coords3D[index];
    elem->dummy4D[0] = vec[0];
    elem->dummy4D[1] = vec[1];
    elem->dummy4D[2] = vec[2];
    elem->dummy4D[3] = 1.0f;
    return this->dummy4D;
  }
}

//! FIXME: write doc.

const SoCoordinateElement *
SoCoordinateElement::getInstance(SoState * const state)
{
  return (const SoCoordinateElement *)
    (getConstElement(state, classStackIndex));
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoCoordinateElement::getNum(void) const
{
  return this->numCoords;
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoCoordinateElement::is3D() const
{
  return this->areCoords3D;
}

/*!
  Returns a pointer to the 3D coordinate array. This method is not part of the
  OIV API.
*/
const SbVec3f *
SoCoordinateElement::getArrayPtr3() const
{
  return this->coords3D;
}

/*!
  Returns a pointer to the 4D coordinate array. This method is not part of the
  OIV API.
*/
const SbVec4f *
SoCoordinateElement::getArrayPtr4() const
{
  return this->coords4D;
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbVec3f
SoCoordinateElement::getDefault3()
{
  return SbVec3f(0, 0, 0);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbVec4f
SoCoordinateElement::getDefault4()
{
  return SbVec4f(0, 0, 0, 1);
}

//! FIXME: write doc.

void
SoCoordinateElement::print(FILE * /* file */) const
{
}
