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
  \class SoGLShininessElement Inventor/elements/SoGLShininessElement.h
  \brief The SoGLShininessElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLShininessElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLShininessElement);

/*!
  This static method initializes static data for the
  SoGLShininessElement class.
*/

void
SoGLShininessElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLShininessElement, inherited);
}

/*!
  The destructor.
*/

SoGLShininessElement::~SoGLShininessElement()
{
}

//! FIXME: write doc.

void
SoGLShininessElement::init(SoState *state)
{
  inherited::init(state);
  this->current = 0.2f;
  this->updategl();
}

//! FIXME: write doc.

void
SoGLShininessElement::push(SoState * state)
{
  inherited::push(state);
  SoGLShininessElement * const prev =
    (SoGLShininessElement *)this->getNextInStack();
  this->current = prev->current;
}

//! FIXME: write doc.

void
SoGLShininessElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  this->current = ((SoGLShininessElement*)prevTopElement)->current;
}

//! FIXME: write doc.

void
SoGLShininessElement::send(const int index)
{
  int realindex = index < this->numValues ? index : this->numValues - 1;
  if (this->values && this->values[realindex] != this->current) {
    this->current = this->values[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLShininessElement::setElt(const int32_t numValues,
                             const float * const values)
{
  inherited::setElt(numValues, values);
}

//! FIXME: write doc.

void
SoGLShininessElement::updategl()
{
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,
              this->current*128);
}
