/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoGLLightIdElement Inventor/elements/SoGLLightIdElement.h
  \brief The SoGLLightIdElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLightIdElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

int SoGLLightIdElement::maxGLSources = -1;

/*!
  \fn SoGLLightIdElement::maxGLSources

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoGLLightIdElement);

/*!
  This static method initializes static data for the
  SoGLLightIdElement class.
*/

void
SoGLLightIdElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLLightIdElement, inherited);
}

/*!
  The destructor.
*/

SoGLLightIdElement::~SoGLLightIdElement(void)
{
}

//! FIXME: write doc.

void
SoGLLightIdElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoGLLightIdElement::push(SoState * state)
{
  inherited::push(state);
  this->data = ((SoGLLightIdElement*)this->getNextInStack())->data;
}

//! FIXME: write doc.

void
SoGLLightIdElement::pop(SoState * state,
                        const SoElement * prevTopElement)
{
  int idx = this->data + 1;
  int prevdata = ((SoGLLightIdElement*)prevTopElement)->data;
  // disable used light sources
  while (idx <= prevdata) {
    glDisable((GLenum)((int32_t)GL_LIGHT0 + idx));
    idx++;
  }
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::increment(SoState * const state,
                              SoNode * const /* node */)
{
  SoGLLightIdElement * element = (SoGLLightIdElement *)
    getElement(state, getClassStackIndex());

  assert(element);

  element->data++;
  if (element->data >= SoGLLightIdElement::getMaxGLSources()) {
    element->data--;
#if COIN_DEBUG
    SoDebugError::postWarning("SoGLLightIdElement::increment", "no GL light available");
#endif
    return -1;
  }
  glEnable((GLenum)((int32_t)GL_LIGHT0 + element->data));

  return element->data;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoGLLightIdElement::getMaxGLSources()
{
  // FIXME: consider context. pederb, 20001012
  if (SoGLLightIdElement::maxGLSources == -1) {
    // NB: don't try to be clever and move this code to the
    // initClass() method, as it won't work -- the GL variables may
    // not have been initialized yet when it's called. --mortene
    GLint val;
    glGetIntegerv(GL_MAX_LIGHTS, &val);
    SoGLLightIdElement::maxGLSources = (int32_t)val;
  }

  return SoGLLightIdElement::maxGLSources;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoGLLightIdElement::increment(SoState * const state)
{
  return increment(state, NULL);
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoGLLightIdElement::get(SoState * const state)
{
  return SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoGLLightIdElement::getDefault()
{
  return -1;
}
