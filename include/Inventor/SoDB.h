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

#ifndef COIN_SODB_H
#define COIN_SODB_H

// FIXME: use configure to clean up this. 20000130 mortene.
#ifdef _WIN32
typedef struct fd_set fd_set;
#else // ! _WIN32
#include <unistd.h> // fd_set
#endif

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SoType.h>
#include <Inventor/sensors/SoSensorManager.h>

class SbName;
class SbTime;
class SoBase;
class SoField;
class SoFieldConverter;
class SoInput;
class SoNode;
class SoPath;
class SoSensor;
class SoSeparator;
class SoTimerSensor;
class SoVRMLGroup;

class SoDB_HeaderInfo;

typedef void SoDBHeaderCB(void * data, SoInput * in);


class SoDB {
public:
  static void init(void);

  static const char * getVersion(void);
  static SbBool read(SoInput * in, SoPath *& path);
  static SbBool read(SoInput * in, SoBase *& base);
  static SbBool read(SoInput * in, SoNode *& rootnode);
  static SoSeparator * readAll(SoInput * in);
  static SbBool isValidHeader(const char * teststring);
  static SbBool registerHeader(const SbString & headerstring,
                               SbBool isbinary,
                               float ivversion,
                               SoDBHeaderCB * precallback,
                               SoDBHeaderCB * postcallback,
                               void * userdata);
  static SbBool getHeaderData(const SbString & headerstring,
                              SbBool & isbinary,
                              float & ivversion,
                              SoDBHeaderCB *& precallback,
                              SoDBHeaderCB *& postcallback,
                              void *& userdata,
                              SbBool substringok = FALSE);
  static int getNumHeaders(void);
  static SbString getHeaderString(const int i);
  static SoField * createGlobalField(const SbName & name, SoType type);
  static SoField * getGlobalField(const SbName & name);
  static void renameGlobalField(const SbName & from, const SbName & to);

  static void setRealTimeInterval(const SbTime & interval);
  static const SbTime & getRealTimeInterval(void);
  static void enableRealTimeSensor(SbBool on);

  static SoSensorManager * getSensorManager(void);
  static void setDelaySensorTimeout(const SbTime & t);
  static const SbTime & getDelaySensorTimeout(void);
  static int doSelect(int nfds, fd_set * readfds, fd_set * writefds,
                      fd_set * exceptfds, struct timeval * usertimeout);

  static void addConverter(SoType from, SoType to, SoType converter);
  static SoFieldConverter * createConverter(SoType from, SoType to);

  static SbBool isInitialized(void);

  static void startNotify(void);
  static SbBool isNotifying(void);
  static void endNotify(void);

private:
  static void clean(void);
  static void updateRealTimeFieldCB(void * data, SoSensor * sensor);

  static SbList<SoDB_HeaderInfo *> * headerlist;
  static SoSensorManager * sensormanager;
  static SoTimerSensor * globaltimersensor;
  static SbTime * realtimeinterval;
  static SbDict * converters;
  static int notificationcounter;
  static SbBool isinitialized;
};

#endif // !COIN_SODB_H
