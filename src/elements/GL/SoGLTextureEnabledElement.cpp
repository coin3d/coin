/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoGLTextureEnabledElement Inventor/elements/SoGLTextureEnabledElement.h
  \brief The SoGLTextureEnabledElement class is an element which controls whether texturing is enabled or not.
  \ingroup elements

  \sa SoGLTexture3EnabledElement
*/

#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <assert.h>

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
  Sets the state of this element. Used for enabling GL_TEXTURE_2D and 
  disabling GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_EXT.
*/
void
SoGLTextureEnabledElement::set(SoState * const state,
                               SoNode * const node,
                               const SbBool enabled)
{
  inherited::set(state, node, enabled);
  SoShapeStyleElement::setTextureEnabled(state, enabled);
}

/*!
  Enables GL_TEXTURE_RECTANGLE_EXT/NV. GL_TEXTURE_2D will be
  disabled if it's enabled earlier.

  \since Coin 2.2
*/
void 
SoGLTextureEnabledElement::enableRectangle(SoState * state, SoNode * node)
{
  SoInt32Element::set(classStackIndex, state, node, 2);
  SoShapeStyleElement::setTextureEnabled(state, TRUE);
}

SoGLTextureEnabledElement::Mode 
SoGLTextureEnabledElement::getMode(SoState * state)
{
  return (Mode) SoInt32Element::get(classStackIndex, state);
}

// doc from parent
void
SoGLTextureEnabledElement::init(SoState * state)
{
  inherited::init(state);
  this->updategl(DISABLED, TEXTURE2D);
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTextureEnabledElement::push(SoState * state)
{
  SoGLTextureEnabledElement * prev = (SoGLTextureEnabledElement*) this->getNextInStack();

  this->data = prev->data;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTextureEnabledElement::pop(SoState * state,
                               const SoElement * prevTopElement)
{
  SoGLTextureEnabledElement * prev = (SoGLTextureEnabledElement*) prevTopElement;
  if (this->data != prev->data) {
    this->updategl((Mode) this->data, (Mode) prev->data);
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
  Return current state of this element. 
*/
SbBool
SoGLTextureEnabledElement::get(SoState * const state)
{
  return inherited::get(state);
}


/*!
  Returns default state of this element (FALSE).
*/
SbBool
SoGLTextureEnabledElement::getDefault(void)
{
  return inherited::getDefault();
}

void
SoGLTextureEnabledElement::setElt(int32_t value)
{
  if (this->data != value) {
    this->updategl((Mode) value, (Mode)this->data);
    this->data = value;
  }
}


//
// updates GL state (obsoleted)
//
void
SoGLTextureEnabledElement::updategl(void)
{
  assert(0 && "obsoleted");
  if (this->data) glEnable(GL_TEXTURE_2D);
  else glDisable(GL_TEXTURE_2D);
}

void 
SoGLTextureEnabledElement::updategl(const Mode newvalue, const Mode oldvalue)
{
  switch (oldvalue) {
  case DISABLED:
    break;
  case TEXTURE2D:
    glDisable(GL_TEXTURE_2D);
    break;
  case RECTANGLE:
    glDisable(GL_TEXTURE_RECTANGLE_EXT);
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
  default:
    assert(0 && "should not happen");
    break;
  }
}
