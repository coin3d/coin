/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoGLLightModelElement Inventor/elements/SoGLLightModelElement.h
  \brief The SoGLLightModelElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLightModelElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

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
  A constructor.  Can't be used directly.

  \sa void * SoGLLightModelElement::createInstance(void)
*/

SoGLLightModelElement::SoGLLightModelElement()
{
  setTypeId(SoGLLightModelElement::classTypeId);
  setStackIndex(SoGLLightModelElement::classStackIndex);
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
  SoGLLightModelElement *top = (SoGLLightModelElement*)this->next;
  top->current = this->current;
}

//! FIXME: write doc.

void
SoGLLightModelElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLLightModelElement *prev = (SoGLLightModelElement*)prevTopElement;
  prev->current = this->current;
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
