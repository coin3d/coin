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
  \class SoGLTexture3EnabledElement Inventor/elements/SoGLTexture3EnabledElement.h
  \brief The SoGLTexture3EnabledElement class is a lazy element which controls
  whether 3D texturing is enabled or not.
  \ingroup elements

  This element is evaluated in SoShape::shouldGLRender() so you'll
  normally not have to worry about this being a lazy element. However,
  if you implement your own shape and need to disable texturing while
  rendering, use the forceSend() method to change the GL state without
  changing the state of the element.

  \since 2001-11-26
*/

#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/actions/SoGLRenderAction.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/misc/SoGL.h> // GL wrapper.


SO_ELEMENT_SOURCE(SoGLTexture3EnabledElement);


// doc from parent
void
SoGLTexture3EnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLTexture3EnabledElement, inherited);
}

/*!
  The destructor.
*/
SoGLTexture3EnabledElement::~SoGLTexture3EnabledElement(void)
{
}

/*!
  Sets the state of this element.
*/
void
SoGLTexture3EnabledElement::set(SoState * const state,
                               SoNode * const node,
                               const SbBool enabled)
{
  SoInt32Element::set(classStackIndex, state, node, (int32_t) enabled);
  SoShapeStyleElement::setTexture3Enabled(state, enabled);
}


// doc from parent
void
SoGLTexture3EnabledElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state;
  this->data = SoGLTexture3EnabledElement::getDefault();
  this->glstate = 0;
  const cc_glglue * glw = sogl_glue_instance(state);
  if (cc_glglue_has_3d_textures(glw)) glDisable(GL_TEXTURE_3D);
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTexture3EnabledElement::push(SoState * state)
{
  inherited::push(state);
  this->glstate =
    ((SoGLTexture3EnabledElement*)this->getNextInStack())->glstate;
  this->state = state;
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTexture3EnabledElement::pop(SoState * state,
                               const SoElement * prevTopElement)
{
  this->glstate = ((SoGLTexture3EnabledElement*)prevTopElement)->glstate;
  inherited::pop(state, prevTopElement);
}

/*!
  Evaluates the element. After this call the GL state will be the same
  as the state of the element.
*/
void
SoGLTexture3EnabledElement::lazyEvaluate(void) const
{
  if (this->data != this->glstate) {
    ((SoGLTexture3EnabledElement*)this)->updategl();
  }
}

// doc in parent
SbBool
SoGLTexture3EnabledElement::isLazy(void) const
{
  return TRUE;
}

/*!
  Updates the GL state without changing the state of the element. If GL
  state already is the same as \a onoff, nothing will happen.
*/
void
SoGLTexture3EnabledElement::forceSend(SoState * const state,
                                      const SbBool onoff)
{
  SoGLTexture3EnabledElement * te = (SoGLTexture3EnabledElement *)
    SoElement::getConstElement(state, classStackIndex);
  if (te->glstate != onoff) {
    te->glstate = onoff;
    const cc_glglue * glw = sogl_glue_instance(state);
    if (cc_glglue_has_3d_textures(glw)) {
      if (onoff) glEnable(GL_TEXTURE_3D);
      else glDisable(GL_TEXTURE_3D);
    }
  }
}


/*!
  Sets the state of the element.
*/
void
SoGLTexture3EnabledElement::set(SoState * const state, const SbBool enabled)
{
  SoGLTexture3EnabledElement::set(state, NULL, enabled);
}


/*!
  Return current state of this element. This is not the same as the
  current GL state, since this is a lazy element.
*/
SbBool
SoGLTexture3EnabledElement::get(SoState * const state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}


/*!
  Returns default state of this element (FALSE).
*/
SbBool
SoGLTexture3EnabledElement::getDefault()
{
  return FALSE;
}

//
// updates GL state if needed
//
void
SoGLTexture3EnabledElement::updategl(void)
{
  this->glstate = this->data;
  const cc_glglue * glw = sogl_glue_instance(this->state);
  if (cc_glglue_has_3d_textures(glw)) {
    if (this->data) glEnable(GL_TEXTURE_3D);
    else glDisable(GL_TEXTURE_3D);
  }
}
