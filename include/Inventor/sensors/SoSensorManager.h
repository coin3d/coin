/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSENSORMANAGER_H
#define COIN_SOSENSORMANAGER_H

#include <Inventor/lists/SbList.h>
#include <Inventor/SbTime.h>

// FIXME: do this properly through something configure based.
// 19991214 mortene.
// Note: fd_set is in time.h under AIX
#if defined(_WIN32) || defined(__BEOS__) || defined(_AIX)
typedef struct fd_set fd_set;
#endif //_WIN32 || __BEOS__

class SoDelayQueueSensor;
class SoTimerQueueSensor;
class SoTimerSensor;
class SbTime;

class COIN_DLL_EXPORT SoSensorManager {
public:
  SoSensorManager(void);
  ~SoSensorManager();

  void insertDelaySensor(SoDelayQueueSensor * s);
  void insertTimerSensor(SoTimerQueueSensor * s);
  void removeDelaySensor(SoDelayQueueSensor * s);
  void removeTimerSensor(SoTimerQueueSensor * s);

  void setChangedCallback(void (*func)(void *), void * data);

  void rescheduleTimer(SoTimerSensor * s);
  void removeRescheduledTimer(SoTimerQueueSensor * s);

  void processDelayQueue(SbBool isidle);
  void processImmediateQueue(void);
  void processTimerQueue(void);

  SbBool isDelaySensorPending(void);
  SbBool isTimerSensorPending(SbTime & tm);

  void setDelaySensorTimeout(const SbTime & t);
  const SbTime & getDelaySensorTimeout(void);

  int doSelect(int nfds, fd_set * readfds, fd_set * writefds,
               fd_set * exceptfds, struct timeval * userTimeOut);

private:
  void notifyChanged(void);

  int mergeTimerQueues(void);
  int mergeDelayQueues(void);

  SbBool processingtimerqueue, processingdelayqueue;

  SbList<SoDelayQueueSensor *> delayqueue, delaywaitqueue;
  SbList<SoTimerQueueSensor *> timerqueue, timerwaitqueue;

  void (*queueChangedCB)(void *);
  void * queueChangedCBData;

  SbTime delaysensortimeout;
};

#endif // !COIN_SOSENSORMANAGER_H
