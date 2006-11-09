#ifndef COIN_SODBP_H
#define COIN_SODBP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/SoDB.h>
#include <Inventor/SbString.h>
#include <Inventor/misc/SbHash.h>

class SoSensor;

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

typedef SbHash<int16_t, uint32_t> UInt32ToInt16Map;

// *************************************************************************

class SoDBP {
public:
  static void variableArgsSanityCheck(void);

  static void clean(void);
  static void updateRealTimeFieldCB(void * data, SoSensor * sensor);
  static void listWin32ProcessModules(void);

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