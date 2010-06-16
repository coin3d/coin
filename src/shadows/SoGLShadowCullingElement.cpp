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
  \class SoGLShadowCullingElement Inventor/elements/SoGLShadowCullingElement.h
  \brief The SoGLShadowCullingElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  \since Coin 2.5
*/

#include <Inventor/annex/FXViz/elements/SoGLShadowCullingElement.h>
#include <assert.h>
#include <Inventor/system/gl.h>
#include "coindefs.h"

/*!
  \fn SoGLShadowCullingElement::Mode

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoGLShadowCullingElement);

// doc in parent
void
SoGLShadowCullingElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLShadowCullingElement, inherited);
}

/*!
  The destructor.
*/

SoGLShadowCullingElement::~SoGLShadowCullingElement(void)
{
}

//! FIXME: write doc.

void
SoGLShadowCullingElement::set(SoState * const state,
                              SoNode * const node,
                              const int mode)
{
  SoInt32Element::set(classStackIndex, state, node, mode);
}

//! FIXME: write doc.

void
SoGLShadowCullingElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

int
SoGLShadowCullingElement::get(SoState * const state)
{
  return SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

int
SoGLShadowCullingElement::getDefault(void)
{
  return AS_IS_CULLING;
}

void 
SoGLShadowCullingElement::push(SoState * COIN_UNUSED_ARG(state))
{
  SoGLShadowCullingElement * prev = (SoGLShadowCullingElement*) this->getNextInStack();

  this->data = prev->data;
}

void 
SoGLShadowCullingElement::pop(SoState * COIN_UNUSED_ARG(state), const SoElement * prevTopElement)
{
  SoGLShadowCullingElement * prev = (SoGLShadowCullingElement*) prevTopElement;
  if (prev->data != this->data) {
    this->updateGL(prev->data, this->data);
  }
}

void 
SoGLShadowCullingElement::setElt(int32_t value)
{
  if (this->data != value) {
    this->updateGL(this->data, value);
  }
  inherited::setElt(value);
}

void 
SoGLShadowCullingElement::updateGL(int32_t COIN_UNUSED_ARG(oldvalue), int32_t COIN_UNUSED_ARG(value))
{
  // nothing to do yet. We might support more culling modes in the future though
}


#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  BOOST_CHECK_MESSAGE(SoGLShadowCullingElement::getClassStackIndex() != -1,
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
