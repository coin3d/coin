#ifndef COIN_SOINDEXEDNURBSCURVE_H
#define COIN_SOINDEXEDNURBSCURVE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFFloat.h>

template<class Master> class SoNurbsP;

class COIN_DLL_API SoIndexedNurbsCurve : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoIndexedNurbsCurve);

public:
  static void initClass(void);
  SoIndexedNurbsCurve(void);

  SoSFInt32 numControlPoints;
  SoMFInt32 coordIndex;
  SoMFFloat knotVector;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  void sendPrimitive(SoAction *,  SoPrimitiveVertex *);

protected:
  virtual ~SoIndexedNurbsCurve();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  virtual SoDetail * createLineSegmentDetail(SoRayPickAction * action,
                                             const SoPrimitiveVertex * v1,
                                             const SoPrimitiveVertex * v2,
                                             SoPickedPoint * pp);
private:
  class SoIndexedNurbsCurveP * pimpl;
  friend class SoIndexedNurbsCurveP;
  friend class SoNurbsP<SoIndexedNurbsCurve>;
};

#endif // !COIN_SOINDEXEDNURBSCURVE_H
