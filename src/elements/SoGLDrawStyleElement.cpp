/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoGLDrawStyleElement Inventor/elements/SoGLDrawStyleElement.h
  \brief The SoGLDrawStyleElement updates the current draw style in OpenGL.
  \ingroup elements
*/

#include <Inventor/elements/SoGLDrawStyleElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLDrawStyleElement);

/*!
  This static method initializes static data for the
  SoGLDrawStyleElement class.
*/

void
SoGLDrawStyleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLDrawStyleElement, inherited);
}

/*!
  The destructor.
*/

SoGLDrawStyleElement::~SoGLDrawStyleElement(void)
{
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::push(SoState * state)
{
  SoGLDrawStyleElement * prev = (SoGLDrawStyleElement*)
    this->getNextInStack();
  // copy data to avoid unessesary GL calls
  this->data = prev->data;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLDrawStyleElement * prev = (SoGLDrawStyleElement*) prevTopElement;
  if (this->data != prev->data) this->updategl();
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::setElt(int32_t style)
{
  if (style != this->data) {
    this->data = (int32_t)style;
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::updategl(void)
{
  switch ((Style)this->data) {
  case SoDrawStyleElement::FILLED:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  case SoDrawStyleElement::LINES:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  case SoDrawStyleElement::POINTS:
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    break;
  case SoDrawStyleElement::INVISIBLE:
    // handled in SoShape::shouldGLRender()
    break;
  default:
    assert(0 && "unsupported switch case");
    break;
  }
}
