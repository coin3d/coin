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
  \class SoGLTextureEnabledElement Inventor/elements/SoGLTextureEnabledElement.h
  \brief The SoGLTextureEnabledElement class is a lazy element which controls whether texturing is enabled or not.
  \ingroup elements

  This element is evaluated in SoShape::shouldGLRender() so you'll
  normally not have to worry about this being a lazy element. However,
  if you implement your own shape and need to disable texturing while
  rendering, use the forceSend() method to change the GL state without
  changing the state of the element.

  \sa SoGLTexture3EnabledElement
*/

#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

SO_ELEMENT_SOURCE(SoGLTextureEnabledElement);

// doc from parent
void
SoGLTextureEnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLTextureEnabledElement, inherited);
}

/*!
  The destructor.
*/
SoGLTextureEnabledElement::~SoGLTextureEnabledElement(void)
{
}

/*!
  Sets the state of this element.
*/
void
SoGLTextureEnabledElement::set(SoState * const state,
                               SoNode * const node,
                               const SbBool enabled)
{
  SoInt32Element::set(classStackIndex, state, node, (int32_t) enabled);
  SoShapeStyleElement::setTextureEnabled(state, enabled);
}


// doc from parent
void
SoGLTextureEnabledElement::init(SoState * state)
{
  inherited::init(state);
  this->data = SoGLTextureEnabledElement::getDefault();
  this->glstate = 0;
  glDisable(GL_TEXTURE_2D);
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTextureEnabledElement::push(SoState * state)
{
  inherited::push(state);
  this->glstate = ((SoGLTextureEnabledElement*)this->getNextInStack())->glstate;
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTextureEnabledElement::pop(SoState * state,
                               const SoElement * prevTopElement)
{
  this->glstate = ((SoGLTextureEnabledElement*)prevTopElement)->glstate;
  inherited::pop(state, prevTopElement);
}

/*!
  Evaluates the element. After this call the GL state will be the same
  as the state of the element.
*/
void
SoGLTextureEnabledElement::lazyEvaluate(void) const
{
  if (this->data != this->glstate) {
    this->updategl(this->data);
  }
}

// doc in parent
SbBool
SoGLTextureEnabledElement::isLazy(void) const
{
  return TRUE;
}

/*!
  Updates the GL state without changing the state of the element. If GL
  state already is the same as \a onoff, nothing will happen.
*/
void
SoGLTextureEnabledElement::forceSend(SoState * const state,
                                     const SbBool onoff)
{
  const SoGLTextureEnabledElement * te = (const SoGLTextureEnabledElement *)
    SoElement::getConstElement(state, classStackIndex);
  if (te->glstate != onoff) {
    te->updategl(onoff);
  }
}


/*!
  Sets the state of the element.
*/
void
SoGLTextureEnabledElement::set(SoState * const state, const SbBool enabled)
{
  SoGLTextureEnabledElement::set(state, NULL, enabled);
}


/*!
  Return current state of this element. This is not the same as the
  current GL state, since this is a lazy element.
*/
SbBool
SoGLTextureEnabledElement::get(SoState * const state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}


/*!
  Returns default state of this element (FALSE).
*/
SbBool
SoGLTextureEnabledElement::getDefault(void)
{
  return FALSE;
}

//
// updates GL state if needed
//
void
SoGLTextureEnabledElement::updategl(const int32_t newstate) const
{
  // cast away constness since GL state is not really a part of the
  // element value.
  ((SoGLTextureEnabledElement*)this)->glstate = newstate;
  if (newstate) glEnable(GL_TEXTURE_2D);
  else glDisable(GL_TEXTURE_2D);
}
