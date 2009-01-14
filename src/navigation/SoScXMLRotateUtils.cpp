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

#include "navigation/SoScXMLRotateUtils.h"

#include <assert.h>
#include <map>

#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include <Inventor/SbViewVolume.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbTime.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/SoScXMLEvent.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>

#include "navigation/SoScXMLSpinUtils.h"

// *************************************************************************

class RotateData {
public:
  RotateData(void) {
    this->projector.reset(new SbSphereSheetProjector);
    SbViewVolume volume;
    volume.ortho(-1, 1, -1, 1, -1, 1);
    this->projector->setViewVolume(volume);
    this->logsize = 0;
  }

  SbVec2f downposn;
  boost::intrusive_ptr<SoCamera> cameraclone;
  boost::scoped_ptr<SbSphereSheetProjector> projector;

  struct log {
    SbVec2f posn;
    SbTime time;
  } mouselog[3];
  int logsize;
};

// *************************************************************************

/*
  \class SoScXMLRotateInvoke

  Abstract base class for the rotation-related invoke handlers.
  Provides a common method for getting hold of the trans-object data storage.
  (since ScXML does not support the datamodel system yet)
*/

SoScXMLRotateInvoke::RotateDataMap SoScXMLRotateInvoke::rotatedatamap;

SCXML_OBJECT_ABSTRACT_SOURCE(SoScXMLRotateInvoke);

void
SoScXMLRotateInvoke::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(SoScXMLRotateInvoke, SoScXMLNavigationInvoke);
}

RotateData *
SoScXMLRotateInvoke::getRotateData(const ScXMLStateMachine * statemachine)
{
  RotateData * data = NULL;
  RotateDataMap::iterator findit =
    SoScXMLRotateInvoke::rotatedatamap.find(statemachine);
  if (findit == SoScXMLRotateInvoke::rotatedatamap.end()) {
    data = new RotateData;
    RotateDataEntry entry(statemachine, data);
    SoScXMLRotateInvoke::rotatedatamap.insert(entry);
  } else {
    data = findit->second;
  }
  return data;
}

void
SoScXMLRotateInvoke::freeRotateData(const ScXMLStateMachine * statemachine)
{
  RotateDataMap::iterator findit =
    SoScXMLRotateInvoke::rotatedatamap.find(statemachine);
  if (findit != SoScXMLRotateInvoke::rotatedatamap.end()) {
    RotateData * data = findit->second;
    delete data;
    SoScXMLRotateInvoke::rotatedatamap.erase(findit);
  }
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLSetRotate);

void
SoScXMLSetRotate::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLSetRotate, SoScXMLRotateInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "SetRotate");
}

void
SoScXMLSetRotate::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("SetRotate",
                       "No support for non-SoScXMLStateMachine objects");
    return;
  }
  const SoScXMLStateMachine * statemachine =
    static_cast<const SoScXMLStateMachine *>(statemachinearg);

  RotateData * data = SoScXMLRotateInvoke::getRotateData(statemachine);
  assert(data);

  data->logsize = 0;

  // get mouse position
  const ScXMLEvent * ev = statemachine->getCurrentEvent();
  if (!ev || !ev->isOfType(SoScXMLEvent::getClassTypeId())) {
    SoDebugError::post("SetRotate", "Need SoEvent but statemachine has none.");
    return;
  }
  const SoEvent * soev = static_cast<const SoScXMLEvent *>(ev)->getSoEvent();
  if (!soev) {
    SoDebugError::post("SetRotate", "Need SoEvent.");
    return;
  }

  const SbViewportRegion & vp = statemachine->getViewportRegion();
  data->downposn = soev->getNormalizedPosition(vp);
  data->mouselog[0].posn = data->downposn;
  data->mouselog[0].time = soev->getTime();

  data->logsize = 1;

  // store current camera position
  SoCamera * camera = statemachine->getActiveCamera();
  data->cameraclone = static_cast<SoCamera *>(camera->copy());
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLUpdateRotate);

void
SoScXMLUpdateRotate::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLUpdateRotate, SoScXMLRotateInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "UpdateRotate");
}

