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
  \class SoGLLinePatternElement Inventor/elements/SoGLLinePatternElement.h
  \brief The SoGLLinePatternElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLinePatternElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
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
