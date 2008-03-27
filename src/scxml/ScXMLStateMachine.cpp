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

#include <Inventor/scxml/ScXMLStateMachine.h>

#include <assert.h>
#include <list>
#include <vector>
#include <map>
#include <algorithm>

#include <boost/scoped_ptr.hpp>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLDocument.h>
#include <Inventor/scxml/ScXMLState.h>
#include <Inventor/scxml/ScXMLInitial.h>
#include <Inventor/scxml/ScXMLFinal.h>
#include <Inventor/scxml/ScXMLOnEntry.h>
#include <Inventor/scxml/ScXMLOnExit.h>
#include <Inventor/scxml/ScXMLInvoke.h>
#include <Inventor/scxml/ScXMLTransition.h>
#include <Inventor/scxml/ScXMLAnchor.h>
#include <Inventor/scxml/ScXMLHistory.h>

#include <Inventor/scxml/ScXMLEvent.h>

// *************************************************************************

struct EventInfo {
  const ScXMLEvent * eventptr;
  SbBool deallocate;
};

class ScXMLStateMachineP {
public:
  ScXMLStateMachineP(void)
    : pub(NULL),
      active(FALSE), finished(FALSE),
      name(SbName::empty()), description(NULL),
      currentevent(NULL),
      isprocessingqueue(FALSE),
      initializer(NULL)
  {
  }

  ScXMLStateMachine * pub;

  SbBool active;
  SbBool finished;

  SbName name;
  ScXMLDocument * description;

  // delete callbacks:
  typedef std::pair<ScXMLStateMachineDeleteCB *, void *> DeleteCBInfo;
  typedef std::vector<DeleteCBInfo> DeleteCallbackList;
  DeleteCallbackList deletecallbacklist;
  void invokeDeleteCallbacks(void);

  // state change callbacks:
  typedef std::pair<ScXMLStateChangeCB *, void *> StateChangeCBInfo;
  typedef std::vector<StateChangeCBInfo> StateChangeCallbackList;
  StateChangeCallbackList statechangecallbacklist;
  void invokeStateChangeCallbacks(const char * identifier, SbBool enterstate);

  const ScXMLEvent * currentevent;

  std::list<EventInfo> eventqueue;
  SbBool isprocessingqueue;

  std::list<EventInfo> internaleventqueue;

  void queueInternalEvent(const ScXMLEvent * event, SbBool dealloc = FALSE);
  void queueInternalEvent(const SbName & eventid);


  boost::scoped_ptr<ScXMLTransition> initializer;

  std::vector<const ScXMLObject *> activestatelist;

  // name->object map
  typedef std::map<const char *, const ScXMLObject *> IdentifierMap;
  IdentifierMap identifiermap;

  void fillIdentifierMap(const ScXMLObject * object);
  const ScXMLObject * getObjectByIdentifier(SbName identifier) const;

  typedef std::pair<const ScXMLObject *, const ScXMLTransition *> StateTransition;
  typedef std::vector<StateTransition> TransitionList;

  static void findTransitions(TransitionList & transitions, const ScXMLObject * stateobj, const ScXMLEvent * event);

  void exitState(const ScXMLObject * object);
  void enterState(const ScXMLObject * object);

}; // ScXMLStateMachineP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

SCXML_OBJECT_SOURCE(ScXMLStateMachine);

void
ScXMLStateMachine::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLStateMachine, ScXMLObject, SCXML_DEFAULT_NS, "statemachine");
}

ScXMLStateMachine::ScXMLStateMachine(void)
{
  PRIVATE(this)->pub = this;
}

ScXMLStateMachine::~ScXMLStateMachine(void)
{
  PRIVATE(this)->invokeDeleteCallbacks();

  if (PRIVATE(this)->description) {
    delete PRIVATE(this)->description;
    PRIVATE(this)->description = NULL;
  }
}

// *************************************************************************

