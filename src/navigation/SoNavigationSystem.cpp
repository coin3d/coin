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

#include <Inventor/navigation/SoNavigationSystem.h>

#include <assert.h>
#include <stdio.h>

#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/navigation/SoNavigationState.h>
#include <Inventor/navigation/SoNavigationControl.h>
#include <Inventor/navigation/SoNavigationMode.h>
#include <Inventor/navigation/SoIdleMode.h>
#include <Inventor/navigation/SoRotateMode.h>
#include <Inventor/navigation/SoZoomMode.h>
#include <Inventor/navigation/SoPanMode.h>
#include <Inventor/navigation/SoPickMode.h>
#include <Inventor/navigation/SoPitchMode.h>
#include <Inventor/navigation/SoRollMode.h>
#include <Inventor/navigation/SoYawMode.h>
#include <Inventor/navigation/SoCenterMode.h>
#include <Inventor/navigation/SoOrthoCenterMode.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "misc/SbHash.h"

#include "navigation/SoEvent.h"

// *************************************************************************

/*!
  \page navigation Navigation

  The SoSceneManager class implements a navigation system that allows
  for user interaction without having to deploy a full GUI toolkit
  viewer.

  This is done by attaching an object of type SoNavigationSystem to
  the SoSceneManager that will listen to all events sent to the scene
  and perform user-based camera navigation based on those events.

  A set of builtin navigation systems can be used for certain common
  navigation setups, but it is also possible to create ones own navigation
  modes, or  patch together a new one from existing navigation submodes.

  Builtin modes are named in the following set of defines:
  \li SO_IDLER_SYSTEM
  \li SO_EXAMINER_SYSTEM
  \li SO_PANNER_SYSTEM
  \li SO_ZOOMER_SYSTEM
  \li SO_ROTATER_SYSTEM

  The SO_EXAMINER_SYSTEM is a navigation system that implements most of
  the examiner viewer navigation.  Not complete yet, but that is the
  goal.

  The SO_PANNER_SYSTEM just implements panning when the mouse button
  is pressed.  Similarly, the SO_ZOOMER_SYSTEM and SO_ROTATER_SYSTEM
  implements just zooming and rotating when the mouse is pressed.
*/

/*
  FIXME: there is never sent a mode change callback for the initial
  mode.  Maybe the first event to be sent to processEvent should
  invoke one?
*/

/*!
  \class SoNavigationSystem Inventor/navigation/SoNavigationSystem.h
  \brief Class for managing user-based viewer navigation.

  Container class for navigation systems.

  \ingroup navigation
*/

/*!
  \enum SoNavigationSystem::TransitionType

  This enum defines the types of mode transitions supported by the
  SoNavigationSystem class.
*/

/*!
  \enum SoNavigationSystem::INITIAL

  This transition is used to set the default/initial submode of the
  navigation system.
*/

/*!
  \enum SoNavigationSystem::STACK

  This mode transition means that the target mode of the transition is
  stacked on top of the current mode, and this mode will be returned
  to when the target mode is done.
*/

/*!
  \enum SoNavigationSystem::SWITCH

  This mode transition means that the target mode will replace the
  current mode in the mode stack so the current mode is not the mode
  returned to when the new mode finishes.
*/

/*!
  \enum SoNavigationSystem::FINISH

  This mode transition will make the system return to the previous
  mode - the mode under it on the mode stack.
*/

/*!
  \enum SoNavigationSystem::ABORT

  This mode transition will abort the current mode and make the system
  return to the previous mode - the mode under it on the mode stack.
*/

/*!
  \typedef void SoNavigationModeChangeCB(void * closure, SoNavigationSystem * system)

  This function type defines the signature the SoNavigationSystem mode
  change callbacks must use.

  \relates SoNavigationSystem

  \sa SoNavigationSystem::addModeChangeCallback, SoNavigationSystem::removeModeChangeCallback
*/

// *************************************************************************
// private type declarations

struct SoNavigationModeTransition {
  SoNavigationSystem::TransitionType type;
  SoNavigationMode * mode1;
  SoNavigationMode * mode2;
  const SoEvent * trigger;
  const SoEvent * condition;
};

class SoNavigationSystemP {
public:
  SoNavigationSystemP(SoNavigationSystem * api);
  ~SoNavigationSystemP(void);

  SbName name;
  SoNavigationControl * ctrl;
  SoNavigationState * state;
  SbPList * modechangecbs;
  SbList<SoNavigationMode *> * modes;
  SbList<SoNavigationModeTransition *> * transitions;

  typedef SbHash<SoNavigationSystem *, uintptr_t> NameDict;
  static NameDict * namedict;

  static void initBuiltinSystems(void);
  static void initExaminerSystem(void);
  static void initPannerSystem(void);
  static void initZoomerSystem(void);
  static void initRotaterSystem(void);
  static void initCentererSystem(void);
  static void initPickerSystem(void);
  static void initIdleSystem(void);
  static void initPlaneViewerSystem(void);

};

SoNavigationSystemP::NameDict * SoNavigationSystemP::namedict = NULL;

// *************************************************************************

/*!
  Initializes the builtin navigation systems.

  \sa SoNavigationSystem::cleanClass
*/

