#ifndef COIN_SBPROFILINGDATA_H
#define COIN_SBPROFILINGDATA_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif // COIN_INTERNAL

#include <map>

#include <Inventor/SbTime.h>

class SoNode;

class SbProfilingData {
public:
  SbProfilingData();

  SbProfilingData & operator+=(const SbProfilingData &);

  ~SbProfilingData();
  void clear();

  SbTime getChildTiming(SoNode *, SoNode *) const;
  void setChildTiming(SoNode *, SoNode *, const SbTime &);
  SbBool hasGLCache(SoNode *) const;
  void setHasGLCache(SoNode *, SbBool);

private:
  SbProfilingData(const SbProfilingData &) {}; // disabled
  SbProfilingData & operator=(const SbProfilingData &) {}; // disabled

  typedef std::map<SoNode *, SbTime> ChildTimingMap;
  
  struct NodeProfile {
    ChildTimingMap child_map;
    SbBool has_glcache;
  };
  
  typedef std::map<SoNode *, NodeProfile *> NodeProfileMap;
   
  NodeProfileMap profile_map;
};

#endif // HAVE_SCENE_PROFILING

#endif // !COIN_SBPROFILINGDATA_H
