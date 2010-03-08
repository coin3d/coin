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
  \class SoCallback SoCallback.h Inventor/nodes/SoCallback.h
  \brief The SoCallback class is a node type which provides a means of setting callback hooks in the scene graph.
  \ingroup nodes

  By inserting SoCallback nodes in a scene graph, the application
  programmer can set up functions to be executed at certain points in
  the traversal.

  The callback function will be executed during traversal of \e any
  action, so check the type of the \a action argument of the callback
  function if you only want to run your code at specific actions.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Callback {
    }
  \endcode
*/

// *************************************************************************

/*! \file SoCallback.h */
#include <Inventor/nodes/SoCallback.h>

#include <Inventor/actions/SoActions.h> // SoCallback uses all of them.

#include "nodes/SoSubNodeP.h"

// *************************************************************************

/*!
  \typedef void SoCallbackCB(void * userdata, SoAction * action)
  Signature that callback functions need to have.
*/

// *************************************************************************

SO_NODE_SOURCE(SoCallback);

// *************************************************************************

/*!
  Constructor.
*/
SoCallback::SoCallback(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCallback);

  this->cbfunc = NULL;
}

/*!
  Destructor.
*/
SoCallback::~SoCallback()
{
}

// Doc from superclass.
void
SoCallback::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCallback, SO_FROM_INVENTOR_1);
}

/*!
  Set up the \a function to call at traversal of this node. \a
  userdata will be passed back as the first argument of the callback
  \a function. Setting \a function to NULL removes the previously set 
  callback function.

  If you want a callback only for a specific action, you must (in your
  callback function) remember to check which action invoked the
  callback, for instance like this:

  \code
  
  void mycallback(void * userdata, SoAction * action)
  {
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
      // do stuff specific for GL rendering here.
    }
  }

  \endcode

  Please note that SoCallback nodes under a Separator might be
  included in a cache. Cached nodes are not traversed, and you'll not
  receive any callbacks. If you want to make sure that your callback
  is called every time the scene graph is rendered, you must
  invalidate the render cache in your callback:

  \code
  
  void mycallback(void * userdata, SoAction * action)
  {
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
      SoCacheElement::invalidate(action->getState());
    }
  }

  \endcode

  If you want to invalidate all caches (for instance also the bounding
  box cache), you can do this in your callback:

  \code
  
  void mycallback(void * userdata, SoAction * action)
  {
    SoState * state = action->getState();
    if (state->isElementEnabled(SoCacheElement::getClassStackIndex())) {
      SoCacheElement::invalidate(state);
    }
  }

  \endcode
*/
void
SoCallback::setCallback(SoCallbackCB * function, void * userdata)
{
  this->cbfunc = function;
  this->cbdata = userdata;
}

// Doc from superclass.
void
SoCallback::doAction(SoAction * action)
{
  if (this->cbfunc) this->cbfunc(this->cbdata, action);
}

// Doc from superclass.
void
SoCallback::callback(SoCallbackAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::GLRender(SoGLRenderAction * action)
{
  // FIXME: we've had a user report which indicates that SGI Inventor
  // may also cache OpenGL calls made from within a callback into
  // renderlists. Investigate, and consider whether or not we should
  // follow suit. 20051110 mortene.

  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::getMatrix(SoGetMatrixAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::handleEvent(SoHandleEventAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::pick(SoPickAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::search(SoSearchAction * action)
{
  SoCallback::doAction(action);
  SoNode::search(action);
}

// Doc from superclass.
void
SoCallback::write(SoWriteAction * action)
{
  SoCallback::doAction(action);
  SoNode::write(action);
}

// Doc from superclass.
void
SoCallback::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass. Overridden from parent class to also copy the
// callback function pointer and userdata.
void
SoCallback::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
  inherited::copyContents(from, copyconnections);

  SoCallback * fromnode = (SoCallback *)from;
  this->cbfunc = fromnode->cbfunc;
  this->cbdata = fromnode->cbdata;
}
