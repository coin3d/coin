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

#include "profiler/SoProfilerStatsElement.h"

#include <Inventor/annex/Profiler/nodes/SoProfilerStats.h>
#include <boost/intrusive_ptr.hpp>

class SoProfilerStatsElementP {
public:

  boost::intrusive_ptr<SoProfilerStats> stats;
};

#define PRIVATE(x) ((x)->pimpl)

SO_ELEMENT_SOURCE(SoProfilerStatsElement);

void SoProfilerStatsElement::initClass()
{
   SO_ELEMENT_INIT_CLASS(SoProfilerStatsElement, SoElement);
}


SoProfilerStatsElement *
SoProfilerStatsElement::get(SoState * state)
{
  SoProfilerStatsElement * elt = static_cast<SoProfilerStatsElement *>(state->getElementNoPush(SoProfilerStatsElement::classStackIndex));
  assert(elt && "ProfilerStatsElement not in state - did Someone[tm] forget to add it?");
  return elt;
}

void
SoProfilerStatsElement::set(SoState * state, SoProfilerStats * stats)
{
  PRIVATE(SoProfilerStatsElement::get(state))->stats = stats;
}

SoProfilerStatsElement::~SoProfilerStatsElement()
{
}

const SoProfilerStats * SoProfilerStatsElement::stats() const
{
  return PRIVATE(this)->stats.get();
}  

SbBool SoProfilerStatsElement::matches(const SoElement*) const
{
  return FALSE;
}

SoElement * SoProfilerStatsElement::copyMatchInfo() const
{  
  SoProfilerStatsElement * elem =
    static_cast<SoProfilerStatsElement *>(this->getTypeId().createInstance());
  PRIVATE(elem)->stats = PRIVATE(this)->stats;
  return elem;
}

#endif // HAVE_SCENE_PROFILING
