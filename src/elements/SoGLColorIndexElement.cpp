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
  \class SoGLColorIndexElement Inventor/elements/SoGLColorIndexElement.h
  \brief The SoGLColorIndexElement class sets the current OpenGL color.
  \ingroup elements

  This element is only used when the OpenGL canvas is in colorindex
  mode, ie where colors for individual pixels are fetched from a color
  lookup table ("CLUT"). The usual thing to do is to set up a canvas
  in RGBA truecolor mode.

  One common use for colorindex mode OpenGL canvases is to use one in
  the overlay planes (which are usually limited to only 2 or 4
  available colors), if supported by the OpenGL hardware and / or
  driver.

  \sa SoColorIndex
*/

#include <Inventor/elements/SoGLColorIndexElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

SO_ELEMENT_SOURCE(SoGLColorIndexElement);

// doc in parent
void
SoGLColorIndexElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLColorIndexElement, inherited);
}


// doc in parent
void
SoGLColorIndexElement::init(SoState * state)
{
  this->state = state;
}

/*!
  The destructor.
*/
SoGLColorIndexElement::~SoGLColorIndexElement()
{
}

/*!
  Returns \c TRUE if the current GL context is in color index mode.
*/
SbBool
SoGLColorIndexElement::isColorIndexMode(SoState * state)
{
  return SoGLLazyElement::isColorIndex(state);
}

/*!
  Sets current color indices.
*/
void
SoGLColorIndexElement::set(SoState * const state, SoNode * const node,
                           const int32_t numindices,
                           const int32_t * const indices)
{
  SoLazyElement::setColorIndices(state, node, numindices, indices);
}

/*!
  Returns number of color indices in element.
*/
int32_t
SoGLColorIndexElement::getNum(void) const
{
  return SoLazyElement::getInstance(this->state)->getNumColorIndices();
}

/*!
  Returns the current element.
*/
const SoGLColorIndexElement *
SoGLColorIndexElement::getInstance(SoState *state)
{
  return (const SoGLColorIndexElement *)
    state->getElementNoPush(classStackIndex);
}
