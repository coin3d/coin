/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#if !defined(COIN_EXCLUDE_SOSENSOR)
#include <Inventor/sensors/SoSensor.h>
#endif // !COIN_EXCLUDE_SOSENSOR
#if !defined(COIN_EXCLUDE_SODELAYQUEUESENSOR)
#include <Inventor/sensors/SoDelayQueueSensor.h>
#endif // !COIN_EXCLUDE_SODELAYQUEUESENSOR
#if !defined(COIN_EXCLUDE_SOTIMERQUEUESENSOR)
#include <Inventor/sensors/SoTimerQueueSensor.h>
#endif // !COIN_EXCLUDE_SOTIMERQUEUESENSOR
#if !defined(COIN_EXCLUDE_SOALARMSENSOR)
#include <Inventor/sensors/SoAlarmSensor.h>
#endif // !COIN_EXCLUDE_SOALARMSENSOR
#if !defined(COIN_EXCLUDE_SOFIELDSENSOR)
#include <Inventor/sensors/SoFieldSensor.h>
#endif // !COIN_EXCLUDE_SOFIELDSENSOR
#if !defined(COIN_EXCLUDE_SOIDLESENSOR)
#include <Inventor/sensors/SoIdleSensor.h>
#endif // !COIN_EXCLUDE_SOIDLESENSOR
#if !defined(COIN_EXCLUDE_SONODESENSOR)
#include <Inventor/sensors/SoNodeSensor.h>
#endif // !COIN_EXCLUDE_SONODESENSOR
#if !defined(COIN_EXCLUDE_SOONESHOTSENSOR)
#include <Inventor/sensors/SoOneShotSensor.h>
#endif // !COIN_EXCLUDE_SOONESHOTSENSOR
#if !defined(COIN_EXCLUDE_SOPATHSENSOR)
#include <Inventor/sensors/SoPathSensor.h>
#endif // !COIN_EXCLUDE_SOPATHSENSOR
#if !defined(COIN_EXCLUDE_SOTIMERSENSOR)
#include <Inventor/sensors/SoTimerSensor.h>
#endif // !COIN_EXCLUDE_SOTIMERSENSOR

#endif // !__SOSENSORS_H__
