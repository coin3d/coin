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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SBSPHEREPROJECTOR_H__
#define __SBSPHEREPROJECTOR_H__

#include <Inventor/projectors/SbProjector.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbRotation.h>


class SbSphereProjector : public SbProjector {
  typedef SbProjector inherited;

public:
  ~SbSphereProjector();

  SbVec3f projectAndGetRotation(const SbVec2f & point, SbRotation & rot);
  virtual SbRotation getRotation(const SbVec3f & point1,
				 const SbVec3f & point2) = 0;

  void setSphere(const SbSphere & sph);
  const SbSphere & getSphere(void) const;
  void setOrientToEye(SbBool orientToEye);
  SbBool isOrientToEye(void) const;
  void setFront(SbBool inFront);
  SbBool isFront(void) const;
  SbBool isPointInFront(const SbVec3f & point) const;
  virtual void setWorkingSpace(const SbMatrix & space);

protected:
  SbSphereProjector(SbBool orientToEye);
  SbSphereProjector(const SbSphere & s, SbBool orientToEye);

  SbBool intersectSphereFront(const SbLine & l, SbVec3f & result);

  SbBool intersectFront;
  SbSphere sphere;
  SbBool orientToEye;
  SbBool needSetup;
  SbVec3f lastPoint;
};

#endif // !__SBSPHEREPROJECTOR_H__
