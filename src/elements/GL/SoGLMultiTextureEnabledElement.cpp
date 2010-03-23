/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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
#include "coindefs.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>
#include <cassert>

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
  this->cachecontext = ((SoGLRenderAction*)action)->getCacheContext();
}

// Documented in superclass. Overridden to track GL state.
void
SoGLMultiTextureEnabledElement::push(SoState * state)
{
  SoGLMultiTextureEnabledElement * prev = (SoGLMultiTextureEnabledElement*) this->getNextInStack();

  this->cachecontext = prev->cachecontext;

  // copy state from previous element
  inherited::push(state);
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

// Documented in superclass. Overridden to track GL state.
void
SoGLMultiTextureEnabledElement::pop(SoState * COIN_UNUSED_ARG(state),
                                    const SoElement * prevTopElement)
{
  SoGLMultiTextureEnabledElement * prev = (SoGLMultiTextureEnabledElement*) prevTopElement;
  const int maxunits = SbMax(this->getMaxUnits(), prev->getMaxUnits());
  
  for (int i = 0; i < maxunits; i++) {
    Mode oldmode = prev->getMode(i);
    Mode newmode =  this->getMode(i);
    if (oldmode != newmode) {
      this->updategl(i, newmode, oldmode);
    }
  }
}

void
SoGLMultiTextureEnabledElement::setElt(const int unit, const int value)
{
  Mode oldmode = this->getMode(unit);
  Mode newmode = (Mode) value;

  if (oldmode != newmode) {
    inherited::setElt(unit, value);
    this->updategl(unit, newmode, oldmode);
  }
}

//
// updates GL state
//
void
SoGLMultiTextureEnabledElement::updategl(const int unit)
{
  const cc_glglue * glue = cc_glglue_instance(this->cachecontext);
  cc_glglue_glActiveTexture(glue, (GLenum) (int(GL_TEXTURE0) + unit));
  if (this->isEnabled(unit)) glEnable(GL_TEXTURE_2D);
  else glDisable(GL_TEXTURE_2D);
  cc_glglue_glActiveTexture(glue, (GLenum) GL_TEXTURE0);
}

void
SoGLMultiTextureEnabledElement::updategl(const int unit, const Mode newvalue, const Mode oldvalue)
{
  const cc_glglue * glue = cc_glglue_instance(this->cachecontext);
  cc_glglue_glActiveTexture(glue, (GLenum) (int(GL_TEXTURE0) + unit));

  switch (oldvalue) {
  case DISABLED:
    break;
  case TEXTURE2D:
    glDisable(GL_TEXTURE_2D);
    break;
  case RECTANGLE:
    glDisable(GL_TEXTURE_RECTANGLE_EXT);
    break;
  case CUBEMAP:
    glDisable(GL_TEXTURE_CUBE_MAP);
    break;
  case TEXTURE3D:
    glDisable(GL_TEXTURE_3D);
    break;
  default:
    assert(0 && "should not happen");
    break;
  }
  switch (newvalue) {
  case DISABLED:
    break;
  case TEXTURE2D:
    glEnable(GL_TEXTURE_2D);
    break;
  case RECTANGLE:
    glEnable(GL_TEXTURE_RECTANGLE_EXT);
    break;
  case CUBEMAP:
    glEnable(GL_TEXTURE_CUBE_MAP);
    break;
  case TEXTURE3D:
    glEnable(GL_TEXTURE_3D);
    break;
  default:
    assert(0 && "should not happen");
    break;
  }
  cc_glglue_glActiveTexture(glue, (GLenum) GL_TEXTURE0);

}

