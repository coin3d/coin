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
  \class SoWindowElement Inventor/elements/SoWindowElement.h
  \brief The SoWindowElement class is used to store current window attributes.
  \ingroup elements

  In Coin, this element is not API-compatible with SGI Inventor, since 
  it contains platform specific stuff, which we want to avoid.
  
  Instead of the platform specific types we use void pointers. We're
  sorry for any inconvenience this might cause people using this element.
*/

#include <Inventor/elements/SoWindowElement.h>
#include <assert.h>

/*!
  \fn SoWindowElement::window

  The window id.
*/

/*!
  \fn SoWindowElement::context

  The current context.
*/

/*!
  \fn SoWindowElement::display

  The current display.
*/

/*!
  \fn SoWindowElement::glRenderAction

  The current render action.
*/

SO_ELEMENT_SOURCE(SoWindowElement);

// doc in parent
void
SoWindowElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoWindowElement, inherited);
}

/*!
  The destructor.
*/
SoWindowElement::~SoWindowElement(void)
{
}

// doc in parent
void
SoWindowElement::init(SoState * state)
{
  inherited::init(state);
  this->window = 0;
  this->context = NULL;
  this->display = NULL;
  this->glRenderAction = NULL;
}

// doc in parent
void
SoWindowElement::push(SoState * state)
{
  inherited::push(state);
}

// doc in parent
void
SoWindowElement::pop(SoState * state,
                     const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
}

// doc in parent
SbBool
SoWindowElement::matches(const SoElement * /* element */) const
{
  assert(0 && "should never be called.");
  return TRUE;
}

// doc in parent
SoElement *
SoWindowElement::copyMatchInfo(void) const
{
  assert(0 && "should never be called.");
  return NULL;
}

/*!
  Sets data for this element.
*/
void
SoWindowElement::set(SoState * state,
                     void * window,
                     void * context,
                     void * display,
                     SoGLRenderAction * action)
{
  SoWindowElement * elem = (SoWindowElement*)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->window = window;
    elem->context = context;
    elem->display = display;
    elem->glRenderAction = action;
  }
}


/*!
  Returns data for this element.
*/
void
SoWindowElement::get(SoState * state,
                     void * & window,
                     void * & context,
                     void * & display,
                     SoGLRenderAction * & action)
{
  const SoWindowElement * elem = (const SoWindowElement*)
    SoElement::getConstElement(state, classStackIndex);
  
  window = elem->window;
  context = elem->context;
  display = elem->display;
  action = elem->glRenderAction;
}
