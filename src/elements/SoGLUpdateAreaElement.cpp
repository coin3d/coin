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
  \class SoGLUpdateAreaElement Inventor/elements/SoGLUpdateAreaElement.h
  \brief The SoGLUpdateAreaElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLUpdateAreaElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

/*!
  \fn SoGLUpdateAreaElement::origin

  FIXME: write doc
*/

/*!
  \fn SoGLUpdateAreaElement::size

  FIXME: write doc
*/

SO_ELEMENT_SOURCE(SoGLUpdateAreaElement);

/*!
  This static method initializes static data for the
  SoGLUpdateAreaElement class.
*/

void
SoGLUpdateAreaElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLUpdateAreaElement, inherited);
}

/*!
  The destructor.
*/

SoGLUpdateAreaElement::~SoGLUpdateAreaElement()
{
}

// doc from parent
void
SoGLUpdateAreaElement::init(SoState * state)
{
  inherited::init(state);
  this->origin = getDefaultOrigin();
  this->size = getDefaultSize();

  // set these to dummy values. scissor test will be disabled
  this->screensize.setValue(0, 0);
  this->screenorigin.setValue(0, 0);

  // scissorstate is used to keep track of current scissor
  // test state.
  this->scissorstate = FALSE;

  // disabled by default
  glDisable(GL_SCISSOR_TEST);
}

// doc from parent
void
SoGLUpdateAreaElement::push(SoState * state)
{
  inherited::push(state);
  SoGLUpdateAreaElement * prev = (SoGLUpdateAreaElement*)
    this->getNextInStack();
  this->scissorstate = prev->scissorstate;
}

// doc from parent
void
SoGLUpdateAreaElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  this->scissorstate = ((SoGLUpdateAreaElement*)prevTopElement)->scissorstate;
  this->updategl();
  inherited::pop(state, prevTopElement);
}

// doc from parent
SbBool
SoGLUpdateAreaElement::matches(const SoElement * element) const
{
  const SoGLUpdateAreaElement * elem = (SoGLUpdateAreaElement*) element;
  return
    this->origin == elem->origin &&
    this->size == elem->size;
}

// doc from parent
SoElement *
SoGLUpdateAreaElement::copyMatchInfo() const
{
  SoGLUpdateAreaElement * elem = (SoGLUpdateAreaElement*) this->getTypeId().createInstance();
  elem->origin = this->origin;
  elem->size = this->size;
  return elem;
}


/*!  
  Sets the update area. This can, for instance, be used when
  rendering in the front buffer, to only render parts of the scene
  during a window expose event.  \a origin and \a size is in
  normalized window coordinates.
*/
void
SoGLUpdateAreaElement::set(SoState * const state,
                           const SbVec2f & origin,
                           const SbVec2f & size)
{
  SoGLUpdateAreaElement * e = (SoGLUpdateAreaElement *)
    inherited::getElement(state, SoGLUpdateAreaElement::classStackIndex);
  if (e) {
    e->origin = origin;
    e->size = size;
    SbVec2s winsize = SoViewportRegionElement::get(state).getWindowSize();
    e->screenorigin[0] = (short) (origin[0]*float(winsize[0]));
    e->screenorigin[1] = (short) (origin[1]*float(winsize[1]));
    e->screensize[0] = (short) (size[0]*float(winsize[0]));
    e->screensize[1] = (short) (size[1]*float(winsize[1]));
    
    e->updategl();
  }
}


/*!
  Returns the current update area.

  \sa set()
*/
SbBool
SoGLUpdateAreaElement::get(SoState * const state,
                           SbVec2f & origin,
                           SbVec2f & size)
{
  const SoGLUpdateAreaElement * e = (const SoGLUpdateAreaElement *)
    inherited::getConstElement(state, SoGLUpdateAreaElement::classStackIndex);
  origin = e->origin;
  size = e->size;

  return e->isDefault();
}

/*!
  Returns the default update area origin, (0,0).
*/
SbVec2f
SoGLUpdateAreaElement::getDefaultOrigin(void)
{
  return SbVec2f(0.0f, 0.0f);
}

/*!
  Returns the default update area size, (1,1).
*/
SbVec2f
SoGLUpdateAreaElement::getDefaultSize(void)
{
  return SbVec2f(1.0f, 1.0f);
}

// return TRUE if element contains the default values
SbBool
SoGLUpdateAreaElement::isDefault(void) const
{
  return
    this->origin == getDefaultOrigin() &&
    this->size == getDefaultSize();
}

//
// keeps GL in sync
//
void
SoGLUpdateAreaElement::updategl(void)
{
  if (this->isDefault()) {
    if (this->scissorstate) {
      glDisable(GL_SCISSOR_TEST);
      this->scissorstate = FALSE;
    }
  }
  else {
    if (!this->scissorstate) {
      glEnable(GL_SCISSOR_TEST);
      this->scissorstate = TRUE;
    }
    glScissor((GLint) this->screenorigin[0],
              (GLint) this->screenorigin[1],
              (GLint) this->screensize[0],
              (GLint) this->screensize[1]);
  }
}