void
ScXMLStateMachine::setName(const SbName & nameobj)
{
  PRIVATE(this)->name = nameobj;
}

const SbName &
ScXMLStateMachine::getName(void) const
{
  return PRIVATE(this)->name;
}

void
ScXMLStateMachine::setDescription(ScXMLDocument * document)
{
  PRIVATE(this)->description = document;
  PRIVATE(this)->initializer.reset(NULL);
  PRIVATE(this)->active = FALSE;
  PRIVATE(this)->finished = FALSE;
  PRIVATE(this)->activestatelist.clear();
  PRIVATE(this)->identifiermap.clear();
  PRIVATE(this)->fillIdentifierMap(PRIVATE(this)->description);
}

const ScXMLDocument *
ScXMLStateMachine::getDescription(void) const
{
  return PRIVATE(this)->description;
}

// *************************************************************************

/*!
  Fire up the engine.
*/
void
ScXMLStateMachine::initialize(void)
{
  assert(!PRIVATE(this)->active);
  PRIVATE(this)->active = TRUE;
  PRIVATE(this)->finished = FALSE;
  PRIVATE(this)->activestatelist.clear();
  PRIVATE(this)->isprocessingqueue = TRUE;
  this->processOneEvent(NULL); // process the initialstate initializer
  PRIVATE(this)->isprocessingqueue = FALSE;
  this->processEventQueue();
}

// *************************************************************************

/*!
*/
void
ScXMLStateMachine::queueEvent(const ScXMLEvent * event, SbBool dealloc)
{
  EventInfo info;
  info.eventptr = event;
  info.deallocate = dealloc;
  PRIVATE(this)->eventqueue.push_back(info);
}

/*!
*/
void
ScXMLStateMachine::queueEvent(const SbName & eventid)
{
  ScXMLEvent * event = new ScXMLEvent;
  event->setIdentifier(eventid);
  this->queueEvent(event, TRUE);
}

/*
*/
void
ScXMLStateMachineP::queueInternalEvent(const ScXMLEvent * event, SbBool dealloc)
{
  EventInfo info;
  info.eventptr = event;
  info.deallocate = dealloc;
  this->internaleventqueue.push_back(info);
}

/*
*/
void
ScXMLStateMachineP::queueInternalEvent(const SbName & eventid)
{
  ScXMLEvent * event = new ScXMLEvent;
  event->setIdentifier(eventid);
  this->queueInternalEvent(event, TRUE);
}

/*!
  Recursive calling of this function will be handled by letting the
  recursive call return immediately, while the first call will
  continue processing the event queue until it is empty.
*/
SbBool
ScXMLStateMachine::processEventQueue(void)
{
  if (PRIVATE(this)->isprocessingqueue) return FALSE;

  SbBool didtransitions = FALSE;

  PRIVATE(this)->isprocessingqueue = TRUE;

  while (!(PRIVATE(this)->eventqueue.empty()) ||
         !(PRIVATE(this)->internaleventqueue.empty())) {

    // all internal events have first priority
    while (!(PRIVATE(this)->internaleventqueue.empty())) {
      EventInfo info = PRIVATE(this)->internaleventqueue.front();
      if (this->processOneEvent(info.eventptr)) {
        didtransitions = TRUE;
      }
      PRIVATE(this)->internaleventqueue.pop_front();
      if (info.deallocate) { delete info.eventptr; }
    }

    // then one of the external events if any
    if (!(PRIVATE(this)->eventqueue.empty())) {
      EventInfo info = PRIVATE(this)->eventqueue.front();
      if (this->processOneEvent(info.eventptr)) {
        didtransitions = TRUE;
      }
      PRIVATE(this)->eventqueue.pop_front();
      if (info.deallocate) { delete info.eventptr; }
    }
  }

  PRIVATE(this)->isprocessingqueue = FALSE;
  return didtransitions;
}