void
SoNavigationSystem::initClass(void)
{
  SoNavigationSystemP::namedict = new SoNavigationSystemP::NameDict;
  SoNavigationSystemP::initBuiltinSystems();
  cc_coin_atexit_static_internal((coin_atexit_f*) cleanClass);
}

/*!
  Thisfunction cleans up after the SoNavigationSystem class.

  \sa SoNavigationSystem::initClass
*/

void
SoNavigationSystem::cleanClass(void)
{
#define DELETE_SYSTEM(systemname) \
  do { \
    SoNavigationSystem * system = \
      SoNavigationSystem::getByName(systemname); \
    SoNavigationSystem::unregisterSystem(system); \
    delete system; \
  } while ( FALSE )

  DELETE_SYSTEM(SO_IDLER_SYSTEM);
  DELETE_SYSTEM(SO_EXAMINER_SYSTEM);
  DELETE_SYSTEM(SO_ZOOMER_SYSTEM);
  DELETE_SYSTEM(SO_ROTATER_SYSTEM);
  DELETE_SYSTEM(SO_PANNER_SYSTEM);
  DELETE_SYSTEM(SO_CENTERER_SYSTEM);
  DELETE_SYSTEM(SO_PICKER_SYSTEM);
  DELETE_SYSTEM(SO_PLANEVIEWER_SYSTEM);

  delete SoNavigationSystemP::namedict;
  SoNavigationSystemP::namedict = NULL;
}

/*!
  This function registers a navigation system object so it can later
  be fetched by name through SoNavigationSystem::getByName().

  Registered navigation systems can be enabled for an SoSceneManager
  through use of an SoViewerNavigationMode node in the scene graph.

  \sa SoNavigationSystem::unregisterSystem, SoNavigationSystem::getByName
*/

SbBool
SoNavigationSystem::registerSystem(SoNavigationSystem * system)
{
  assert(SoNavigationSystemP::namedict);
  SbName name = system->getName();
  if (SoNavigationSystem::getByName(name) != NULL) {
    return FALSE;
  }
  SoNavigationSystemP::namedict->put((uintptr_t) name.getString(), system);
  return TRUE;
}

/*!
  This function unregisters a navigation system from the dictionary.

  \sa SoNavigationSystem::registerSystem, SoNavigationSystem::getByName
*/

SbBool
SoNavigationSystem::unregisterSystem(SoNavigationSystem * system)
{
  assert(SoNavigationSystemP::namedict);
  SbName name = system->getName();
  if (SoNavigationSystem::getByName(name) == NULL) {
    return FALSE;
  }
  SoNavigationSystemP::namedict->remove((uintptr_t) name.getString());
  return TRUE;
}

/*!
  This function returns whether or not this particular navigation system
  instance is in the register.
*/

SbBool
SoNavigationSystem::isRegistered(SoNavigationSystem * system)
{
  assert(SoNavigationSystemP::namedict);
  SbName name = system->getName();
  SoNavigationSystem * registered = SoNavigationSystem::getByName(name);
  return (registered == system) ? TRUE : FALSE;
}

/*!
  This function returns the navigation system that has been registered
  under the given name.  NULL is returned if no such system has been
  registered.

  \sa SoNavigationSystem::registerSystem, SoNavigationSystem::unregisterSystem
*/

SoNavigationSystem *
SoNavigationSystem::getByName(SbName name)
{
  assert(SoNavigationSystemP::namedict);
  SoNavigationSystem * navsys = NULL;
  SoNavigationSystemP::namedict->get((uintptr_t) name.getString(), navsys);
  return navsys;
}

/*!
  This function returns a distinct copy of the navigation system defined
  by the given name, or NULL if no such navigation system has been
*/

SoNavigationSystem *
SoNavigationSystem::createByName(SbName name)
{
  SoNavigationSystem * orig = SoNavigationSystem::getByName(name);
  if (!orig) return NULL;
  return orig->clone();
}

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.  The \a name argument is the name of the system, the
  name it will be registered under with
  SoSceneManager::registerSystem().
*/

SoNavigationSystem::SoNavigationSystem(SbName name)
{
  PRIVATE(this) = new SoNavigationSystemP(this);
  PRIVATE(this)->name = SbName(name);
}

/*!
  Destructor.
*/

