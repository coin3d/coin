#ifndef COIN_SOPROFILERSTATS_H
#define COIN_SOPROFILERSTATS_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

  void GLRender(SoGLRenderAction * action) override;
  void handleEvent(SoHandleEventAction * action) override;
  void rayPick(SoRayPickAction * action) override;
  void getBoundingBox(SoGetBoundingBoxAction * action) override;
  void getPrimitiveCount(SoGetPrimitiveCountAction * action) override;
  void audioRender(SoAudioRenderAction * action) override;

  void callback(SoCallbackAction * action) override;
  void getMatrix(SoGetMatrixAction * action) override;
  void pick(SoPickAction * action) override;
  void search(SoSearchAction * action) override;
  void write(SoWriteAction * action) override;

  const SbProfilingData & getProfilingData(SoType actiontype) const;

protected:
  virtual ~SoProfilerStats();

  void notify (SoNotList *l) override;

private:
  SbPimplPtr<class SoProfilerStatsP> pimpl;

  SoProfilerStats(const SoProfilerStats & rhs);
  SoProfilerStats & operator=(const SoProfilerStats & rhs);

}; // SoProfilerStats

#endif // !COIN_SOPROFILERSTATS_H