/*!
  Processes one event.
  This is an internal inner event-loop utility function.
*/
SbBool
ScXMLStateMachine::processOneEvent(const ScXMLEvent * event)
{
  // this function seriously needs more structuring
  assert(PRIVATE(this)->isprocessingqueue);
  this->setCurrentEvent(event);

  // FIXME: make sure a transition to self will execute exitState(),
  // invokables in transition, and then enterState() again...I don't
  // think that is handled currently.

  if (0) {
    if (event) SoDebugError::postInfo("process", "event: %s",
                                      event->getIdentifier().getString());
    else       SoDebugError::postInfo("process", "NULL event");
  }

  if (0) {
    //SoDebugError::postInfo("process", "active states: %d",
    //                       PRIVATE(this)->activestatelist.size());
    std::vector<const ScXMLObject *>::iterator it =
      PRIVATE(this)->activestatelist.begin();
    while (it != PRIVATE(this)->activestatelist.end()) {
      SoDebugError::postInfo("ScXMLStateMachine::processOneEvent",
                             "active state: %s", (*it)->getAttribute("id"));
      ++it;
    }
  }
                      
  ScXMLStateMachineP::TransitionList transitions;
  if (PRIVATE(this)->activestatelist.size() == 0) {
    if (PRIVATE(this)->initializer.get() == NULL) {
      PRIVATE(this)->initializer.reset(new ScXMLTransition);
      PRIVATE(this)->initializer->setTargetXMLAttr(PRIVATE(this)->description->getAttribute("initialstate"));
    }
    transitions.push_back(ScXMLStateMachineP::StateTransition(NULL, PRIVATE(this)->initializer.get()));
  } else {
    for (int c = 0; c < static_cast<int>(PRIVATE(this)->activestatelist.size()); ++c) {
      // containers are also active states and must be checked
      const ScXMLObject * stateobj = PRIVATE(this)->activestatelist.at(c);
      while (stateobj != NULL) {
        ScXMLStateMachineP::findTransitions(transitions, stateobj, event);
        stateobj = stateobj->getContainer();
      }
    }
  }

  // no transitions means no changes, just return
  if (transitions.size() == 0) {
    this->setCurrentEvent(NULL);
    return FALSE;
  }

  // we handle all targetless transitions first
  {
    ScXMLStateMachineP::TransitionList::iterator transit = transitions.begin();
    while (transit != transitions.end()) {
      if (transit->second->isTargetLess()) {
        const_cast<ScXMLTransition *>(transit->second)->invoke(this);
      }
      ++transit;
    }
  }

  std::vector<const ScXMLObject *> newstateslist;

  // handle those with targets next
  ScXMLStateMachineP::TransitionList::iterator transit = transitions.begin();
  while (transit != transitions.end()) {
    if (transit->second->isTargetLess()) { ++transit; continue; }

    const char * targetid = transit->second->getTargetXMLAttr();
    const ScXMLObject * targetstate = PRIVATE(this)->getObjectByIdentifier(targetid);
    if (!targetstate) {
      SoDebugError::post("ScXMLStateMachine::processOneEvent",
                         "transition to unknown state '%s' failed.", targetid);
      ++transit;
      continue;
    }

    const ScXMLObject * sourcestate = transit->first;
    std::vector<const ScXMLObject *> sourcestates;

    if (sourcestate != NULL) { // ignore sourcestate NULL (initializer)
      // find all activestate object contained within source state
      std::vector<const ScXMLObject *>::iterator activeit =
        PRIVATE(this)->activestatelist.begin();
      while (activeit != PRIVATE(this)->activestatelist.end()) {
        if ((*activeit)->isContainedIn(sourcestate)) {
          const ScXMLObject * active = *activeit;
          sourcestates.push_back(active); // remember, to remove from activelist
          while (active != sourcestate) {
            //SoDebugError::post("process",
            //       "found activestate as substate of transition source");

            PRIVATE(this)->exitState(active); // exit substates of transition point
            active = active->getContainer();
            assert(active);
          }
        }
        ++activeit;
      }

      while (!targetstate->isContainedIn(sourcestate)) {
        //SoDebugError::postInfo("process", "going up to find common ancestor");
        PRIVATE(this)->exitState(sourcestate);
        sourcestate = sourcestate->getContainer();
      }
    }

    // executable content in the transition
    //SoDebugError::postInfo("process", "executing transition code");
    const_cast<ScXMLTransition *>(transit->second)->invoke(this);

    {
      std::vector<const ScXMLObject *> path;
      //SoDebugError::postInfo("process", "finding target-path from sourcestate %p",
      //                       sourcestate);
      while (sourcestate != targetstate) {
        path.push_back(targetstate);
        targetstate = targetstate->getContainer();
      }
      targetstate = PRIVATE(this)->getObjectByIdentifier(targetid); // restore

      //SoDebugError::postInfo("process", "reversing downward path");
      std::reverse(path.begin(), path.end());

      std::vector<const ScXMLObject *>::iterator pathit = path.begin();
      while (pathit != path.end()) {
        // SoDebugError::postInfo("process", "entering down towards target");
        PRIVATE(this)->enterState(*pathit);
        ++pathit;
      }
    }

    //SoDebugError::post("process", "list of source states to remove - %d",
    //                   sourcestates.size());
    // remove source states form activestates
    std::vector<const ScXMLObject *>::iterator it = sourcestates.begin();
    while (it != sourcestates.end()) {
      std::vector<const ScXMLObject *>::iterator findit =
        std::find(PRIVATE(this)->activestatelist.begin(),
                  PRIVATE(this)->activestatelist.end(), *it);
      if (findit != PRIVATE(this)->activestatelist.end()) {
        //SoDebugError::post("process", "erasing old activestate");
        PRIVATE(this)->activestatelist.erase(findit);
      } else {
        SoDebugError::post("ScXMLStateMachine::processOneEvent",
                           "source state not found in activestate list");
      }
      ++it;
    }

    // add targetstate to active states
    if (std::find(PRIVATE(this)->activestatelist.begin(), PRIVATE(this)->activestatelist.end(), targetstate) == PRIVATE(this)->activestatelist.end()) {
      newstateslist.push_back(targetstate);
    }

    ++transit;
  }

  //SoDebugError::postInfo("process", "new states to potentially append to activestates: %d",
  //                       newstateslist.size());


  // inspect target states for substates + <initial> children
  std::vector<const ScXMLObject *>::iterator appendit = newstateslist.begin();
  while (appendit != newstateslist.end()) {
    SbBool pushedsubstate = FALSE;
    const ScXMLObject * newstate = *appendit;

    SbBool settled = FALSE;
    while (!settled) {
      settled = TRUE;
      if (newstate->isOfType(ScXMLState::getClassTypeId())) {
        const ScXMLState * state = static_cast<const ScXMLState *>(newstate);
        if (state->getNumStates() > 0 || state->getNumParallels() > 0) {
          do {
            const ScXMLInitial * initial = state->getInitial();
            if (!initial) {
              SoDebugError::post("ScXMLStateMachine::processOneEvent",
                                 "state '%s' has substates but no <initial>.",
                                 state->getIdXMLAttr());
              break;
            }
            const ScXMLTransition * transition = initial->getTransition();
            if (!transition) {
              SoDebugError::post("ScXMLStateMachine::processOneEvent",
                                 "state '%s' has <initial> without a transition.",
                                 state->getIdXMLAttr());
              break;
            }
            const char * targetid = transition->getTargetXMLAttr();
            if (!targetid) {
              SoDebugError::post("ScXMLStateMachine::processOneEvent",
                                 "state '%s' has <initial> with a targetless transition.",
                                 state->getIdXMLAttr());
              break;
            }
            const ScXMLObject * targetobj = PRIVATE(this)->getObjectByIdentifier(targetid);
            if (!targetobj) {
              SoDebugError::post("ScXMLStateMachine::processOneEvent",
                                 "could not find target of state \"%s\"'s <initial> transition.",
                                 state->getIdXMLAttr());
              break;
            }
            
            if (targetobj->getContainer() != state) {
              SoDebugError::post("ScXMLStateMachine::processOneEvent",
                                 "target of state \"%s\"'s <initial> transition is not an immediate child of the state",
                                 state->getIdXMLAttr());
              break;
            }
            
            // perform executable code
            const_cast<ScXMLTransition *>(transition)->invoke(this);
            
            PRIVATE(this)->enterState(targetobj);

            newstate = targetobj;
            settled = FALSE; // need to loop over on new state one more time
          } while ( FALSE );
        
        } else {
          // no substates in this state - can be marked as the deepest active state
          if (state->getInitial()) { // just checking
            SoDebugError::post("ScXMLStateMachine::processOneEvent",
                               "state '%s' has <initial> but no sub-states.",
                               state->getIdXMLAttr());
          }
          
          PRIVATE(this)->activestatelist.push_back(state);
          pushedsubstate = TRUE; // need to avoid adding parent state before doing outer loop
          ++appendit;
        }
      } else {
        // non-ScXMLState object (ScXMLFinal for instance)
        if (newstate != *appendit) {
          PRIVATE(this)->activestatelist.push_back(newstate);
          pushedsubstate = TRUE; // need to avoid adding parent state before doing outer loop
          ++appendit;
        }
      }
    }
    if (!pushedsubstate) {
      PRIVATE(this)->activestatelist.push_back(*appendit);
      ++appendit;
    }
  }

  // if all active states are <final> states of the root scxml element,
  // we should set 'finished' to true and stop/hinder event processing

  this->setCurrentEvent(NULL);
  return TRUE; // transitions have been taken
}

