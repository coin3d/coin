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

#include "navigation/SoScXMLSeekUtils.h"

#include <assert.h>

#include <Inventor/SbTime.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/SoScXMLEvent.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>

class SeekData {
public:
  SeekData(void) {
    seeking = FALSE;
  }
  SbBool seeking;
  SbTime seekstart;
  float seektime;
  SbVec3f camerastartposition, cameraendposition;
  SbRotation camerastartorient, cameraendorient;
};

// *************************************************************************

SCXML_OBJECT_ABSTRACT_SOURCE(SoScXMLSeekInvoke);

void
SoScXMLSeekInvoke::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(SoScXMLSeekInvoke, SoScXMLNavigationInvoke);
}

SeekData *
SoScXMLSeekInvoke::getSeekData(const ScXMLStateMachine * statemachine)
{
  SeekData * data = NULL;
  SeekDataMap::iterator findit =
    SoScXMLSeekInvoke::seekdatamap.find(statemachine);
  if (findit == SoScXMLSeekInvoke::seekdatamap.end()) {
    data = new SeekData;
    SeekDataEntry entry(statemachine, data);
    SoScXMLSeekInvoke::seekdatamap.insert(entry);
  } else {
    data = findit->second;
  }
  return data;
}

void
SoScXMLSeekInvoke::freeSeekData(const ScXMLStateMachine * statemachine)
{
  SeekDataMap::iterator findit =
    SoScXMLSeekInvoke::seekdatamap.find(statemachine);
  if (findit != SoScXMLSeekInvoke::seekdatamap.end()) {
    SeekData * data = findit->second;
    delete data;
    SoScXMLSeekInvoke::seekdatamap.erase(findit);
  }
}

SoScXMLSeekInvoke::SeekDataMap SoScXMLSeekInvoke::seekdatamap;

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLSetSeek);

void
SoScXMLSetSeek::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLSetSeek, SoScXMLSeekInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "SetSeek");
}

SoScXMLSetSeek::SoScXMLSetSeek(void)
  : searchaction(NULL), getmatrixaction(NULL), raypickaction(NULL)
{
}

SoScXMLSetSeek::~SoScXMLSetSeek(void)
{
  if (this->raypickaction) {
    delete this->raypickaction;
    this->raypickaction = NULL;
  }
  if (this->getmatrixaction) {
    delete this->getmatrixaction;
    this->getmatrixaction = NULL;
  }
  if (this->searchaction) {
    delete this->searchaction;
    this->searchaction = NULL;
  }
}

void
SoScXMLSetSeek::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("SetSeek",
                       "No support for non-SoScXMLStateMachine objects");
    return;
  }

  SoScXMLStateMachine * statemachine =
    static_cast<SoScXMLStateMachine *>(statemachinearg);

  SeekData * data = SoScXMLSeekInvoke::getSeekData(statemachine);
  assert(data);

  // get mouse position
  const ScXMLEvent * ev = statemachine->getCurrentEvent();
  if (!ev || !ev->isOfType(SoScXMLEvent::getClassTypeId())) {
    SoDebugError::post("SetSeek", "Need SoEvent but statemachine has none.");
    return;
  }
  const SoEvent * soev = static_cast<const SoScXMLEvent *>(ev)->getSoEvent();
  if (!soev) {
    SoDebugError::post("SetSeek", "Need SoEvent.");
    return;
  }

  SbVec2s screenpos = soev->getPosition();

  const SbViewportRegion & vp = statemachine->getViewportRegion();
  if (!this->raypickaction) {
    this->raypickaction = new SoRayPickAction(vp);
  } else {
    this->raypickaction->setViewportRegion(vp);
  }

  SoNode * sceneroot = statemachine->getSceneGraphRoot();
  SoCamera * camera = statemachine->getActiveCamera();

  this->raypickaction->setPoint(screenpos);
  this->raypickaction->setRadius(2);
  this->raypickaction->apply(sceneroot);
  
  SoPickedPoint * picked = this->raypickaction->getPickedPoint();
  if (!picked) {
    static const SbName eventid("sim.coin3d.coin.navigation.seekmiss");
    statemachine->queueEvent(eventid);
    return;
  }

  if (!this->getmatrixaction) {
    this->getmatrixaction = new SoGetMatrixAction(vp);
  }

  SbVec3f hitpoint = picked->getPoint();

  data->camerastartposition = camera->position.getValue();
  data->camerastartorient = camera->orientation.getValue();

  data->seekstart = SbTime::getTimeOfDay();
  data->seektime = 2.0f;

  // move point to the camera coordinate system, consider
  // transformations before camera in the scene graph
  SbMatrix cameramatrix, camerainverse;
  this->getCameraCoordinateSystem(camera, sceneroot,
                                  cameramatrix, camerainverse);
  camerainverse.multVecMatrix(hitpoint, hitpoint);
  
  float fd = 25;
  fd *= (hitpoint - camera->position.getValue()).length()/100.0f;
  camera->focalDistance = fd;

  SbVec3f dir = hitpoint - data->camerastartposition;
  dir.normalize();

  // find a rotation that rotates current camera direction into new
  // camera direction.
  SbVec3f olddir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), olddir);
  SbRotation diffrot(olddir, dir);
  data->cameraendposition = hitpoint - fd * dir;
  data->cameraendorient = camera->orientation.getValue() * diffrot;

  data->seeking = TRUE;
}


