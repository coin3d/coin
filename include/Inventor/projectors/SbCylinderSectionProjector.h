/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SBCYLINDERSECTIONPROJECTOR_H
#define COIN_SBCYLINDERSECTIONPROJECTOR_H

#include <Inventor/projectors/SbCylinderProjector.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbRotation.h>


class COIN_DLL_API SbCylinderSectionProjector : public SbCylinderProjector {
  typedef SbCylinderProjector inherited;

public:
  SbCylinderSectionProjector(const float edgetol = 0.9f,
                             const SbBool orienttoeye = TRUE);
  SbCylinderSectionProjector(const SbCylinder & cyl,
                             const float edgetol = 0.9f,
                             const SbBool orienttoeye = TRUE);

  virtual SbProjector * copy(void) const;
  virtual SbVec3f project(const SbVec2f & point);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2);
  void setTolerance(const float edgetol);
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
  float sqrtoldist;
};

#endif // !COIN_SBCYLINDERSECTIONPROJECTOR_H
