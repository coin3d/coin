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

// FIXME: remove this when a hack has been fixed.  -mortene.
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>

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
    SoAction * action = state->getAction();
    assert(FALSE && "SoProfilerElement not enabled");
  }

  SoElement * e = state->getElementNoPush(SoProfilerElement::classStackIndex);
  return static_cast<SoProfilerElement *>(e);
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
  this->grandtotal = SbTime::zero();

  this->typeaccum_map.clear();
  this->nameaccum_map.clear();
  this->namestack.truncate(0);

  this->data.clear();

  this->start_time = SbTime::zero();
}

// *************************************************************************

void
SoProfilerElement::accumulateRenderTime(SoType parenttype, SoType childtype,
                                        SbTime t)
{
  // (note: the input arg 'parenttype' will be SoType::badType() for
  // the root node)

  if (this->typeaccum_map.getNumElements() == 0) {
    // FIXME: is this actually used any more?  -mortene.
    this->grandtotal = SbTime::zero();
  }
  this->grandtotal += t;

  // for hash lookup
  int16_t typekey = childtype.getKey();

  struct NodetypeAccumulations acc;
  SbBool first = ! this->typeaccum_map.get(typekey, acc);

  if (first) {
    acc.rendertime = t;
    acc.rendertimemax = t;
    acc.count = 1;
  }
  else {
    acc.rendertime += t;
    if (t.getValue() > acc.rendertimemax.getValue()) { acc.rendertimemax = t; }
    ++acc.count;
  }

  this->typeaccum_map.put(typekey, acc);


  // subtract from type of parent node, because SoGroup, SoSwitch, etc
  // includes timing from their child nodes:

  if (parenttype == SoType::badType()) { return; } // (happens at root node)

  // for hash lookup
  typekey = parenttype.getKey();
  first = ! this->typeaccum_map.get(typekey, acc);

  if (first) {
    acc.rendertime = -t;
    acc.rendertimemax = SbTime::zero();
    acc.count = 0;
  }
  else {
    // FIXME: rendertimemax will be wrong, i presume.  -mortene.
    acc.rendertime -= t;
  }

  this->typeaccum_map.put(typekey, acc);
}

void
SoProfilerElement::accumulateRenderTime(const char * name, SbTime t)
{
  struct NodenameAccumulations acc;
  SbBool first = ! this->nameaccum_map.get(name, acc);

  if (first) {
    acc.name = name;
    acc.rendertime = t;
    acc.rendertimemax = t;
    acc.count = 1;
  }
  else {
    acc.rendertime += t;
    if (t.getValue() > acc.rendertimemax.getValue()) { acc.rendertimemax = t; }
    ++acc.count;
  }

  this->nameaccum_map.put(name, acc);

}

void
SoProfilerElement::setTimingProfile(SoNode * node, SbTime t, SoNode * parent)
{
  this->accumulateRenderTime(parent ? parent->getTypeId() : SoType::badType(),
                             node->getTypeId(), t);

  if (parent != NULL) {
    this->data.setChildTiming(parent, node, t);
  }

  if (this->namestack.getLength() > 0) {
    this->accumulateRenderTime(this->namestack[this->namestack.getLength()-1], t);
  }
}

/*!
  Sets the name of the current subscenegraph.
  Empty name (nonnamed nodes) is allowed.
*/

void
SoProfilerElement::pushProfilingName(const SbName & name)
{
  static const SbName empty("");
  if (name == empty) return;

  // FIXME: detect and ignore implicit (writeaction-) names here (and
  // for pop()).

  this->namestack.push(name.getString());
}

/*!
  Restores the name of the current subscenegraph to what it was before.
  Empty name (nonnamed nodes) is allowed.
*/

void
SoProfilerElement::popProfilingName(const SbName & name)
{
  static const SbName empty("");
  if (name == empty) return;
  assert(this->namestack.getLength() > 0);
  this->namestack.pop();
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
  this->start_time = SbTime::getTimeOfDay();
}

SbTime 
SoProfilerElement::timeSinceTraversalStart()
{
  return SbTime::getTimeOfDay() - this->start_time;
}

const SbProfilingData &
SoProfilerElement::getProfilingData() const
{
  return data;
}

// *************************************************************************

const SbList<int16_t> &
SoProfilerElement::accumulatedRenderTimeForTypeKeys(void) const
{
  // FIXME: ugly as hell to have a static list here. there are
  // mt-safety issues, for instance. actually straightforward to fix,
  // methinks: just pass in the list as an input argument.  -mortene.
  static SbList<int16_t> l;

  l.truncate(0);
  this->typeaccum_map.makeKeyList(l);
  return l;
}

SbTime
SoProfilerElement::getAccumulatedRenderTimeForType(uint16_t nodetypekey) const
{
  struct NodetypeAccumulations acc;
  const SbBool present = this->typeaccum_map.get(nodetypekey, acc);
  assert(present);
  return acc.rendertime;
}

SbTime
SoProfilerElement::getMaxRenderTimeForType(uint16_t nodetypekey) const
{
  struct NodetypeAccumulations acc;
  const SbBool present = this->typeaccum_map.get(nodetypekey, acc);
  assert(present);
  return acc.rendertimemax;
}

uint32_t
SoProfilerElement::getAccumulatedRenderCountForType(uint16_t nodetypekey) const
{
  struct NodetypeAccumulations acc;
  const SbBool present = this->typeaccum_map.get(nodetypekey, acc);
  assert(present);
  return acc.count;
}

// *************************************************************************
const SbList<const char *> &
SoProfilerElement::accumulatedRenderTimeForNameKeys(void) const
{
  // FIXME: ugly as hell to have a static list here. there are
  // mt-safety issues, for instance. actually straightforward to fix,
  // methinks: just pass in the list as an input argument.  -mortene.
  static SbList<const char *> l;

  l.truncate(0);
  this->nameaccum_map.makeKeyList(l);
  return l;
}

SbTime
SoProfilerElement::getAccumulatedRenderTimeForName(const char * nodename) const
{
  struct NodenameAccumulations acc;
  const SbBool present = this->nameaccum_map.get(nodename, acc);
  assert(present);
  return acc.rendertime;
}

SbTime
SoProfilerElement::getMaxRenderTimeForName(const char * nodename) const
{
  struct NodenameAccumulations acc;
  const SbBool present = this->nameaccum_map.get(nodename, acc);
  assert(present);
  return acc.rendertimemax;
}

uint32_t
SoProfilerElement::getAccumulatedRenderCountForName(const char * nodename) const
{
  struct NodenameAccumulations acc;
  const SbBool present = this->nameaccum_map.get(nodename, acc);
  assert(present);
  return acc.count;
}

// *************************************************************************

#endif // HAVE_SCENE_PROFILING
