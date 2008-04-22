/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoShaderProgram SoShaderProgram.h Inventor/nodes/SoShaderProgram.h
  \brief The SoShaderProgram class is used to specify a set of vertex/geometry/fragment objects.

  This node can store one of each of SoVertexShader, SoGeometryShader
  and SoFragmentShader in its shaderObject field. Coin will load all
  shader objects specified there, and attach all objects into a
  program before binding it as the current shader program.

  See \link coin_shaders Shaders in Coin \endlink for more information
  on how to set up a scene graph with shaders.

  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    ShaderProgram {
      shaderObject []
    }
  \endcode

  \sa SoShaderObject
  \sa SoShaderProgram
  \since Coin 2.5
*/

/*!
  \var SoMFNode SoShaderProgram::shaderObject

  The shader objects.

*/

#include <Inventor/nodes/SoShaderProgram.h>

#include <assert.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/nodes/SoShaderObject.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include "nodes/SoSubNodeP.h"
#include "shaders/SoGLShaderProgram.h"

// *************************************************************************

class SoShaderProgramP
{
public:
  SoShaderProgramP(SoShaderProgram * ownerptr);
  ~SoShaderProgramP();

  void GLRender(SoGLRenderAction * action);

  SoShaderProgramEnableCB * enablecb;
  void * enablecbclosure;

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

// doc from parent
void
SoShaderProgram::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShaderProgram,
                              SO_FROM_COIN_2_5|SO_FROM_INVENTOR_5_0);

  SO_ENABLE(SoGLRenderAction, SoGLShaderProgramElement);
}

/*!
  Constructor.
*/
SoShaderProgram::SoShaderProgram(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShaderProgram);

  SO_NODE_ADD_FIELD(shaderObject, (NULL));
  this->shaderObject.setNum(0);
  this->shaderObject.setDefault(TRUE);


  PRIVATE(this) = new SoShaderProgramP(this);
  PRIVATE(this)->enablecb = NULL;
  PRIVATE(this)->enablecbclosure = NULL;
}

/*!
  Destructor.
*/
SoShaderProgram::~SoShaderProgram()
{
  delete PRIVATE(this);
}

// doc from parent
void
SoShaderProgram::GLRender(SoGLRenderAction * action)
{
  PRIVATE(this)->GLRender(action);
}

// doc from parent
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
      SoNodeProfiling profiling;
      profiling.preTraversal(action);
      node->search(action);
      profiling.postTraversal(action);
      action->popCurPath();
      if (action->isFound()) return;
    }
  }
#endif // disabled
}

/*!
  Adds a callback which is called every time this program is enabled/disabled.
*/
void
SoShaderProgram::setEnableCallback(SoShaderProgramEnableCB * cb,
                                   void * closure)
{
  PRIVATE(this)->enablecb = cb;
  PRIVATE(this)->enablecbclosure = closure;
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

  this->glShaderProgram.removeShaderObjects();
  this->glShaderProgram.setEnableCallback(this->enablecb,
                                          this->enablecbclosure);

  SoGLShaderProgramElement::set(state, PUBLIC(this), &this->glShaderProgram);

  int i, cnt = PUBLIC(this)->shaderObject.getNum();

  // load shader objects
  for (i = 0; i <cnt; i++) {
    SoNode * node = PUBLIC(this)->shaderObject[i];
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject *)node)->GLRender(action);
    }
  }

  // enable shader after all shader objects have been loaded
  SoGLShaderProgramElement::enable(state, TRUE);

  // update parameters after all shader objects have been added and enabled

  for (i = 0; i <cnt; i++) {
    SoNode * node = PUBLIC(this)->shaderObject[i];
    if (node->isOfType(SoShaderObject::getClassTypeId())) {
      ((SoShaderObject *)node)->updateParameters(state);
    }
  }
}

void
SoShaderProgramP::sensorCB(void * data, SoSensor *)
{
  // nothing to do now
}


#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoShaderProgram * node = new SoShaderProgram;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
