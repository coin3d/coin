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
  \class SoGLLinePatternElement Inventor/elements/SoGLLinePatternElement.h
  \brief The SoGLLinePatternElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLinePatternElement.h>
#include "coindefs.h"


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <cassert>

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
}

//! FIXME: write doc.

void
SoGLLinePatternElement::push(SoState * state)
{
  SoGLLinePatternElement * prev = (SoGLLinePatternElement*)
    this->getNextInStack();

  this->data = prev->data;
  // capture element since we might or might not change the GL state
  prev->capture(state);
}

//! FIXME: write doc.

void
SoGLLinePatternElement::pop(SoState * COIN_UNUSED_ARG(state),
                            const SoElement * prevTopElement)
{
  SoGLLinePatternElement * prev = (SoGLLinePatternElement*) prevTopElement;
  if (this->data != prev->data) this->updategl();
}

//! FIXME: write doc.

void
SoGLLinePatternElement::setElt(int32_t pattern)
{
  if (pattern != this->data) {
    this->data = pattern;
    this->updategl();
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
    // Enable line stipple before setting the pattern. This is
    // needed to work around a bug in the nVidia 2.1.1 drivers.
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, (GLushort) this->data);
  }
}
