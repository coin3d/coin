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

#ifndef COIN_SOHANDLEEVENTACTION_H
#define COIN_SOHANDLEEVENTACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbViewportRegion.h>

class SoEvent;
class SoPickedPoint;
class SoPickedPointList;
class SoRayPickAction;


class COIN_DLL_EXPORT SoHandleEventAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoHandleEventAction);

public:
  SoHandleEventAction(const SbViewportRegion & viewportregion);
  virtual ~SoHandleEventAction();
  static void initClass(void);

  void setViewportRegion(const SbViewportRegion & newregion);
  const SbViewportRegion & getViewportRegion(void) const;
  void setEvent(const SoEvent * ev);
  const SoEvent * getEvent(void) const;
  void setHandled(void);
  SbBool isHandled(void) const;
  void setGrabber(SoNode * node);
  void releaseGrabber(void);
  SoNode * getGrabber(void) const;
  void setPickRoot(SoNode * node);
  SoNode * getPickRoot(void) const;
  void setPickRadius(const float radiusinpixels);
  const SoPickedPoint * getPickedPoint(void);
  const SoPickedPointList & getPickedPointList(void);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  SoRayPickAction * getPickAction(void);

  SbViewportRegion viewport;
  const SoEvent * event;
  SoNode * grabber;
  SoNode * pickroot;
  SbBool pickvalid;
  SbBool didpickall;
  SoRayPickAction * pickaction;
  SoNode * applynode;
};

#endif // !COIN_SOHANDLEEVENTACTION_H
