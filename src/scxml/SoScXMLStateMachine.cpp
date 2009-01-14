/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
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

#include <Inventor/scxml/SoScXMLStateMachine.h>

#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/SoScXMLEvent.h>

// *************************************************************************

/*!
  \class SoScXMLStateMachine SoScXMLStateMachine.h Inventor/scxml/SoScXMLStateMachine.h
  \brief Integration level for using Coin events with SCXML-based state machines.

  This class is part of integrating the Coin types with the SCXML
  subsystem.  SoScXMLStateMachines adds convenience for using SCXML
  state machines together with Coin events.

  \since Coin 3.0
  \ingroup scxml
*/

class SoScXMLStateMachine::PImpl {
public:
  PImpl(void)
    : scenegraphroot(NULL), activecamera(NULL), viewport(100, 100)
  { }
  ~PImpl(void) { }

  // hold a couple of custom non-SoEvent-based events
  boost::scoped_ptr<ScXMLEvent> preGLRenderEvent;
  boost::scoped_ptr<ScXMLEvent> postGLRenderEvent;

  boost::intrusive_ptr<SoNode> scenegraphroot;
  boost::intrusive_ptr<SoCamera> activecamera;
  SbViewportRegion viewport;
};

#define PRIVATE(obj) ((obj)->pimpl)

SCXML_OBJECT_SOURCE(SoScXMLStateMachine);

void
SoScXMLStateMachine::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(SoScXMLStateMachine, ScXMLStateMachine, SCXML_COIN_NS, "statemachine");
}

SoScXMLStateMachine::SoScXMLStateMachine(void)
{
  // initialize custom events
  PRIVATE(this)->preGLRenderEvent.reset(new ScXMLEvent);
  PRIVATE(this)->preGLRenderEvent->setIdentifier("sim.coin3d.coin.GLRender.PRE_RENDER");
  PRIVATE(this)->postGLRenderEvent.reset(new ScXMLEvent);
  PRIVATE(this)->postGLRenderEvent->setIdentifier("sim.coin3d.coin.GLRender.POST_RENDER");
}

SoScXMLStateMachine::~SoScXMLStateMachine(void)
{
}

void
SoScXMLStateMachine::setSceneGraphRoot(SoNode * root)
{
  PRIVATE(this)->scenegraphroot = root;
}

SoNode *
SoScXMLStateMachine::getSceneGraphRoot(void) const
{
  return PRIVATE(this)->scenegraphroot.get();
}

void
SoScXMLStateMachine::setActiveCamera(SoCamera * camera)
{
  PRIVATE(this)->activecamera = camera;
}

SoCamera *
SoScXMLStateMachine::getActiveCamera(void) const
{
  return PRIVATE(this)->activecamera.get();
}

void
SoScXMLStateMachine::setViewportRegion(const SbViewportRegion & vp)
{
  PRIVATE(this)->viewport = vp;
}

const SbViewportRegion &
SoScXMLStateMachine::getViewportRegion(void) const
{
  return PRIVATE(this)->viewport;
}

void
SoScXMLStateMachine::preGLRender(void)
{
  this->queueEvent(PRIVATE(this)->preGLRenderEvent.get(), FALSE);
  this->processEventQueue();
}

void
SoScXMLStateMachine::postGLRender(void)
{
  this->queueEvent(PRIVATE(this)->postGLRenderEvent.get(), FALSE);
  this->processEventQueue();
}

/*!
  This function makes the state machine process an SoEvent.
*/
SbBool
SoScXMLStateMachine::processSoEvent(const SoEvent * event)
{
  // FIXME: Not sure if this check should be here and not somewhere else,
  // but removing this again makes us crash on NULL scenegraphs. kintel 20080729.
  if (PRIVATE(this)->scenegraphroot.get()) {
    boost::scoped_ptr<SoScXMLEvent> wrapperevent;
    wrapperevent.reset(new SoScXMLEvent);
    wrapperevent->setSoEvent(event);
    wrapperevent->setUpIdentifier();
    this->queueEvent(wrapperevent.get(), FALSE);
    return this->processEventQueue();
  }
  else {
    return false;
  }
}

#undef PRIVATE