SoNavigationSystem::~SoNavigationSystem(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  This method creates a clone of this navigation system.  Only the mode
  network setup is copied - callback lists are not.
*/

SoNavigationSystem *
SoNavigationSystem::clone(void) const
{
  SoNavigationSystem * clone = new SoNavigationSystem(this->getName());
  int i;
  const int nummodes = PRIVATE(this)->modes->getLength();
  for ( i = 0; i < nummodes; i++ ) {
    PRIVATE(clone)->modes->append((*(PRIVATE(this)->modes))[i]->clone());
  }
  const int numtransitions = PRIVATE(this)->transitions->getLength();
  for ( i = 0; i < numtransitions; i++ ) {
    SoNavigationModeTransition * origt = (*(PRIVATE(this)->transitions))[i];
    SoNavigationModeTransition * tcopy = new SoNavigationModeTransition;
    tcopy->type = origt->type;
    tcopy->mode1 = NULL;
    if (origt->mode1) {
      int idx = PRIVATE(this)->modes->find(origt->mode1);
      assert(idx != -1);
      tcopy->mode1 = (*(PRIVATE(clone)->modes))[idx];
    }
    tcopy->mode2 = NULL;
    if (origt->mode2) {
      int idx = PRIVATE(this)->modes->find(origt->mode2);
      assert(idx != -1);
      tcopy->mode2 = (*(PRIVATE(clone)->modes))[idx];
    }
    tcopy->trigger = NULL;
    if (origt->trigger) {
      tcopy->trigger = SoEvent_Clone(origt->trigger);
    }
    tcopy->condition = NULL;
    if (origt->condition) {
      tcopy->condition = SoEvent_Clone(origt->condition);
    }
    PRIVATE(clone)->transitions->append(tcopy);
    if (tcopy->type == INITIAL) {
      PRIVATE(clone)->state->reset();
      PRIVATE(clone)->state->push(tcopy->mode1, NULL);
    }
  }
  return clone;
}

/*!
  Returns the name of the navigation system.
*/

SbName
SoNavigationSystem::getName(void) const
{
  return PRIVATE(this)->name;
}

void
SoNavigationSystem::setSceneGraph(SoNode * scenegraph)
{
  PRIVATE(this)->ctrl->setSceneGraph(scenegraph);
}

/*!
  Sets the pointer to the camera that the navigation system should
  manipulate.
*/

void
SoNavigationSystem::setCamera(SoCamera * camera)
{
  PRIVATE(this)->ctrl->setCamera(camera);
}

/*!
  Sets the viewport region information needed for interpreting mouse events
  correctly.
*/

void
SoNavigationSystem::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->ctrl->setViewport(vp);
}

/*!
  Repositions the camera so that the complete model gets in the view.
*/

void
SoNavigationSystem::viewAll(void)
{
  PRIVATE(this)->ctrl->viewAll();
}

/*!
  This method places the camera so that it has the best view
  possible of the part \a path points to, given the in-vector and
  up-vector constraints.
*/

void
SoNavigationSystem::viewPart(SoPath * path, const SbVec3f & in, const SbVec3f & up)
{
  PRIVATE(this)->ctrl->viewPart(path, in, up);
}

/*!
  This method finds the first path to \a node and invokes the
  path-based viewPart() instead.  It's just a convenience function for
  avoiding to have to find the path yourself.

  \sa viewPart
*/

void
SoNavigationSystem::viewPart(SoNode * node, const SbVec3f & in, const SbVec3f & up)
{
  SoNode * root = PRIVATE(this)->ctrl->getSceneGraph();
  if (!root) return;
  SbBool searchingchildren = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  SoSearchAction sa;
  sa.setInterest(SoSearchAction::FIRST);
  sa.setNode(node);
  sa.apply(root);
  SoBaseKit::setSearchingChildren(searchingchildren);
  if (!sa.getPath()) return;
  this->viewPart(sa.getPath(), in, up);
}

/*!
  Returns the navigation control class used by the navigation
  systems.
*/

SoNavigationControl *
SoNavigationSystem::getNavigationControl(void) const
{
  return PRIVATE(this)->ctrl;
}

/*!
  Adds a callback that will be called each time the navigation
  system switches navigation mode.  Intended usage for this is for the
  application to switch mouse cursor representation and similar
  things.

  \sa removeModeChangeCallback, getCurrentModeName
*/

void
SoNavigationSystem::addModeChangeCallback(SoNavigationModeChangeCB * cb, void * closure)
{
  if (!PRIVATE(this)->modechangecbs) {
    PRIVATE(this)->modechangecbs = new SbPList;
  }
  PRIVATE(this)->modechangecbs->append((void *) cb);
  PRIVATE(this)->modechangecbs->append(closure);
}

/*!
  Removes a callback set with addModeChangeCallback().

  \sa addModeChangeCallback
*/

void
SoNavigationSystem::removeModeChangeCallback(SoNavigationModeChangeCB * cb, void * closure)
{
  if (!PRIVATE(this)->modechangecbs) return;
  const int max = PRIVATE(this)->modechangecbs->getLength();
  assert((max % 2) == 0);
  int i;
  for (i = 0; i < max; i += 2) {
    SoNavigationModeChangeCB * cb2 =
      (SoNavigationModeChangeCB *) (*(PRIVATE(this)->modechangecbs))[i];
    void * closure2 = (*(PRIVATE(this)->modechangecbs))[i+1];
    if (cb == cb2 && closure == closure2) {
      PRIVATE(this)->modechangecbs->remove(i+1);
      PRIVATE(this)->modechangecbs->remove(i);
      return;
    }
  }
}

/*!
  Invokes all the mode change callbacks.

  \sa addModeChangeCallback, removeModeChangeCallback
*/

void
SoNavigationSystem::invokeModeChangeCallbacks(void)
{
  if (!PRIVATE(this)->modechangecbs) return;
  const int max = PRIVATE(this)->modechangecbs->getLength();
  assert((max % 2) == 0);
  int i;
  for (i = 0; i < max; i += 2) {
    SoNavigationModeChangeCB * cb =
      (SoNavigationModeChangeCB *) (*(PRIVATE(this)->modechangecbs))[i];
    void * closure = (*(PRIVATE(this)->modechangecbs))[i+1];
    cb(closure, this);
  }
}

