/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoGLLineWidthElement Inventor/elements/SoGLLineWidthElement.h
  \brief The SoGLLineWidthElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLineWidthElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

float SoGLLineWidthElement::sizerange[2] = {-1.0f, -1.0f};

SO_ELEMENT_SOURCE(SoGLLineWidthElement);

/*!
  This static method initializes static data for the
  SoGLLineWidthElement class.
*/

void
SoGLLineWidthElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLLineWidthElement, inherited);
}

/*!
  The destructor.
*/

SoGLLineWidthElement::~SoGLLineWidthElement(void)
{
}

//! FIXME: write doc.

void
SoGLLineWidthElement::init(SoState * state)
{
  inherited::init(state);
  this->current = -1.0f;
}

//! FIXME: write doc.

void
SoGLLineWidthElement::push(SoState * state)
{
  inherited::push(state);

  SoGLLineWidthElement * prev = (SoGLLineWidthElement*)this->getNextInStack();
  this->current = prev->current;
}

//! FIXME: write doc.

void
SoGLLineWidthElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLLineWidthElement * prev = (SoGLLineWidthElement*)prevTopElement;
  this->current = prev->current;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLLineWidthElement::setElt(float width)
{
  inherited::setElt(width);
  // this is a lazy element. Do nothing
}


// doc in parent
void
SoGLLineWidthElement::lazyEvaluate(void) const
{
  if (this->data != this->current) {
    // cast away constness
    SoGLLineWidthElement *elem = (SoGLLineWidthElement*)this;
    elem->current = elem->data;
    elem->updategl();
  }
}

// doc in parent
SbBool
SoGLLineWidthElement::isLazy(void) const
{
  return TRUE;
}

//! FIXME: write doc.

void
SoGLLineWidthElement::updategl()
{
  if (SoGLLineWidthElement::sizerange[0] < 0.0f) {
    GLfloat vals[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, vals);

    // Matthias Koenig reported on coin-discuss that the OpenGL
    // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
    // lowest linewidth, but it will still set the return value of
    // glGetError() to GL_INVALID_VALUE if this size is attempted
    // used. This is a workaround for what looks like an OpenGL bug.
    if (vals[0] < 1.0f) vals[0] = SbMin(1.0f, vals[1]);

    SoGLLineWidthElement::sizerange[0] = vals[0];
    SoGLLineWidthElement::sizerange[1] = vals[1];
  }

  float useval = this->current;

  if (useval < SoGLLineWidthElement::sizerange[0])
    useval = SoGLLineWidthElement::sizerange[0];
  if (useval > SoGLLineWidthElement::sizerange[1])
    useval = SoGLLineWidthElement::sizerange[1];

  glLineWidth(useval);
}
