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

#ifndef COIN_SBPLANEPROJECTOR_H
#define COIN_SBPLANEPROJECTOR_H

#include <Inventor/projectors/SbProjector.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbPlane.h>


class COIN_DLL_EXPORT SbPlaneProjector : public SbProjector {
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
