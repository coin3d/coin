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
  \class SoGLDrawStyleElement Inventor/elements/SoGLDrawStyleElement.h
  \brief The SoGLDrawStyleElement updates the current draw style in OpenGL.
*/

#include <Inventor/elements/SoGLDrawStyleElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

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
  inherited::push(state);
  // copy data to avoid unessesary GL calls
  this->data = ((SoGLDrawStyleElement*)this->getNextInStack())->data;
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLDrawStyleElement * prev = (SoGLDrawStyleElement*) prevTopElement;
  if (this->data != prev->data) this->updategl();
  inherited::pop(state, prevTopElement);
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
SoGLDrawStyleElement::updategl()
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
    assert(0 && "FIXME: handle unsupported switch case");
    break;
  }
}
