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
  \class SoProfilerElement SoProfilerElement.h Inventor/annex/Profiler/SoProfilerElement.h
  \brief The SoProfilerElement element class is for registering statistics during scene graph traversals.

  \ingroup profiler
*/

// *************************************************************************

#include <Inventor/annex/Profiler/elements/SoProfilerElement.h>

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
    return NULL;
  }

  SoElement * elt = state->getElementNoPush(SoProfilerElement::classStackIndex);
  return static_cast<SoProfilerElement *>(elt);
}

SoProfilerElement::~SoProfilerElement(void)
{
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
  if (element == this) return TRUE;
  const SoProfilerElement * pelement =
    static_cast<const SoProfilerElement *>(element);
  if (pelement->getProfilingData() == this->getProfilingData()) return TRUE;
  return FALSE;
}

SoElement *
SoProfilerElement::copyMatchInfo(void) const
{
  SoProfilerElement * elem =
    static_cast<SoProfilerElement *>(this->getTypeId().createInstance());
  elem->getProfilingData() = this->getProfilingData();
  return elem;
}

// *************************************************************************

/*
void
SoProfilerElement::setTimingProfile(SoNode * node, SbTime t, SoNode * parent)
{
  assert(node);
  this->data.accumulateTraversalTime(parent ? parent->getTypeId() : SoType::badType(),
                                     node->getTypeId(), t);

  if (parent == NULL) return;
  this->data.setChildTiming(parent, node, t);
}
*/

// *************************************************************************

/*
void
SoProfilerElement::setHasGLCache(SoNode * node, SbBool hascache)
{
  this->data.setHasGLCache(node, hascache);
}

void
SoProfilerElement::startTraversalClock()
{
  this->data.setActionStartTime(SbTime::getTimeOfDay());
}

SbTime
SoProfilerElement::timeSinceTraversalStart()
{
  if (this->data.getActionStartTime() == SbTime::zero())
    return SbTime::zero();
  return SbTime::getTimeOfDay() - this->data.getActionStartTime();
}
*/

const SbProfilingData &
SoProfilerElement::getProfilingData(void) const
{
  return this->data;
}

SbProfilingData &
SoProfilerElement::getProfilingData(void)
{
  return this->data;
}

// *************************************************************************
