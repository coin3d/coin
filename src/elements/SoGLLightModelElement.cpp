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
  \class SoGLLightModelElement Inventor/elements/SoGLLightModelElement.h
  \brief The SoGLLightModelElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLightModelElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLLightModelElement);

/*!
  This static method initializes static data for the
  SoGLLightModelElement class.
*/

void
SoGLLightModelElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLLightModelElement, inherited);
}

/*!
  The destructor.
*/

SoGLLightModelElement::~SoGLLightModelElement()
{
}

//! FIXME: write doc.

void
SoGLLightModelElement::init(SoState * state)
{
  inherited::init(state);
  glEnable(GL_LIGHTING);
  this->current = PHONG;
}

//! FIXME: write doc.

void
SoGLLightModelElement::push(SoState * state)
{
  inherited::push(state);
  SoGLLightModelElement * prev = (SoGLLightModelElement*)this->getNextInStack();
  this->current = prev->current;
}

//! FIXME: write doc.

void
SoGLLightModelElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLLightModelElement * prev = (SoGLLightModelElement*)prevTopElement;
  this->current = prev->current;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLLightModelElement::setElt(int32_t val)
{
  // this is a lazy element
  this->data = val;
}

//! FIXME: write doc.

void
SoGLLightModelElement::evaluate() const
{
  if (this->data != this->current) {
    ((SoGLLightModelElement*)this)->current = (Model)this->data;
    ((SoGLLightModelElement*)this)->updategl();
  }
}

//! FIXME: write doc.

void
SoGLLightModelElement::forceSend(const Model model) const
{
  if (model != this->current) {
    ((SoGLLightModelElement*)this)->current = model;
    ((SoGLLightModelElement*)this)->updategl();
  }
}

//! FIXME: write doc.

void
SoGLLightModelElement::updategl()
{
  if (current == PHONG) glEnable(GL_LIGHTING);
  else glDisable(GL_LIGHTING);
}

const SoGLLightModelElement *
SoGLLightModelElement::getInstance(SoState *state)
{
  return (const SoGLLightModelElement *)
    SoElement::getConstElement(state, classStackIndex);
}