/*!
  Processes an event.  Returns TRUE if a mode used the event, and FALSE
  if the event didn't result in anything.
*/

SbBool
SoNavigationSystem::processEvent(const SoEvent * event)
{
  assert(event);
  assert(PRIVATE(this)->state);
  assert(PRIVATE(this)->ctrl);
  SoNavigationMode * mode = PRIVATE(this)->state->getMode();
  if (mode == NULL) return FALSE;

  SbBool retval = FALSE;

  int i;
  const int max = PRIVATE(this)->transitions->getLength();
  // check if we're doing a mode transition
  for (i = 0; i < max; i++) {
    SoNavigationModeTransition * transition =
      (*(PRIVATE(this)->transitions))[i];
    if (transition->mode1 != mode) continue;
    if (transition->type == INITIAL) continue; // trigger is NULL
    if (!SoEvent_Equals(transition->trigger, event)) continue;
    if (transition->condition != NULL) {
      const SoEvent * curtrigger = PRIVATE(this)->state->getTrigger();
      if (curtrigger == NULL) {
        continue;
      }
      if (!SoEvent_Equals(transition->condition, curtrigger)) {
        // SoEvent_Dump(stderr, curtrigger);
        // SoEvent_Dump(stderr, transition->condition);
        continue;
      }
    }
    // we are doing a mode transition
    switch (transition->type) {
    case INITIAL:
      assert(0 && "crazy!");
      break;

    case FINISH:
    case ABORT:
      mode->processEvent(event, PRIVATE(this)->ctrl);
      if (transition->type == FINISH) {
        mode->finish(event, PRIVATE(this)->ctrl);
      } else {
        mode->abort(event, PRIVATE(this)->ctrl);
      }
      PRIVATE(this)->state->pop();
      mode = PRIVATE(this)->state->getMode();
      if (mode) {
        this->invokeModeChangeCallbacks();
        mode->init(event, PRIVATE(this)->ctrl);
        mode->processEvent(event, PRIVATE(this)->ctrl);
      }
      retval = TRUE; // transitions should always be considered handled events
      break;

    case STACK:
    case SWITCH:
      mode->processEvent(event, PRIVATE(this)->ctrl);
      mode->finish(event, PRIVATE(this)->ctrl);
      if (transition->type == SWITCH)
        PRIVATE(this)->state->pop();
      PRIVATE(this)->state->push(transition->mode2, event);
      mode = PRIVATE(this)->state->getMode();
      this->invokeModeChangeCallbacks();
      mode->init(event, PRIVATE(this)->ctrl);
      mode->processEvent(event, PRIVATE(this)->ctrl);
      retval = TRUE; // transitions should always be considered handled events
      break;
    }
    break;
  }

  // no transition - just regular event processing
  if ( mode->processEvent(event, PRIVATE(this)->ctrl) ) {
    retval = TRUE;
  }

  // check if mode aborted/closed itself
  while (mode->isAborted() || mode->isFinished()) {
    if (mode->isAborted()) {
      mode->abort(event, PRIVATE(this)->ctrl);
    } else {
      mode->finish(event, PRIVATE(this)->ctrl);
    }
    PRIVATE(this)->state->pop();
    this->invokeModeChangeCallbacks();
    mode = PRIVATE(this)->state->getMode();
    mode->init(event, PRIVATE(this)->ctrl);
    mode->processEvent(event, PRIVATE(this)->ctrl);
    retval = TRUE; // transition means handled...
  }

  return retval;
}

/*!
  Adds a submode to a navigation system.
*/

void
SoNavigationSystem::addMode(SoNavigationMode * mode)
{
  assert(PRIVATE(this)->modes);
  if (PRIVATE(this)->modes->find(mode) != -1) return;
  PRIVATE(this)->modes->append(mode);
}

/*!
  Adds a mode transition.
*/

void
SoNavigationSystem::addModeTransition(SoNavigationMode * mode,
                                      TransitionType type,
                                      const SoEvent * trigger,
                                      const SoEvent * condition)
{
  assert(PRIVATE(this)->modes);
  assert(PRIVATE(this)->modes->find(mode) != -1);
  assert(mode);
  assert(type == INITIAL || type == ABORT || type == FINISH);
  assert(trigger || type == INITIAL);

  if (type == INITIAL) {
    PRIVATE(this)->state->reset();
    PRIVATE(this)->state->push(mode, NULL);
  }

  SoNavigationModeTransition * transition = new SoNavigationModeTransition;
  transition->type = type;
  transition->mode1 = mode;
  transition->mode2 = NULL;
  transition->trigger = NULL;
  if (trigger) {
    transition->trigger = SoEvent_Clone(trigger);
  }
  transition->condition = NULL;
  if (condition) {
    transition->condition = SoEvent_Clone(condition);
  }
  PRIVATE(this)->transitions->append(transition);
}

/*!
  Adds a mode transition.
*/

