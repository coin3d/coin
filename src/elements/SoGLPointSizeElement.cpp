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
  \class SoGLPointSizeElement Inventor/elements/SoGLPointSizeElement.h
  \brief The SoGLPointSizeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLPointSizeElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

float SoGLPointSizeElement::sizerange[2] = {-1.0f, -1.0f};

SO_ELEMENT_SOURCE(SoGLPointSizeElement);

/*!
  This static method initializes static data for the SoGLPointSizeElement
  class.
*/

void
SoGLPointSizeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLPointSizeElement, inherited);
}

/*!
  The destructor.
*/

SoGLPointSizeElement::~SoGLPointSizeElement(void)
{
}

//! FIXME: write doc.

void
SoGLPointSizeElement::init(SoState * state)
{
  inherited::init(state);
  this->current = -1.0f;
}

//! FIXME: write doc.

void
SoGLPointSizeElement::push(SoState * state)
{
  inherited::push(state);
  SoGLPointSizeElement *top = (SoGLPointSizeElement*)this->next;
  top->current = this->current;
}

//! FIXME: write doc.

void
SoGLPointSizeElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLPointSizeElement *prev = (SoGLPointSizeElement*)prevTopElement;
  prev->current = this->current;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLPointSizeElement::setElt(float val)
{
  inherited::setElt(val);
  // this is a lazy element, do nothing
}

//! FIXME: write doc.

void
SoGLPointSizeElement::evaluate() const
{
  if (this->data != current) {
    // cast away constness
    SoGLPointSizeElement *elem = (SoGLPointSizeElement*)this;
    elem->current = this->data;
    elem->updategl();
  }
}

//! FIXME: write doc.

void
SoGLPointSizeElement::updategl()
{
  if (SoGLPointSizeElement::sizerange[1] < 0.0f) {
    GLfloat vals[2];
    glGetFloatv(GL_POINT_SIZE_RANGE, vals);

    // Matthias Koenig reported on coin-discuss that the OpenGL
    // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
    // lowest pointsize, but it will still set the return value of
    // glGetError() to GL_INVALID_VALUE if this size is attempted
    // used. This is a workaround for what looks like an OpenGL bug.
    if (vals[0] < 1.0f) vals[0] = SbMin(1.0f, vals[1]);

    SoGLPointSizeElement::sizerange[0] = vals[0];
    SoGLPointSizeElement::sizerange[1] = vals[1];
  }

  float useval = this->current;

  if (useval < SoGLPointSizeElement::sizerange[0])
    useval = SoGLPointSizeElement::sizerange[0];
  if (useval > SoGLPointSizeElement::sizerange[1])
    useval = SoGLPointSizeElement::sizerange[1];

  glPointSize(useval);
}
