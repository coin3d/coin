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
  \class SoGLEmissiveColorElement Inventor/elements/SoGLEmissiveColorElement.h
  \brief The SoGLEmissiveColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLEmissiveColorElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLEmissiveColorElement);

/*!
  This static method initializes static data for the
  SoGLEmissiveColorElement class.
*/

void
SoGLEmissiveColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLEmissiveColorElement, inherited);
}

/*!
  The destructor.
*/

SoGLEmissiveColorElement::~SoGLEmissiveColorElement()
{
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::init(SoState * state)
{
  inherited::init(state);
  this->current.setValue(0.0f, 0.0f, 0.0f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::push(SoState * state)
{
  inherited::push(state);
  SoGLEmissiveColorElement * const prev =
    (SoGLEmissiveColorElement *)this->getNextInStack();
  this->current = prev->current;
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::pop(SoState *state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  this->current = ((SoGLEmissiveColorElement*)prevTopElement)->current;
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::send(const int index)
{
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (colors && colors[realindex] != this->current) {
    this->current = colors[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::setElt(const int32_t numColors,
                                 const SbColor * const colors)
{
  inherited::setElt(numColors, colors);
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::updategl()
{
  float col[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  this->current.getValue(col[0], col[1], col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, col);
}
