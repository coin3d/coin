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

#include <Inventor/scxml/ScXMLTransition.h>

#include <assert.h>
#include <string.h>
#include <algorithm>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLEvent.h>
#include <Inventor/scxml/ScXMLInvoke.h>

#include "scxml/ScXMLCommonP.h"

// *************************************************************************

class ScXMLTransitionP {
public:
};

SCXML_OBJECT_SOURCE(ScXMLTransition);

void
ScXMLTransition::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLTransition, ScXMLObject, SCXML_DEFAULT_NS, "transition");
}

// *************************************************************************

ScXMLTransition::ScXMLTransition(void)
  : event(NULL), cond(NULL), target(NULL), anchor(NULL),
    needprefixmatching(FALSE),
    eventkey(SbName::empty()), targetkey(SbName::empty())
{
}

ScXMLTransition::~ScXMLTransition(void)
{
  this->setEventXMLAttr(NULL);
  this->setCondXMLAttr(NULL);
  this->setTargetXMLAttr(NULL);
  this->setAnchorXMLAttr(NULL);

  {
    std::vector<ScXMLInvoke *>::iterator invokeit = this->invokelist.begin();
    while (invokeit != this->invokelist.end()) {
      delete *invokeit;
      ++invokeit;
    }
    this->invokelist.clear();
  }
}

// *************************************************************************

SbBool
ScXMLTransition::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->setEventXMLAttr(this->getAttribute("event"));
  this->cond = const_cast<char *>(this->getAttribute("cond"));
  this->target = const_cast<char *>(this->getAttribute("target"));
  this->anchor = const_cast<char *>(this->getAttribute("anchor"));

  if (this->target && this->anchor) {
    // only one of 'target' and 'anchor' may be specified
    return FALSE;
  }

  return TRUE;
}

void
ScXMLTransition::setEventXMLAttr(const char * eventstr)
{
  if (this->event && this->event != this->getAttribute("event")) {
    delete [] this->event;
  }
  this->event = NULL;
  this->eventkey = SbName::empty();
  this->needprefixmatching = FALSE;

  if (eventstr) {
    // You can do *-matching on event identifiers in transitions.
    // According to the spec., the allowed thing to do is to let the
    // event attribute _end_ with ".*", which should match zero or
    // more succeeding tokens - we need in other words not implement
    // any form of generic pattern matching here...
    const char * ptr = strstr(eventstr, ".*");
    if ((ptr != NULL) && (strlen(ptr) == 2)) {
      this->needprefixmatching = TRUE;
      // we'll chop off the pattern matching key and use the boolean
      const int len = strlen(eventstr) - 1;
      this->event = new char [ len ];
      strncpy(this->event, eventstr, len - 1);
      this->event[ len - 1 ] = '\0';
      this->eventkey = this->event;
    } else {
      this->event = new char [ strlen(eventstr) + 1 ];
      strcpy(this->event, eventstr);
      this->eventkey = this->event;
    }
  }
}

// const char * ScXMLTransition::getEventXMLAttr(void) const

void
ScXMLTransition::setCondXMLAttr(const char * condstr)
{
  if (this->cond && this->cond != this->getAttribute("cond")) {
    delete [] this->cond;
  }
  this->cond = NULL;
  if (condstr) {
    this->cond = new char [ strlen(condstr) + 1 ];
    strcpy(this->cond, condstr);
  }
}

// const char * ScXMLTransition::getCondXMLAttr(void) const

void
ScXMLTransition::setTargetXMLAttr(const char * targetstr)
{
  if (this->target && this->target != this->getAttribute("target")) {
    delete [] this->target;
  }
  this->target = NULL;
  if (targetstr) {
    this->target = new char [ strlen(targetstr) + 1 ];
    strcpy(this->target, targetstr);
  }
}

// const char * ScXMLTransition::getTargetXMLAttr(void) const

void
ScXMLTransition::setAnchorXMLAttr(const char * anchorstr)
{
  if (this->anchor && this->anchor != this->getAttribute("anchor")) {
    delete [] this->anchor;
  }
  this->anchor = NULL;
  if (anchorstr) {
    this->anchor = new char [ strlen(anchorstr) + 1 ];
    strcpy(this->anchor, anchorstr);
  }
}

// const char * ScXMLTransition::getAnchorXMLAttr(void) const

// *************************************************************************

/*!
  Returns whether this is a conditionless SCXML transition or not.

  A conditionless transition should always be taken.
*/

SbBool
ScXMLTransition::isConditionLess(void) const
{
  static const SbName globallkey("*");
  return (((this->eventkey == SbName::empty()) ||
           (this->eventkey == globallkey)) &&
          (this->cond == NULL));
}

/*!
  Returns whether this is a transition without a target setting or not.

  When a targetless transition is taken, the state machine's state
  does not change. This differs from setting the target to its own
  state, which will cause the state machine to leave the state and
  reenter it again.
*/

SbBool
ScXMLTransition::isTargetLess(void) const
{
  return (this->target == NULL);
}

// *************************************************************************

/*!
  This function returns TRUE if the transition matches the given \a eventobj
  object and FALSE otherwise.
*/
SbBool
ScXMLTransition::isEventMatch(const ScXMLEvent * eventobj) const
{
  static const SbName globallkey("*");
  assert(eventobj);
  SbName eventid = eventobj->getIdentifier();

  if ((this->eventkey == SbName::empty()) ||
      (this->eventkey == globallkey)) return TRUE;

  if (!this->needprefixmatching) {
    return (eventid == this->eventkey);
  }

  if (this->eventkey == eventid) return TRUE;

  const int keylen = strlen(this->eventkey.getString());
  if (keylen < static_cast<int>(strlen(eventid.getString()))) {
    if ((eventid.getString()[keylen] == '.') &&
        strncmp(this->eventkey.getString(), eventid.getString(), keylen) == 0) {
      return TRUE; // this->event matches up to eventobj token separator
    }
  }

  return FALSE;
}

// *************************************************************************
// executable content

SCXML_LIST_OBJECT_API_IMPL(ScXMLTransition, ScXMLInvoke, invokelist, Invoke, Invokes);

void
ScXMLTransition::invoke(ScXMLStateMachine * statemachine)
{
  std::vector<ScXMLInvoke *>::const_iterator it = this->invokelist.begin();
  while (it != this->invokelist.end()) {
    (*it)->invoke(statemachine);
    ++it;
  }
}
