#ifndef COIN_SOEVENTCALLBACK_H
#define COIN_SOEVENTCALLBACK_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/lists/SbList.h>

#ifndef COIN_INTERNAL
// Added for Inventor compliance
#include <Inventor/actions/SoHandleEventAction.h>
#endif // !COIN_INTERNAL

class SoEventCallback;
class SoPath;
class SoEvent;
class SoPickedPoint;
class SoHandleEventAction;

typedef void SoEventCallbackCB(void * userdata, SoEventCallback * node);

class COIN_DLL_API SoEventCallback : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoEventCallback);

public:
  static void initClass(void);
  SoEventCallback(void);

  void setPath(SoPath * path);
  const SoPath * getPath(void);

  void addEventCallback(SoType eventtype, SoEventCallbackCB * f,
                        void * userdata = NULL);
  void removeEventCallback(SoType eventtype, SoEventCallbackCB * f,
                           void * userdata = NULL);

  SoHandleEventAction * getAction(void) const;
  const SoEvent * getEvent(void) const;
  const SoPickedPoint * getPickedPoint(void) const;

  void setHandled(void);
  SbBool isHandled(void) const;

  void grabEvents(void);
  void releaseEvents(void);

protected:
  virtual ~SoEventCallback();

  virtual void handleEvent(SoHandleEventAction * action);

private:

  struct CallbackInfo {
    SoEventCallbackCB * func;
    SoType eventtype;
    void * userdata;

    // AIX native compiler xlC needs equality and inequality operators
    // to compile templates where these operators are referenced (even
    // if they are actually never used).

    SbBool operator==(const CallbackInfo & cbi) {
      return this->func == cbi.func && this->eventtype == cbi.eventtype && this->userdata == cbi.userdata;
    }
    SbBool operator!=(const CallbackInfo & cbi) {
      return !(*this == cbi);
    }
  };

  SbList<CallbackInfo> callbacks;
  SoHandleEventAction * heaction;
  SoPath * path;

};

#endif // !COIN_SOEVENTCALLBACK_H
