#ifndef COIN_SBSPHERESECTIONPROJECTOR_H
#define COIN_SBSPHERESECTIONPROJECTOR_H

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

#include <Inventor/projectors/SbSphereProjector.h>
#include <Inventor/SbPlane.h>

class COIN_DLL_API SbSphereSectionProjector : public SbSphereProjector {
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