void
SoScXMLSetSeek::getCameraCoordinateSystem(SoCamera * cameraarg,
                                          SoNode * root,
                                          SbMatrix & matrix,
                                          SbMatrix & inverse)
{
  if (!this->searchaction) {
    this->searchaction = new SoSearchAction;
  }
  this->searchaction->reset();
  this->searchaction->setSearchingAll(TRUE);
  this->searchaction->setInterest(SoSearchAction::FIRST);
  this->searchaction->setNode(cameraarg);
  this->searchaction->apply(root);

  matrix = inverse = SbMatrix::identity();
  if (this->searchaction->getPath()) {
    assert(this->getmatrixaction);
    this->getmatrixaction->apply(this->searchaction->getPath());
    matrix = this->getmatrixaction->getMatrix();
    inverse = this->getmatrixaction->getInverse();
  }
  this->searchaction->reset();
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLUpdateSeek);

void
SoScXMLUpdateSeek::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLUpdateSeek, SoScXMLSeekInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "UpdateSeek");
 
}

void
SoScXMLUpdateSeek::invoke(ScXMLStateMachine * statemachinearg)
{
  if (!statemachinearg->isOfType(SoScXMLStateMachine::getClassTypeId())) {
    SoDebugError::post("SetZoom",
                       "No support for non-SoScXMLStateMachine objects");
    return;
  }

  SoScXMLStateMachine * statemachine =
    static_cast<SoScXMLStateMachine *>(statemachinearg);

  SeekData * data = SoScXMLSeekInvoke::getSeekData(statemachine);
  assert(data);
  if (!data->seeking) {
    return;
  }

  SbTime currenttime = SbTime::getTimeOfDay();

  float t = static_cast<float>((currenttime.getValue() - data->seekstart.getValue()) / data->seektime);
  if (t >= 1.0f) t = 1.0f;

  SbBool end = (t == 1.0f);
  
  t = (float) ((1.0 - cos(M_PI*t)) * 0.5);
  
  SoCamera * camera = statemachine->getActiveCamera();

  camera->position = data->camerastartposition +
    (data->cameraendposition - data->camerastartposition) * t;

  camera->orientation = 
    SbRotation::slerp(data->camerastartorient, data->cameraendorient, t);

  if (end) {
    static const SbName eventid("sim.coin3d.coin.navigation.seekend");
    statemachine->queueEvent(eventid);
  }
}

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLEndSeek);

void
SoScXMLEndSeek::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLEndSeek, SoScXMLSeekInvoke,
                           SCXML_COIN_NS, "sim.coin3d.coin", "EndSeek");
}

void
SoScXMLEndSeek::invoke(ScXMLStateMachine * statemachine)
{
  SeekData * data = SoScXMLSeekInvoke::getSeekData(statemachine);
  assert(data);
  data->seeking = FALSE;
  SoScXMLSeekInvoke::freeSeekData(statemachine);
}

// *************************************************************************
