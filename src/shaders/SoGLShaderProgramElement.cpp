/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/elements/SoGLShaderProgramElement.h>

#include <assert.h>

#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/nodes/SoGLShaderProgram.h>

// *************************************************************************

SO_ELEMENT_SOURCE(SoGLShaderProgramElement);

// *************************************************************************

void
SoGLShaderProgramElement::initClass()
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
}

void
SoGLShaderProgramElement::set(SoState* const state, SoNode *const node,
                              SoGLShaderProgram* program)
{
  SoGLShaderProgramElement* element = 
    (SoGLShaderProgramElement*)inherited::getElement(state,classStackIndex,node);
  element->shaderProgram = program;
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
  inherited::push(state);

  SoGLShaderProgramElement *last=(SoGLShaderProgramElement *)getNextInStack();
  assert(last);

  if (last->shaderProgram) last->shaderProgram->disable();
}

void
SoGLShaderProgramElement::pop(SoState * state, const SoElement *prev)
{
  SoGLShaderProgramElement *elem = (SoGLShaderProgramElement *)prev;
  if (elem->shaderProgram) elem->shaderProgram->disable();
  if (this->shaderProgram) this->shaderProgram->enable();

  inherited::pop(state, prev);
}
