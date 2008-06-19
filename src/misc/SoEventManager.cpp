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

#include <Inventor/SoEventManager.h>

#include <vector>
#include <algorithm>

#include <Inventor/SbViewportRegion.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoViewerNavigationMode.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/navigation/SoNavigationSystem.h>
#include <Inventor/misc/SoState.h>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>

class SoEventManagerP {
public:
  SoEventManagerP(void) { }
  ~SoEventManagerP() {}
  SoNavigationSystem * searchForNavigationMode(SoNode * root, 
                                               SoNavigationSystem * defsys);

  SoEventManager::NavigationState navigationstate;
  SoNavigationSystem * navigationsystem;
  SoHandleEventAction * handleeventaction;
  SoSearchAction * searchaction;
  SbBool deletehandleeventaction;
  SoCamera * camera;
  SoNode * scene;

  std::vector<SoScXMLStateMachine *> statemachines;
};

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

SoEventManager::SoEventManager(void)
{
  PRIVATE(this)->navigationsystem = SoNavigationSystem::createByName(SO_EXAMINER_SYSTEM);
  PRIVATE(this)->navigationstate = SoEventManager::NO_NAVIGATION;
  PRIVATE(this)->handleeventaction = new SoHandleEventAction(SbViewportRegion(400, 400));
  PRIVATE(this)->deletehandleeventaction = TRUE;
  PRIVATE(this)->searchaction = new SoSearchAction;

  PRIVATE(this)->camera = NULL;
  PRIVATE(this)->scene = NULL;

#if 0
  ScXMLStateMachine * sm = ScXML::readFile("coin:scxml/navigation/examiner.xml");
  if (sm && sm->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    sm->initialize();
    this->addSoScXMLStateMachine(static_cast<SoScXMLStateMachine *>(sm));
  } else {
    SoDebugError::post("SoEventManager",
                       "failed to created default camera control state machine");
  }
#endif
}

SoEventManager::~SoEventManager()
{
  delete PRIVATE(this)->searchaction;

  if (PRIVATE(this)->navigationsystem) {
    delete PRIVATE(this)->navigationsystem;
    PRIVATE(this)->navigationsystem = NULL;
  }
  if (PRIVATE(this)->deletehandleeventaction) {
    delete PRIVATE(this)->handleeventaction;
    PRIVATE(this)->handleeventaction = NULL;
  }
  for (int c = this->getNumSoScXMLStateMachines() - 1; c >= 0; --c) {
    SoScXMLStateMachine * sm = this->getSoScXMLStateMachine(c);
    this->removeSoScXMLStateMachine(sm);
    delete sm;
  }
}

/*!
  This method sets the navigation mode of the scene manager.  The navigation
  mode decides the system the user uses to navigate the 3D model.

  SoEventManager::NO_NAVIGATION is the default setting.

  \sa SoEventManager::NavigationState, getNavigationState
*/

void
SoEventManager::setNavigationState(NavigationState state)
{
  PRIVATE(this)->navigationstate = state;
}

/*!
  This method returns which state the camera navigation system is in.

  \sa SoEventManager::NavigationState, setNavigationState
*/
SoEventManager::NavigationState
SoEventManager::getNavigationState(void) const
{
  return PRIVATE(this)->navigationstate;
}

/*!
  This method sets the navigation system to use.

  \param system The SoEventManager keeps its own copy of the system,
  remember to delete your own.

  \sa SoNavigationSystem, getNavigationSystem
*/

void
SoEventManager::setNavigationSystem(SoNavigationSystem * system)
{
  if (PRIVATE(this)->navigationsystem) {
    PRIVATE(this)->navigationsystem->setCamera(NULL);
    PRIVATE(this)->navigationsystem->setSceneGraph(NULL);
    //NOTE: Need to delete this, as we may hold the navigationsystem
    //from our constructor, which is the only one of its kind.
    delete PRIVATE(this)->navigationsystem;
  }

  if (!system) {
    PRIVATE(this)->navigationsystem = NULL; // SoNavigationSystem::getByName(SO_IDLER_SYSTEM);
    return;
  }

  //Keep a unique copy.
  PRIVATE(this)->navigationsystem = system->clone();
  PRIVATE(this)->navigationsystem->setSceneGraph(PRIVATE(this)->scene);
  PRIVATE(this)->navigationsystem->setCamera(PRIVATE(this)->camera);
  PRIVATE(this)->navigationsystem->setViewport(PRIVATE(this)->handleeventaction->getViewportRegion());
  PRIVATE(this)->navigationsystem->invokeModeChangeCallbacks();
}

/*!
  This method returns the current navigation system in use.
  NULL means that no navigation system is in use.

  \sa SoNavigationSystem, setNavigationSystem
*/

