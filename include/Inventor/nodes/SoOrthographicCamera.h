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

#ifndef __SOORTHOGRAPHICCAMERA_H__
#define __SOORTHOGRAPHICCAMERA_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoCamera.h>

#if defined(COIN_EXCLUDE_SOORTHOGRAPHICCAMERA)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOORTHOGRAPHICCAMERA

// *************************************************************************

class SoOrthographicCamera : public SoCamera {
    typedef SoCamera inherited;

  SO_NODE_HEADER(SoOrthographicCamera);

public:
  static void initClass(void);
  SoOrthographicCamera(void);

  SoSFFloat height;

  virtual void scaleHeight(float scaleFactor);
  virtual SbViewVolume getViewVolume(float useAspectRatio = 0.0f) const;

protected:
  virtual ~SoOrthographicCamera();

  virtual void viewBoundingBox(const SbBox3f &box, float aspect, float slack);
};

#endif // !__SOORTHOGRAPHICCAMERA_H__
