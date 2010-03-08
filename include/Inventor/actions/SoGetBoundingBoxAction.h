#ifndef COIN_SOGETBOUNDINGBOXACTION_H
#define COIN_SOGETBOUNDINGBOXACTION_H

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
#include <Inventor/tools/SbLazyPimplPtr.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbXfBox3f.h>

class SoGetBoundingBoxActionP;

class COIN_DLL_API SoGetBoundingBoxAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGetBoundingBoxAction);

public:
  static void initClass(void);

  SoGetBoundingBoxAction(const SbViewportRegion & vp);
  virtual ~SoGetBoundingBoxAction(void);

  enum ResetType { TRANSFORM = 0x1, BBOX = 0x2, ALL = TRANSFORM | BBOX };

  void setViewportRegion(const SbViewportRegion & newregion);
  const SbViewportRegion & getViewportRegion(void) const;

  SbBox3f getBoundingBox(void) const;
  SbXfBox3f & getXfBoundingBox(void);

  const SbVec3f & getCenter(void) const;

  void setInCameraSpace(const SbBool flag);
  SbBool isInCameraSpace(void) const;

  void setResetPath(const SoPath * path, const SbBool resetbefore = TRUE,
                    const ResetType what = ALL);
  const SoPath * getResetPath(void) const;
  SbBool isResetPath(void) const;
  SbBool isResetBefore(void) const;
  SoGetBoundingBoxAction::ResetType getWhatReset(void) const;


  void checkResetBefore(void);
  void checkResetAfter(void);

  void extendBy(const SbBox3f & box);
  void extendBy(const SbXfBox3f & box);

  void setCenter(const SbVec3f & center, const SbBool transformcenter);
  SbBool isCenterSet(void) const;
  void resetCenter(void);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  enum { CENTER_SET = 0x1, CAMERA_SPACE = 0x2, RESET_BEFORE= 0x4 };

  SbXfBox3f bbox;
  SbVec3f center;
  SbViewportRegion vpregion;
  ResetType resettype;
  const SoPath * resetpath;
  unsigned int flags;

private:
  SbLazyPimplPtr<SoGetBoundingBoxActionP> pimpl;

  SoGetBoundingBoxAction(const SoGetBoundingBoxAction & rhs);
  SoGetBoundingBoxAction & operator = (const SoGetBoundingBoxAction & rhs);
}; // SoGetBoundingBoxAction

#endif // !COIN_SOGETBOUNDINGBOXACTION_H
