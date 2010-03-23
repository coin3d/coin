#ifndef COIN_SODBP_H
#define COIN_SODBP_H

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/SoDB.h>
#include <Inventor/SbString.h>

#include "misc/SbHash.h"

class SoSensor;
class SbRWMutex;

// *************************************************************************

class SoDB_HeaderInfo {
public:
  SoDB_HeaderInfo(const SbString & hs, const SbBool bin, const float ver,
                  SoDBHeaderCB * pre, SoDBHeaderCB * post, void * ud)
    : headerstring(hs), isbinary(bin), ivversion(ver),
        preload_cb(pre), postload_cb(post), userdata(ud)
    { }

  SbString headerstring;
  SbBool isbinary;
  float ivversion;
  SoDBHeaderCB * preload_cb, * postload_cb;
  void * userdata;
};

typedef SbHash<uint32_t, int16_t> UInt32ToInt16Map;

// *************************************************************************

class SoDBP {
public:
  struct EnvVars {
    static const char * COIN_PROFILER;
    static const char * COIN_PROFILER_OVERLAY;
  };

  static void variableArgsSanityCheck(void);

  static void clean(void);
  static void removeRealTimeFieldCB(void);
  static void updateRealTimeFieldCB(void * data, SoSensor * sensor);
  static void listWin32ProcessModules(void);

#ifdef COIN_THREADSAFE
  static SbRWMutex * globalmutex;
#endif // COIN_THREADSAFE
  static SbList<SoDB_HeaderInfo *> * headerlist;
  static SoSensorManager * sensormanager;
  static SoTimerSensor * globaltimersensor;
  static UInt32ToInt16Map * converters;
  static int notificationcounter;
  static SbBool isinitialized;

  static SbBool is3dsFile(SoInput * in);
  static SoSeparator * read3DSFile(SoInput * in);

  static void progress(const SbName & itemid,
                       float fraction,
                       SbBool interruptible);

  struct ProgressCallbackInfo {
    SoDB::ProgressCallbackType * func;
    void * userdata;

    int operator==(const ProgressCallbackInfo & a) {
      return (a.func == this->func) && (a.userdata == this->userdata);
    }
  };
  static SbList<struct ProgressCallbackInfo> * progresscblist;
};

#endif // !COIN_SODBP_H
