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

#include "navigation/SoScXMLPanUtils.h"

#include <assert.h>
//#include <boost/intrusive_ptr.hpp>

#include <Inventor/SbPlane.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>
#include <Inventor/scxml/SoScXMLEvent.h>


class PanData {
public:

  //boost::intrusive_ptr<SoCamera> camera;
  SbVec2f lastpos;
  SbPlane panplane;

};

// *************************************************************************

SCXML_OBJECT_ABSTRACT_SOURCE(SoScXMLPanInvoke);

void
SoScXMLPanInvoke::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(SoScXMLPanInvoke, SoScXMLNavigationInvoke);
}

PanData *
SoScXMLPanInvoke::getPanData(const ScXMLStateMachine * statemachine)
{
  PanData * data = NULL;
  PanDataMap::iterator findit =
    SoScXMLPanInvoke::pandatamap.find(statemachine);
  if (findit == SoScXMLPanInvoke::pandatamap.end()) {
    data = new PanData;
    PanDataEntry entry(statemachine, data);
    SoScXMLPanInvoke::pandatamap.insert(entry);
  } else {
    data = findit->second;
  }
  return data;
}

void
SoScXMLPanInvoke::freePanData(const ScXMLStateMachine * statemachine)
{
  PanDataMap::iterator findit =
    SoScXMLPanInvoke::pandatamap.find(statemachine);
  if (findit != SoScXMLPanInvoke::pandatamap.end()) {
    PanData * data = findit->second;
    delete data;
    SoScXMLPanInvoke::pandatamap.erase(findit);
  }
}

SoScXMLPanInvoke::PanDataMap SoScXMLPanInvoke::pandatamap;

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLSetPan);

void
SoScXMLSetPan::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLSetPan, SoScXMLPanInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "SetPan");
}

void
SoScXMLSetPan::invoke(ScXMLStateMachine * statemachinearg)
{
  SoScXMLStateMachine * statemachine = this->castToSo(statemachinearg);
  if (!statemachine) return;

  PanData * data = SoScXMLPanInvoke::getPanData(statemachine);
  assert(data);

  // get mouse position
  const SoEvent * soev = this->getSoEvent(statemachine);
  if (!soev) {
    SoDebugError::post("SetPan", "Need SoEvent.");
    return;
  }

  const SbViewportRegion & vp = statemachine->getViewportRegion();
  data->lastpos = soev->getNormalizedPosition(vp);

  // find plane for panning
  SoCamera * camera = statemachine->getActiveCamera();
  if (camera == NULL) { // can happen for empty scenegraph
    data->panplane = SbPlane(SbVec3f(0, 0, 1), 0);
  } else {
    const SbViewportRegion & vp = statemachine->getViewportRegion();
    SbViewVolume vv = camera->getViewVolume(vp.getViewportAspectRatio());
    data->panplane = vv.getPlane(camera->focalDistance.getValue());
  }
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLUpdatePan);

void
SoScXMLUpdatePan::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLUpdatePan, SoScXMLPanInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "UpdatePan");
 
}

void
SoScXMLUpdatePan::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("UpdatePan",
                       "No support for non-SoScXMLStateMachine objects");
    return;
  }
  const SoScXMLStateMachine * statemachine =
    static_cast<const SoScXMLStateMachine *>(statemachinearg);

  PanData * data = SoScXMLPanInvoke::getPanData(statemachine);
  assert(data);

  // get mouse position
  const ScXMLEvent * ev = statemachine->getCurrentEvent();
  if (!ev || !ev->isOfType(SoScXMLEvent::getClassTypeId())) {
    SoDebugError::post("UpdatePan", "Need SoEvent but statemachine has none.");
    return;
  }

  const SoEvent * soev = static_cast<const SoScXMLEvent *>(ev)->getSoEvent();
  if (!soev) {
    SoDebugError::post("UpdatePan", "Need SoEvent.");
    return;
  }

  const SbViewportRegion & vp = statemachine->getViewportRegion();
  SbVec2f position = soev->getNormalizedPosition(vp);

  float vpaspect = vp.getViewportAspectRatio();

  SoCamera * camera = statemachine->getActiveCamera();

  SoScXMLNavigationInvoke::panCamera(camera, vpaspect, data->panplane,
                                     data->lastpos, position);

  data->lastpos = position;
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLEndPan);

void
SoScXMLEndPan::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLEndPan, SoScXMLPanInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "EndPan");
}

void
SoScXMLEndPan::invoke(ScXMLStateMachine * statemachine)
{
  SoScXMLPanInvoke::freePanData(statemachine);
}

// *************************************************************************
