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

#ifndef __SBPLANEPROJECTOR_H__
#define __SBPLANEPROJECTOR_H__

#include <Inventor/projectors/SbProjector.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbPlane.h>


class SbPlaneProjector : public SbProjector {
  typedef SbProjector inherited;

public:
  SbPlaneProjector(const SbBool orient = FALSE);
  SbPlaneProjector(const SbPlane & plane, const SbBool orient = FALSE);
  virtual SbProjector * copy(void) const;

  virtual SbVec3f project(const SbVec2f & point);
  void setPlane(const SbPlane & plane);
  const SbPlane & getPlane(void) const;
  void setOrientToEye(const SbBool orientToEye);
  SbBool isOrientToEye(void) const;
  virtual SbVec3f getVector(const SbVec2f & mousePosition1,
                            const SbVec2f & mousePosition2);
  virtual SbVec3f getVector(const SbVec2f & mousePosition);
  void setStartPosition(const SbVec2f & mousePosition);
  void setStartPosition(const SbVec3f & point);

protected:
  void setupPlane(void);

protected:
  SbPlane plane, nonOrientPlane;
  SbBool orientToEye;
  SbBool needSetup;
  SbVec3f lastPoint;
};

#endif // !__SBPLANEPROJECTOR_H__
