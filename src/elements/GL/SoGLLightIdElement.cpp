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
  \class SoGLLightIdElement Inventor/elements/SoGLLightIdElement.h
  \brief The SoGLLightIdElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

// *************************************************************************

#include <Inventor/elements/SoGLLightIdElement.h>

#include <cassert>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/C/glue/gl.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/system/gl.h>

#include "glue/glp.h"
#include "rendering/SoGL.h"

// *************************************************************************

SO_ELEMENT_SOURCE(SoGLLightIdElement);

// *************************************************************************

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
  // capture element since we change the GL state here
  this->capture(state);

  int idx = this->data + 1;
  int prevdata = ((SoGLLightIdElement*)prevTopElement)->data;
  // disable used light sources
  while (idx <= prevdata) {
    glDisable((GLenum)((int32_t)GL_LIGHT0 + idx));
    idx++;
  }
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::increment(SoState * const state,
                              SoNode * const /* node */)
{
  SoGLLightIdElement * element = (SoGLLightIdElement *)
    getElement(state, getClassStackIndex());

  if (element) {
    const cc_glglue * glue = sogl_glue_instance(state);
    element->data++;
    int maxl = cc_glglue_get_max_lights(glue);

    if (element->data >= maxl) {
      element->data--;
#if COIN_DEBUG
      static SbBool warn = TRUE;

      if (warn) { // warn only once
        warn = FALSE;
        SoDebugError::postWarning("SoGLLightIdElement::increment",
                                  "Number of concurrent light sources in "
                                  "scene exceeds %d, which is the maximum "
                                  "number of concurrent light sources "
                                  "supported by this OpenGL implementation. "
                                  "Some light sources will be ignored.\n\n"

                                  "(Note to application "
                                  "programmers: this error is often caused by "
                                  "a missing SoState::pop() call in extension "
                                  "shape nodes -- audit your GLRender() "
                                  "method(s)).",

                                  maxl);
      }
#endif
      return -1;
    }
    glEnable((GLenum)((int32_t)GL_LIGHT0 + element->data));

    return element->data;
  }
  return -1;
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::getMaxGLSources(void)
{
  // FIXME: should also make a likewise method available as part of
  // the So*GLWidget classes. 20020802 mortene.

  SoDebugError::postWarning("SoGLLightIdElement::getMaxGLSources",
                            "This function is obsoleted. It should not "
                            "be used because its interface is fubar: "
                            "the number of light sources available from "
                            "the OpenGL driver depends on the context, and "
                            "this function does not know which context this "
                            "information is requested for.");

  GLint val;
  glGetIntegerv(GL_MAX_LIGHTS, &val);

  GLenum err = sogl_glerror_debugging() ? glGetError() : GL_NO_ERROR;
  assert(err == GL_NO_ERROR &&
         "GL error when calling glGetInteger() -- no current GL context?");

  return (int32_t)val;
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::increment(SoState * const state)
{
  return increment(state, NULL);
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::get(SoState * const state)
{
  return SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::getDefault()
{
  return -1;
}
