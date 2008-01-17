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
#include <Inventor/SoType.h>
#include <Inventor/lists/SbList.h>

class SoNode;

class SbProfilingData {
public:
  typedef void * NodeKey; // void * because it's not to be dereferenced
  typedef int16_t NodeTypeKey;
  typedef const char * NodeNameKey;

  SbProfilingData();
  ~SbProfilingData();

  SbProfilingData & operator+=(const SbProfilingData &);

  void clear(void);

  SbTime getChildTiming(SoNode * parent, SoNode * child) const;
  void setChildTiming(SoNode * parent, SoNode * child, const SbTime & time);

  SbBool hasGLCache(SoNode * nodeptr) const;
  void setHasGLCache(SoNode * nodeptr, SbBool cached);

  void accumulateTraversalTime(SoType parenttype, SoType childtype, SbTime t);

  void getTraversalStatsForTypesKeyList(SbList<NodeTypeKey> & keys_out) const;
  void getTraversalStatsForType(NodeTypeKey type,
                                SbTime & total, SbTime & max, uint32_t & count) const;

  void getTraversalStatsForNamesKeyList(SbList<NodeNameKey> & keys_out) const;
  void getTraversalStatsForName(NodeNameKey name,
                                SbTime & total, SbTime & max, uint32_t & count) const;

private:
  SbProfilingData(const SbProfilingData & rhs) {}; // disabled
  SbProfilingData & operator = (const SbProfilingData &rhs) {}; // disabled

  // ***********************************************************************
  // semi-hierarchical map on node instances

  struct ChildProfile {
    SbTime traversaltime;
  };

  typedef std::map<NodeKey, ChildProfile> ChildTimingMap;

  struct NodeProfile {
    ChildTimingMap child_map;
    SbBool has_glcache;
  };

  typedef std::map<NodeKey, NodeProfile *> NodeProfileMap;

  NodeProfileMap profile_map;

  // ***********************************************************************
  // map by type

  struct NodeTypeProfile {
    SbTime traversaltimetotal;
    SbTime traversaltimemax; // largest in accumulation
    uint32_t count;
  };

  typedef std::map<NodeTypeKey, NodeTypeProfile *> TypeProfileMap;

  TypeProfileMap typeaccum_map;

  // ***********************************************************************
  // map by name

  struct NodeNameProfile {
    SbTime traversaltimetotal;
    SbTime traversaltimemax; // largest in accumulation
    uint32_t count;
  };

  typedef std::map<NodeNameKey, NodeNameProfile *> NameProfileMap;

  NameProfileMap nameaccum_map;

}; // SbProfilingData

#endif // HAVE_SCENE_PROFILING

#endif // !COIN_SBPROFILINGDATA_H
