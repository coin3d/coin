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


class SoGetBoundingBoxAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGetBoundingBoxAction);

public:

  enum ResetType {
    TRANSFORM = 0x1,
    BBOX      = 0x2,
    ALL       = TRANSFORM | BBOX
  };

  SoGetBoundingBoxAction(const SbViewportRegion & viewportRegion);
  virtual ~SoGetBoundingBoxAction();

  static void initClass(void);

  void setViewportRegion(const SbViewportRegion & newRegion);
  const SbViewportRegion & getViewportRegion() const;
  SbBox3f getBoundingBox() const;
  SbXfBox3f & getXfBoundingBox();
  const SbVec3f & getCenter() const;
  void setInCameraSpace(const SbBool flag);
  SbBool isInCameraSpace() const;
  void setResetPath(const SoPath * path,
                    const SbBool resetBefore = TRUE,
                    const ResetType what = ALL);
  const SoPath * getResetPath() const;
  SbBool isResetPath() const;
  SbBool isResetBefore() const;
  ResetType getWhatReset() const;


  void checkResetBefore(); // for OI compability
  void checkResetAfter();

  void extendBy(const SbBox3f & box);
  void extendBy(const SbXfBox3f & box);

  void setCenter(const SbVec3f & center,
                 const SbBool transformCenter);
  SbBool isCenterSet() const;
  void resetCenter();

protected:
  virtual void beginTraversal(SoNode * const node);

private:
  SbXfBox3f bbox;
  SbVec3f center;
  SbViewportRegion vpRegion;
  ResetType resetType;
  const SoPath * resetPath;
  unsigned int flags;
};

#endif // !COIN_SOGETBOUNDINGBOXACTION_H
