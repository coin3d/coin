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

#ifndef COIN_SBSPHERESECTIONPROJECTOR_H
#define COIN_SBSPHERESECTIONPROJECTOR_H

#include <Inventor/projectors/SbSphereProjector.h>
#include <Inventor/SbPlane.h>


class COIN_DLL_EXPORT SbSphereSectionProjector : public SbSphereProjector {
  typedef SbSphereProjector inherited;

public:
  SbSphereSectionProjector(const float edgetol = 0.9f,
                           const SbBool orienttoeye = TRUE);
  SbSphereSectionProjector(const SbSphere & sph,
                           const float edgetol = 0.9f,
                           const SbBool orienttoeye = TRUE);

  virtual SbProjector * copy(void) const;
  virtual SbVec3f project(const SbVec2f & point);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2);
  void setTolerance(const float edgetol);
  float getTolerance(void) const;
  void setRadialFactor(const float rad = 0.0f);
  float getRadialFactor(void) const;
  SbBool isWithinTolerance(const SbVec3f & point);

protected:
  virtual void setupTolerance(void);

  float tolerance;
  float tolDist;
  float radialFactor;
  SbVec3f planePoint;
  SbVec3f planeDir;
  float planeDist;
  SbPlane tolPlane;

private:
  float sqrtoldist;
};

#endif // !COIN_SBSPHERESECTIONPROJECTOR_H
