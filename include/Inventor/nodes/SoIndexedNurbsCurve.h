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

#ifndef COIN_SOINDEXEDNURBSCURVE_H
#define COIN_SOINDEXEDNURBSCURVE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFFloat.h>


class COIN_DLL_EXPORT SoIndexedNurbsCurve : public SoShape {
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
  void * nurbsrenderer;
  void doNurbs(SoAction * action, const SbBool glrender, const SbBool drawaspoints);

  static void tessBegin(int type, void * data);
  static void tessTexCoord(float * texcoord, void * data);
  static void tessNormal(float * normal, void * data);
  static void tessVertex(float * vertex, void * data);
  static void tessEnd(void * data);
};

#endif // !COIN_SOINDEXEDNURBSCURVE_H
