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
  \class SoGLShadeModelElement Inventor/elements/SoGLShadeModelElement.h
  \brief The SoGLShadeModelElement class is used to control the GL shade model.

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

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
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
  ((SoGLShadeModelElement*)this->next)->glflat = this->glflat;
  ((SoGLShadeModelElement*)this->next)->flat = this->flat;
}

// doc from parent
void
SoGLShadeModelElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  ((SoGLShadeModelElement*)prevTopElement)->glflat = this->glflat;
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
  elem->flat = flat;
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

/*!
  Updates the GL state.
*/
void
SoGLShadeModelElement::evaluate(void) const
{
  ((SoGLShadeModelElement*)this)->updategl(this->flat);
}

/*!
  Force GL shading model to flat or smooth. This will not change the
  state of the element.
*/
void
SoGLShadeModelElement::forceSend(const SbBool flat) const
{
  ((SoGLShadeModelElement*)this)->updategl(flat);
}

// set correct GL state
void
SoGLShadeModelElement::updategl(const SbBool flatshade)
{
  if (flatshade != this->glflat) {
    this->glflat = flatshade;
    if (flatshade) glShadeModel(GL_FLAT);
    else glShadeModel(GL_SMOOTH);
  }
}
