/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoGLTextureEnabledElement Inventor/elements/SoGLTextureEnabledElement.h
  \brief The SoGLTextureEnabledElement class is an element which controls whether texturing is enabled or not.
  \ingroup elements

  \sa SoGLTexture3EnabledElement
*/

#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGLRenderAction.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>
#include <assert.h>

#define MAX_UNITS 16

SO_ELEMENT_SOURCE(SoGLMultiTextureEnabledElement);

// doc from parent
void
SoGLMultiTextureEnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLMultiTextureEnabledElement, inherited);
}

/*!
  The destructor.
*/
SoGLMultiTextureEnabledElement::~SoGLMultiTextureEnabledElement(void)
{
}

// doc from parent
void
SoGLMultiTextureEnabledElement::init(SoState * state)
{
  inherited::init(state);

  SoAction * action = state->getAction();
  assert(action->isOfType(SoGLRenderAction::getClassTypeId()));

  // fetch cache context from action since SoGLCacheContextElement
  // might not be initialized yet.
  SoGLRenderAction * glaction = (SoGLRenderAction*) action;
  this->glue = cc_glglue_instance(glaction->getCacheContext());
}

// Documented in superclass. Overridden to track GL state.
void
SoGLMultiTextureEnabledElement::push(SoState * state)
{
  SoGLMultiTextureEnabledElement * prev = (SoGLMultiTextureEnabledElement*) this->getNextInStack();

  this->glue = prev->glue;
  // copy state from previous element
  inherited::push(state);
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

// Documented in superclass. Overridden to track GL state.
void
SoGLMultiTextureEnabledElement::pop(SoState * state,
                                    const SoElement * prevTopElement)
{
  SoGLMultiTextureEnabledElement * prev = (SoGLMultiTextureEnabledElement*) prevTopElement;
  for (int i = 0; i < MAX_UNITS; i++) {
    if (this->isEnabled(i) != prev->isEnabled(i)) {
      this->updategl(i);
    }
  }
}

void
SoGLMultiTextureEnabledElement::setElt(const int unit, const SbBool value)
{
  if (this->isEnabled(unit) != value) {
    inherited::setElt(unit, value);
    this->updategl(unit);
  }
}


//
// updates GL state
//
void
SoGLMultiTextureEnabledElement::updategl(const int unit)
{
  cc_glglue_glActiveTexture(this->glue, (GLenum) (int(GL_TEXTURE0) + unit));
  if (this->isEnabled(unit)) glEnable(GL_TEXTURE_2D);
  else glDisable(GL_TEXTURE_2D);
  cc_glglue_glActiveTexture(this->glue, (GLenum) GL_TEXTURE0);
}

#undef MAX_UNITS
