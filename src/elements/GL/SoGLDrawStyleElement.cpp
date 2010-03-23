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
  \class SoGLDrawStyleElement Inventor/elements/SoGLDrawStyleElement.h
  \brief The SoGLDrawStyleElement updates the current draw style in OpenGL.
  \ingroup elements
*/

#include <Inventor/elements/SoGLDrawStyleElement.h>
#include "coindefs.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <cassert>

SO_ELEMENT_SOURCE(SoGLDrawStyleElement);

// doc in superclass
void
SoGLDrawStyleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLDrawStyleElement, inherited);
}

SoGLDrawStyleElement::~SoGLDrawStyleElement(void)
{
}

// doc in superclass
void
SoGLDrawStyleElement::init(SoState * state)
{
  inherited::init(state);
}

// doc in superclass
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

// doc in superclass
void
SoGLDrawStyleElement::pop(SoState * COIN_UNUSED_ARG(state),
                          const SoElement * prevTopElement)
{
  SoGLDrawStyleElement * prev = (SoGLDrawStyleElement*) prevTopElement;
  if (this->data != prev->data) this->updategl();
}

// doc in superclass
void
SoGLDrawStyleElement::setElt(int32_t style)
{
  if (style != this->data) {
    this->data = (int32_t)style;
    this->updategl();
  }
}

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
