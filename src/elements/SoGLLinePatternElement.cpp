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
  \class SoGLLinePatternElement Inventor/elements/SoGLLinePatternElement.h
  \brief The SoGLLinePatternElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLinePatternElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLLinePatternElement);

/*!
  This static method initializes static data for the
  SoGLLinePatternElement class.
*/

void
SoGLLinePatternElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLLinePatternElement, inherited);
}

/*!
  The destructor.
*/

SoGLLinePatternElement::~SoGLLinePatternElement(void)
{
}

//! FIXME: write doc.

void
SoGLLinePatternElement::init(SoState * state)
{
  inherited::init(state);
  updategl();
}

//! FIXME: write doc.

void
SoGLLinePatternElement::push(SoState * state)
{
  inherited::push(state);
  ((SoGLLinePatternElement*)this->next)->data = this->data;
}

//! FIXME: write doc.

void
SoGLLinePatternElement::pop(SoState * state,
                            const SoElement * prevTopElement)
{
  SoGLLinePatternElement *prev = (SoGLLinePatternElement*) prevTopElement;
  if (prev->data != this->data) prev->updategl();
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLLinePatternElement::setElt(int32_t pattern)
{
  if (pattern != this->data) {
    this->data = pattern;
    updategl();
  }
}

//! FIXME: write doc.

void
SoGLLinePatternElement::updategl()
{
  //
  // FIXME: store flag to keep enable/disable state, pederb 990624
  //
  if (this->data == (int32_t) CONTINUOUS) {
    glDisable(GL_LINE_STIPPLE);
  }
  else {
    glLineStipple(1, (GLushort) this->data);
    glEnable(GL_LINE_STIPPLE);
  }
}