// *************************************************************************

/*!
  Returns whether the state machine is active or not.
*/
SbBool
ScXMLStateMachine::isActive(void) const
{
  return PRIVATE(this)->active;
}

/*!
  Returns whether the state machine has run to completion or not.
*/
SbBool
ScXMLStateMachine::isFinished(void) const
{
  return PRIVATE(this)->finished;
}

// *************************************************************************

/*!
  Sets a pointer for the event that is 'current' during event processing.
  This is an internal method, and updating the current event is handled
  automatically.
*/
void
ScXMLStateMachine::setCurrentEvent(const ScXMLEvent * event)
{
  PRIVATE(this)->currentevent = event;
}

/*!
  This method returns the current event during event processing, and NULL
  when not processing events.

  Event processing is in special cases done with NULL as the current event,
  as for instance during state machine initialization.
*/
const ScXMLEvent *
ScXMLStateMachine::getCurrentEvent(void) const
{
  return PRIVATE(this)->currentevent;
}

// *************************************************************************

/*!
  Returns the number of active states in the state machine.  This number
  should currently be 1, but in the future, when <parallel> is implemented,
  it can be more.
*/
int
ScXMLStateMachine::getNumActiveStates(void) const
{
  return static_cast<int>(PRIVATE(this)->activestatelist.size());
}

