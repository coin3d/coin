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
  \class SoDepthBufferElement Inventor/elements/SoDepthBufferElement.h
  \brief The SoDepthBufferElement controls the depth buffer settings.

  \ingroup elements
  \COIN_CLASS_EXTENSION
  \since Coin 3.0
*/

#include <Inventor/elements/SoDepthBufferElement.h>

#include <cassert>

#include "coindefs.h"
#include "SbBasicP.h"

SO_ELEMENT_SOURCE(SoDepthBufferElement);

/*!
  This function initializes the SoDepthBufferElement class type for Coin.
*/
void
SoDepthBufferElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoDepthBufferElement, inherited);
}

/*!
  The destructor.
*/
SoDepthBufferElement::~SoDepthBufferElement(void)
{
}

/*!
  Internal Coin method.
*/
void
SoDepthBufferElement::init(SoState * state)
{
  inherited::init(state);
  this->test = TRUE;
  this->write = TRUE;
  this->function = LEQUAL;
  this->range.setValue(0.0f, 1.0f);
}

/*!
  Internal Coin method.
*/
void
SoDepthBufferElement::push(SoState * state)
{
  const SoDepthBufferElement * prev = coin_assert_cast<const SoDepthBufferElement *>
    (
     this->getNextInStack()
     );
  this->test = prev->test;
  this->write = prev->write;
  this->function = prev->function;
  this->range = prev->range;
  prev->capture(state);
}

/*!
  Internal Coin method.
*/
void
SoDepthBufferElement::pop(SoState * COIN_UNUSED_ARG(state),
                          const SoElement * COIN_UNUSED_ARG(prevTopElement))
{
}

/*!
  Set this element's values.
*/
void
SoDepthBufferElement::set(SoState * state,
                          SbBool test,
                          SbBool write,
                          DepthWriteFunction function,
                          SbVec2f range)
{
  SoDepthBufferElement * elem =
    static_cast<SoDepthBufferElement *>(SoElement::getElement(state, classStackIndex));

  elem->setElt(test, write, function, range);
}

/*!
  Fetches this element's values.
*/
void
SoDepthBufferElement::get(SoState * state,
                          SbBool & test_out,
                          SbBool & write_out,
                          DepthWriteFunction & function_out,
                          SbVec2f & range_out)
{
  const SoDepthBufferElement * elem =
    static_cast<const SoDepthBufferElement *>(SoElement::getConstElement(state, classStackIndex));
  test_out = elem->test;
  write_out = elem->write;
  function_out = elem->function;
  range_out = elem->range;
}

/*!
  Returns the depth test enabled state.
*/
SbBool
SoDepthBufferElement::getTestEnable(SoState * state)
{
  const SoDepthBufferElement * elem =
    static_cast<const SoDepthBufferElement *>(SoElement::getConstElement(state, classStackIndex));
  return elem->test;
}

/*!
  Returns the depth buffer write enabled state.
*/
SbBool
SoDepthBufferElement::getWriteEnable(SoState * state)
{
  const SoDepthBufferElement * elem =
    static_cast<const SoDepthBufferElement *>(SoElement::getConstElement(state, classStackIndex));
  return elem->write;
}

/*!
  Returns the set depth buffer write function.
*/
SoDepthBufferElement::DepthWriteFunction
SoDepthBufferElement::getFunction(SoState * state)
{
  const SoDepthBufferElement * elem =
    static_cast<const SoDepthBufferElement *>(SoElement::getConstElement(state, classStackIndex));
  return elem->function;
}

/*!
  Returns the depth buffer value range used.
*/
SbVec2f
SoDepthBufferElement::getRange(SoState * state)
{
  const SoDepthBufferElement * elem =
    static_cast<const SoDepthBufferElement *>(SoElement::getConstElement(state, classStackIndex));
  return elem->range;
}

/*!
  Internal Coin method.
*/
SbBool
SoDepthBufferElement::matches(const SoElement * element) const
{
  const SoDepthBufferElement * elem =
    static_cast<const SoDepthBufferElement *>(element);

  return (elem->test == this->test)
    && (elem->write == this->write)
    && (elem->function == this->function)
    && (elem->range == this->range);
}

/*!
  Internal Coin method.
*/
SoElement *
SoDepthBufferElement::copyMatchInfo(void) const
{
  SoDepthBufferElement * elem =
    static_cast<SoDepthBufferElement *>(this->getTypeId().createInstance());
  elem->test = this->test;
  elem->write = this->write;
  elem->function = this->function;
  elem->range = this->range;
  return elem;
}

/*!
  Virtual method to set the state to get derived elements updated.
*/
void
SoDepthBufferElement::setElt(SbBool test, SbBool write, DepthWriteFunction function, SbVec2f range)
{
  this->test = test;
  this->write = write;
  this->function = function;
  this->range = range;
}
