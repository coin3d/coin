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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOINDEXEDNURBSSURFACE_H
#define COIN_SOINDEXEDNURBSSURFACE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFFloat.h>


class COIN_DLL_API SoIndexedNurbsSurface : public SoShape {
    typedef SoShape inherited;

  SO_NODE_HEADER(SoIndexedNurbsSurface);

public:
  static void initClass(void);
  SoIndexedNurbsSurface(void);

  SoSFInt32 numUControlPoints;
  SoSFInt32 numVControlPoints;
  SoMFInt32 coordIndex;
  SoMFFloat uKnotVector;
  SoMFFloat vKnotVector;
  SoSFInt32 numSControlPoints;
  SoSFInt32 numTControlPoints;
  SoMFInt32 textureCoordIndex;
  SoMFFloat sKnotVector;
  SoMFFloat tKnotVector;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  void sendPrimitive(SoAction *,  SoPrimitiveVertex *);

protected:
  virtual ~SoIndexedNurbsSurface();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  SoDetail * createTriangleDetail(SoRayPickAction * action,
                                  const SoPrimitiveVertex * v1,
                                  const SoPrimitiveVertex * v2,
                                  const SoPrimitiveVertex * v3,
                                  SoPickedPoint * pp);
private:
  void * nurbsrenderer;
  void doNurbs(SoAction * action, const SbBool glrender);

  static void tessBegin(int , void * data);
  static void tessTexCoord(float * texcoord, void * data);
  static void tessNormal(float * normal, void * data);
  static void tessVertex(float * vertex, void * data);
  static void tessEnd(void * data);

};

#endif // !COIN_SOINDEXEDNURBSSURFACE_H