void
SoNavigationSystem::addModeTransition(SoNavigationMode * mode1,
                                      SoNavigationMode * mode2,
                                      TransitionType type,
                                      const SoEvent * trigger,
                                      const SoEvent * condition)
{
  assert(PRIVATE(this)->modes);
  assert(mode1); assert(mode2);
  assert(PRIVATE(this)->modes->find(mode1) != -1);
  assert(PRIVATE(this)->modes->find(mode2) != -1);
  assert(type == STACK || type == SWITCH);
  assert(trigger);

  SoNavigationModeTransition * transition = new SoNavigationModeTransition;
  transition->type = type;
  transition->mode1 = mode1;
  transition->mode2 = mode2;
  transition->trigger = SoEvent_Clone(trigger);
  transition->condition = NULL;
  if (condition) {
    transition->condition = SoEvent_Clone(condition);
  }
  PRIVATE(this)->transitions->append(transition);
}

/*!
  Returns the name of the current submode.
*/

SbName
SoNavigationSystem::getCurrentModeName(void) const
{
  SoNavigationMode * mode = PRIVATE(this)->state->getMode();
  if (!mode) return SbName::empty();
  return mode->getModeName();
}

/*!
  Returns the submode with the given name.

  NULL is returned if no mode with the given name exists in the system.
*/

SoNavigationMode *
SoNavigationSystem::getMode(SbName name) const
{
  const int num = PRIVATE(this)->modes->getLength();
  int i;
  for ( i = 0; i < num; i++ ) {
    SoNavigationMode * mode = (*(PRIVATE(this)->modes))[i];
    if (mode->getModeName() == name) {
      return mode;
    }
  }
  return NULL;
}

/*!
  Returns the object for the current submode.
*/

SoNavigationMode *
SoNavigationSystem::getCurrentMode(void) const
{
  return PRIVATE(this)->state->getMode();
}

// *************************************************************************

#undef PRIVATE

// *************************************************************************
// private implementation

SoNavigationSystemP::SoNavigationSystemP(SoNavigationSystem * api)
{
  this->ctrl = new SoNavigationControl;
  this->state = new SoNavigationState;
  this->modechangecbs = NULL;
  this->modes = new SbList<SoNavigationMode *>;
  this->transitions = new SbList<SoNavigationModeTransition *>;
}

SoNavigationSystemP::~SoNavigationSystemP(void)
{
  if (this->modechangecbs) {
    delete this->modechangecbs;
  }
  do { // clean up transitions
    const int num = this->transitions->getLength();
    int i;
    for (i = 0; i < num; i++) {
      SoNavigationModeTransition * transition = (*(this->transitions))[i];
      if (transition->trigger) { delete transition->trigger; }
      if (transition->condition) { delete transition->condition; }
      delete transition;
    }
    delete this->transitions;
  } while (FALSE);
  do {
    const int num = this->modes->getLength();
    int i;
    for (i = 0; i < num; i++) {
      SoNavigationMode * mode = (*(this->modes))[i];
      delete mode;
    }
    delete this->modes;
  } while (FALSE);
  delete this->ctrl;
  delete this->state;
}

// *************************************************************************

