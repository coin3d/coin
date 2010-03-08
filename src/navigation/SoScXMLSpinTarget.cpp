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

#include <Inventor/navigation/SoScXMLSpinTarget.h>

/*!
  \class SoScXMLSpinTarget SoScXMLSpinTarget.h Inventor/scxml/SoScXMLSpinTarget.h
  \brief Navigation system event target for spinning operations.

  \ingroup navigation
*/

#include <cassert>
#include <cmath>
#include <cfloat>

#include <boost/intrusive_ptr.hpp>

#include <Inventor/SbViewVolume.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbLine.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbTime.h>
#include <Inventor/SbRotation.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/scxml/ScXMLEvent.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>
#include <Inventor/navigation/SoScXMLNavigation.h>

#include "scxml/SbStringConvert.h"
#include "coindefs.h"

namespace {

class SpinData : public SoScXMLNavigationTarget::Data {
public:
  SpinData(void) : spinning(FALSE) { }

  SbBool spinning;

  boost::intrusive_ptr<SoCamera> camera;
  SbTime updatetime;
  SbRotation spinrotation;
};

static SoScXMLNavigationTarget::Data * NewSpinData(void) { return new SpinData; }

} // namespace

class SoScXMLSpinTarget::PImpl {
public:
  static SbName BEGIN;
  static SbName UPDATE;
  static SbName END;
};

SbName SoScXMLSpinTarget::PImpl::BEGIN;
SbName SoScXMLSpinTarget::PImpl::UPDATE;
SbName SoScXMLSpinTarget::PImpl::END;

// *************************************************************************

#define PRIVATE

SCXML_OBJECT_SOURCE(SoScXMLSpinTarget);

void
SoScXMLSpinTarget::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(SoScXMLSpinTarget, SoScXMLNavigationTarget, "SoScXMLNavigationTarget");

#define EVENT_PREFIX COIN_NAVIGATION_SPIN_EVENT_PREFIX
  SoScXMLSpinTarget::PImpl::BEGIN   = SbName(EVENT_PREFIX ".BEGIN");
  SoScXMLSpinTarget::PImpl::UPDATE  = SbName(EVENT_PREFIX ".UPDATE");
  SoScXMLSpinTarget::PImpl::END     = SbName(EVENT_PREFIX ".END");
#undef EVENT_PREFIX
}

void
SoScXMLSpinTarget::cleanClass(void)
{
  SoScXMLSpinTarget::classTypeId = SoType::badType();
}

SoScXMLSpinTarget * SoScXMLSpinTarget::theSingleton = NULL;

SoScXMLSpinTarget *
SoScXMLSpinTarget::constructSingleton(void)
{
  assert(SoScXMLSpinTarget::theSingleton == NULL);
  SoScXMLSpinTarget::theSingleton =
    static_cast<SoScXMLSpinTarget *>(SoScXMLSpinTarget::classTypeId.createInstance());
  return SoScXMLSpinTarget::theSingleton;
}

void
SoScXMLSpinTarget::destructSingleton(void)
{
  assert(SoScXMLSpinTarget::theSingleton != NULL);
  delete SoScXMLSpinTarget::theSingleton;
  SoScXMLSpinTarget::theSingleton = NULL;
}

SoScXMLSpinTarget *
SoScXMLSpinTarget::singleton(void)
{
  assert(SoScXMLSpinTarget::theSingleton != NULL);
  return SoScXMLSpinTarget::theSingleton;
}

const SbName &
SoScXMLSpinTarget::BEGIN(void)
{
  return PImpl::BEGIN;
}

const SbName &
SoScXMLSpinTarget::UPDATE(void)
{
  return PImpl::UPDATE;
}

const SbName &
SoScXMLSpinTarget::END(void)
{
  return PImpl::END;
}

SoScXMLSpinTarget::SoScXMLSpinTarget(void)
{
  this->setEventTargetType(SOSCXML_NAVIGATION_TARGETTYPE);
  this->setEventTargetName("Spin");
}

SoScXMLSpinTarget::~SoScXMLSpinTarget(void)
{
}

SbBool
SoScXMLSpinTarget::processOneEvent(const ScXMLEvent * event)
{
  assert(event);


  SbName sessionid = this->getSessionId(event);
  if (sessionid == SbName::empty()) { return FALSE; }

  const SbName & eventname = event->getEventName();

  if (eventname == BEGIN()) {
    SpinData * data = static_cast<SpinData *>(this->getSessionData(sessionid, NewSpinData));
    assert(data);

    assert(!data->spinning);

    SoCamera * camera = inherited::getActiveCamera(event, sessionid);
    if unlikely (!camera) { return FALSE; }

    data->spinning = TRUE;

    data->camera = static_cast<SoCamera *>(camera->copy());

    double dtime = 0.0;
    SbRotation spinrot;
    if (!inherited::getEventDouble(event, "from", dtime) ||
        !inherited::getEventSbRotation(event, "rotation", spinrot)) {
      return FALSE;
    }

    data->updatetime = SbTime(dtime);
    data->spinrotation = spinrot;

    return TRUE;
  }

  else if (eventname == UPDATE()) {
    SpinData * data = static_cast<SpinData *>(this->getSessionData(sessionid, NewSpinData));
    assert(data);

    SoCamera * camera = inherited::getActiveCamera(event, sessionid);
    if unlikely (!camera) { return FALSE; }

    assert(data->camera.get());
    if unlikely (camera->getTypeId() != data->camera->getTypeId()) {
      SoDebugError::post("SoScXMLSpinTarget::processOneEvent",
                         "while processing %s: camera type was changed",
                         eventname.getString());
      return FALSE;
    }

    assert(data->spinning);

    const SbTime now(SbTime::getTimeOfDay());
    double secs = now.getValue() - data->updatetime.getValue();
    data->updatetime = now;

    SbRotation deltarot = data->spinrotation;
    deltarot.scaleAngle(float(secs * 5.0));
    SoScXMLSpinTarget::reorientCamera(camera, deltarot);

    return TRUE;
  }

  else if (eventname == END()) {
    this->freeSessionData(sessionid);
    return TRUE;
  }

  else {
    SoDebugError::post("SoScXMLSpinUtils::processOneEvent",
                       "while processing %s: unknown event",
                       eventname.getString());
    return FALSE;
  }
  return TRUE;
}

// Rotate camera around its focal point.
void
SoScXMLSpinTarget::reorientCamera(SoCamera * camera, const SbRotation & rot)
{
  if (camera == NULL) return;

  // Find global coordinates of focal point.
  SbVec3f direction;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * direction;

  // Set new orientation value by accumulating the new rotation.
  camera->orientation = rot * camera->orientation.getValue();

  // Reposition camera so we are still pointing at the same old focal point.
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  camera->position = focalpoint - camera->focalDistance.getValue() * direction;

  // some custom code to support UTMCamera cameras
  static const SoType utmcamtype(SoType::fromName("UTMCamera"));
  if (utmcamtype != SoType::badType() && camera->isOfType(utmcamtype)) {
    SbVec3d offset;
    offset.setValue(camera->position.getValue());
    SoSFVec3d * utmpositionfield =
      (SoSFVec3d *) camera->getField("utmposition");
    assert(utmpositionfield);
    utmpositionfield->setValue(utmpositionfield->getValue()+offset);
    camera->position.setValue(0.0f, 0.0f, 0.0f);
  }
}

#undef EVENT_PREFIX
#undef PRIVATE
