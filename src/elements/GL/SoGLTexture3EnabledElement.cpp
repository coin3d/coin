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
  \class SoGLTexture3EnabledElement Inventor/elements/SoGLTexture3EnabledElement.h
  \brief The SoGLTexture3EnabledElement class is an element which controls
  whether 3D texturing is enabled or not.
  \ingroup elements

  \since 2001-11-26
*/

#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/actions/SoGLRenderAction.h>

#ifdef HAVE_CONFIG_H
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
  inherited::set(state, node, enabled);
  SoShapeStyleElement::setTexture3Enabled(state, enabled);
}


// doc from parent
void
SoGLTexture3EnabledElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state;
  this->updategl();
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTexture3EnabledElement::push(SoState * state)
{
  SoGLTexture3EnabledElement * prev = (SoGLTexture3EnabledElement*) this->getNextInStack();

  this->data = prev->data;
  this->state = state;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

// Documented in superclass. Overridden to track GL state.
void
SoGLTexture3EnabledElement::pop(SoState * state,
                                const SoElement * prevTopElement)
{
  SoGLTexture3EnabledElement * prev = (SoGLTexture3EnabledElement*) prevTopElement;
  if (this->data != prev->data) {
    this->updategl();
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
  Return current state of this element. 
*/
SbBool
SoGLTexture3EnabledElement::get(SoState * const state)
{
  return (SbBool) inherited::get(state);
}

/*!
  Returns default state of this element (FALSE).
*/
SbBool
SoGLTexture3EnabledElement::getDefault(void)
{
  return inherited::getDefault();
}

void
SoGLTexture3EnabledElement::setElt(int32_t value)
{
  if (this->data != value) {
    this->data = value;
    this->updategl();
  }
}


//
// updates GL state if needed
//
void
SoGLTexture3EnabledElement::updategl(void)
{
  const cc_glglue * glw = sogl_glue_instance(this->state);
  if (cc_glglue_has_3d_textures(glw)) {
    if (this->data) glEnable(GL_TEXTURE_3D);
    else glDisable(GL_TEXTURE_3D);
  }
}