/*!
  Returns the Nth active state.
*/
const ScXMLObject *
ScXMLStateMachine::getActiveState(int idx) const
{
  assert(idx >= 0 && idx < static_cast<int>(PRIVATE(this)->activestatelist.size()));
  return PRIVATE(this)->activestatelist.at(idx);
}

/*!
  Returns the state machine state object with the given identifier, or NULL
  if no object is found that has the given identifier.
*/
const ScXMLObject *
ScXMLStateMachine::getState(const char * identifier) const
{
  return PRIVATE(this)->getObjectByIdentifier(identifier);
}

// *************************************************************************

/*!
  Registers a callback to be called when the state machine object is being
  deleted.
*/
void
ScXMLStateMachine::addDeleteCallback(ScXMLStateMachineDeleteCB * cb, void * userdata)
{
  PRIVATE(this)->deletecallbacklist.push_back(ScXMLStateMachineP::DeleteCBInfo(cb, userdata));
}

/*!
  Unregisters a callback to be called when the state machine object is being
  deleted.
*/
void
ScXMLStateMachine::removeDeleteCallback(ScXMLStateMachineDeleteCB * cb, void * userdata)
{
  ScXMLStateMachineP::DeleteCallbackList::iterator it =
    std::find(PRIVATE(this)->deletecallbacklist.begin(),
              PRIVATE(this)->deletecallbacklist.end(),
              ScXMLStateMachineP::DeleteCBInfo(cb, userdata));
  if (it != PRIVATE(this)->deletecallbacklist.end()) {
    PRIVATE(this)->deletecallbacklist.erase(it);
  }
}

