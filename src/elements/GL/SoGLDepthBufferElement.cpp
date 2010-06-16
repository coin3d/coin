/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGLDepthBufferElement Inventor/elements/SoGLDepthBufferElement.h
  \brief The SoGLDepthBufferElement controls the OpenGL depth buffer.

  \ingroup elements
  \COIN_CLASS_EXTENSION
  \since Coin 3.0
*/
#include <Inventor/elements/SoGLDepthBufferElement.h>
#include "coindefs.h"

#include <cassert>

#include <Inventor/system/gl.h>

SO_ELEMENT_SOURCE(SoGLDepthBufferElement);

/*!
  Initializes the SoGLDepthBufferElement class type.
*/

void
SoGLDepthBufferElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLDepthBufferElement, inherited);
}

/*!
  The destructor.
*/
SoGLDepthBufferElement::~SoGLDepthBufferElement(void)
{
}

/*!
  Internal Coin method.
*/
void
SoGLDepthBufferElement::init(SoState * state)
{
  inherited::init(state);
}

/*!
  Internal Coin method.
*/
void
SoGLDepthBufferElement::push(SoState * state)
{
  SoGLDepthBufferElement * prev =
    static_cast<SoGLDepthBufferElement *>(this->getNextInStack());
  this->test = prev->test;
  this->write = prev->write;
  this->function = prev->function;
  this->range = prev->range;
  prev->capture(state);
}

/*!
  Internal Coin method.
*/
void
SoGLDepthBufferElement::pop(SoState * COIN_UNUSED_ARG(state),
                            const SoElement * prevTopElement)
{
  const SoGLDepthBufferElement * prev =
    static_cast<const SoGLDepthBufferElement *>(prevTopElement);
  if (this->test != prev->test || this->write != prev->write ||
      this->function != prev->function || this->range != prev->range) {
    this->updategl();
  }
}

/*!
  Set this element's values.
*/
void
SoGLDepthBufferElement::setElt(SbBool test, SbBool write, DepthWriteFunction function, SbVec2f range)
{
  SbBool update =
    (test != this->test) ||
    (write != this->write) ||
    (function != this->function) ||
    (range != this->range);

  inherited::setElt(test, write, function, range);

  if (update) {
    this->updategl();
  }
}

/*!
  This method performs the OpenGL updates.
*/
void
SoGLDepthBufferElement::updategl(void) const
{
  if (this->test) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }

  if (this->write) {
    glDepthMask(GL_TRUE);
  } else {
    glDepthMask(GL_FALSE);
  }

  switch (this->function) {
  case NEVER:     glDepthFunc(GL_NEVER);     break;
  case ALWAYS:    glDepthFunc(GL_ALWAYS);    break;
  case LESS:      glDepthFunc(GL_LESS);      break;
  case LEQUAL:    glDepthFunc(GL_LEQUAL);    break;
  case EQUAL:     glDepthFunc(GL_EQUAL);     break;
  case GEQUAL:    glDepthFunc(GL_GEQUAL);    break;
  case GREATER:   glDepthFunc(GL_GREATER);   break;
  case NOTEQUAL:  glDepthFunc(GL_NOTEQUAL);  break;
  default: assert(!"unknown depth function");
  }

  glDepthRange(this->range[0], this->range[1]);
}
