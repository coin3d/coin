/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLTimeSensor SoVRMLTimeSensor.h Inventor/VRMLnodes/SoVRMLTimeSensor.h
  \brief The SoVRMLTimeSensor class is a multi-purpose time event generator.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  TimeSensor {
    exposedField SFTime   cycleInterval 1       # (0,inf)
    exposedField SFBool   enabled       TRUE
    exposedField SFBool   loop          FALSE
    exposedField SFTime   startTime     0       # (-inf,inf)
    exposedField SFTime   stopTime      0       # (-inf,inf)
    eventOut     SFTime   cycleTime
    eventOut     SFFloat  fraction_changed      # [0, 1]
    eventOut     SFBool   isActive
    eventOut     SFTime   time
  }
  \endverbatim

  TimeSensor nodes generate events as time passes. TimeSensor nodes
  can be used for many purposes including: 

  - driving continuous simulations and animations; 
  - controlling periodic activities (e.g., one per minute);
  - initiating single occurrence events such as an alarm clock.

  The TimeSensor node contains two discrete eventOuts: \e isActive and
  \e cycleTime. The \e isActive eventOut sends TRUE when the TimeSensor
  node begins running, and FALSE when it stops running. The \e cycleTime
  eventOut sends a time event at \e startTime and at the beginning of
  each new cycle (useful for synchronization with other time-based
  objects).  The remaining eventOuts generate continuous events. The
  \e fraction_changed eventOut, an SFFloat in the closed interval [0,1],
  sends the completed fraction of the current cycle. The \e time eventOut
  sends the absolute time for a given simulation tick.  

  If the enabled exposedField is TRUE, the TimeSensor node is enabled
  and may be running. If a set_enabled FALSE event is received while
  the TimeSensor node is running, the sensor performs the following
  actions: 
  
  - evaluates and sends all relevant outputs;
  - sends a FALSE value for isActive;
  - disables itself.

  Events on the exposedFields of the TimeSensor node (e.g., \e
  set_startTime) are processed and their corresponding eventOuts
  (e.g., startTime_changed) are sent regardless of the state of the
  enabled field. The remaining discussion assumes enabled is TRUE.

  The e\ loop, \e startTime, and \e stopTime exposedFields and the
  isActive eventOut and their effects on the TimeSensor node are discussed
  in detail in 4.6.9, Time-dependent nodes
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.9>).
  The "cycle" of a TimeSensor node lasts for cycleInterval
  seconds. The value of cycleInterval shall be greater than zero.  

  A cycleTime eventOut can be used for synchronization purposes such
  as sound with animation. The value of a cycleTime eventOut will be
  equal to the time at the beginning of the current cycle. A cycleTime
  eventOut is generated at the beginning of every cycle, including the
  cycle starting at startTime. The first cycleTime eventOut for a
  TimeSensor node can be used as an alarm (single pulse at a specified
  time).

  When a TimeSensor node becomes active, it generates an isActive =
  TRUE event and begins generating time, fraction_changed, and
  cycleTime events which may be routed to other nodes to drive
  animation or simulated behaviours. The behaviour at read time is
  described below. The time event sends the absolute time for a given
  tick of the TimeSensor node (time fields and events represent the
  number of seconds since midnight GMT January 1, 1970).

  fraction_changed events output a floating point value in the closed
  interval [0, 1]. At startTime the value of fraction_changed is
  0. After startTime, the value of fraction_changed in any cycle will
  progress through the range (0.0, 1.0]. At startTime + N ×
  cycleInterval, for N = 1, 2, ..., that is, at the end of every
  cycle, the value of fraction_changed is 1.

  Let \e now represent the time at the current simulation tick. Then
  the time and fraction_changed eventOuts can then be computed as:

  \verbatim
  time = now 
  temp = (now - startTime) / cycleInterval 
  f = fractionalPart(temp) 
  if (f == 0.0 && now > startTime) fraction_changed = 1.0 
  else fraction_changed = f 
  \endverbatim
  
  where fractionalPart(x) is a function that returns the fractional
  part, (that is, the digits to the right of the decimal point), of a
  nonnegative floating point number.

  A TimeSensor node can be set up to be active at read time by
  specifying loop TRUE (not the default) and stopTime less than or
  equal to startTime (satisfied by the default values). The time
  events output absolute times for each tick of the TimeSensor node
  simulation.  The time events shall start at the first simulation
  tick greater than or equal to startTime. time events end at
  stopTime, or at startTime + N × cycleInterval for some positive
  integer value of N, or loop forever depending on the values of the
  other fields. An active TimeSensor node shall stop at the first
  simulation tick when now >= stopTime > startTime.

  No guarantees are made with respect to how often a TimeSensor node
  generates time events, but a TimeSensor node shall generate events
  at least at every simulation tick. TimeSensor nodes are guaranteed
  to generate final time and fraction_changed events. If loop is FALSE
  at the end of the Nth cycleInterval and was TRUE at startTime + M ×
  cycleInterval for all 0 < M < N, the final time event will be
  generated with a value of (startTime + N × cycleInterval) or
  stopTime (if stopTime > startTime), whichever value is less. If loop
  is TRUE at the completion of every cycle, the final event is
  generated as evaluated at stopTime (if stopTime > startTime) or
  never.

  An active TimeSensor node ignores set_cycleInterval and
  set_startTime events. An active TimeSensor node also ignores
  set_stopTime events for set_stopTime less than or equal to
  startTime.  For example, if a set_startTime event is received while
  a TimeSensor node is active, that set_startTime event is ignored
  (the startTime field is not changed, and a startTime_changed
  eventOut is not generated).  If an active TimeSensor node receives a
  set_stopTime event that is less than the current time, and greater
  than startTime, it behaves as if the stopTime requested is the
  current time and sends the final events based on the current time
  (note that stopTime is set as specified in the eventIn).
  
  A TimeSensor read from a VRML file shall generate isActive TRUE,
  time and fraction_changed events if the sensor is enabled and all
  conditions for a TimeSensor to be active are met.  

