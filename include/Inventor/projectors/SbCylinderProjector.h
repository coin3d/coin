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
