/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOSENSORS_H__
#define __SOSENSORS_H__

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

#endif // !__SOSENSORS_H__