*/

/*!
  \var SoSFTime SoVRMLTimeSensor::cycleInterval
  The cycle interval. Default value is 1. Must be > 0.
*/

/*!
  \var SoSFBool SoVRMLTimeSensor::enabled
  Used to enable/disable timer. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLTimeSensor::loop
  TRUE if timer should loop. Default value is FALSE.
*/

/*!
  \var SoSFTime SoVRMLTimeSensor::startTime
  The timer start time. Default value is 0.0.
*/

/*!
  \var SoSFTime SoVRMLTimeSensor::stopTime
  The timer stop time. Default value is 0.0.
*/

/*!
  \var SoEngineOutput SoVRMLTimeSensor::cycleTime
  An eventOut that is sent when a new cycle is started.
*/

/*!
  \var SoEngineOutput SoVRMLTimeSensor::fraction_changed
  An eventOut that is sent for each tick, containing a number between 0 and 1.
*/

/*!
  \var SoEngineOutput SoVRMLTimeSensor::isActive
  An eventOut that is sent when the timer is enabled/disabled.
*/

/*!
  \var SoEngineOutput SoVRMLTimeSensor::time
  An eventOut that is sent for each tick, containing the current time.
*/

#include <Inventor/VRMLnodes/SoVRMLTimeSensor.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/engines/SoSubNodeEngineP.h>
#include <Inventor/SoDB.h>

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLTimeSensorP {
public:
  double starttime;
  double stoptime;
  double currtime;
  double cycletime;
  double cyclestart;
  float fraction;
  SbBool loop;
  SbBool running;
};

#endif // DOXYGEN_SKIP_THIS

SO_NODEENGINE_SOURCE(SoVRMLTimeSensor);

