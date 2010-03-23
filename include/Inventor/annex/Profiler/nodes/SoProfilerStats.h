#ifndef COIN_SOPROFILERSTATS_H
#define COIN_SOPROFILERSTATS_H

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

#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/tools/SbPimplPtr.h>

class SbProfilingData;
class SoSeparator;

class COIN_DLL_API SoProfilerStats : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoProfilerStats);

public:
  static void initClass(void);
  SoProfilerStats(void);

  //fields for storing information on rendering time spent per node type
  SoMFName renderedNodeType;
  SoMFTime renderingTimePerNodeType;
  SoMFTime renderingTimeMaxPerNodeType;
  SoMFUInt32 renderedNodeTypeCount;

  SoMFName profiledAction;
  SoMFTime profiledActionTime;
  SoMFNode separatorsCullRoots;

  SoSFTrigger profilingUpdate;

  // FIXME: below are suggestions for fields exposing future profiling
  // functionality.  -mortene.

  //fields for storing time spent rendering in each node in the scene graph
  //  SoMFNode renderedNode;
  //  SoMFTime renderingTimePerNode;

  //fields for storing time spent handling events in each node in the scene
  //graph
  //  SoMFNode handlingNode;
  //  SoMFTime handlingTimePerNode;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void audioRender(SoAudioRenderAction * action);

  virtual void callback(SoCallbackAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);

  const SbProfilingData & getProfilingData(SoType actiontype) const;

protected:
  virtual ~SoProfilerStats();

  virtual void notify (SoNotList *l);

private:
  SbPimplPtr<class SoProfilerStatsP> pimpl;

  SoProfilerStats(const SoProfilerStats & rhs);
  SoProfilerStats & operator=(const SoProfilerStats & rhs);

}; // SoProfilerStats

#endif // !COIN_SOPROFILERSTATS_H
