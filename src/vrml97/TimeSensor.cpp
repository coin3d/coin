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
SoVRMLTimeSensor::writeInstance(SoOutput * out)
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

  inherited::writeInstance(out);

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
