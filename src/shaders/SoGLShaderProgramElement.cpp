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

#include <Inventor/elements/SoGLShaderProgramElement.h>

#include <assert.h>

#include <Inventor/elements/SoGLCacheContextElement.h>
#include "SoGLShaderProgram.h"

// *************************************************************************

SO_ELEMENT_SOURCE(SoGLShaderProgramElement);

// *************************************************************************

void
SoGLShaderProgramElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLShaderProgramElement, inherited);
}

SoGLShaderProgramElement::~SoGLShaderProgramElement()
{
  this->shaderProgram = NULL;
}

void
SoGLShaderProgramElement::init(SoState *state)
{
  inherited::init(state);
  this->shaderProgram = NULL;
  this->enabled = FALSE;
}

void
SoGLShaderProgramElement::enable(SoState * const state, const SbBool onoff)
{
  SoGLShaderProgramElement* element =
    (SoGLShaderProgramElement*) SoElement::getElement(state,classStackIndex);
  element->enabled = onoff;
  element->objectids.truncate(0);
  
  if (element->shaderProgram) {
    if (onoff) {
      if (!element->shaderProgram->isEnabled()) element->shaderProgram->enable(state);
    }
    else {
      if (element->shaderProgram->isEnabled()) element->shaderProgram->disable(state);
    }
    element->shaderProgram->getShaderObjectIds(element->objectids);
  }
}

void
SoGLShaderProgramElement::set(SoState* const state, SoNode *const node,
                              SoGLShaderProgram* program)
{
  SoGLShaderProgramElement* element =
    (SoGLShaderProgramElement*)inherited::getElement(state,classStackIndex, node);
  
  if (program != element->shaderProgram) {
    if (element->shaderProgram) element->shaderProgram->disable(state);
  }
  element->shaderProgram = program;
  element->enabled = FALSE;
  element->objectids.truncate(0);
  if (program) program->getShaderObjectIds(element->objectids);
  // don't enable new program here. The node will call enable()
  // after setting up all the objects
}

SoGLShaderProgram *
SoGLShaderProgramElement::get(SoState *state)
{
  const SoElement *element = getConstElement(state, classStackIndex);
  assert(element);
  return ((const SoGLShaderProgramElement *)element)->shaderProgram;
}

void
SoGLShaderProgramElement::push(SoState * state)
{
  SoGLShaderProgramElement * prev = (SoGLShaderProgramElement *) getNextInStack();
  assert(prev);
  this->shaderProgram = prev->shaderProgram;
  this->enabled = prev->enabled;
  this->nodeId = prev->nodeId;
  this->objectids = prev->objectids;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

void
SoGLShaderProgramElement::pop(SoState * state, const SoElement * prevTopElement)
{
  SoGLShaderProgramElement * elem = (SoGLShaderProgramElement *)prevTopElement;
  if (this->shaderProgram != elem->shaderProgram) {
    if (elem->shaderProgram) {
      elem->shaderProgram->disable(state);
      elem->enabled = FALSE;
    }
    if (this->shaderProgram) {
      if (this->enabled) this->shaderProgram->enable(state);
    }
  }
  else if (this->shaderProgram) {
    if (this->enabled != elem->enabled) {
      if (this->enabled) this->shaderProgram->enable(state);
      else this->shaderProgram->disable(state);
    }
  }
  elem->shaderProgram = NULL;
}


SbBool
SoGLShaderProgramElement::matches(const SoElement * element) const
{
  SoGLShaderProgramElement * elem = (SoGLShaderProgramElement*) element;
  return (this->enabled == elem->enabled) && (this->objectids == elem->objectids);
}

SoElement *
SoGLShaderProgramElement::copyMatchInfo(void) const
{
  SoGLShaderProgramElement * elem = 
    (SoGLShaderProgramElement*) inherited::copyMatchInfo();
  
  elem->enabled = this->enabled;
  elem->objectids = this->objectids;
  return elem;
}
