#ifndef COIN_SOCAMERA_H
#define COIN_SOCAMERA_H

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
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>

#include <Inventor/SbVec3f.h>
#include <Inventor/SbBox3f.h>

#define SO_ASPECT_SQUARE        1.0f
#define SO_ASPECT_VIDEO         (4.0f/3.0f)
#define SO_ASPECT_35mm_ACADEMY  1.371
#define SO_ASPECT_16mm          1.369
#define SO_ASPECT_35mm_FULL     1.33333
#define SO_ASPECT_70mm          2.287
#define SO_ASPECT_CINEMASCOPE   2.35
#define SO_ASPECT_HDTV          (16.0f/9.0f)
#define SO_ASPECT_PANAVISION    2.361
#define SO_ASPECT_35mm          (3.0f/2.0f)
#define SO_ASPECT_VISTAVISION   2.301

class SoPath;

class COIN_DLL_API SoCamera : public SoNode {
  typedef SoNode inherited;

  SO_NODE_ABSTRACT_HEADER(SoCamera);

public:
  static void initClass(void);

  enum ViewportMapping {
    CROP_VIEWPORT_FILL_FRAME,
    CROP_VIEWPORT_LINE_FRAME,
    CROP_VIEWPORT_NO_FRAME,
    ADJUST_CAMERA,
    LEAVE_ALONE
  };

  SoSFEnum viewportMapping;
  SoSFVec3f position;
  SoSFRotation orientation;
  SoSFFloat aspectRatio;
  SoSFFloat nearDistance;
  SoSFFloat farDistance;
  SoSFFloat focalDistance;

  void pointAt(const SbVec3f & targetpoint);
  void pointAt(const SbVec3f & targetpoint, const SbVec3f & upvector);
  virtual void scaleHeight(float scalefactor) = 0;
  virtual SbViewVolume getViewVolume(float useaspectratio = 0.0f) const = 0;
  void viewAll(SoNode * const sceneroot, const SbViewportRegion & vpregion,
               const float slack = 1.0f);
  void viewAll(SoPath * const path, const SbViewportRegion & vpregion,
               const float slack = 1.0f);
  SbViewportRegion getViewportBounds(const SbViewportRegion & region) const;

  enum StereoMode {
    MONOSCOPIC,
    LEFT_VIEW,
    RIGHT_VIEW
  };

  void setStereoMode(StereoMode mode);
  StereoMode getStereoMode(void) const;

  void setStereoAdjustment(float adjustment);
  float getStereoAdjustment(void) const;
  void setBalanceAdjustment(float adjustment);
  float getBalanceAdjustment(void) const;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  SoCamera(void);
  virtual ~SoCamera();

  virtual void viewBoundingBox(const SbBox3f & box, float aspect,
                               float slack) = 0;
  virtual void jitter(int numpasses, int curpass,
                      const SbViewportRegion & vpreg,
                      SbVec3f & jitteramount) const;

private:
  void getView(SoAction * action, SbViewVolume & resultvv,
               SbViewportRegion & resultvp,
               const SbBool considermodelmatrix = TRUE);

  void drawCroppedFrame(SoGLRenderAction * action,
                        const int viewportmapping,
                        const SbViewportRegion & oldvp,
                        const SbViewportRegion & newvp);

  void lookAt(const SbVec3f & dir, const SbVec3f & up);

  StereoMode stereomode;
  float stereoadjustment;
  float balanceadjustment;
};

#endif // !COIN_SOCAMERA_H