void
SoNavigationSystemP::initExaminerSystem(void)
{
  SoKeyboardEvent * escapedown = new SoKeyboardEvent;
  escapedown->setKey(SoKeyboardEvent::ESCAPE);
  escapedown->setState(SoButtonEvent::DOWN);

  SoKeyboardEvent * shiftdown = new SoKeyboardEvent;
  shiftdown->setKey(SoKeyboardEvent::LEFT_SHIFT);
  shiftdown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * shiftup = new SoKeyboardEvent;
  shiftup->setKey(SoKeyboardEvent::LEFT_SHIFT);
  shiftup->setState(SoButtonEvent::UP);

  SoKeyboardEvent * ctrldown = new SoKeyboardEvent;
  ctrldown->setKey(SoKeyboardEvent::LEFT_CONTROL);
  ctrldown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * ctrlup = new SoKeyboardEvent;
  ctrlup->setKey(SoKeyboardEvent::LEFT_CONTROL);
  ctrlup->setState(SoButtonEvent::UP);

  SoKeyboardEvent * key_sdown = new SoKeyboardEvent;
  key_sdown->setKey(SoKeyboardEvent::S);
  key_sdown->setState(SoButtonEvent::DOWN);

  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button1up = new SoMouseButtonEvent;
  button1up->setButton(SoMouseButtonEvent::BUTTON1);
  button1up->setState(SoButtonEvent::UP);

  SoMouseButtonEvent * button2down = new SoMouseButtonEvent;
  button2down->setButton(SoMouseButtonEvent::BUTTON2);
  button2down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button2up = new SoMouseButtonEvent;
  button2up->setButton(SoMouseButtonEvent::BUTTON2);
  button2up->setState(SoButtonEvent::UP);

  SoMouseButtonEvent * button3down = new SoMouseButtonEvent;
  button3down->setButton(SoMouseButtonEvent::BUTTON3);
  button3down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button3up = new SoMouseButtonEvent;
  button3up->setButton(SoMouseButtonEvent::BUTTON3);
  button3up->setState(SoButtonEvent::UP);

  SoMouseButtonEvent * mwheeldown = new SoMouseButtonEvent;
  mwheeldown->setButton(SoMouseButtonEvent::BUTTON4);
  mwheeldown->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * mwheelup = new SoMouseButtonEvent;
  mwheelup->setButton(SoMouseButtonEvent::BUTTON5);
  mwheelup->setState(SoButtonEvent::DOWN);


  // set up some standard navigation systems
  SoNavigationSystem * examiner =
    new SoNavigationSystem(SO_EXAMINER_SYSTEM);

  SoIdleMode * examineridle =
    new SoIdleMode(SO_EXAMINER_IDLE_MODE);
  SoRotateMode * examinerrotate =
    new SoRotateMode(SO_EXAMINER_ROTATE_MODE);
  SoIdleMode * examinerwaitforzoom =
    new SoIdleMode(SO_EXAMINER_WAITFORZOOM_MODE);
  SoZoomMode * examinerzoom =
    new SoZoomMode(SO_EXAMINER_ZOOM_MODE);
  SoIdleMode * examinerwaitforpan =
    new SoIdleMode(SO_EXAMINER_WAITFORPAN_MODE);
  SoPanMode * examinerpan =
    new SoPanMode(SO_EXAMINER_PAN_MODE);
  SoIdleMode * examinerwaitforcenter =
    new SoIdleMode(SO_EXAMINER_WAITFORCENTER_MODE);
  SoCenterMode * examinercenter =
    new SoCenterMode(SO_EXAMINER_CENTER_MODE);

  examiner->addMode(examineridle);
  examiner->addMode(examinerrotate);
  examiner->addMode(examinerwaitforzoom);
  examiner->addMode(examinerwaitforpan);
  examiner->addMode(examinerwaitforcenter);
  examiner->addMode(examinerzoom);
  examiner->addMode(examinerpan);
  examiner->addMode(examinercenter);

  examiner->addModeTransition(examineridle, SoNavigationSystem::INITIAL);
  examiner->addModeTransition(examineridle, examinerrotate,
                              SoNavigationSystem::STACK, button1down);
  examiner->addModeTransition(examineridle, examinerwaitforzoom,
                              SoNavigationSystem::STACK, shiftdown);
  examiner->addModeTransition(examineridle, examinerwaitforpan,
                              SoNavigationSystem::STACK, ctrldown);
  examiner->addModeTransition(examineridle, examinerzoom,
                              SoNavigationSystem::STACK, button2down);
  examiner->addModeTransition(examineridle, examinerzoom,
                              SoNavigationSystem::STACK, mwheeldown);
  examiner->addModeTransition(examineridle, examinerzoom,
                              SoNavigationSystem::STACK, mwheelup);
  examiner->addModeTransition(examineridle, examinerpan,
                              SoNavigationSystem::STACK, button3down);
  examiner->addModeTransition(examineridle, examinerwaitforcenter,
                              SoNavigationSystem::STACK, key_sdown);

  examiner->addModeTransition(examinerrotate, SoNavigationSystem::FINISH, button1up, button1down);
  examiner->addModeTransition(examinerrotate, SoNavigationSystem::ABORT, escapedown);

  examiner->addModeTransition(examinerzoom, SoNavigationSystem::FINISH, button1up, button1down);
  examiner->addModeTransition(examinerzoom, SoNavigationSystem::FINISH, button2up, button2down);
  examiner->addModeTransition(examinerzoom, SoNavigationSystem::ABORT, escapedown);

  examiner->addModeTransition(examinerpan, SoNavigationSystem::FINISH, button1up, button1down);
  examiner->addModeTransition(examinerpan, SoNavigationSystem::FINISH, button3up, button3down);
  examiner->addModeTransition(examinerpan, SoNavigationSystem::ABORT, escapedown);

  examiner->addModeTransition(examinerwaitforzoom, SoNavigationSystem::FINISH, shiftup, shiftdown);
  examiner->addModeTransition(examinerwaitforpan, SoNavigationSystem::FINISH, ctrlup, ctrldown);
  examiner->addModeTransition(examinerwaitforcenter, SoNavigationSystem::FINISH, key_sdown);


  examiner->addModeTransition(examinerwaitforzoom, examinerzoom,
                              SoNavigationSystem::SWITCH, button1down);
  examiner->addModeTransition(examinerwaitforpan, examinerpan,
                              SoNavigationSystem::SWITCH, button1down);
  examiner->addModeTransition(examinerwaitforcenter, examinercenter,
                              SoNavigationSystem::SWITCH, button1down);


  SoNavigationSystem::registerSystem(examiner);

  delete escapedown;
  delete key_sdown;
  delete button1down;
  delete button1up;
  delete button2down;
  delete button2up;
  delete button3down;
  delete button3up;
  delete shiftdown;
  delete shiftup;
  delete ctrldown;
  delete ctrlup;
  delete mwheeldown;
  delete mwheelup;
}

