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

#include "scxml/SoScXMLScheduleTimerEventInvoke.h"

#include <assert.h>
#include <map>

#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLStateMachine.h>

// *************************************************************************

class SoScXMLScheduleTimerEventInvokeP {
public:
  typedef std::map<const SoSensor *, const char *> SensorNameMap;
  typedef std::map<const SoSensor *, ScXMLStateMachine *> SensorMachineMap;

  typedef std::pair<const char *, const ScXMLStateMachine *> ReverseKey;
  typedef std::map<ReverseKey, const SoSensor *> ReverseMap;

  static SensorNameMap sensornamemap;
  static SensorMachineMap sensormachinemap;
  static ReverseMap reversemap;

  static void registerSensorMapping(const SoSensor * sensor, SbName name, ScXMLStateMachine * statemachine);

  static SoAlarmSensor * getSensor(SbName sensorname, const ScXMLStateMachine * statemachine);

  static ScXMLStateMachine * getStateMachine(const SoSensor * sensor);
  static SbName getName(const SoSensor * sensor);

  static void stateMachineDeleteCB(void * userdata, const ScXMLStateMachine * statemachine);
  static void timerSensorCB(void * userdata, SoSensor * sensor);
};

SoScXMLScheduleTimerEventInvokeP::SensorNameMap SoScXMLScheduleTimerEventInvokeP::sensornamemap;

SoScXMLScheduleTimerEventInvokeP::SensorMachineMap SoScXMLScheduleTimerEventInvokeP::sensormachinemap;

SoScXMLScheduleTimerEventInvokeP::ReverseMap SoScXMLScheduleTimerEventInvokeP::reversemap;

// *************************************************************************

SCXML_OBJECT_SOURCE(SoScXMLScheduleTimerEventInvoke);

void
SoScXMLScheduleTimerEventInvoke::initClass(void)
{
  SCXML_INVOKE_INIT_CLASS(SoScXMLScheduleTimerEventInvoke, ScXMLInvoke,
                          SCXML_COIN_NS, "sim.coin3d.coin", "ScheduleTimerEvent");
}

SoScXMLScheduleTimerEventInvoke::SoScXMLScheduleTimerEventInvoke(void)
  : initialized(FALSE), relativetimeval(0.0), timereventname(SbName::empty())
{
}

SoScXMLScheduleTimerEventInvoke::~SoScXMLScheduleTimerEventInvoke(void)
{
  if (this->initialized) {
    // FIXME: unregister data from static maps
  }
}

SbBool
SoScXMLScheduleTimerEventInvoke::handleXMLAttributes(void)
{
  assert(!this->initialized);

  if (!inherited::handleXMLAttributes()) return FALSE;

  if (!this->srcexpr) {
    SoDebugError::postWarning("ScXML::Invoke",
                              "ScheduleTimerSensor problem: "
                              "no 'srcexpr' info to set up timer event from. "
                              "No TimerEvent would ever be created.");
    return FALSE;
  }

  // srcexpr format: "delaysecs:eventname"

  relativetimeval = atof(this->srcexpr);
  if (relativetimeval <= 0.0) {
    SoDebugError::postWarning("ScXML::Invoke",
                              "ScheduleTimerSensor problem: "
                              "'srcexpr' time value less than or equal zero. "
                              "No TimerEvent would ever be created.");
    return FALSE;
  }

  const char * nameptr = strchr(this->srcexpr, ':');
  if (!nameptr || (strlen(nameptr) == 1)) {
    SoDebugError::postWarning("ScXML::Invoke",
                              "ScheduleTimerSensor problem: "
                              "'srcexpr' contained no name. "
                              "No TimerEvent would be created.");
    return FALSE;
  }
  ++nameptr;

  SbString namestring;
  namestring.sprintf("sim.coin3d.coin.TimerEvent.%s", nameptr);
  this->timereventname = SbName(namestring.getString());

  // assuming now that everything was set up fine
  this->initialized = TRUE;
  return TRUE;
}

