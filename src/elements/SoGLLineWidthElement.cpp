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
  \class SoGLLineWidthElement Inventor/elements/SoGLLineWidthElement.h
  \brief The SoGLLineWidthElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLineWidthElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

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

  SoGLLineWidthElement *top = (SoGLLineWidthElement*)this->next;
  top->current = this->current;
}

//! FIXME: write doc.

void
SoGLLineWidthElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLLineWidthElement *prev = (SoGLLineWidthElement*)prevTopElement;
  prev->current = this->current;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLLineWidthElement::setElt(float width)
{
  inherited::setElt(width);
  // this is a lazy element. Do nothing
}

//! FIXME: write doc.

void
SoGLLineWidthElement::evaluate() const
{
  if (this->data != this->current) {
    // cast away constness
    SoGLLineWidthElement *elem = (SoGLLineWidthElement*)this;
    elem->current = elem->data;
    elem->updategl();
  }
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
