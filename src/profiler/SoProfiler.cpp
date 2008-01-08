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
  \page profiling Profiling support in Coin

  <h>Enabling profiling in Coin</h>
  To enable profiling in Coin, set the environment variable COIN_PROFILER
  to any positive value. When profiling is enabled, Coin will gather
  profiling data during every scene graph traversal. 

  <h>Read the profiling data</h>
  The SoProfilerStats node can be used to fetch the profiling data in the
  scene graph. If it is positioned anywhere in the scene graph, the
  fields of the node will be updated every time SoGLRenderedAction is 
  applied to the scene graph, with profiling data gathered from every 
  traversal through the scene graph, since the last SoGLRenderAction, up 
  to the point where SoProfilerStats is located. Depending of how you
  wish to use the data, either attach sensors to the fields, or connect
  the the fields on other coin nodes to the fields on SoProfilerStats.

  <h>Using SoProfilerTopKit</a>
  
  <h>Creating a custom profiler overlay</h>

  <h>Enabling the default profiling display</h>

  \since Coin 3.0
*/

/**************************************************************************/

#include <Inventor/annex/Profiler/SoProfiler.h>

#include "profiler/SoProfilerElement.h"
#include <Inventor/annex/Profiler/nodekits/SoNodeVisualize.h>
#include <Inventor/annex/Profiler/nodekits/SoProfilerOverlayKit.h>
#include <Inventor/annex/Profiler/nodekits/SoProfilerTopKit.h>
#include <Inventor/annex/Profiler/nodekits/SoScrollingGraphKit.h>
#include <Inventor/annex/Profiler/nodekits/SoProfilerVisualizeKit.h>
#include <Inventor/annex/Profiler/nodes/SoProfilerStats.h>
#include <Inventor/annex/Profiler/engines/SoProfilerTopEngine.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include "misc/SoDBP.h"

// *************************************************************************

namespace {
  SbBool initialized = FALSE;

  SbBool profiler_active = FALSE;
  SbBool overlay_active = FALSE;
  SbBool profiler_enabled = FALSE;
};

void
SoProfiler::init(void)
{
  if (initialized) return;

  SoNodeKit::init();
  SoProfilerOverlayKit::initClass();
  SoProfilerVisualizeKit::initClass();
  SoProfilerTopKit::initClass();
  SoProfilerStats::initClass();
  SoProfilerTopEngine::initClass();
  SoScrollingGraphKit::initClass();
  SoNodeVisualize::initClass();

  
  profiler_active = TRUE;
  profiler_enabled = TRUE;

  const char * env = coin_getenv(SoDBP::EnvVars::COIN_PROFILER_OVERLAY);
  if (env && (atoi(env) > 0)) {
    overlay_active = TRUE;
  }

  initialized = TRUE;
}

SbBool
SoProfiler::isActive(void)
{
  return profiler_enabled && profiler_active;
}

SbBool
SoProfiler::overlayActive(void)
{
  return profiler_enabled && overlay_active;
}

void
SoProfiler::enable(SbBool enable)
{
  if (!initialized) {
    assert(!"SoProfiler module not initialized");
    SoDebugError::post("SoProfiler::enable", "module not initialized");
    return;
  }
  profiler_enabled = enable;
}

SbBool
SoProfiler::isEnabled(void)
{
  return profiler_enabled;
}