void
SoNavigationSystemP::initPannerSystem(void)
{
  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button1up = new SoMouseButtonEvent;
  button1up->setButton(SoMouseButtonEvent::BUTTON1);
  button1up->setState(SoButtonEvent::UP);
  SoKeyboardEvent * spacedown = new SoKeyboardEvent;
  spacedown->setKey(SoKeyboardEvent::SPACE);
  spacedown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * escapedown = new SoKeyboardEvent;
  escapedown->setKey(SoKeyboardEvent::ESCAPE);
  escapedown->setState(SoButtonEvent::DOWN);

  SoNavigationSystem * panner = new SoNavigationSystem(SO_PANNER_SYSTEM);
  SoIdleMode * panner_idle = new SoIdleMode(SO_PANNER_IDLE_MODE);
  SoPanMode * panner_pan = new SoPanMode(SO_PANNER_PAN_MODE);
  panner->addMode(panner_idle);
  panner->addMode(panner_pan);
  panner->addModeTransition(panner_idle, SoNavigationSystem::INITIAL);
  panner->addModeTransition(panner_idle, panner_pan,
                            SoNavigationSystem::STACK, button1down);
  panner->addModeTransition(panner_pan, panner_pan,
                            SoNavigationSystem::SWITCH, spacedown);
  panner->addModeTransition(panner_pan, SoNavigationSystem::FINISH, button1up);
  panner->addModeTransition(panner_pan, SoNavigationSystem::ABORT, escapedown);

  SoNavigationSystem::registerSystem(panner);

  delete spacedown;
  delete escapedown;
  delete button1down;
  delete button1up;
}

void
SoNavigationSystemP::initZoomerSystem(void)
{
  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button1up = new SoMouseButtonEvent;
  button1up->setButton(SoMouseButtonEvent::BUTTON1);
  button1up->setState(SoButtonEvent::UP);
  SoKeyboardEvent * spacedown = new SoKeyboardEvent;
  spacedown->setKey(SoKeyboardEvent::SPACE);
  spacedown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * escapedown = new SoKeyboardEvent;
  escapedown->setKey(SoKeyboardEvent::ESCAPE);
  escapedown->setState(SoButtonEvent::DOWN);

  SoNavigationSystem * zoomer = new SoNavigationSystem(SO_ZOOMER_SYSTEM);
  SoIdleMode * zoomer_idle = new SoIdleMode(SO_ZOOMER_IDLE_MODE);
  SoZoomMode * zoomer_zoom = new SoZoomMode(SO_ZOOMER_ZOOM_MODE);
  zoomer->addMode(zoomer_idle);
  zoomer->addMode(zoomer_zoom);
  zoomer->addModeTransition(zoomer_idle, SoNavigationSystem::INITIAL);
  zoomer->addModeTransition(zoomer_idle, zoomer_zoom,
                            SoNavigationSystem::STACK, button1down);
  zoomer->addModeTransition(zoomer_zoom, zoomer_zoom,
                            SoNavigationSystem::SWITCH, spacedown);
  zoomer->addModeTransition(zoomer_zoom, SoNavigationSystem::FINISH, button1up);
  zoomer->addModeTransition(zoomer_zoom, SoNavigationSystem::ABORT, escapedown);

  SoNavigationSystem::registerSystem(zoomer);

  delete spacedown;
  delete escapedown;
  delete button1down;
  delete button1up;
}

void
SoNavigationSystemP::initRotaterSystem(void)
{
  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button1up = new SoMouseButtonEvent;
  button1up->setButton(SoMouseButtonEvent::BUTTON1);
  button1up->setState(SoButtonEvent::UP);
  SoKeyboardEvent * spacedown = new SoKeyboardEvent;
  spacedown->setKey(SoKeyboardEvent::SPACE);
  spacedown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * escapedown = new SoKeyboardEvent;
  escapedown->setKey(SoKeyboardEvent::ESCAPE);
  escapedown->setState(SoButtonEvent::DOWN);

  SoNavigationSystem * rotater = new SoNavigationSystem(SO_ROTATER_SYSTEM);
  SoIdleMode * rotater_idle = new SoIdleMode(SO_ROTATER_IDLE_MODE);
  SoRotateMode * rotater_rotate = new SoRotateMode(SO_ROTATER_ROTATE_MODE);
  rotater->addMode(rotater_idle);
  rotater->addMode(rotater_rotate);
  rotater->addModeTransition(rotater_idle, SoNavigationSystem::INITIAL);
  rotater->addModeTransition(rotater_idle, rotater_rotate,
                             SoNavigationSystem::STACK, button1down);
  rotater->addModeTransition(rotater_rotate, rotater_rotate,
                             SoNavigationSystem::SWITCH, spacedown);
  rotater->addModeTransition(rotater_rotate, SoNavigationSystem::FINISH, button1up);
  rotater->addModeTransition(rotater_rotate, SoNavigationSystem::ABORT, escapedown);

  SoNavigationSystem::registerSystem(rotater);

  delete spacedown;
  delete escapedown;
  delete button1down;
  delete button1up;
}

