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
#include <Inventor/actions/SoSearchAction.h>
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

private:
  SoShaderProgram * owner;
  SoGLShaderProgram glShaderProgram;

  static void sensorCB(void * data, SoSensor *);
  SoNodeSensor * sensor;
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

SoShaderProgram::SoShaderProgram(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShaderProgram);

  SO_NODE_ADD_FIELD(shaderObject, (NULL));
  this->shaderObject.setNum(0);
  this->shaderObject.setDefault(TRUE);

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
SoShaderProgram::search(SoSearchAction * action)
{
  // Include this node in the search.
  SoNode::search(action);
  if (action->isFound()) return;

  // we really can't do this since this node hasn't got an SoChildList
  // instance
#if 0 // disabled, not possible to search under this node
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    // FIXME: not implemented -- 20050129 martin
  }
  else { // traverse all shader objects
    int num = this->shaderObject.getNum();
    for (int i=0; i<num; i++) {
      SoNode * node = this->shaderObject[i];
      action->pushCurPath(i, node);
      node->search(action);
      action->popCurPath();
      if (action->isFound()) return;
    }
  }
#endif // disabled
}

// *************************************************************************

SoShaderProgramP::SoShaderProgramP(SoShaderProgram * ownerptr)
{
  PUBLIC(this) = ownerptr;
  this->sensor = new SoNodeSensor(SoShaderProgramP::sensorCB, this);
  this->sensor->attach(ownerptr);
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

  const uint32_t cachecontext = SoGLCacheContextElement::get(state);
  const cc_glglue * glctx = cc_glglue_instance(cachecontext);

  this->glShaderProgram.removeShaderObjects();
  SoGLShaderProgramElement::set(state, PUBLIC(this), &this->glShaderProgram);

  int i, cnt = PUBLIC(this)->shaderObject.getNum();

  // load shader objects
  for (i = 0; i <cnt; i++) {
    SoNode * node = PUBLIC(this)->shaderObject[i];
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject *)node)->GLRender(action);
    }
  }
  // update parameters after all shader objects have been added and enabled
  this->glShaderProgram.enable(glctx);

  for (i = 0; i <cnt; i++) {
    SoNode * node = PUBLIC(this)->shaderObject[i];
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject *)node)->updateParameters(cachecontext);
    }
  }

  SoGLTextureEnabledElement::set(state, TRUE);
}

void
SoShaderProgramP::sensorCB(void * data, SoSensor *)
{
  // nothing to do now
}
