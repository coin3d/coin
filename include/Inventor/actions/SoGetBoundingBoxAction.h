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

#ifndef COIN_SOGETBOUNDINGBOXACTION_H
#define COIN_SOGETBOUNDINGBOXACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbXfBox3f.h>


class COIN_DLL_EXPORT SoGetBoundingBoxAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGetBoundingBoxAction);

public:
  SoGetBoundingBoxAction(const SbViewportRegion & vp);
  virtual ~SoGetBoundingBoxAction();

  static void initClass(void);

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
  virtual void beginTraversal(SoNode * const node);

private:
  enum { CENTER_SET = 0x1, CAMERA_SPACE = 0x2, RESET_BEFORE= 0x4 };

  SbXfBox3f bbox;
  SbVec3f center;
  SbViewportRegion vpregion;
  ResetType resettype;
  const SoPath * resetpath;
  unsigned int flags;
};

#endif // !COIN_SOGETBOUNDINGBOXACTION_H
