/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoGLLightIdElement Inventor/elements/SoGLLightIdElement.h
  \brief The SoGLLightIdElement class is yet to be documented.

  FIXME: write doc.
*/


//
// FIXME: might be smart to make this element lazy?
//

#include <Inventor/elements/SoGLLightIdElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif

#include <Inventor/misc/SoGL.h>
#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

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
  A constructor.  Can't be used directly.

  \sa void * SoGLLightIdElement::createInstance(void)
*/

SoGLLightIdElement::SoGLLightIdElement()
{
    setTypeId(SoGLLightIdElement::classTypeId);
    setStackIndex(SoGLLightIdElement::classStackIndex);
    this->data = getDefault();
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
  ((SoGLLightIdElement*)this->next)->data = this->data;
}

//! FIXME: write doc.

void
SoGLLightIdElement::pop(SoState * state,
                        const SoElement * prevTopElement)
{
  int idx = ((SoGLLightIdElement*)prevTopElement)->data + 1;
  // disable used light sources
  while (idx <= this->data) {
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