/*
  Invoke all the delete callbacks.
*/

void
ScXMLStateMachineP::invokeDeleteCallbacks(void)
{
  DeleteCallbackList::const_iterator it = this->deletecallbacklist.begin();
  while (it != this->deletecallbacklist.end()) {
    (it->first)(it->second, PUBLIC(this));
    ++it;
  }
}

// *************************************************************************

/*!
  \var ScXMLStateChangeCB

  This callback type is for notifying listeners on when the state machine
  enters and exits states that are tagged as "tasks" for logging purposes.
  This is what the Boolean "task" attribute in the state element sets up.

  The \a success argument is currently unsupported (will always be TRUE),
  but has been preemptively added to avoid a signature change later.

  \sa addStateChangeCallback
*/

/*!
  Registers a callback to be called when the state machine object is being
  deleted.
*/
void
ScXMLStateMachine::addStateChangeCallback(ScXMLStateChangeCB * callback, void * userdata)
{
  PRIVATE(this)->statechangecallbacklist.push_back(ScXMLStateMachineP::StateChangeCBInfo(callback, userdata));
}

/*!
  Unregisters a callback to be called when the state machine object is being
  deleted.
*/
void
ScXMLStateMachine::removeStateChangeCallback(ScXMLStateChangeCB * callback, void * userdata)
{
  ScXMLStateMachineP::StateChangeCallbackList::iterator findit =
    std::find(PRIVATE(this)->statechangecallbacklist.begin(),
              PRIVATE(this)->statechangecallbacklist.end(),
              ScXMLStateMachineP::StateChangeCBInfo(callback, userdata));
  if (findit != PRIVATE(this)->statechangecallbacklist.end()) {
    PRIVATE(this)->statechangecallbacklist.erase(findit);
  }
}

/*
  Invoke all the state change callbacks.
*/
void
ScXMLStateMachineP::invokeStateChangeCallbacks(const char * identifier, SbBool enterstate)
{
  StateChangeCallbackList::const_iterator it =
    this->statechangecallbacklist.begin();
  while (it != this->statechangecallbacklist.end()) {
    (it->first)(it->second, PUBLIC(this), identifier, enterstate, TRUE);
    ++it;
  }
}

// *************************************************************************