void
SoScXMLUpdateRotate::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("UpdateRotate",
                       "No support for non-SoScXMLStateMachine objects");
    return;
  }
  const SoScXMLStateMachine * statemachine =
    static_cast<const SoScXMLStateMachine *>(statemachinearg);

  RotateData * data = SoScXMLRotateInvoke::getRotateData(statemachine);
  assert(data);

  SoCamera * camera = statemachine->getActiveCamera();
  assert(camera);
  assert(data->cameraclone.get());
  if (camera->getTypeId() != data->cameraclone->getTypeId()) {
    SoDebugError::post("UpdateRotate", "Change of camera type");
    return;
  }

  // get mouse position
  const ScXMLEvent * ev = statemachine->getCurrentEvent();
  if (!ev || !ev->isOfType(SoScXMLEvent::getClassTypeId())) {
    SoDebugError::post("UpdateRotate", "Need SoEvent but statemachine has none.");
    return;
  }
  const SoEvent * soev = static_cast<const SoScXMLEvent *>(ev)->getSoEvent();
  if (!soev || !soev->isOfType(SoLocation2Event::getClassTypeId())) {
    SoDebugError::post("UpdateRotate", "Need SoEvent.");
    return;
  }

  const SbViewportRegion & vp = statemachine->getViewportRegion();
  SbVec2f currentposn = soev->getNormalizedPosition(vp);

  // update mouse log
  data->mouselog[2].time = data->mouselog[1].time;
  data->mouselog[2].posn = data->mouselog[1].posn;
  data->mouselog[1].time = data->mouselog[0].time;
  data->mouselog[1].posn = data->mouselog[0].posn;
  data->mouselog[0].posn = currentposn;
  data->mouselog[0].time = soev->getTime();
  data->logsize += 1;

  // find rotation
  data->projector->project(data->downposn);
  SbRotation rot;
  data->projector->projectAndGetRotation(currentposn, rot);
  rot.invert();

  // restore camera to original position and do full reorientation
  camera->copyFieldValues(data->cameraclone.get());
  SoScXMLNavigationInvoke::reorientCamera(camera, rot);
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLEndRotate);

void
SoScXMLEndRotate::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLEndRotate, SoScXMLRotateInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "EndRotate");
}

SoScXMLEndRotate::SoScXMLEndRotate(void)
  : sendspinstart(TRUE)
{
}

SbBool
SoScXMLEndRotate::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  if (this->srcexpr) {
    // check arguments if spinning-event should be sent
  }

  return TRUE;
}

void
SoScXMLEndRotate::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("EndRotate",
                       "No support for non-SoScXMLStateMachine objects");
    return;
  }

  const SoScXMLStateMachine * statemachine =
    static_cast<const SoScXMLStateMachine *>(statemachinearg);

  if (!this->sendspinstart) {
    SoScXMLRotateInvoke::freeRotateData(statemachine);
    return;
  }

  RotateData * data = SoScXMLRotateInvoke::getRotateData(statemachine);
  assert(data);

  SbBool triggerspin = FALSE;

  SbRotation spinrotation;

  if (data->logsize > 2) {
    SbTime stoptime = (SbTime::getTimeOfDay() - data->mouselog[0].time);
    if (stoptime.getValue() < 0.100) {
      SbVec3f from = data->projector->project(data->mouselog[2].posn);
      SbVec3f to = data->projector->project(data->mouselog[0].posn);
      spinrotation = data->projector->getRotation(from, to);

      SbTime delta = data->mouselog[0].time - data->mouselog[2].time;
      double deltatime = delta.getValue();
      spinrotation.invert();
      spinrotation.scaleAngle(float(0.200 / deltatime));

      SbVec3f axis;
      float radians;
      spinrotation.getValue(axis, radians);
      if ((radians > 0.01f) && (deltatime < 0.300)) {
        triggerspin = TRUE;
      }
    }
  }

  if (!triggerspin) {
    SoScXMLRotateInvoke::freeRotateData(statemachine);
    return;
  }

  {
    SpinData * spindata = SoScXMLSpinInvoke::getSpinData(statemachine);
    
    spindata->camera = statemachine->getActiveCamera();
    spindata->spinrotation = spinrotation;
    spindata->updatetime = data->mouselog[0].time;

    static const SbName eventid("sim.coin3d.coin.navigation.SetSpin");
    const_cast<SoScXMLStateMachine *>(statemachine)->queueEvent(eventid);
  }

  // might just set camera to NULL instead of freeing the structure
  // (the point is to not keep a reference to the camera so it can't die)
  SoScXMLRotateInvoke::freeRotateData(statemachine);
}

// *************************************************************************