void
SoNavigationSystemP::initCentererSystem(void)
{
  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);

  SoNavigationSystem * centerer = new SoNavigationSystem(SO_CENTERER_SYSTEM);
  SoIdleMode * centerer_idle = new SoIdleMode(SO_CENTERER_IDLE_MODE);
  SoCenterMode * centerer_center = new SoCenterMode(SO_CENTERER_CENTER_MODE);

  centerer->addMode(centerer_idle);
  centerer->addMode(centerer_center);
  centerer->addModeTransition(centerer_idle, SoNavigationSystem::INITIAL);
  centerer->addModeTransition(centerer_idle, centerer_center,
                              SoNavigationSystem::STACK, button1down);

  SoNavigationSystem::registerSystem(centerer);

  delete button1down;
}

void
SoNavigationSystemP::initPickerSystem(void)
{
  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);

  SoNavigationSystem * picker = new SoNavigationSystem(SO_PICKER_SYSTEM);
  SoIdleMode * picker_idle = new SoIdleMode(SO_PICKER_IDLE_MODE);
  SoPickMode * picker_pick = new SoPickMode(SO_PICKER_PICK_MODE);
  picker->addMode(picker_idle);
  picker->addMode(picker_pick);
  picker->addModeTransition(picker_idle, SoNavigationSystem::INITIAL);
  picker->addModeTransition(picker_idle, picker_pick,
                            SoNavigationSystem::STACK, button1down);

  SoNavigationSystem::registerSystem(picker);

  delete button1down;
}

void
SoNavigationSystemP::initIdleSystem(void)
{
  SoNavigationSystem * idler =
    new SoNavigationSystem(SO_IDLER_SYSTEM);
  SoIdleMode * idleridle =
    new SoIdleMode(SO_IDLER_IDLE_MODE);
  idler->addMode(idleridle);
  idler->addModeTransition(idleridle, SoNavigationSystem::INITIAL);

  SoNavigationSystem::registerSystem(idler);
}

void
SoNavigationSystemP::initPlaneViewerSystem(void)
{
  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button1up = new SoMouseButtonEvent;
  button1up->setButton(SoMouseButtonEvent::BUTTON1);
  button1up->setState(SoButtonEvent::UP);

  SoMouseButtonEvent * button2down = new SoMouseButtonEvent;
  button2down->setButton(SoMouseButtonEvent::BUTTON2);
  button2down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button2up = new SoMouseButtonEvent;
  button2up->setButton(SoMouseButtonEvent::BUTTON2);
  button2up->setState(SoButtonEvent::UP);

  SoKeyboardEvent * escapedown = new SoKeyboardEvent;
  escapedown->setKey(SoKeyboardEvent::ESCAPE);
  escapedown->setState(SoButtonEvent::DOWN);

  SoKeyboardEvent * shiftdown = new SoKeyboardEvent;
  shiftdown->setKey(SoKeyboardEvent::LEFT_SHIFT);
  shiftdown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * shiftup = new SoKeyboardEvent;
  shiftup->setKey(SoKeyboardEvent::LEFT_SHIFT);
  shiftup->setState(SoButtonEvent::UP);

  SoNavigationSystem * planeviewer = new SoNavigationSystem(SO_PLANEVIEWER_SYSTEM);
  SoIdleMode * idle = new SoIdleMode(SO_PLANEVIEWER_IDLE_MODE);
  SoPanMode * pan = new SoPanMode(SO_PLANEVIEWER_PAN_MODE);

  SoIdleMode * waitforzoom =
    new SoIdleMode(SO_PLANEVIEWER_WAITFORZOOM_MODE);
  SoZoomMode * zoom =
    new SoZoomMode(SO_PLANEVIEWER_ZOOM_MODE);

  planeviewer->addMode(idle);
  planeviewer->addMode(pan);
  planeviewer->addMode(zoom);
  planeviewer->addMode(waitforzoom);

  planeviewer->addModeTransition(idle, SoNavigationSystem::INITIAL);
  planeviewer->addModeTransition(idle, waitforzoom,
                                 SoNavigationSystem::STACK, shiftdown);
  planeviewer->addModeTransition(waitforzoom, SoNavigationSystem::FINISH, shiftup, shiftdown);
  planeviewer->addModeTransition(idle, pan,
                                 SoNavigationSystem::STACK, button1down);
  planeviewer->addModeTransition(pan, SoNavigationSystem::FINISH, button1up);
  planeviewer->addModeTransition(idle, zoom,
                                 SoNavigationSystem::STACK, button2down);
  planeviewer->addModeTransition(zoom, SoNavigationSystem::FINISH, button2up, button2down);

  planeviewer->addModeTransition(zoom, SoNavigationSystem::FINISH, button1up, button1down);

  planeviewer->addModeTransition(pan, SoNavigationSystem::ABORT, escapedown);
  planeviewer->addModeTransition(zoom, SoNavigationSystem::ABORT, escapedown);

  planeviewer->addModeTransition(waitforzoom, zoom, SoNavigationSystem::SWITCH, button1down);

  SoNavigationSystem::registerSystem(planeviewer);

  delete shiftdown;
  delete shiftup;
  delete escapedown;
  delete button1down;
  delete button2down;
  delete button1up;
  delete button2up;
}


void
SoNavigationSystemP::initBuiltinSystems(void)
{
  initExaminerSystem();
  initPannerSystem();
  initZoomerSystem();
  initRotaterSystem();
  initCentererSystem();
  initPickerSystem();
  initIdleSystem();
  initPlaneViewerSystem();
}