void
ScXMLStateMachineP::fillIdentifierMap(const ScXMLObject * object)
{
  assert(object);

  if (object->getAttribute("id") != NULL) {
    SbName identifier = object->getAttribute("id");
    this->identifiermap.insert(std::pair<const char *, const ScXMLObject *>(identifier.getString(), object));
  }

  if (object->isOfType(ScXMLAnchor::getClassTypeId())) {
    const ScXMLAnchor * anchor = static_cast<const ScXMLAnchor *>(object);
  }
  else if (object->isOfType(ScXMLDocument::getClassTypeId())) {
    const ScXMLDocument * doc = static_cast<const ScXMLDocument *>(object);
    int c;
    for (c = 0; c < doc->getNumStates(); ++c) {
      this->fillIdentifierMap(doc->getState(c));
    }
    for (c = 0; c < doc->getNumParallels(); ++c) {
      this->fillIdentifierMap(doc->getParallel(c));
    }
    for (c = 0; c < doc->getNumFinals(); ++c) {
      this->fillIdentifierMap(doc->getFinal(c));
    }
  }
  else if (object->isOfType(ScXMLFinal::getClassTypeId())) {
    const ScXMLFinal * final = static_cast<const ScXMLFinal *>(object);
  }
  else if (object->isOfType(ScXMLHistory::getClassTypeId())) {
    const ScXMLHistory * history = static_cast<const ScXMLHistory *>(object);
    if (history->getTransition()) {
      this->fillIdentifierMap(history->getTransition());
    }
  }
  else if (object->isOfType(ScXMLInitial::getClassTypeId())) {
    const ScXMLInitial * initial = static_cast<const ScXMLInitial *>(object);
    if (initial->getTransition()) {
      this->fillIdentifierMap(initial->getTransition());
    }
  }
  else if (object->isOfType(ScXMLInvoke::getClassTypeId())) {
    const ScXMLInvoke * invoke = static_cast<const ScXMLInvoke *>(object);
  }
  else if (object->isOfType(ScXMLOnEntry::getClassTypeId())) {
    const ScXMLOnEntry * onentry = static_cast<const ScXMLOnEntry *>(object);
    int c;
    for (c = 0; c < onentry->getNumInvokes(); ++c) {
      this->fillIdentifierMap(onentry->getInvoke(c));
    }
  }
  else if (object->isOfType(ScXMLOnExit::getClassTypeId())) {
    const ScXMLOnExit * onexit = static_cast<const ScXMLOnExit *>(object);
    int c;
    for (c = 0; c < onexit->getNumInvokes(); ++c) {
      this->fillIdentifierMap(onexit->getInvoke(c));
    }
  }
  else if (object->isOfType(ScXMLState::getClassTypeId())) {
    const ScXMLState * state = static_cast<const ScXMLState *>(object);
    int c;
    if (state->getOnEntry()) {
      this->fillIdentifierMap(state->getOnEntry());
    }
    if (state->getOnExit()) {
      this->fillIdentifierMap(state->getOnExit());
    }
    for (c = 0; c < state->getNumTransitions(); ++c) {
      this->fillIdentifierMap(state->getTransition(c));
    }
    if (state->getInitial()) {
      this->fillIdentifierMap(state->getInitial());
    }
    for (c = 0; c < state->getNumStates(); ++c) {
      this->fillIdentifierMap(state->getState(c));
    }
    for (c = 0; c < state->getNumParallels(); ++c) {
      this->fillIdentifierMap(state->getParallel(c));
    }
    for (c = 0; c < state->getNumFinals(); ++c) {
      this->fillIdentifierMap(state->getFinal(c));
    }
    for (c = 0; c < state->getNumHistories(); ++c) {
      this->fillIdentifierMap(state->getHistory(c));
    }
    for (c = 0; c < state->getNumAnchors(); ++c) {
      this->fillIdentifierMap(state->getAnchor(c));
    }
    if (state->getInvoke()) {
      this->fillIdentifierMap(state->getInvoke());
    }
  }
  else if (object->isOfType(ScXMLTransition::getClassTypeId())) {
    const ScXMLTransition * transition = static_cast<const ScXMLTransition *>(object);
    int c;
    for (c = 0; c < transition->getNumInvokes(); ++c) {
      this->fillIdentifierMap(transition->getInvoke(c));
    }
  }
  else {
    SoDebugError::postInfo("fillIdentifierMap", "unsupported object type %s",
                           object->getTypeId().getName().getString());
  }
}

const ScXMLObject *
ScXMLStateMachineP::getObjectByIdentifier(SbName identifier) const
{
  std::map<const char *, const ScXMLObject *>::const_iterator it =
    this->identifiermap.find(identifier.getString());
  if (it != this->identifiermap.end()) {
    return it->second;
  }
  return NULL;
}

