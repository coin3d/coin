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
  \class SoCullElement SoCullElement.h Inventor/elements/SoCullElement.h
  \brief The SoCullElement class is used internally for render and pick culling.
  \ingroup elements

  The element holds all planes the geometry should be inside, and
  keeps a bitflag to signal which planes need to be tested.

  This element is an extension for Coin, and is not available in the
  original Open Inventor.

  The maximum number of planes in this element is 32, which should be
  more than enough, since the view frustum is represented by 6 planes,
  and the maximum number of OpenGL clipping planes is typically 6 or
  8.

  This element is designed for fast culling, and will not do optimal
  view frustum culling; a box might not be culled even though it is
  outside the view frustum. The assumption is that the view frustum is
  small compared to the world model. The element simply records all
  planes to be culled against, and the graph is not culled until it is
  completely outside one of the planes.
*/

#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbViewVolume.h>
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
  this->vvindex = -1;
}

// doc from parent
void
SoCullElement::push(SoState * state)
{
  SoCullElement * prev = (SoCullElement *)
    this->getNextInStack();

  this->flags = prev->flags;
  this->numplanes = prev->numplanes;
  this->vvindex = prev->vvindex;
  for (int i = 0; i < prev->numplanes; i++) this->plane[i] = prev->plane[i];
}

/*!
  Sets the current view volume. In effect, this adds six planes to
  the list of culling planes.  If a view volume has already been
  set, the old view volume planes are overwritten by the new ones.
  The view volume must be in the world coordinate systems.
*/
void
SoCullElement::setViewVolume(SoState * state, const SbViewVolume & vv)
{
  SoCullElement * elem = (SoCullElement *)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    if (elem->numplanes + 6 > SoCullElement::MAXPLANES) { // _very_ unlikely
#if COIN_DEBUG
      SoDebugError::postWarning("SoCullElement::setViewVolume",  "too many planes");
#endif // COIN_DEBUG
      return;
    }
    int i;
    SbPlane vvplane[6];
    vv.getViewVolumePlanes(vvplane);
    if (elem->vvindex >= 0) { // overwrite old view volume
      for (i = 0; i < 6; i++) {
        elem->plane[elem->vvindex+i] = vvplane[i];
        elem->flags &= ~(1<<(elem->vvindex+i));
      }
    }
    else {
      elem->vvindex = elem->numplanes;
      for (i = 0; i < 6; i++) elem->plane[elem->numplanes++] = vvplane[i];
    }
  }
}

/*!
  Add plane geometry must be inside. The plane must be in the world
  coordinate system.
*/
void
SoCullElement::addPlane(SoState * state, const SbPlane &newplane)
{
  SoCullElement * elem = (SoCullElement *)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    if (elem->numplanes >= SoCullElement::MAXPLANES) {  // _very_ unlikely
#if COIN_DEBUG
      SoDebugError::postWarning("SoCullElement::addPlane",  "too many planes");
#endif // COIN_DEBUG
      return;
    }
    elem->plane[elem->numplanes++] = newplane;
  }
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
  return SoCullElement::docull(state, box, transform, TRUE);
}

/*!
  Cull against \a box. If \a transform is \c TRUE, the box is
  assumed to be in object space, and will be transformed into world
  space using the model matrix.  Returns \c TRUE if box is outside one
  of the planes. This method will not update the element state, just
  perform a cull test against active planes.
*/
SbBool
SoCullElement::cullTest(SoState * state, const SbBox3f & box, const SbBool transform)
{
  return SoCullElement::docull(state, box, transform, FALSE);
}

/*!
  Returns \c TRUE if the current geometry is completely inside all
  planes. There is no need to do a cull test if this is the case.
*/
SbBool
SoCullElement::completelyInside(SoState * state)
{
  // use SoState::getConstElement() to avoid cache dependency on this element
  const SoCullElement * elem = (const SoCullElement *)
    state->getConstElement(classStackIndex);
  unsigned int mask = 0x0001 << elem->numplanes;
  return elem->flags == (mask-1);
}

// Documented in superclass. Overridden to assert that this method is
// not called for this element.
SbBool
SoCullElement::matches(const SoElement *) const
{
  assert(0 && "should not get here");
  return FALSE;
}

// Documented in superclass. Overridden to assert that this method is
// not called for this element.
SoElement *
SoCullElement::copyMatchInfo(void) const
{
  assert(0 && "should not get here");
  return NULL;
}

//
// private method which does the actual culling
//
SbBool
SoCullElement::docull(SoState * state, const SbBox3f & box, const SbBool transform,
                      const SbBool updateelem)
{
  // try to avoid a push if possible
  SoCullElement * elem = (SoCullElement *)
    state->getElementNoPush(classStackIndex);
  
  if (!elem) return FALSE;

  int i, j;
  SbVec3f min, max;
  min = box.getMin();
  max = box.getMax();
  SbVec3f pts[8];

  SbMatrix mm;
  SbBool identity = ! transform;
  if (transform) {
    mm = SoModelMatrixElement::get(state, identity);
  }

  // create the 8 box corner points
  for (i = 0; i < 8; i++) {
    pts[i][0] = i & 1 ? min[0] : max[0];
    pts[i][1] = i & 2 ? min[1] : max[1];
    pts[i][2] = i & 4 ? min[2] : max[2];
    if (!identity) mm.multVecMatrix(pts[i], pts[i]);
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
  if (updateelem && (flags != elem->flags)) {
    // force a push if necessary
    elem = (SoCullElement *)
      SoElement::getElement(state, classStackIndex);
    elem->flags = flags;
  }
  return FALSE;
}
