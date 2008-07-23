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

#include <Inventor/scxml/ScXMLState.h>

#include <assert.h>
#include <string.h>
#include <algorithm>
#include <vector>

#include <boost/scoped_ptr.hpp>

#include <Inventor/C/tidbits.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLOnExit.h>
#include <Inventor/scxml/ScXMLOnEntry.h>
#include <Inventor/scxml/ScXMLTransition.h>
#include <Inventor/scxml/ScXMLInitial.h>
#include <Inventor/scxml/ScXMLFinal.h>
#include <Inventor/scxml/ScXMLHistory.h>
#include <Inventor/scxml/ScXMLAnchor.h>
#include <Inventor/scxml/ScXMLInvoke.h>

#include "scxml/ScXMLCommonP.h"

// *************************************************************************

/*!
  \class ScXMLState ScXMLState.h Inventor/scxml/ScXMLState.h
  \brief Implementation of the &lt;state&gt; and &lt;parallel&gt; SCXML elements.


  \since Coin 3.0
  \ingroup scxml
*/

class ScXMLState::PImpl {
public:
  boost::scoped_ptr<ScXMLOnEntry> onentryptr;
  boost::scoped_ptr<ScXMLOnExit> onexitptr;
  std::vector<ScXMLTransition *> transitionlist;
  boost::scoped_ptr<ScXMLInitial> initialptr;
  std::vector<ScXMLState *> statelist;
  std::vector<ScXMLState *> parallellist;
  std::vector<ScXMLFinal *> finallist;
  std::vector<ScXMLHistory *> historylist;
  std::vector<ScXMLAnchor *> anchorlist;
  // datamodel
  boost::scoped_ptr<ScXMLInvoke> invokeptr;

  PImpl(void)
    : onentryptr(NULL), onexitptr(NULL), initialptr(NULL), invokeptr(NULL)
  {
  }

  ~PImpl(void)
  {
    {
      std::vector<ScXMLTransition *>::iterator it = this->transitionlist.begin();
      while (it != this->transitionlist.end()) {
        delete *it;
        ++it;
      }
      this->transitionlist.clear();
    }

    {
      std::vector<ScXMLState *>::iterator it = this->statelist.begin();
      while (it != this->statelist.end()) {
        delete *it;
        ++it;
      }
      this->statelist.clear();
    }

    {
      std::vector<ScXMLState *>::iterator it = this->parallellist.begin();
      while (it != this->parallellist.end()) {
        delete *it;
        ++it;
      }
      this->parallellist.clear();
    }

    {
      std::vector<ScXMLFinal *>::iterator it = this->finallist.begin();
      while (it != this->finallist.end()) {
        delete *it;
        ++it;
      }
      this->finallist.clear();
    }

    {
      std::vector<ScXMLHistory *>::iterator it = this->historylist.begin();
      while (it != this->historylist.end()) {
        delete *it;
        ++it;
      }
      this->historylist.clear();
    }

    {
      std::vector<ScXMLAnchor *>::iterator it = this->anchorlist.begin();
      while (it != this->anchorlist.end()) {
        delete *it;
        ++it;
      }
      this->anchorlist.clear();
    }
  }
};

#define PRIVATE(obj) ((obj)->pimpl)

SCXML_OBJECT_SOURCE(ScXMLState);

/*!
*/
void
ScXMLState::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLState, ScXMLObject, SCXML_DEFAULT_NS, "state");
}

// *************************************************************************

ScXMLState::ScXMLState(void)
  : isparallel(FALSE), istask(FALSE),
    id(NULL), src(NULL), task(NULL)
{
}

ScXMLState::~ScXMLState(void)
{
  this->setIdAttribute(NULL);
  this->setSrcAttribute(NULL);
  this->setTaskAttribute(NULL);
}

// *************************************************************************

void
ScXMLState::setIsParallel(SbBool isparallelarg)
{
  this->isparallel = isparallelarg;
}

SbBool
ScXMLState::isParallel(void) const
{
  return this->isparallel;
}

// *************************************************************************

void
ScXMLState::setIdAttribute(const char * idstr)
{
  if (this->id && this->id != this->getXMLAttribute("id")) {
    delete [] this->id;
  }
  this->id = NULL;
  if (idstr) {
    this->id = new char [ strlen(idstr) + 1 ];
    strcpy(this->id, idstr);
  }
}

// const char * ScXMLState::getIdAttribute(void) const

void
ScXMLState::setSrcAttribute(const char * srcstr)
{
  if (this->src && this->src != this->getXMLAttribute("src")) {
    delete [] this->src;
  }
  this->src = NULL;
  if (srcstr) {
    this->src = new char [ strlen(srcstr) + 1 ];
    strcpy(this->src, srcstr);
  }
}

// const char * ScXMLState::getSrcAttribute(void) const

void
ScXMLState::setTaskAttribute(const char * taskstr)
{
  if (this->task && this->task != this->getXMLAttribute("task")) {
    delete [] this->task;
  }
  this->task = NULL;
  this->istask = FALSE;
  if (taskstr) {
    this->task = new char [ strlen(taskstr) + 1 ];
    strcpy(this->task, taskstr);
    // acceptable truth-true values for boolean argument:
    if (strlen(this->task) == 4 &&
        coin_strncasecmp(this->task, "true", 4) == 0) {
      this->istask = TRUE;
    }
    else if (strcmp(this->task, "1") == 0) {
      this->istask = TRUE;
    }
  }
}

// const char * ScXMLState::getTaskAttribute(void) const

SbBool
ScXMLState::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->id = const_cast<char *>(this->getXMLAttribute("id"));
  this->src = const_cast<char *>(this->getXMLAttribute("src"));
  this->task = NULL;
  this->setTaskAttribute(this->getXMLAttribute("task"));

  if (!this->id) { return FALSE; }

  return TRUE;
}

// *************************************************************************

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLOnEntry, PRIVATE(this)->onentryptr, OnEntry);

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLOnExit, PRIVATE(this)->onexitptr, OnExit);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLTransition, PRIVATE(this)->transitionlist, Transition, Transitions);

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLInitial, PRIVATE(this)->initialptr, Initial);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLState, PRIVATE(this)->statelist, State, States);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLState, PRIVATE(this)->parallellist, Parallel, Parallels);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLFinal, PRIVATE(this)->finallist, Final, Finals);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLHistory, PRIVATE(this)->historylist, History, Histories);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLAnchor, PRIVATE(this)->anchorlist, Anchor, Anchors);

// datamodel

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLInvoke, PRIVATE(this)->invokeptr, Invoke);

void
ScXMLState::invoke(ScXMLStateMachine * statemachine)
{
  if (PRIVATE(this)->invokeptr.get()) {
    PRIVATE(this)->invokeptr->invoke(statemachine);
  }
}

// *************************************************************************

/*!
  Returns TRUE if this is an "atomic state", which means that it has no
  sub-states but contains executable content.
*/
SbBool
ScXMLState::isAtomicState(void) const
{
  return ((PRIVATE(this)->statelist.size() == 0) &&
          (PRIVATE(this)->parallellist.size() == 0) &&
          (PRIVATE(this)->invokeptr.get() != NULL));
}

/*!
  Returns TRUE if this state was tagged as a "task".  "Tasks" will cause
  state change callbacks to be invoked in the ScXMLStateMachine as they
  are entered and exited, but other states will not.

  \sa ScXMLStateMachine::addStateChangeCallback
*/
SbBool
ScXMLState::isTask(void) const
{
  return this->istask;
}

#undef PRIVATE