void
ScXMLStateMachineP::findTransitions(TransitionList & transitions, const ScXMLObject * stateobj, const ScXMLEvent * event)
{
  assert(stateobj);

  if (stateobj->isOfType(ScXMLHistory::getClassTypeId())) {
    const ScXMLHistory * history = static_cast<const ScXMLHistory *>(stateobj);
    if (history->getTransition() && history->getTransition()->isEventMatch(event)) {
      StateTransition transition(stateobj, history->getTransition());
      TransitionList::iterator findit = 
        std::find(transitions.begin(), transitions.end(), transition);
      if (findit == transitions.end()) {
        transitions.push_back(transition);
      }
    }
  }
  else if (stateobj->isOfType(ScXMLInitial::getClassTypeId())) {
    const ScXMLInitial * initial = static_cast<const ScXMLInitial *>(stateobj);
    if (initial->getTransition() && initial->getTransition()->isEventMatch(event)) {
      StateTransition transition(stateobj, initial->getTransition());
      TransitionList::iterator findit = 
        std::find(transitions.begin(), transitions.end(), transition);
      if (findit == transitions.end()) {
        transitions.push_back(transition);
      }
    }
  }
  else if (stateobj->isOfType(ScXMLState::getClassTypeId())) {
    const ScXMLState * state = static_cast<const ScXMLState *>(stateobj);
    for (int j = 0; j < state->getNumTransitions(); ++j) {
      if (state->getTransition(j)->isEventMatch(event)) {
        StateTransition transition(stateobj, state->getTransition(j));
        TransitionList::iterator findit = 
          std::find(transitions.begin(), transitions.end(), transition);
        if (findit == transitions.end()) {
          transitions.push_back(transition);
        }
      }
    }
  }
}

// *************************************************************************

void
ScXMLStateMachineP::exitState(const ScXMLObject * object)
{
  assert(object);
  if (object->isOfType(ScXMLState::getClassTypeId())) {
    const ScXMLState * state = static_cast<const ScXMLState *>(object);
    const char * id = state->getIdXMLAttr();
    if (state->isTask()) {
      this->invokeStateChangeCallbacks(id, FALSE);
    }
    const ScXMLOnExit * onexit = state->getOnExit();
    if (onexit) {
      const_cast<ScXMLOnExit *>(onexit)->invoke(PUBLIC(this));
    }
  }
}

void
ScXMLStateMachineP::enterState(const ScXMLObject * object)
{
  assert(object);

  if (object->isOfType(ScXMLFinal::getClassTypeId())) {
    // When entering a <final>, ParentID.done should be posted
    const ScXMLFinal * final = static_cast<const ScXMLFinal *>(object);
    const ScXMLObject * container = final->getContainer();
    assert(container);
    const char * id = container->getAttribute("id");
    if (!id || strlen(id) == 0) {
      if (container->isOfType(ScXMLDocument::getClassTypeId())) {
        // there is not ParentID to post a ParentID.done event in
        // this case. study SCXML state to see what to do?
        this->finished = TRUE;
        this->active = FALSE;
      } else {
        SoDebugError::post("ScXMLStateMachineP::enterState",
                           "<final> container has no id - can't post done-event");
      }
      return;
    }
    SbString eventstr;
    eventstr.sprintf("%s.done", id);
    this->queueInternalEvent(eventstr.getString());
  }
  else if (object->isOfType(ScXMLState::getClassTypeId())) {
    const ScXMLState * state = static_cast<const ScXMLState *>(object);
    const char * id = state->getIdXMLAttr();
    if (state->isTask()) {
      this->invokeStateChangeCallbacks(id, TRUE);
    }
    const ScXMLOnEntry * onentry = state->getOnEntry();
    if (onentry) {
      const_cast<ScXMLOnEntry *>(onentry)->invoke(PUBLIC(this));
    }
  }
}

// *************************************************************************

#undef PUBLIC
#undef PRIVATE
