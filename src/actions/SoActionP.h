#ifndef COIN_SOACTIONP_H
#define COIN_SOACTIONP_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "misc/SoCompactPathList.h"

#include <Inventor/annex/Profiler/nodes/SoProfilerStats.h>
#include <Inventor/actions/SoAction.h>

#ifdef HAVE_NODEKITS
#include <Inventor/annex/Profiler/nodekits/SoProfilerOverlayKit.h>
#else /// !HAVE_NODEKITS
class SoProfilerOverlayKit;
#endif // !HAVE_NODEKITS

class SoActionP {
public:
  SoAction::AppliedCode appliedcode;
  int returnindex;
  union AppliedData {
    SoNode * node;
    SoPath * path;
    struct {
      const SoPathList * pathlist;
      const SoPathList * origpathlist;
      SoCompactPathList * compactlist;
    } pathlistdata;
  } applieddata;
  SbBool terminated;
  SbList <SbList<int> *> pathcodearray;
  int prevenabledelementscounter;

  static SoNode * getProfilerOverlay(void);
  static SoProfilerStats * getProfilerStatsNode(void);
}; // SoActionP

#endif // !COIN_SOACTIONP_H
