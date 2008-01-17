#ifndef COIN_SOPROFILERELEMENT_H
#define COIN_SOPROFILERELEMENT_H

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

#include <Inventor/elements/SoSubElement.h>
#include <Inventor/elements/SoElement.h>

#include <Inventor/lists/SbList.h>
#include <Inventor/SbTime.h>
#include <Inventor/SoType.h>

#include "misc/SbHash.h"
#include "profiler/SbProfilingData.h"

// *************************************************************************

class SoState;

class SoProfilerElement : public SoElement {
  typedef SoElement inherited;
  SO_ELEMENT_HEADER(SoProfilerElement);

public:
  static void initClass(void);

  static SoProfilerElement * get(SoState * state);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  void clear(void);
   
  void setTimingProfile(SoNode * node, SbTime t, SoNode * parent = NULL);

  void setHasGLCache(SoNode * node, SbBool hascache = TRUE);

  SbTime timeSinceTraversalStart(void);
  void startTraversalClock(void);

  const SbProfilingData & getProfilingData(void) const;

protected:
  virtual ~SoProfilerElement(void);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * elt);

private:
  SbProfilingData data;

  SbTime traversal_start_time;

}; // SoProfilerElement

#endif // HAVE_SCENE_PROFILING

#endif // !COIN_SOPROFILERELEMENT_H
