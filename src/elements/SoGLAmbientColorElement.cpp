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
  \class SoGLAmbientColorElement Inventor/elements/SoGLAmbientColorElement.h
  \brief The SoGLAmbientColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLAmbientColorElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLAmbientColorElement);

/*!
  This static method initializes static data for the
  SoGLAmbientColorElement class.
*/

void
SoGLAmbientColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLAmbientColorElement, inherited);
}

/*!
  The destructor.
*/

SoGLAmbientColorElement::~SoGLAmbientColorElement()
{
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::init(SoState * state)
{
  inherited::init(state);
  this->current.setValue(0.2f, 0.2f, 0.2f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::push(SoState * state)
{
  inherited::push(state);
  SoGLAmbientColorElement * const prev =
    (SoGLAmbientColorElement *)this->getNextInStack();
  this->current = prev->current;
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  this->current = ((SoGLAmbientColorElement*)prevTopElement)->current;
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::send(const int index)
{
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (colors && colors[realindex] != this->current) {
    this->current = colors[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::setElt(const int32_t numColors,
                                const SbColor * const colors)
{
  inherited::setElt(numColors, colors);
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::updategl()
{
  float col[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  this->current.getValue(col[0], col[1], col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, col);
}
