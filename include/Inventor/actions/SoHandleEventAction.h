#ifndef COIN_SOHANDLEEVENTACTION_H
#define COIN_SOHANDLEEVENTACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>

class SbViewportRegion;
class SoEvent;
class SoPickedPoint;
class SoPickedPointList;

class SoHandleEventActionP;

class COIN_DLL_API SoHandleEventAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoHandleEventAction);

public:
  static void initClass(void);

  SoHandleEventAction(const SbViewportRegion & viewportregion);
  virtual ~SoHandleEventAction(void);

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
  SbPimplPtr<SoHandleEventActionP> pimpl;

  // NOT IMPLEMENTED:
  SoHandleEventAction(const SoHandleEventAction & rhs);
  SoHandleEventAction & operator = (const SoHandleEventAction & rhs);
}; // SoHandleEventAction

#ifndef COIN_INTERNAL
// For SGI / TGS Open Inventor compile-time compatibility.
#include <Inventor/actions/SoRayPickAction.h>
#endif // COIN_INTERNAL

#endif // !COIN_SOHANDLEEVENTACTION_H
