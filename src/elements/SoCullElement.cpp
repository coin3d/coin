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
  \class SoCullElement SoCullElement.h Inventor/elements/SoCullElement.h
  \brief The SoCullElement class is used internally for render and pick culling.

  The element holds all planes the geometry should be inside, and
  keeps a bitflag to signal which planes need to be tested.

  This element is an extension for Coin, and is not available in the
  original Open Inventor.

  The maximum number of planes in this element is 32, which should be
  more than enough, since the view frustum is represented by 6 planes,
  and the maximum number of OpenGL clipping planes is typically 6 or
  8.
*/

#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/SbBox3f.h>
#include <string.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_ELEMENT_SOURCE(SoCullElement);

// doc from parent
void
SoCullElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoCullElement, inherited);
}

/*!
  The destructor.
*/
SoCullElement::~SoCullElement()
{
}

// doc from parent
void
SoCullElement::init(SoState * state)
{
  this->numplanes = 0;
  this->flags = 0;
}

// doc from parent
void
SoCullElement::push(SoState * state)
{
  SoCullElement * elem = (SoCullElement *)
    this->next;

  elem->flags = this->flags;
  elem->numplanes = this->numplanes;
#if 0 // OBSOLETED: insecure wrt to the implementation of the
      // default SbPlane constructor. 20000517 mortene.
  if (this->numplanes > 0) {
    memcpy(elem->plane, this->plane, sizeof(SbPlane) * this->numplanes);
  }
#else
  for (int i=0; i < this->numplanes; i++) elem->plane[i] = this->plane[i];
#endif
}

/*!
  Add planes geometry must be inside. The planes must be in the world
  coordinate system.
*/
void
SoCullElement::addPlanes(SoState * state, const SbPlane * planes, int numplanes)
{
  SoCullElement * elem = (SoCullElement *)
    SoElement::getElement(state, classStackIndex);
  if (elem->numplanes + numplanes > 32) {  // _very_ unlikely
#if COIN_DEBUG
    SoDebugError::postWarning("SoCullElement::addPlanes",  "too many planes");
#endif // COIN_DEBUG
    return;
  }

#if 0 // OBSOLETED: insecure wrt to the implementation of the
      // default SbPlane constructor. 20000517 mortene.
  memcpy(&elem->plane[elem->numplanes], planes, sizeof(SbPlane) * numplanes);
#else
  for (int i=elem->numplanes; i < numplanes; i++) elem->plane[i] = planes[i];
#endif
  elem->numplanes += numplanes;
}

/*!
  Cull against \a box. If \a transform is \c TRUE, the box is assumed
  to be in object space, and will be transformed into world space
  using the model matrix.  Returns \c TRUE if box is outside one of
  the planes, and updates the element to detect when geometry is
  completely inside all planes.
*/
SbBool
SoCullElement::cullBox(SoState * state, const SbBox3f & box, const SbBool transform)
{
  // try to avoid a push if possible, get const element first
  SoCullElement * elem = (SoCullElement *)
    SoElement::getConstElement(state, classStackIndex);

  int i, j;
  SbVec3f min, max;
  min = box.getMin();
  max = box.getMax();
  SbVec3f pts[8];

  const SbMatrix & mm = SoModelMatrixElement::get(state);

  // create the 8 box corner points
  for (i = 0; i < 8; i++) {
    pts[i][0] = i & 1 ? min[0] : max[0];
    pts[i][1] = i & 2 ? min[1] : max[1];
    pts[i][2] = i & 4 ? min[2] : max[2];
    if (transform) mm.multVecMatrix(pts[i], pts[i]);
  }

  const int n = elem->numplanes;
  unsigned int flags = elem->flags;
  SbPlane * planes = elem->plane;
  unsigned int mask = 0x0001;

  for (i = 0; i < n; i++, mask<<=1) {
    if (!(flags & mask)) {
      int in = 0;
      int out = 0;
      for (j = 0; j < 8; j++) {
        if (planes[i].isInHalfSpace(pts[j])) in++;
        else out++;
      }
      if (in == 8) {
        flags |= mask;
      }
      else if (out == 8) {
        return TRUE;
      }
    }
  }
  if (flags != elem->flags) {
    // force a push if necessary
    elem = (SoCullElement *)
      SoElement::getElement(state, classStackIndex);
    elem->flags = flags;
  }
  return FALSE;
}

/*!
  Returns \c TRUE if the current geometry is completely inside all
  planes. There is no need to do a cull test if this is the case.
*/
SbBool
SoCullElement::completelyInside(SoState * state)
{
  const SoCullElement * elem = (const SoCullElement *)
    SoElement::getConstElement(state, classStackIndex);
  unsigned int mask = 0x0001 << elem->numplanes;
  return elem->flags == (mask-1);
}

/*!
  Overloaded to assert that this method is not called for this
  element.
*/
SbBool
SoCullElement::matches(const SoElement *) const
{
  assert(0 && "should not get here");
  return FALSE;
}

/*!
  Overloaded to assert that this method is not called for this
  element.
*/
SoElement *
SoCullElement::copyMatchInfo(void) const
{
  assert(0 && "should not get here");
  return NULL;
}
