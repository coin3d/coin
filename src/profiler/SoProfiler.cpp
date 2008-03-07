#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

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

  Sorry about the state of this documentation - it is still in its infancy.

  <h>Enabling profiling in Coin</h>

  To enable profiling in Coin, set the environment variable COIN_PROFILER
  to any positive value. When profiling is enabled, Coin will gather
  profiling data during every scene graph traversal.  Amongst the profiling
  data gathered, you have traversal timings, whether nodes are cached,
  and whether nodes are culled.

  <h>Enabling the default profiling display</h>

  To get some profiling data shown on the screen, you also need to set the
  COIN_PROFILER_OVERLAY environment variable to something other than 0.
  This will give you the default profiling graphics, which shows a top-list
  of node timings categorized by node types, a scrolling graph of action
  traversal timings, and a scene graph navigator for closer scene graph
  inspection.

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

  \since Coin 3.0
*/

/**************************************************************************/

#include <Inventor/annex/Profiler/SoProfiler.h>
#include "profiler/SoProfilerP.h"

#include <string>
#include <vector>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoType.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodekits/SoNodeKit.h>

#include <Inventor/annex/Profiler/elements/SoProfilerElement.h>
#include <Inventor/annex/Profiler/nodekits/SoNodeVisualize.h>
#include <Inventor/annex/Profiler/nodekits/SoProfilerOverlayKit.h>
#include <Inventor/annex/Profiler/nodekits/SoProfilerTopKit.h>
#include <Inventor/annex/Profiler/nodekits/SoScrollingGraphKit.h>
#include <Inventor/annex/Profiler/nodekits/SoProfilerVisualizeKit.h>
#include <Inventor/annex/Profiler/nodes/SoProfilerStats.h>
#include <Inventor/annex/Profiler/engines/SoProfilerTopEngine.h>

#include "misc/SoDBP.h"

// *************************************************************************

namespace {

  namespace profiler {
    static SbBool initialized = FALSE;

    static SbBool enabled = FALSE;
    static SbBool active = FALSE;
    static SbBool syncgl = FALSE;

    namespace overlay {
      static SbBool active = FALSE;
      static float redraw_rate = -1.0f;
    };

  };

  void
  tokenize(const std::string & input, const std::string & delimiters, std::vector<std::string> & tokens, int count = -1)
  {
    std::string::size_type last_pos = 0, pos = 0;
    while (TRUE) {
      --count;
      pos = input.find_first_of(delimiters, last_pos);
      if ((pos == std::string::npos) || (count == 0)) {
        tokens.push_back(input.substr(last_pos));
        break;
      } else {
        tokens.push_back(input.substr(last_pos, pos - last_pos));
        last_pos = pos + 1;
      }
    }
  }

} // namespace


/*!
  Initializes the Coin scene graph profiling subsystem.
*/

void
SoProfiler::init(void)
{
#ifdef HAVE_SCENE_PROFILING

  if (profiler::initialized) return;

  SoNodeKit::init();
  SoProfilerOverlayKit::initClass();
  SoProfilerVisualizeKit::initClass();
  SoProfilerTopKit::initClass();
  SoProfilerStats::initClass();
  SoProfilerTopEngine::initClass();
  SoScrollingGraphKit::initClass();
  SoNodeVisualize::initClass();

  profiler::active = TRUE;
  profiler::enabled = TRUE;

  SoProfilerP::parseCoinProfilerOverlayVariable();

  profiler::initialized = TRUE;
#endif // HAVE_SCENE_PROFILING
}

/*!
  Returns whether profiling is active or not.
*/

SbBool
SoProfiler::isActive(void)
{
  return profiler::enabled && profiler::active;
}

/*!
  Returns whether profiling is shown in an overlay fashion on the GL canvas
  or not.
*/

SbBool
SoProfiler::isOverlayActive(void)
{
  return profiler::enabled && profiler::overlay::active;
}

void
SoProfiler::enable(SbBool enable)
{
#ifdef HAVE_SCENE_PROFILING
  if (!profiler::initialized) {
    assert(!"SoProfiler module not initialized");
    SoDebugError::post("SoProfiler::enable", "module not initialized");
    return;
  }
  profiler::enabled = enable;
#endif // HAVE_SCENE_PROFILING
}

/*!
  Returns whether profiling is enabled or not.
*/

SbBool
SoProfiler::isEnabled(void)
{
  return profiler::enabled;
}

#ifdef HAVE_SCENE_PROFILING

SbBool
SoProfilerP::shouldContinuousRender(void)
{
  return profiler::overlay::redraw_rate != -1.0f;
}

float
SoProfilerP::getContinuousRenderDelay(void)
{
  return profiler::overlay::redraw_rate;
}

SbBool
SoProfilerP::shouldSyncGL(void)
{
  return profiler::syncgl;
}

