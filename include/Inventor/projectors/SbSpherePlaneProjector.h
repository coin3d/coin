#ifndef COIN_SBSPHEREPLANEPROJECTOR_H
#define COIN_SBSPHEREPLANEPROJECTOR_H

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

#include <Inventor/projectors/SbSphereSectionProjector.h>

class COIN_DLL_API SbSpherePlaneProjector : public SbSphereSectionProjector {
  typedef SbSphereSectionProjector inherited;

public:
  SbSpherePlaneProjector(const float edgetol = 0.9f,
                         const SbBool orienttoeye = TRUE);
  SbSpherePlaneProjector(const SbSphere & sph,
                         const float edgetol = 0.9f,
                         const SbBool orienttoeye = TRUE);

  virtual SbProjector * copy(void) const;
  virtual SbVec3f project(const SbVec2f & point);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2);

protected:
  SbRotation getRotation(const SbVec3f & point1, const SbBool tol1,
                         const SbVec3f & point2, const SbBool tol2);
};

#endif // !COIN_SBSPHEREPLANEPROJECTOR_H