// Doc in parent
void
SoVRMLTimeSensor::initClass(void) // static
{
  SO_NODEENGINE_INTERNAL_INIT_CLASS(SoVRMLTimeSensor);
}

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoVRMLTimeSensor::SoVRMLTimeSensor(void)
{
  THIS = new SoVRMLTimeSensorP;

  SO_NODEENGINE_INTERNAL_CONSTRUCTOR(SoVRMLTimeSensor);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(cycleInterval, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(enabled, (TRUE));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(loop, (FALSE));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(startTime, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(stopTime, (0.0f));
  SO_VRMLNODE_ADD_EVENT_IN(timeIn); // private

  SO_NODEENGINE_ADD_OUTPUT(cycleTime, SoSFTime);
  SO_NODEENGINE_ADD_OUTPUT(fraction_changed, SoSFFloat);
  SO_NODEENGINE_ADD_OUTPUT(isActive, SoSFBool);
  SO_NODEENGINE_ADD_OUTPUT(time, SoSFTime);

  this->isActive.enable(FALSE);
  this->cycleTime.enable(FALSE);

  THIS->fraction = 0.0;
  THIS->cyclestart = 0.0;
  THIS->cycletime = 1.0;
  THIS->running = FALSE;
  THIS->loop = FALSE;
  THIS->starttime = 0.0;
  THIS->stoptime = 0.0;

  this->timeIn.enableNotify(FALSE);
  SoField * realtime = SoDB::getGlobalField("realTime");
  this->timeIn.connectFrom(realtime);

  // we always connect and just disable notification when timer
  // is not active, since it is currently not possible to disconnect
  // from a field in the inputChanged() method. inputChanged() is
  // triggered by notify(), and if a field is disconnected while the
  // master field is notifying, bad things will happen in
  // SoAuditorList.

  // FIXME: Maybe we should consider making a version of SoAuditorList
  // that handles disconnects in the notification loop? I think
  // it might be difficult though.   pederb, 2001-11-06
}

/*!
  Destructor.
*/
SoVRMLTimeSensor::~SoVRMLTimeSensor()
{
}

// Doc in parent
void
SoVRMLTimeSensor::notify(SoNotList * list)
{
  inherited::notify(list);
}

// Documented in superclass. Overridden to not write connection to
// realTime global field.
void
SoVRMLTimeSensor::write(SoWriteAction * action)
{
  // Note: the code in this method matches that of SoElapsedTime and
  // SoOneShot and SoTimeSensor, so if any bugs are found and
  // corrected, remember to pass on the updates.

  // Disconnect from realTime field.
  SoField * connectfield = NULL;
  SbBool connectfromrealTime =
    this->timeIn.getConnectedField(connectfield) &&
    connectfield == SoDB::getGlobalField("realTime");
  SbBool defaultflag = this->timeIn.isDefault();
  if (connectfromrealTime) {
    this->timeIn.disconnect();
    this->timeIn.setDefault(TRUE);
  }

  inherited::write(action);

  // Re-connect to realTime field.
  if (connectfromrealTime) {
    this->timeIn.connectFrom(connectfield);
    this->timeIn.setDefault(defaultflag);
  }
}


// Doc in parent
void
SoVRMLTimeSensor::evaluate(void)
{
  SO_ENGINE_OUTPUT(time, SoSFTime, setValue(THIS->currtime));
  SO_ENGINE_OUTPUT(isActive, SoSFBool, setValue(THIS->running));
  SO_ENGINE_OUTPUT(cycleTime, SoSFTime, setValue(THIS->cyclestart));
  SO_ENGINE_OUTPUT(fraction_changed, SoSFFloat, setValue(THIS->fraction));
}

// Doc in parent
void
SoVRMLTimeSensor::inputChanged(SoField * which)
{
  // Default is to not do any notification when we return from this
  // function to SoEngine::notify(). This is an optimization for this
  // engine to avoid transmission of notification to all slave fields
  // each time the timeIn field is updated.
  this->fraction_changed.enable(FALSE);
  this->isActive.enable(FALSE);
  this->cycleTime.enable(FALSE);

  if (which == &this->enabled) {
    SbBool on = this->enabled.getValue();

    if (!on) this->timeIn.enableNotify(FALSE);

    if (THIS->running && !on) {
      THIS->running = FALSE;
      this->fraction_changed.enable(TRUE);
      this->isActive.enable(TRUE);
    }
    else if (!THIS->running && on) {
      which = &this->startTime; // warning, hack
    }
  }

  if (which == &this->loop) {
    THIS->loop = this->loop.getValue();
    if (THIS->loop == TRUE && !this->timeIn.isNotifyEnabled())
      which = &this->startTime; // warning hack
  }

  if (which == &this->startTime) {
    double currtime = this->timeIn.getValue().getValue();
    THIS->starttime = currtime;
    if (!THIS->running) {
      THIS->starttime = this->startTime.getValue().getValue();
      if (currtime >= THIS->starttime) {
        SbBool old = this->timeIn.enableNotify(TRUE);
        assert(old == FALSE);
        which = &this->timeIn; // warning, hack
      }
    }
  }

  if (which == &this->timeIn) {
    double currtime = this->timeIn.getValue().getValue();
    if (!THIS->running) {
      if (currtime >= THIS->starttime) {
        this->isActive.enable(TRUE);
        this->cycleTime.enable(TRUE);
        THIS->cyclestart = THIS->starttime;
        THIS->running = TRUE;
      }
      else return; // wait for startTime
    }
    THIS->currtime = currtime;
    this->time.enable(TRUE);
    this->fraction_changed.enable(TRUE);

    SbBool stopit = FALSE;
    if (currtime >= THIS->stoptime && THIS->stoptime > THIS->starttime) stopit = TRUE;

    double difftime = currtime - THIS->cyclestart;

    if (difftime > THIS->cycletime) {
      this->cycleTime.enable(TRUE);
      double num = difftime / THIS->cycletime;
      THIS->cyclestart += THIS->cycletime * floor(num);
      difftime = currtime - THIS->cyclestart;

      if (THIS->loop == FALSE) stopit = TRUE;
    }
    THIS->fraction = difftime / THIS->cycletime;

    if (stopit) {
      THIS->running = FALSE;
      this->isActive.enable(TRUE);
      this->fraction_changed.enable(FALSE);
      this->timeIn.enableNotify(FALSE);
    }
  }
  else if (which == &this->stopTime) {
    THIS->stoptime = this->stopTime.getValue().getValue();
  }
  else if (which == &this->cycleInterval) {
    THIS->cycletime = this->cycleInterval.getValue().getValue();
  }
}

// Doc in parent
void
SoVRMLTimeSensor::handleEvent(SoHandleEventAction * action)
{
  inherited::handleEvent(action);
}