void 
SoProfilerP::parseCoinProfilerVariable(void)
{
  // variable COIN_PROFILER
  // - on
  // - syncgl - implies on
  // - [nocaching - implies on] // todo

  const char * env = coin_getenv(SoDBP::EnvVars::COIN_PROFILER);
  if (env == NULL) return;
  std::vector<std::string> parameters;
  tokenize(env, ":", parameters);
  if ((parameters.size() == 1) &&
      (parameters[0].find_first_not_of("+-0123456789 \t") == std::string::npos)) {
    // just have a numeral value (or nothing) - old semantics
    profiler::enabled = atoi(parameters[0].data()) > 0 ? TRUE : FALSE;
  }
  else if (parameters.size() > 0) {
    std::vector<std::string>::iterator it = parameters.begin();
    while (it != parameters.end()) {
      if ((*it).compare("on") == 0) {
        profiler::enabled = TRUE;
      }
      else if ((*it).compare("off") == 0) {
        profiler::enabled = FALSE;
      }
      else if ((*it).compare("syncgl") == 0) {
        profiler::enabled = TRUE;
        profiler::syncgl = TRUE;
      }
      else {
        SoDebugError::postWarning("SoProfilerP::parseCoinProfilerVariable",
                                  "invalid token '%s'", (*it).data());
      }
      ++it;
    }
  }
}

void
SoProfilerP::parseCoinProfilerOverlayVariable(void)
{
  const char * env = coin_getenv(SoDBP::EnvVars::COIN_PROFILER_OVERLAY);
  if (env == NULL) return;
  std::vector<std::string> parameters;
  tokenize(env, ":", parameters);

  if (parameters.size() == 1 && atoi(parameters[0].data()) > 0) {
    // old behaviour, default setup
    profiler::overlay::active = TRUE;
    // SoDebugError::postInfo("SoProfiler::initialize", "default old behaviour parsing");
  }
  else if (parameters.size() > 0) {
    // SoDebugError::postInfo("SoProfiler::initialize", "new tokenized parsing");
    
    for (std::vector<std::string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
      profiler::overlay::active = TRUE;
      
      std::vector<std::string> param, subargs;
      tokenize(*it, "=", param, 2);
      if (param.size() > 1) {
        tokenize(param[1], ",", subargs);
      }

      // configure if the profiling system should continuously
      // schedule redraws to get more "live" data on the overlay
      if (param[0].compare("autoredraw") == 0) {
        if (param.size() == 1) {
          // no argument ->continuous redraws
          profiler::overlay::redraw_rate = 0.0f;
        } else {
          // argument decides redraw-delay-rate
          profiler::overlay::redraw_rate = static_cast<float>(atof(param[1].data()));
        }
        if (profiler::overlay::redraw_rate < 0.0f) {
          profiler::overlay::redraw_rate = -1.0f; // -1 exact means no redraws
        }
      }
      
      // configure if and how we should display toplists
      else if (param[0].compare("toplist") == 0) {
        enum TopListType { NODE_TYPE, NODE_NAME, ACTION_TYPE, INVALID } toplisttype = INVALID;
        if (subargs.size() > 0 && subargs[0].compare("nodes") == 0) {
          // top-list based on node type statistics
          toplisttype = NODE_TYPE;
        }
        else if (subargs.size() > 0 && subargs[0].compare("names") == 0) {
          // top-list based on named node statistics
          toplisttype = NODE_NAME;
        }
        else if (subargs.size() > 0 && subargs[0].compare("actions") == 0) {
          // top-list based on action type statistics
          toplisttype = ACTION_TYPE;
        }
        else if (subargs.size() > 0) {
          // default to node type
          toplisttype = INVALID;
        }
        if (toplisttype != INVALID) {
          std::vector<std::string>::iterator it = subargs.begin();
          ++it;
          while (it != subargs.end()) {
            std::vector<std::string> subarg;
            tokenize((*it).data(), "=", subarg, 2);
            if (subarg[0].compare("header") == 0) {
              // display header on toplist
            }
            else if (subarg[0].compare("lines") == 0) {
              // needs numeric argument
              if (subarg.size() == 2) {
                int lines = atoi(subarg[1].data());
              } else {
                // error: toplist.lines needs a numeric argument`
              }
            }
            else if ((subarg[0].compare("action") == 0) && (toplisttype != ACTION_TYPE)) {
              // need name of action
              if (subarg.size() == 2) {
                SoType actiontype = SoType::fromName(subarg[1].data());
                if (actiontype.isDerivedFrom(SoAction::getClassTypeId())) {
                } else {
                  // error - must specify action type
                }
              } else {
                // error: toplist.action needs an SoAction-derived type name
              }
            }
            else if ((subarg[0].compare("inclusive") == 0) && (toplisttype != ACTION_TYPE)) {
              // no argument
            }
            else if ((subarg[0].compare("exclusive") == 0) && (toplisttype != ACTION_TYPE)) {
              // no argument
            }
            ++it;
          }
        } else {
        }
      }

      // configure charts
      else if (param[0].compare("graph") == 0) {
      }
      
      // configure scene graph view
      else if (param[0].compare("sceneview") == 0) {
      }


      // fallthrough
      else {
        SoDebugError::postWarning("SoProfiler::initialize",
                                  "Unknown COIN_PROFILER_OVERLAY parameter '%s'.",
                                  param[0].data());
      }
    }

    profiler::overlay::active = TRUE;
  }
  else {
    // env variable is empty - don't activate overlay parts
  }
}

#endif // HAVE_SCENE_PROFILING
