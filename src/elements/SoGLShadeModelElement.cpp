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
  \class SoGLShadeModelElement Inventor/elements/SoGLShadeModelElement.h
  \brief The SoGLShadeModelElement class is used to control the GL shade model.
  \ingroup elements

  It keeps track of the current shade model (flat or smooth) and
  evaluates lazily when the GL state should be updated. Flat-shaded
  triangles are much faster to draw (at least in SW) than smooth
  shaded triangles, and is needed to draw triangle strips with normal
  or material binding per face. This element will normally be set to
  smooth shading, but might be changed be some shapes which need flat
  shading when rendering.
*/

#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/misc/SoState.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLShadeModelElement);

// doc from parent
void
SoGLShadeModelElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLShadeModelElement, inherited);
}

/*!
  The destructor.
*/
SoGLShadeModelElement::~SoGLShadeModelElement()
{
}

// doc from parent
void
SoGLShadeModelElement::init(SoState * state)
{
  inherited::init(state);
  this->flat = FALSE;
  this->glflat = TRUE; // force smooth setting
  this->updategl(this->flat);
}

// doc from parent
void
SoGLShadeModelElement::push(SoState * state)
{
  inherited::push(state);
  SoGLShadeModelElement * prev = (SoGLShadeModelElement*)
    this->getNextInStack();

  this->glflat = prev->glflat;
  this->flat = prev->flat;
}

// doc from parent
void
SoGLShadeModelElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  this->glflat = ((SoGLShadeModelElement*)prevTopElement)->glflat;
  inherited::pop(state, prevTopElement);
}


// doc from parent
SbBool
SoGLShadeModelElement::matches(const SoElement * /* element */) const
{
  assert(0 && "should not get here");
  return FALSE;
}

// doc from parent
SoElement *
SoGLShadeModelElement::copyMatchInfo() const
{
  assert(0 && "should not get here");
  return NULL;
}

/*!
  Sets the current shade model.
*/
void
SoGLShadeModelElement::set(SoState * state, const SbBool flat)
{
  SoGLShadeModelElement * elem = (SoGLShadeModelElement*)
    state->getElement(classStackIndex);
  if (elem) {
    elem->flat = flat;
  }
}

/*!
  Returns current element. Will not cause cache dependencies.
*/
const SoGLShadeModelElement *
SoGLShadeModelElement::getInstance(SoState * state)
{
  return (const SoGLShadeModelElement*)
    state->getConstElement(classStackIndex);
}

// doc in parent
void
SoGLShadeModelElement::lazyEvaluate(void) const
{
  if (this->flat != this->glflat) {
    this->updategl(this->flat);
  }
}

// doc in parent
SbBool
SoGLShadeModelElement::isLazy(void) const
{
  return TRUE;
}

/*!
  Force GL shading model to flat or smooth. This will not change the
  state of the element.
*/
void
SoGLShadeModelElement::forceSend(SoState * const state,
                                 const SbBool flat)
{
  const SoGLShadeModelElement * se = (const SoGLShadeModelElement *)
    SoElement::getConstElement(state, classStackIndex);
  if (se->glflat != flat) se->updategl(flat);
}

// set correct GL state
void
SoGLShadeModelElement::updategl(const SbBool flatshade) const
{
  // cast away constness, since GL state is not part of the element
  // value.
  ((SoGLShadeModelElement*)this)->glflat = flatshade;
  if (flatshade) glShadeModel(GL_FLAT);
  else glShadeModel(GL_SMOOTH);
}
