/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
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

#include "navigation/SoScXMLSpinUtils.h"

#include <assert.h>

#include <Inventor/SbViewportRegion.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/SoScXMLEvent.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>

// SetSpin    - initialize the rotate mode
// UpdateSpin - move camera
// EndSpin

// *************************************************************************

SoScXMLSpinInvoke::SpinDataMap SoScXMLSpinInvoke::spindatamap;

SCXML_OBJECT_ABSTRACT_SOURCE(SoScXMLSpinInvoke);

void
SoScXMLSpinInvoke::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(SoScXMLSpinInvoke, SoScXMLNavigationInvoke);
}

SpinData *
SoScXMLSpinInvoke::getSpinData(const ScXMLStateMachine * statemachine)
{
  SpinData * data = NULL;
  SpinDataMap::iterator findit =
    SoScXMLSpinInvoke::spindatamap.find(statemachine);
  if (findit == SoScXMLSpinInvoke::spindatamap.end()) {
    data = new SpinData;
    SpinDataEntry entry(statemachine, data);
    SoScXMLSpinInvoke::spindatamap.insert(entry);
  } else {
    data = findit->second;
  }
  return data;
}

void
SoScXMLSpinInvoke::freeSpinData(const ScXMLStateMachine * statemachine)
{
  SpinDataMap::iterator findit =
    SoScXMLSpinInvoke::spindatamap.find(statemachine);
  if (findit != SoScXMLSpinInvoke::spindatamap.end()) {
    SpinData * data = findit->second;
    delete data;
    SoScXMLSpinInvoke::spindatamap.erase(findit);
  }
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLSetSpin);

void
SoScXMLSetSpin::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLSetSpin, SoScXMLSpinInvoke,
                          SCXML_COIN_NS, "sim.coin3d.coin", "SetSpin");
}

void
SoScXMLSetSpin::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("SetSpin",
                       "No support for non-SoScXMLStateMachine objects");
    return;
  }

  SoScXMLStateMachine * statemachine =
    static_cast<SoScXMLStateMachine *>(statemachinearg);

  SpinData * data = SoScXMLSpinInvoke::getSpinData(statemachine);
  assert(data);

  assert(!data->spinning);
  data->spinning = TRUE;
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLUpdateSpin);

void
SoScXMLUpdateSpin::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLUpdateSpin, SoScXMLSpinInvoke,
                          SCXML_COIN_NS, "sim.coin3d.coin", "UpdateSpin");
}

void
SoScXMLUpdateSpin::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("SetSpin", "No support for non-SoScXMLStateMachine objects");
    return;
  }

  SoScXMLStateMachine * statemachine =
    static_cast<SoScXMLStateMachine *>(statemachinearg);

  SpinData * data = SoScXMLSpinInvoke::getSpinData(statemachine);
  assert(data);

  assert(data->spinning);

  const SbTime now(SbTime::getTimeOfDay());
  double secs = now.getValue() - data->updatetime.getValue();
  data->updatetime = now;

  SbRotation deltarot = data->spinrotation;
  deltarot.scaleAngle(float(secs * 5.0));
  SoScXMLNavigationInvoke::reorientCamera(data->camera.get(), deltarot);
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLEndSpin);

void
SoScXMLEndSpin::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLEndSpin, SoScXMLSpinInvoke,
                          SCXML_COIN_NS, "sim.coin3d.coin", "EndSpin");
}

// FIXME: This is never called if we exit while spinning, causing us to leak memory.
// This is probably caused by a more general failure to clean up
// ScXML state on exit.
void
SoScXMLEndSpin::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("SetSpin", "No support for non-SoScXMLStateMachine objects");
    return;
  }

  SoScXMLStateMachine * statemachine =
    static_cast<SoScXMLStateMachine *>(statemachinearg);

  SpinData * data = SoScXMLSpinInvoke::getSpinData(statemachine);
  assert(data);

  assert(data->spinning);

  SoScXMLSpinInvoke::freeSpinData(statemachine);
}

// *************************************************************************
