#ifndef COIN_SBCYLINDERSHEETPROJECTOR_H
#define COIN_SBCYLINDERSHEETPROJECTOR_H

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

#include <Inventor/projectors/SbCylinderProjector.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbRotation.h>

class COIN_DLL_API SbCylinderSheetProjector : public SbCylinderProjector {
  typedef SbCylinderProjector inherited;

public:
  SbCylinderSheetProjector(const SbBool orienttoeye = TRUE);
  SbCylinderSheetProjector(const SbCylinder & cyl,
                           const SbBool orienttoeye = TRUE);

  virtual SbProjector * copy(void) const;
  virtual SbVec3f project(const SbVec2f & point);
  virtual SbRotation getRotation(const SbVec3f & point1,
                                 const SbVec3f & point2);

protected:
  virtual void setupPlane(void);

  SbVec3f workingProjPoint;
  SbVec3f planeDir;
  SbPlane tolPlane;
};

#endif // !COIN_SBCYLINDERSHEETPROJECTOR_H
