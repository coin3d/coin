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

#ifndef __SOEVENTCALLBACK_H__
#define __SOEVENTCALLBACK_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SoTypeList.h>


class SoEventCallback;
class SoPath;
class SoEvent;
class SoPickedPoint;
class SoHandleEventAction;

typedef void SoEventCallbackCB(void * userData, SoEventCallback * node);


class SoEventCallback : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoEventCallback);

public:
  static void initClass(void);
  SoEventCallback(void);

  void setPath(SoPath * path);
  const SoPath * getPath(void);

  void addEventCallback(SoType eventType, SoEventCallbackCB * f,
                        void * userData = NULL);
  void removeEventCallback(SoType eventType, SoEventCallbackCB * f,
                           void * userData = NULL);

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
  SoHandleEventAction * heaction;
  SbPList callbacks;
  SoTypeList callbackTypes;
};

#endif // !__SOEVENTCALLBACK_H__
