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

#ifndef COIN_SBCYLINDERPROJECTOR_H
#define COIN_SBCYLINDERPROJECTOR_H

#include <Inventor/projectors/SbProjector.h>
#include <Inventor/SbCylinder.h>

class COIN_DLL_EXPORT SbCylinderProjector : public SbProjector {
  typedef SbProjector inherited;

public:
  virtual SbVec3f project(const SbVec2f & point) = 0;
  SbVec3f projectAndGetRotation(const SbVec2f & point, SbRotation & rot);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2) = 0;
  void setCylinder(const SbCylinder & cyl);
  const SbCylinder & getCylinder(void) const;
  void setOrientToEye(const SbBool orienttoeye);
  SbBool isOrientToEye(void) const;
  void setFront(const SbBool infront);
  SbBool isFront(void) const;
  SbBool isPointInFront(const SbVec3f & point) const;
  virtual void setWorkingSpace(const SbMatrix & space);

protected:
  SbCylinderProjector(const SbBool orienttoeye);
  SbCylinderProjector(const SbCylinder & cylinder, const SbBool orienttoeye);

  SbBool intersectCylinderFront(const SbLine & line, SbVec3f & result);

  SbBool intersectFront;
  SbCylinder cylinder;
  SbBool orientToEye;
  SbBool needSetup;
  SbVec3f lastPoint;
};

#endif // !COIN_SBCYLINDERPROJECTOR_H
