/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoGLColorIndexElement Inventor/elements/SoGLColorIndexElement.h
  \brief The SoGLColorIndexElement class is used when rendering in color index mode.

*/

#include <Inventor/elements/SoGLColorIndexElement.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

static int32_t defaultIndexArray[] = {1};

SO_ELEMENT_SOURCE(SoGLColorIndexElement);

// doc in parent
void
SoGLColorIndexElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLColorIndexElement, inherited);
}


// doc in parent
void
SoGLColorIndexElement::init(SoState * /* state */)
{
  this->indices = defaultIndexArray;
  this->numindices = 1;
  
  GLboolean rgba;
  glGetBooleanv(GL_RGBA_MODE, &rgba);
  this->colorindexmode = ! rgba;
}

// doc in parent
void
SoGLColorIndexElement::push(SoState * state)
{
  inherited::push(state);
  // need to copy the colorindexmode member
  SoGLColorIndexElement * prev =
    (SoGLColorIndexElement *) this->getNextInStack();
  this->colorindexmode = prev->colorindexmode;
}

/*!
  The destructor.
*/
SoGLColorIndexElement::~SoGLColorIndexElement()
{
}

/*!
  Returns TRUE if the current GL context is in color index mode.
*/
SbBool
SoGLColorIndexElement::isColorIndexMode(SoState * state)
{
  // use state->getConstElement() instead of SoElement::getConstElement()
  // to avoid cache dependencies.
  const SoGLColorIndexElement * element = (SoGLColorIndexElement*)
    state->getConstElement(classStackIndex);
  return element->colorindexmode;
}

/*!
  Sets current color indices.
*/
void
SoGLColorIndexElement::set(SoState * const state, SoNode * const node,
                           const int32_t numindices,
                           const int32_t * const indices)
{
  SoGLColorIndexElement * elem = (SoGLColorIndexElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->indices = indices;
  elem->numindices = numindices;
}


/*!
  Returns number of color indices in element.
*/
int32_t
SoGLColorIndexElement::getNum(void) const
{
  return this->numindices;
}

/*!
  Sends color index \a index to OpenGL.
*/
void
SoGLColorIndexElement::send(const int index)
{
  assert(index < this->numindices);
  glIndexi((GLint) this->indices[index]);
}

/*!
  Returns the current element.
*/
const SoGLColorIndexElement *
SoGLColorIndexElement::getInstance(SoState *state)
{
  return (const SoGLColorIndexElement *)
    SoElement::getConstElement(state, classStackIndex);
}
