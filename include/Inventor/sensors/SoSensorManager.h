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

#ifndef __SOSENSORMANAGER_H__
#define __SOSENSORMANAGER_H__

#include <Inventor/lists/SbList.h>
#include <Inventor/SbTime.h>

#if defined(COIN_EXCLUDE_SOSENSORMANAGER)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSENSORMANAGER

#if defined(_WIN32) || defined(__BEOS__)
typedef struct fd_set fd_set;
#endif //_WIN32 || __BEOS__

class SoDelayQueueSensor;
class SoTimerQueueSensor;
class SoTimerSensor;
class SbTime;

class SoSensorManager {
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

  void processDelayQueue(SbBool isIdle);
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

#endif // !__SOSENSORMANAGER_H__
