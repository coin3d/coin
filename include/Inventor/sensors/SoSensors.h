#ifndef COIN_SOSENSORS_H
#define COIN_SOSENSORS_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// Includes all sensor class hierarchy "leaf classes" (which again
// includes the sensor superclasses).  This includefile is provided
// for convenience. Try to use it as little as possible, though, as
// including it unnecessary slows down your compile process.

#include <Inventor/sensors/SoSensor.h>
#include <Inventor/sensors/SoDelayQueueSensor.h>
#include <Inventor/sensors/SoTimerQueueSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/sensors/SoPathSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

#endif // !COIN_SOSENSORS_H
