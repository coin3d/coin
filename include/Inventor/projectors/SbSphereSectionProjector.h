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

#ifndef __SBSPHERESECTIONPROJECTOR_H__
#define __SBSPHERESECTIONPROJECTOR_H__

#include <Inventor/projectors/SbSphereProjector.h>
#include <Inventor/SbPlane.h>

class SbSphereSectionProjector : public SbSphereProjector
{
  typedef SbSphereProjector inherited;

public:
  SbSphereSectionProjector(const float edgeTol = 0.9f,
                           const SbBool orientToEye = TRUE);
  SbSphereSectionProjector(const SbSphere & sph,
                           const float edgeTol = 0.9f,
                           const SbBool orientToEye = TRUE);

  virtual SbProjector * copy(void) const;
  virtual SbVec3f project(const SbVec2f & point);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2);
  void setTolerance(const float edgeTol);
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
  float sqrTolDist;
};

#endif // !__SBSPHERESECTIONPROJECTOR_H__