void
SoScXMLScheduleTimerEventInvoke::invoke(const ScXMLStateMachine * statemachine) const
{
  if (!this->initialized) return;

  SoAlarmSensor * sensor =
    SoScXMLScheduleTimerEventInvokeP::getSensor(this->timereventname, statemachine);

  if (!sensor) {
    // need to create a new one
    sensor = new SoAlarmSensor(SoScXMLScheduleTimerEventInvokeP::timerSensorCB, const_cast<SoScXMLScheduleTimerEventInvoke *>(this));

    SoScXMLScheduleTimerEventInvokeP::registerSensorMapping(sensor, this->timereventname, const_cast<ScXMLStateMachine *>(statemachine));

  }

  if (sensor->isScheduled()) {
    sensor->unschedule();
  }

  sensor->setTimeFromNow(this->relativetimeval);
  sensor->schedule();
}

// *************************************************************************

ScXMLStateMachine *
SoScXMLScheduleTimerEventInvokeP::getStateMachine(const SoSensor * sensor)
{
  SensorMachineMap::iterator machineit = sensormachinemap.find(sensor);
  if (machineit != sensormachinemap.end()) {
    return machineit->second;
  }
  return NULL;
}

SbName
SoScXMLScheduleTimerEventInvokeP::getName(const SoSensor * sensor)
{
  SensorNameMap::iterator nameit = sensornamemap.find(sensor);
  if (nameit != sensornamemap.end()) {
    return SbName(nameit->second);
  }
  return SbName::empty();
}

void
SoScXMLScheduleTimerEventInvokeP::timerSensorCB(void * userdata, SoSensor * sensor)
{
  ScXMLStateMachine * statemachine =
    SoScXMLScheduleTimerEventInvokeP::getStateMachine(sensor);
  if (statemachine) {
    SbName eventname = SoScXMLScheduleTimerEventInvokeP::getName(sensor);
    statemachine->queueEvent(eventname);
    statemachine->processEventQueue();
  } else {
    // statemachine has probably been deleted in the mean time.
    // this ought to have deleted the sensor as well though, so this
    // should not have triggered.
  }
}

void
SoScXMLScheduleTimerEventInvokeP::stateMachineDeleteCB(void * userdata, const ScXMLStateMachine * statemachine)
{
  SensorMachineMap::iterator machineit = sensormachinemap.begin();
  while (machineit != sensormachinemap.end()) {
    if (machineit->second == statemachine) {
      const SoSensor * sensor = machineit->first;

      // iterator becomes invalid after the erase() call
      SensorMachineMap::iterator storedit = machineit;
      ++machineit;
      sensormachinemap.erase(storedit);

      // also erase name mapping
      SensorNameMap::iterator nameit = sensornamemap.find(sensor);
      if (nameit != sensornamemap.end()) {
        sensornamemap.erase(nameit);
      }

      // and delete the sensor
      delete sensor;
    } else {
      ++machineit;
    }
  }
}

void
SoScXMLScheduleTimerEventInvokeP::registerSensorMapping(const SoSensor * sensor, SbName name, ScXMLStateMachine * statemachine)
{
  // add to sensornamemap
  sensornamemap.insert(std::pair<const SoSensor *, const char *>(sensor, name.getString()));

  // add to sensormachinemap
  sensormachinemap.insert(std::pair<const SoSensor *, ScXMLStateMachine *>(sensor, statemachine));

  ReverseKey reversekey(name.getString(), statemachine);
  reversemap.insert(std::pair<ReverseKey, const SoSensor *>(reversekey, sensor));
}

SoAlarmSensor *
SoScXMLScheduleTimerEventInvokeP::getSensor(SbName sensorname, const ScXMLStateMachine * statemachine)
{
  ReverseKey key(sensorname.getString(), statemachine);

  ReverseMap::iterator it = reversemap.find(key);
  if (it != reversemap.end()) {
    const SoAlarmSensor * sensor = static_cast<const SoAlarmSensor *>(it->second);
    return const_cast<SoAlarmSensor *>(sensor);
  }

  return NULL;
}
