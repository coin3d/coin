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
  \class SoGLShapeHintsElement Inventor/elements/SoGLShapeHintsElement.h
  \brief The SoGLShapeHintsElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

//
// handling of cases (from the original oiv man-pages):
//
// 1) ordered, solid   :   backface culling, ! twoside lighting
// 2) ordered, ! solid : ! backface culling,   twoside lighting
// 3) ! ordered        : ! backface culling, ! twoside lighting
//
// (this table of rendering rules is duplicated in the SoShapeHints
// classdoc)

//
// I find this strategy quite odd. My gut-feeling says that
// when vertexordering is unknown, two-side-lighting should be
// enabled, since it will be difficult to know if a normal
// points in or out of a polygon...
//

//
// nodes with automatically generated normals should probably
// force two-side lighting when vertexordering is unknown, since
// it is impossible to know if normals are pointing in or out.
//
// use SoGLShapeHintsElement::forceSend(twoside) for this purpose
//

#include <Inventor/elements/SoGLShapeHintsElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

// for the "glflags" variable
#define SOSH_CCW      0x01
#define SOSH_TWOSIDE  0x02
#define SOSH_CULL     0x04

SO_ELEMENT_SOURCE(SoGLShapeHintsElement);

/*!
  This static method initializes static data for the
  SoGLShapeHintsElement class.
*/

void
SoGLShapeHintsElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLShapeHintsElement, inherited);
}

/*!
  The destructor.
*/

SoGLShapeHintsElement::~SoGLShapeHintsElement(void)
{
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::init(SoState * state)
{
  inherited::init(state);
  glCullFace(GL_BACK);

  // fake this to turn off culling and twoside
  this->glflags = SOSH_CULL | SOSH_TWOSIDE;
  this->updategl(SOSH_CCW); // set CCW, no culling, no twoside
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::push(SoState * state)
{
  inherited::push(state);
  SoGLShapeHintsElement * prev =
    (SoGLShapeHintsElement *) this->getNextInStack();

  this->glflags = prev->glflags;
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);

  SoGLShapeHintsElement * prev = (SoGLShapeHintsElement*) prevTopElement;
  this->glflags = prev->glflags;
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::setElt(VertexOrdering vertexOrdering,
                              ShapeType shapeType,
                              FaceType faceType)
{
  inherited::setElt(vertexOrdering, shapeType, faceType);
  // do nothing since this is a lazy element
}

// doc in parent
void
SoGLShapeHintsElement::lazyEvaluate(void) const
{
  unsigned int flags = this->glflags & SOSH_CCW;
  if (vertexOrdering == CLOCKWISE) flags = 0;
  else if (vertexOrdering == COUNTERCLOCKWISE) flags = SOSH_CCW;

  if (vertexOrdering != UNKNOWN_ORDERING) {
    if (shapeType == SOLID) flags |= SOSH_CULL;
    else flags |= SOSH_TWOSIDE;
  }

  if (flags ^ this->glflags) { // xor to see if some bit has changed
    ((SoGLShapeHintsElement*)this)->updategl(flags);
  }
}

// doc in parent
SbBool
SoGLShapeHintsElement::isLazy(void) const
{
  return TRUE;
}


/*!
  Update gl state. Use this is you only want to modify the
  twoside lighting feature.
*/

void
SoGLShapeHintsElement::forceSend(const SbBool twoside) const
{
  SoGLShapeHintsElement *elem = (SoGLShapeHintsElement*)this;
  unsigned int flags = glflags & ~SOSH_TWOSIDE;
  if (twoside) flags |= SOSH_TWOSIDE;
  elem->updategl(flags);
}

/*!
  Update GL state. Use this if you don't care about the two-side
  lighting state.
*/

void
SoGLShapeHintsElement::forceSend(const SbBool ccw, const SbBool cull) const
{
  SoGLShapeHintsElement *elem = (SoGLShapeHintsElement*)this;
  unsigned int flags = glflags & SOSH_TWOSIDE;
  if (ccw) flags |= SOSH_CCW;
  if (cull) flags |= SOSH_CULL;
  elem->updategl(flags);
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::forceSend(const SbBool ccw, const SbBool cull,
                                 const SbBool twoside) const
{
  SoGLShapeHintsElement *elem = (SoGLShapeHintsElement*)this;
  unsigned int flags = 0;
  if (ccw) flags |= SOSH_CCW;
  if (cull) flags |= SOSH_CULL;
  if (twoside) flags |= SOSH_TWOSIDE;
  elem->updategl(flags);
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::updategl(const unsigned int flags)
{
  unsigned int changed = this->glflags ^ flags;
  this->glflags = flags;

  if (changed & SOSH_CCW) {
    flags & SOSH_CCW ? glFrontFace(GL_CCW) : glFrontFace(GL_CW);
  }
  if (changed & SOSH_TWOSIDE) {
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,
                  flags & SOSH_TWOSIDE ? GL_TRUE : GL_FALSE);
  }
  if (changed & SOSH_CULL) {
    flags & SOSH_CULL ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  }
}
