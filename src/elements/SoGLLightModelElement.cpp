/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoGLLightModelElement Inventor/elements/SoGLLightModelElement.h
  \brief The SoGLLightModelElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLightModelElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

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

// doc in parent
void
SoGLLightModelElement::lazyEvaluate(void) const
{
  if (this->data != this->current) {
    this->updategl((Model) this->data);
  }
}

// doc in parent
SbBool
SoGLLightModelElement::isLazy(void) const
{
  return TRUE;
}

//! FIXME: write doc.

void
SoGLLightModelElement::forceSend(SoState * const state,
                                 const Model model)
{
  const SoGLLightModelElement * le = (const SoGLLightModelElement *)
    SoElement::getConstElement(state, classStackIndex);
  if (model != le->current) {
    le->updategl(model);
  }
}

//! FIXME: write doc.

void
SoGLLightModelElement::updategl(const Model model) const
{
  // cast away constness, since the GL state is not really a part of
  // the element value.
  ((SoGLLightModelElement*)this)->current = model;
  if (model == PHONG) glEnable(GL_LIGHTING);
  else glDisable(GL_LIGHTING);
}

const SoGLLightModelElement *
SoGLLightModelElement::getInstance(SoState *state)
{
  return (const SoGLLightModelElement *)
    SoElement::getConstElement(state, classStackIndex);
}
