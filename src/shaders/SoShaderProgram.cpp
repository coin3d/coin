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
#include <Inventor/nodes/SoShaderObject.h>
#include <Inventor/nodes/SoGLShaderProgram.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoElements.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <iostream>
#include <assert.h>

// *************************************************************************

class SoShaderProgramP 
{
public:
  SoShaderProgramP(SoShaderProgram * ownerptr);
  ~SoShaderProgramP();

  SoShaderProgram * owner;
  SoNodeList        previousChildren;
  SoGLShaderProgram glShaderProgram;

  void GLRender(SoGLRenderAction *action);
  void updateStateMatrixParameters(SoState *state);
  SbBool doesContainStateMatrixParameters;

private:
  static void sensorCB(void *data, SoSensor *);
  SoNodeSensor  *sensor;
  SbBool shouldTraverseShaderObjects;
  void updateProgramAndPreviousChildren();
  void removeFromPreviousChildren(SoNode *shader);
};

#define SELF this->pimpl

// *************************************************************************

SO_NODE_SOURCE(SoShaderProgram);

// *************************************************************************

void SoShaderProgram::initClass()
{
  SO_NODE_INIT_CLASS(SoShaderProgram, SoGroup, "Group");
  SO_ENABLE(SoGLRenderAction, SoGLShaderProgramElement);
}

SoShaderProgram::SoShaderProgram()
{
  SO_NODE_CONSTRUCTOR(SoShaderProgram);

  SO_NODE_ADD_FIELD(shaderObject, (NULL)); shaderObject.deleteValues(0,1);

  SELF = new SoShaderProgramP(this);
}

SoShaderProgram::~SoShaderProgram()
{
  delete SELF;
}

void SoShaderProgram::GLRender(SoGLRenderAction *action)
{
  SELF->GLRender(action);
}

void SoShaderProgram::updateStateMatrixParameters(SoState *state)
{
  SELF->updateStateMatrixParameters(state);
}

SbBool SoShaderProgram::containStateMatrixParameters() const
{
  return SELF->doesContainStateMatrixParameters;
}

/* ***************************************************************************
 *
 * ***************************************************************************/

SoShaderProgramP::SoShaderProgramP(SoShaderProgram * ownerptr)
{
  this->owner  = ownerptr;
  this->sensor = new SoNodeSensor(SoShaderProgramP::sensorCB, this);
  this->sensor->attach(ownerptr);

  this->shouldTraverseShaderObjects      = TRUE;
  this->doesContainStateMatrixParameters = FALSE;
}

SoShaderProgramP::~SoShaderProgramP()
{
  delete this->sensor;
}

void SoShaderProgramP::GLRender(SoGLRenderAction *action)
{
  SoState *state = action->getState();

  // FIXME: (Martin 2004-09-21) find an alternative to invalidating the cache
  SoCacheElement::invalidate(state);

  SoGLShaderProgram* oldProgram = SoGLShaderProgramElement::get(state);
  if (oldProgram) oldProgram->disable();
  
  SoGLShaderProgramElement::set(state, this->owner, &this->glShaderProgram);

  int cnt1 = this->owner->shaderObject.getNum();
  int cnt2 = this->owner->getNumChildren();
  int i;
  
  // load shader objects
  if (this->shouldTraverseShaderObjects) {
    for (i=0; i<cnt1; i++) {
      SoNode *node = this->owner->shaderObject[i];
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
	removeFromPreviousChildren(node);
	((SoShaderObject *)node)->GLRender(action);
      }
    }
    for (i=0; i<cnt2; i++) {
      SoNode *node = this->owner->getChild(i);
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
	removeFromPreviousChildren(node);
	((SoShaderObject *)node)->GLRender(action);
      }
    }
    updateProgramAndPreviousChildren();
  }

  // enable shader
  this->glShaderProgram.enable();

  // update shader object parameters
  if (this->shouldTraverseShaderObjects) {
    SbBool flag = FALSE;
    
    for (i=0; i<cnt1; i++) {
      SoShaderObject *node = (SoShaderObject *)this->owner->shaderObject[i];
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
	node->updateAllParameters();
	if (node->containStateMatrixParameters()) flag = TRUE;
      }
    }
    for (i=0; i<cnt2; i++) {
      SoShaderObject *node = (SoShaderObject *)this->owner->getChild(i);
      if (node->isOfType(SoShaderObject::getClassTypeId())) {
	node->updateAllParameters();
	if (node->containStateMatrixParameters()) flag = TRUE;
      }
    }
    this->doesContainStateMatrixParameters = flag;
  }
  else if (this->doesContainStateMatrixParameters)
    this->updateStateMatrixParameters(state);
  
  this->shouldTraverseShaderObjects = FALSE;
  SoGLTextureEnabledElement::set(state, TRUE);
}

void SoShaderProgramP::updateStateMatrixParameters(SoState*)
{
  int cnt1 = this->owner->shaderObject.getNum();
  int cnt2 = this->owner->getNumChildren();
  int i;
  
  for (i=0; i<cnt1; i++) {
    SoNode *node = this->owner->shaderObject[i];
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject*)node)->updateStateMatrixParameters();
    }
  }
  for (i=0; i<cnt2; i++) {
    SoNode *node = this->owner->getChild(i);
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject*)node)->updateStateMatrixParameters();
    }
  }
}
void SoShaderProgramP::updateProgramAndPreviousChildren()
{
  int i, cnt1, cnt2 = this->previousChildren.getLength();
  
  for (i=cnt2-1; i>=0; i--) {
    SoShaderObject *node = (SoShaderObject*)this->previousChildren[i];
    node->removeGLShaderFromGLProgram(&this->glShaderProgram);
    this->previousChildren.remove(i);
  }
  assert(this->previousChildren.getLength() == 0);
  cnt1 = this->owner->shaderObject.getNum();
  cnt2 = this->owner->getNumChildren();
  for (i=0; i<cnt1; i++)
    this->previousChildren.append(this->owner->shaderObject[i]);
  for (i=0; i<cnt2; i++)
    this->previousChildren.append(this->owner->getChild(i));
  this->previousChildren.truncate(this->previousChildren.getLength());
}

void SoShaderProgramP::removeFromPreviousChildren(SoNode *shader)
{
  if (this->previousChildren.getLength() == 0) return;

  int idx = this->previousChildren.find(shader);
    
  if (idx >= 0) this->previousChildren.remove(idx);
}

void SoShaderProgramP::sensorCB(void *data, SoSensor*)
{
  ((SoShaderProgramP *)data)->shouldTraverseShaderObjects = TRUE;
}
