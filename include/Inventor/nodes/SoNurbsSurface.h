#ifndef COIN_SONURBSSURFACE_H
#define COIN_SONURBSSURFACE_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFFloat.h>

class COIN_DLL_API SoNurbsSurface : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoNurbsSurface);

public:
  static void initClass(void);
  SoNurbsSurface(void);

  SoSFInt32 numUControlPoints;
  SoSFInt32 numVControlPoints;
  SoSFInt32 numSControlPoints;
  SoSFInt32 numTControlPoints;
  SoMFFloat uKnotVector;
  SoMFFloat vKnotVector;
  SoMFFloat sKnotVector;
  SoMFFloat tKnotVector;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  void sendPrimitive(SoAction *,  SoPrimitiveVertex *);

protected:
  virtual ~SoNurbsSurface();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  SoDetail * createTriangleDetail(SoRayPickAction * action,
                                  const SoPrimitiveVertex * v1,
                                  const SoPrimitiveVertex * v2,
                                  const SoPrimitiveVertex * v3,
                                  SoPickedPoint * pp);
private:
  class SoNurbsSurfaceP * pimpl;
  friend class SoNurbsSurfaceP;
};

#endif // !COIN_SONURBSSURFACE_H
