#ifndef COIN_SBPLANEPROJECTOR_H
#define COIN_SBPLANEPROJECTOR_H

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

#include <Inventor/projectors/SbProjector.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbPlane.h>

class COIN_DLL_API SbPlaneProjector : public SbProjector {
  typedef SbProjector inherited;

public:
  SbPlaneProjector(const SbBool orient = FALSE);
  SbPlaneProjector(const SbPlane & plane, const SbBool orient = FALSE);
  virtual SbProjector * copy(void) const;

  virtual SbVec3f project(const SbVec2f & point);
  void setPlane(const SbPlane & plane);
  const SbPlane & getPlane(void) const;
  void setOrientToEye(const SbBool orienttoeye);
  SbBool isOrientToEye(void) const;
  virtual SbVec3f getVector(const SbVec2f & viewpos1,
                            const SbVec2f & viewpos2);
  virtual SbVec3f getVector(const SbVec2f & viewpos);
  void setStartPosition(const SbVec2f & viewpos);
  void setStartPosition(const SbVec3f & point);

protected:
  void setupPlane(void);

protected:
  SbPlane plane, nonOrientPlane;
  SbBool orientToEye;
  SbBool needSetup;
  SbVec3f lastPoint;
};

#endif // !COIN_SBPLANEPROJECTOR_H
