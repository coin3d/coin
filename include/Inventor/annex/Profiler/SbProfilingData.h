#ifndef COIN_SBPROFILINGDATA_H
#define COIN_SBPROFILINGDATA_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbTime.h>
#include <Inventor/SoType.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/tools/SbPimplPtr.h>

class SoNode;
class SoPath;
class SoFullPath;
class SbProfilingDataP;

typedef void * SbProfilingNodeKey; // void since it should not be dereferenced
typedef int16_t SbProfilingNodeTypeKey;
typedef const char * SbProfilingNodeNameKey;

class COIN_DLL_API SbProfilingData {
public:
  SbProfilingData(void);
  SbProfilingData(const SbProfilingData & rhs);
  ~SbProfilingData(void);

  void setActionType(SoType actiontype);
  SoType getActionType(void) const;

  void setActionStartTime(SbTime starttime);
  SbTime getActionStartTime(void) const;
  void setActionStopTime(SbTime stoptime);
  SbTime getActionStopTime(void) const;
  SbTime getActionDuration(void) const;

  // profiling setters
  enum FootprintType {
    MEMORY_SIZE,
    VIDEO_MEMORY_SIZE
  };

  enum NodeFlag {
    GL_CACHED_FLAG,
    CULLED_FLAG
  };

  enum NodeDataQueryFlags {
    INCLUDE_CHILDREN = 0x01
  };

  void setNodeTiming(const SoPath * path, SbTime timing);
  void setNodeTiming(int idx, SbTime timing);
  void preOffsetNodeTiming(int idx, SbTime timing);
  SbTime getNodeTiming(const SoPath * path, unsigned int queryflags = 0) const;
  SbTime getNodeTiming(int idx, unsigned int queryflags = 0) const;

  void setNodeFootprint(const SoPath * path, FootprintType type,
                        size_t footprint);
  void setNodeFootprint(int idx, FootprintType type, size_t footprint);
  size_t getNodeFootprint(const SoPath * path, FootprintType type,
                          unsigned int queryflags = 0) const;
  size_t getNodeFootprint(int idx, FootprintType type,
                          unsigned int queryflags = 0) const;

  void setNodeFlag(const SoPath * path, NodeFlag flag, SbBool on);
  void setNodeFlag(int idx, NodeFlag flag, SbBool on);
  SbBool getNodeFlag(const SoPath * path, NodeFlag flag) const;
  SbBool getNodeFlag(int idx, NodeFlag flag) const;

  int getIndex(const SoPath * path, SbBool create = FALSE);
  int getParentIndex(int idx) const;

  SoType getNodeType(int idx) const;
  SbName getNodeName(int idx) const;

  int getLongestNameLength(void) const;
  int getLongestTypeNameLength(void) const;

  int getNumNodeEntries(void) const;

  typedef void SbProfilingDataCB(void * userdata, const SbProfilingData & data, const SbList<SoNode *> & pointers, SbList<int> & childindices, int idx);
  void reportAll(SbProfilingDataCB * callback, void * userdata) const;

  // read out pre-categorized data
  void getStatsForTypesKeyList(SbList<SbProfilingNodeTypeKey> & keys_out) const;
  void getStatsForType(SbProfilingNodeTypeKey type,
                       SbTime & total, SbTime & max, uint32_t & count) const;

  void getStatsForNamesKeyList(SbList<SbProfilingNodeNameKey> & keys_out) const;
  void getStatsForName(SbProfilingNodeNameKey name,
                       SbTime & total, SbTime & max, uint32_t & count) const;

  // statistics management
  void reset(void);

  SbProfilingData & operator = (const SbProfilingData & rhs);
  SbProfilingData & operator += (const SbProfilingData & rhs);

  int operator == (const SbProfilingData & rhs) const;
  int operator != (const SbProfilingData & rhs) const;


  // debug - return profiling data overhead
  size_t getProfilingDataSize(void) const;

protected:
  SoType actionType;
  SbTime actionStartTime;
  SbTime actionStopTime;

private:
  SbPimplPtr<SbProfilingDataP> pimpl;

  void constructorInit(void);

  SbBool isPathMatch(const SoFullPath * path, int pathlen, int idx);

  int getIndexCreate(const SoFullPath * path, int pathlen);
  int getIndexNoCreate(const SoPath * path, int pathlen) const;
  int getIndexForwardCreate(const SoFullPath * path, int pathlen,
                            int parentindex);
  int getIndexForwardNoCreate(const SoFullPath * path, int pathlen,
                              int parentindex) const;

}; // SbProfilingData

#endif // !COIN_SBPROFILINGDATA_H
