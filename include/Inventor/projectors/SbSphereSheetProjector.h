#ifndef COIN_SBSPHERESHEETPROJECTOR_H
#define COIN_SBSPHERESHEETPROJECTOR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/projectors/SbSphereProjector.h>
#include <Inventor/SbPlane.h>

class COIN_DLL_API SbSphereSheetProjector : public SbSphereProjector {
  typedef SbSphereProjector inherited;

public:
  SbSphereSheetProjector(const SbBool orienttoeye = TRUE);
  SbSphereSheetProjector(const SbSphere & sph, const SbBool orienttoeye = TRUE);

  virtual SbProjector * copy(void) const;
  virtual SbVec3f project(const SbVec2f & point);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2);

protected:
  void setupPlane(void);

  SbVec3f workingProjPoint;
  SbVec3f planePoint;
  SbVec3f planeDir;
  float planeDist;
  SbPlane tolPlane;
};

#endif // !COIN_SBSPHERESHEETPROJECTOR_H
