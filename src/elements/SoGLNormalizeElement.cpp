/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoGLNormalizeElement Inventor/elements/SoGLNormalizeElement.h
  \brief The SoGLNormalizeElement class is for GL optimization only.

  It keeps track of the GL_NORMALIZE flag and enables/disables this
  GL feature depending on the GL matrix and supplied normals.
  It is very important that all transformation nodes calls
  SoGLNormalizeElement::setMatrixState(state, FALSE) if it contains
  a scale operation. Also, geometry nodes should let this
  element know if it is going to use unit-length normals
  (by overloading SoShape::willSendUnitLengthNormals()).
  When the GL matrix does not contain any scale operations, and the normals
  are unit length, GL_NORMALIZE can be disabled.

  This is a lazy element which is evaluated by SoShape.
*/

#include <Inventor/elements/SoGLNormalizeElement.h>

#include <coindefs.h> // COIN_STUB()

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>


SO_ELEMENT_SOURCE(SoGLNormalizeElement);

/*!
  This static method initializes static data for the
  SoGLNormalizeElement class.
*/

void
SoGLNormalizeElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLNormalizeElement, inherited);
}

/*!
  The destructor.
*/

SoGLNormalizeElement::~SoGLNormalizeElement()
{
}

//! FIXME: write doc.

void
SoGLNormalizeElement::init(SoState * state)
{
  inherited::init(state);
  this->unitNormals = FALSE; /* we know nothing about the normals yet */
  this->okMatrix = TRUE;     /* first matrix is identity */

  /* force a glEnable(GL_NORMALIZE) */
  this->glnormalize = FALSE;
  this->updategl(TRUE);
}

//! FIXME: write doc.

void
SoGLNormalizeElement::push(SoState * state)
{
  inherited::push(state);
  SoGLNormalizeElement *elem = (SoGLNormalizeElement*)this->next;
  elem->glnormalize = this->glnormalize;
  elem->unitNormals = this->unitNormals;
  elem->okMatrix = this->okMatrix;
}

//! FIXME: write doc.

void
SoGLNormalizeElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  ((SoGLNormalizeElement*)prevTopElement)->glnormalize = this->glnormalize;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoGLNormalizeElement::matches(const SoElement * /* element */) const
{
  COIN_STUB();
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoGLNormalizeElement::copyMatchInfo() const
{
  COIN_STUB();
  return NULL;
}

/*!
  Should be called only when the node's matrix contains a
  scale transformation (or some other transformation which will create
  non-unit length normals). It should not be called to tell the element
  that the node has a transformation which is ok. The exception
  is when a node causes a new matrix to be loaded, and it
  is certain that this new matrix contains no scaling.
*/
void
SoGLNormalizeElement::setMatrixState(SoState * const state,
                                     const SbBool valid)
{
  SoGLNormalizeElement * e = (SoGLNormalizeElement *)
    inherited::getElement(state, SoGLNormalizeElement::classStackIndex);
  e->okMatrix = valid;

#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::setMatrixState",
                         "%d", valid);
#endif // debug

}

/*!
  Normal and geometry nodes should call this to let the element know if
  the normals are unit length or not.
*/
void
SoGLNormalizeElement::setUnitNormals(SoState * const state,
                                     const SbBool unitNormals)
{
  SoGLNormalizeElement * e = (SoGLNormalizeElement *)
    inherited::getElement(state, SoGLNormalizeElement::classStackIndex);
  e->unitNormals = unitNormals;

#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::setUnitNormals",
                         "%d", unitNormals);
#endif // debug

}


/*
  Lazy update
*/
void
SoGLNormalizeElement::evaluate() const
{
  SbBool normalize = !(this->unitNormals && this->okMatrix);

#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::evaluate",
                         "%d %d %d", unitNormals, okMatrix, normalize);
#endif // debug

  ((SoGLNormalizeElement*)this)->updategl(normalize);
}

/*!
  Turns on/off normalizing without affecting the state. This
  method should be used by shapes supplying their own
  normals. If it is known that the normals are unit length,
  this method should be used to optimize rendering.
  Normalizing is not disabled unless matrix is valid though.
*/
void
SoGLNormalizeElement::forceSend(const SbBool unit) const
{
#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::forceSend",
                         "%d", unit);
#endif // debug
  ((SoGLNormalizeElement*)this)->updategl(!(unit && this->okMatrix));
}

//! FIXME: write doc.

void
SoGLNormalizeElement::updategl(const SbBool normalize)
{
#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::updategl",
                         "norm: %d %d", normalize, glnormalize);
#endif // debug

#if 0 // to disable this optimizing feature
  if (!glnormalize) {
    glnormalize = TRUE;
    glEnable(GL_NORMALIZE);
  }
#else // optimized
  if (normalize != this->glnormalize) {
    this->glnormalize = normalize;
    if (normalize) glEnable(GL_NORMALIZE);
    else glDisable(GL_NORMALIZE); /* yeah, 10% speed increase :-) */
  }
#endif // optimized
}
