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

#ifndef COIN_SOPERSPECTIVECAMERA_H
#define COIN_SOPERSPECTIVECAMERA_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoCamera.h>


class SoPerspectiveCamera : public SoCamera {
  typedef SoCamera inherited;

  SO_NODE_HEADER(SoPerspectiveCamera);

public:
  static void initClass(void);
  SoPerspectiveCamera(void);

  SoSFFloat heightAngle;

  virtual void scaleHeight(float scalefactor);
  virtual SbViewVolume getViewVolume(float useaspectratio = 0.0f) const;

protected:
  virtual ~SoPerspectiveCamera();

  virtual void viewBoundingBox(const SbBox3f & box, float aspect, float slack);
};

#endif // !COIN_SOPERSPECTIVECAMERA_H