SoNavigationSystem *
SoEventManager::getNavigationSystem(void) const
{
  return PRIVATE(this)->navigationsystem;
}

/*!
  Set the node which is top of the scene graph we're managing.  The \a
  sceneroot node reference count will be increased by 1, and any
  previously set scene graph top node will have it's reference count
  decreased by 1.

  \sa getSceneGraph()
*/
void
SoEventManager::setSceneGraph(SoNode * const sceneroot)
{
  // Don't unref() until after we've set up the new root, in case the
  // old root == the new sceneroot. (Just to be that bit more robust.)
  SoNode * oldroot = PRIVATE(this)->scene;
  
  PRIVATE(this)->scene = sceneroot;

  if (PRIVATE(this)->scene) {
    PRIVATE(this)->scene->ref();
   
    // set up navigation mode if scene graph contains a navigation
    // mode node.
    SoNavigationSystem * navsys = 
      PRIVATE(this)->searchForNavigationMode(sceneroot, PRIVATE(this)->navigationsystem);

    if (navsys) {
      this->setNavigationSystem(navsys);
    }
  }
  
  if (oldroot) oldroot->unref();
  
  if (PRIVATE(this)->navigationsystem) {
    PRIVATE(this)->navigationsystem->setSceneGraph(PRIVATE(this)->scene);
    PRIVATE(this)->navigationsystem->setCamera(PRIVATE(this)->camera);
  }

  for (int c = 0; c < this->getNumSoScXMLStateMachines(); ++c) {
    SoScXMLStateMachine * sm = this->getSoScXMLStateMachine(c);
    sm->setSceneGraphRoot(PRIVATE(this)->scene);
    sm->setActiveCamera(PRIVATE(this)->camera);
  }
}

/*!
  Returns pointer to root of scene graph.
 */
SoNode *
SoEventManager::getSceneGraph(void) const
{
  return PRIVATE(this)->scene;
}

/*!  
  Sets the camera to be used.
*/
void 
SoEventManager::setCamera(SoCamera * camera)
{
  if (PRIVATE(this)->camera) {
    PRIVATE(this)->camera->unref();
  }
  PRIVATE(this)->camera = camera;
  if (camera) camera->ref();

  if (PRIVATE(this)->navigationsystem) {
    PRIVATE(this)->navigationsystem->setCamera(PRIVATE(this)->camera);
  }

  for (int i = this->getNumSoScXMLStateMachines() - 1; i >= 0; --i) {
    SoScXMLStateMachine * sm = this->getSoScXMLStateMachine(i);
    sm->setActiveCamera(PRIVATE(this)->camera);
  }
}

/*!
  Returns the current camera.
*/
SoCamera * 
SoEventManager::getCamera(void) const
{
  return PRIVATE(this)->camera;
}


/*!
  Set size of rendering area for the viewport within the current
  window.
*/
void
SoEventManager::setSize(const SbVec2s & newsize)
{
  SbViewportRegion region = PRIVATE(this)->handleeventaction->getViewportRegion();
  SbVec2s origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  PRIVATE(this)->handleeventaction->setViewportRegion(region);
}

/*!
  Set \e only the origin of the viewport region within the rendering
  window.

  \sa setViewportRegion(), setWindowSize()
*/
void
SoEventManager::setOrigin(const SbVec2s & newOrigin)
{
  SbViewportRegion region = PRIVATE(this)->handleeventaction->getViewportRegion();
  SbVec2s size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  PRIVATE(this)->handleeventaction->setViewportRegion(region);
}

void 
SoEventManager::setViewportRegion(const SbViewportRegion & newregion) 
{
  PRIVATE(this)->handleeventaction->setViewportRegion(newregion);
}

SbBool 
SoEventManager::processEvent(const SoEvent * const event)
{
  const SbViewportRegion & vp =
    PRIVATE(this)->handleeventaction->getViewportRegion();

  SbBool status = FALSE;

  int i = 0;
  switch (PRIVATE(this)->navigationstate) { 
  case SoEventManager::NO_NAVIGATION:
    status = this->actuallyProcessEvent(event);
    break;
  case SoEventManager::JUST_NAVIGATION:
    if (PRIVATE(this)->navigationsystem) {
      PRIVATE(this)->navigationsystem->setViewport(vp); 
      if (PRIVATE(this)->navigationsystem->processEvent(event))
        status = TRUE;
    }
    for (i = this->getNumSoScXMLStateMachines() - 1; i >= 0; --i) {
      SoScXMLStateMachine * sm = this->getSoScXMLStateMachine(i);
      if (sm->isActive()) {
        sm->setViewportRegion(vp);
        if (sm->processSoEvent(event))
          status = TRUE;
      }
    }
    break;
  case SoEventManager::MIXED_NAVIGATION:
    if (this->actuallyProcessEvent(event)) {
      status = TRUE;
      break;
    }
    if (PRIVATE(this)->navigationsystem) {
      PRIVATE(this)->navigationsystem->setViewport(vp);
      if (PRIVATE(this)->navigationsystem->processEvent(event))
        status = TRUE;
    }
    for (i = this->getNumSoScXMLStateMachines() - 1; i >= 0; --i) {
      SoScXMLStateMachine * sm = this->getSoScXMLStateMachine(i);
      if (sm->isActive()) {
        sm->setViewportRegion(vp);
        if (sm->processSoEvent(event))
          status = TRUE;
      }
    }
  }
  return status;
}

