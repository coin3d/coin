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

#ifndef __SBCYLINDERSECTIONPROJECTOR_H__
#define __SBCYLINDERSECTIONPROJECTOR_H__

#include <Inventor/projectors/SbCylinderProjector.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbRotation.h>


class SbCylinderSectionProjector : public SbCylinderProjector
{
  typedef SbCylinderProjector inherited;

public:
  SbCylinderSectionProjector(const float edgeTol = 0.9f,
                             const SbBool orientToEye = TRUE);
  SbCylinderSectionProjector(const SbCylinder &cyl,
                             const float edgeTol = 0.9f,
                             const SbBool orientToEye = TRUE);

  virtual SbProjector * copy(void) const;
  virtual SbVec3f project(const SbVec2f & point);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2);
  void setTolerance(const float edgeTol);
  float getTolerance(void) const;
  SbBool isWithinTolerance(const SbVec3f & point);

protected:
  virtual void setupTolerance(void);
  float tolerance;
  float tolDist;
  SbVec3f planeDir;
  SbLine planeLine;
  float planeDist;
  SbPlane tolPlane;

private:
  float sqrTolDist;
};

#endif // !__SBCYLINDERSECTIONPROJECTOR_H__
