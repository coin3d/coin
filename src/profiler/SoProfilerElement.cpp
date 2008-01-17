#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_SCENE_PROFILING

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
  \class SoProfilerElement SoProfilerElement.h Inventor/annex/Profiler/SoProfilerElement.h
  \brief The SoProfilerElement element class is for registering statistics during scene graph traversals.
*/

// *************************************************************************

#include "profiler/SoProfilerElement.h"

#include <assert.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>

// *************************************************************************

SO_ELEMENT_SOURCE(SoProfilerElement);

void
SoProfilerElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoProfilerElement, SoElement);
}

/*!
  This is the required method of fetching the SoProfilerElement instance.
*/
SoProfilerElement *
SoProfilerElement::get(SoState * state)
{
  assert(state);
  if (!state->isElementEnabled(SoProfilerElement::classStackIndex)) {
    assert(!"SoProfilerElement not enabled");
  }

  SoElement * elt = state->getElementNoPush(SoProfilerElement::classStackIndex);
  return static_cast<SoProfilerElement *>(elt);
}

SoProfilerElement::~SoProfilerElement(void)
{
  this->clear(); // must dealloc nodemap entries
}

// *************************************************************************

void
SoProfilerElement::push(SoState *)
{
  // NOTE: if this triggers, someone has probably used
  // SoState->getElement(stackindex) to fetch the element instead of
  // using the required SoProfilerElemenet::get(SoState *)
  assert(!"programming error: SoProfilerElement should not be stack-pushed");
  SoDebugError::post("SoProfilerElement::push",
                     "programming error: SoProfilerElement should not be stack-pushed");
}

void
SoProfilerElement::pop(SoState *, const SoElement *)
{
  // NOTE: if this triggers, someone has probably used
  // SoState->getElement(stackindex) to fetch the element instead of
  // using the required SoProfilerElemenet::get(SoState *)
  assert(!"programming error: SoProfilerElement should not be stack-pushed");
  SoDebugError::post("SoProfilerElement::pop",
                     "programming error: SoProfilerElement should not be stack-pushed");
}

// *************************************************************************

SbBool
SoProfilerElement::matches(const SoElement * element) const
{
  // FIXME: implement properly.  -mortene.
  // (just compare the data)
  return FALSE;
}

SoElement *
SoProfilerElement::copyMatchInfo(void) const
{
  SoProfilerElement * elem =
    static_cast<SoProfilerElement *>(this->getTypeId().createInstance());
  // FIXME: copy data
  return elem;
}

// *************************************************************************

void
SoProfilerElement::clear(void)
{
  this->data.clear();
  this->traversal_start_time = SbTime::zero();
}

// *************************************************************************

void
SoProfilerElement::setTimingProfile(SoNode * node, SbTime t, SoNode * parent)
{
  assert(node);
  this->data.accumulateTraversalTime(parent ? parent->getTypeId() : SoType::badType(),
                                     node->getTypeId(), t);

  if (parent == NULL) return;
  this->data.setChildTiming(parent, node, t);
}

// *************************************************************************

void
SoProfilerElement::setHasGLCache(SoNode * node, SbBool hascache)
{
  this->data.setHasGLCache(node, hascache);
}

void
SoProfilerElement::startTraversalClock()
{
  this->traversal_start_time = SbTime::getTimeOfDay();
}

SbTime 
SoProfilerElement::timeSinceTraversalStart()
{
  return SbTime::getTimeOfDay() - this->traversal_start_time;
}

const SbProfilingData &
SoProfilerElement::getProfilingData() const
{
  return this->data;
}

// *************************************************************************

#endif // HAVE_SCENE_PROFILING