SbBool 
SoEventManager::actuallyProcessEvent(const SoEvent * const event)
{
  assert(PRIVATE(this)->handleeventaction);
  
  SbBool handled = FALSE;
  if ( PRIVATE(this)->handleeventaction->getState() != NULL &&
       PRIVATE(this)->handleeventaction->getState()->getDepth() != 0 ) {
    // recursive invocation - action currently in use
#if COIN_DEBUG
    SoDebugError::post("SoEventManager::processEvent",
                       "Recursive invocation detected. Delay processing event "
                       "until the current event is finished processing.");
#endif // COIN_DEBUG
  } else if ( PRIVATE(this)->scene == NULL ) {
    // nothing
  } else {
    PRIVATE(this)->handleeventaction->setEvent(event);
    PRIVATE(this)->handleeventaction->apply(PRIVATE(this)->scene);
    handled = PRIVATE(this)->handleeventaction->isHandled();
  }
  
  return handled;
}

/*!
  Set the \a action to use for event handling. Overrides the default
  action made in the constructor.
 */
void
SoEventManager::setHandleEventAction(SoHandleEventAction * handleeventaction)
{
  assert(handleeventaction && "SoEventManager::setHandleEventAction, action == NULL");

  // remember old viewport region
  SbViewportRegion region = PRIVATE(this)->handleeventaction->getViewportRegion();
  if (PRIVATE(this)->deletehandleeventaction) {
    delete PRIVATE(this)->handleeventaction;
  }
  PRIVATE(this)->deletehandleeventaction = FALSE;
  PRIVATE(this)->handleeventaction = handleeventaction;
  PRIVATE(this)->handleeventaction->setViewportRegion(region);
}

/*!
  Returns pointer to event handler action.
 */
SoHandleEventAction *
SoEventManager::getHandleEventAction(void) const
{
  return PRIVATE(this)->handleeventaction;
}

//********************************************************************************
// defsys is the default system returned if none is found in root

SoNavigationSystem *
SoEventManagerP::searchForNavigationMode(SoNode * root, 
                                         SoNavigationSystem * defsys)
{
  this->searchaction->reset();
  this->searchaction->setType(SoCamera::getClassTypeId());
  this->searchaction->setInterest(SoSearchAction::FIRST);
  this->searchaction->apply(root);
  SoPath * path = this->searchaction->getPath();
  
  if (!path) return NULL;
  
  path->ref();
  this->searchaction->reset();
  this->searchaction->setType(SoViewerNavigationMode::getClassTypeId());
  this->searchaction->setInterest(SoSearchAction::FIRST);
  this->searchaction->apply(path);
  path->unref();
  path = this->searchaction->getPath();

  if (!path) return NULL;

  path->ref();
  SoViewerNavigationMode * mode = (SoViewerNavigationMode *) path->getTail();
  assert(mode && mode->isOfType(SoViewerNavigationMode::getClassTypeId()));
  SbString modestring = mode->mode.getValue();
  path->unref();

  if (modestring.getLength() == 0) return NULL;

  return SoNavigationSystem::createByName(modestring.getString());
}

int
SoEventManager::getNumSoScXMLStateMachines(void) const
{
  return static_cast<int>(PRIVATE(this)->statemachines.size());
}

SoScXMLStateMachine *
SoEventManager::getSoScXMLStateMachine(int idx) const
{
  assert(idx >= 0 &&
         idx < static_cast<int>(PRIVATE(this)->statemachines.size()));
  return PRIVATE(this)->statemachines.at(idx);
}

void
SoEventManager::addSoScXMLStateMachine(SoScXMLStateMachine * sm)
{
  PRIVATE(this)->statemachines.push_back(sm);
}

void
SoEventManager::removeSoScXMLStateMachine(SoScXMLStateMachine * sm)
{
  std::vector<SoScXMLStateMachine *>::iterator findit =
    std::find(PRIVATE(this)->statemachines.begin(),
              PRIVATE(this)->statemachines.end(), sm);
  if (findit != PRIVATE(this)->statemachines.end()) {
    PRIVATE(this)->statemachines.erase(findit);
  }
}

#undef PRIVATE
#undef PUBLIC
