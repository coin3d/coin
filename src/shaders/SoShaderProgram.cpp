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

#include <Inventor/nodes/SoShaderProgram.h>

#include <assert.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/nodes/SoShaderObject.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include "SoGLShaderProgram.h"

// *************************************************************************

class SoShaderProgramP 
{
public:
  SoShaderProgramP(SoShaderProgram * ownerptr);
  ~SoShaderProgramP();

  void GLRender(SoGLRenderAction * action);
  void updateStateMatrixParameters(void);
  SbBool doesContainStateMatrixParameters;

private:
  SoShaderProgram * owner;
  SoNodeList previousChildren;
  SoGLShaderProgram glShaderProgram;

  static void sensorCB(void * data, SoSensor *);
  SoNodeSensor * sensor;
  SbBool shouldTraverseShaderObjects;
  void updateProgramAndPreviousChildren(void);
  void removeFromPreviousChildren(SoNode * shader);
};

#define PRIVATE(p) ((p)->pimpl)
#define PUBLIC(p) ((p)->owner)

// *************************************************************************

SO_NODE_SOURCE(SoShaderProgram);

// *************************************************************************

void
SoShaderProgram::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShaderProgram,
                              SO_FROM_COIN_2_4|SO_FROM_INVENTOR_5_0);

  SO_ENABLE(SoGLRenderAction, SoGLShaderProgramElement);
}

SoShaderProgram::SoShaderProgram()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShaderProgram);

  SO_NODE_ADD_FIELD(shaderObject, (NULL));
  this->shaderObject.deleteValues(0, 1);

  PRIVATE(this) = new SoShaderProgramP(this);
}

SoShaderProgram::~SoShaderProgram()
{
  delete PRIVATE(this);
}

void
SoShaderProgram::GLRender(SoGLRenderAction * action)
{
  PRIVATE(this)->GLRender(action);
}

void
SoShaderProgram::updateStateMatrixParameters(SoState * state)
{
  PRIVATE(this)->updateStateMatrixParameters();
}

SbBool
SoShaderProgram::containStateMatrixParameters(void) const
{
  return PRIVATE(this)->doesContainStateMatrixParameters;
}

// *************************************************************************

SoShaderProgramP::SoShaderProgramP(SoShaderProgram * ownerptr)
{
  PUBLIC(this) = ownerptr;
  this->sensor = new SoNodeSensor(SoShaderProgramP::sensorCB, this);
  this->sensor->attach(ownerptr);

  this->shouldTraverseShaderObjects = TRUE;
  this->doesContainStateMatrixParameters = FALSE;
}

SoShaderProgramP::~SoShaderProgramP()
{
  delete this->sensor;
}

void
SoShaderProgramP::GLRender(SoGLRenderAction * action)
{
  SoState *state = action->getState();

  // FIXME: (Martin 2004-09-21) find an alternative to invalidating the cache
  SoCacheElement::invalidate(state);

  const cc_glglue * glctx =
    cc_glglue_instance(SoGLCacheContextElement::get(state));

  SoGLShaderProgram* oldProgram = SoGLShaderProgramElement::get(state);
  if (oldProgram) oldProgram->disable(glctx);

  SoGLShaderProgramElement::set(state, PUBLIC(this), &this->glShaderProgram);

  int cnt1 = PUBLIC(this)->shaderObject.getNum();
  int cnt2 = PUBLIC(this)->getNumChildren();
  int i;

  // load shader objects
  if (this->shouldTraverseShaderObjects) {
    for (i=0; i<cnt1; i++) {
      SoNode *node = PUBLIC(this)->shaderObject[i];
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
        this->removeFromPreviousChildren(node);
        ((SoShaderObject *)node)->GLRender(action);
      }
    }
    for (i=0; i<cnt2; i++) {
      SoNode *node = PUBLIC(this)->getChild(i);
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
        this->removeFromPreviousChildren(node);
        ((SoShaderObject *)node)->GLRender(action);
      }
    }
    this->updateProgramAndPreviousChildren();
  }

  // enable shader
  this->glShaderProgram.enable(glctx);

  // update shader object parameters
  if (this->shouldTraverseShaderObjects) {
    SbBool flag = FALSE;

    for (i=0; i<cnt1; i++) {
      SoShaderObject *node = (SoShaderObject *)PUBLIC(this)->shaderObject[i];
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
        node->updateAllParameters();
        if (node->containStateMatrixParameters()) flag = TRUE;
      }
    }
    for (i=0; i<cnt2; i++) {
      SoShaderObject *node = (SoShaderObject *)PUBLIC(this)->getChild(i);
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
        node->updateAllParameters();
        if (node->containStateMatrixParameters()) flag = TRUE;
      }
    }
    this->doesContainStateMatrixParameters = flag;
  }
  else if (this->doesContainStateMatrixParameters)
    this->updateStateMatrixParameters();

  this->shouldTraverseShaderObjects = FALSE;
  SoGLTextureEnabledElement::set(state, TRUE);
}

void
SoShaderProgramP::updateStateMatrixParameters(void)
{
  int cnt1 = PUBLIC(this)->shaderObject.getNum();
  int cnt2 = PUBLIC(this)->getNumChildren();
  int i;

  for (i=0; i<cnt1; i++) {
    SoNode *node = PUBLIC(this)->shaderObject[i];
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject*)node)->updateStateMatrixParameters();
    }
  }
  for (i=0; i<cnt2; i++) {
    SoNode *node = PUBLIC(this)->getChild(i);
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject*)node)->updateStateMatrixParameters();
    }
  }
}

void
SoShaderProgramP::updateProgramAndPreviousChildren(void)
{
  int i, cnt1, cnt2 = this->previousChildren.getLength();

  for (i=cnt2-1; i>=0; i--) {
    SoShaderObject *node = (SoShaderObject*)this->previousChildren[i];
    node->removeGLShaderFromGLProgram(&this->glShaderProgram);
    this->previousChildren.remove(i);
  }
  assert(this->previousChildren.getLength() == 0);
  cnt1 = PUBLIC(this)->shaderObject.getNum();
  cnt2 = PUBLIC(this)->getNumChildren();
  for (i=0; i<cnt1; i++)
    this->previousChildren.append(PUBLIC(this)->shaderObject[i]);
  for (i=0; i<cnt2; i++)
    this->previousChildren.append(PUBLIC(this)->getChild(i));
  this->previousChildren.truncate(this->previousChildren.getLength());
}

void
SoShaderProgramP::removeFromPreviousChildren(SoNode * shader)
{
  if (this->previousChildren.getLength() == 0) return;

  int idx = this->previousChildren.find(shader);

  if (idx >= 0) this->previousChildren.remove(idx);
}

void
SoShaderProgramP::sensorCB(void * data, SoSensor *)
{
  ((SoShaderProgramP *)data)->shouldTraverseShaderObjects = TRUE;
}
