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

#ifndef COIN_SBTESSELATOR_H
#define COIN_SBTESSELATOR_H

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>
#include <stddef.h>

struct SbTVertex;
class SbHeap;
class SbVec3f;

class COIN_DLL_EXPORT SbTesselator {
public:
  SbTesselator(void (*callback)(void * v0, void * v1, void * v2,
                                void * data) = NULL, void * userdata = NULL);
  ~SbTesselator(void);

  void beginPolygon(SbBool keepVertices = FALSE,
                    const SbVec3f & normal = SbVec3f(0.0f, 0.0f, 0.0f));
  void addVertex(const SbVec3f &v, void *data);
  void endPolygon(void);
  void setCallback(void (*callback)(void *v0, void *v1, void *v2, void *data),
                   void *data);

private:
  struct SbTVertex *newVertex(void);
  void cleanUp(void);

  int currVertex;
  SbList <struct SbTVertex*> vertexStorage;
  SbHeap *heap;

  SbTVertex *headV,*tailV;
  int numVerts;
  SbVec3f polyNormal;
  int X,Y;
  int polyDir;
  void (*callback)(void *v0,void *v1,void *v2,void *data);
  void *callbackData;
  SbBool hasNormal;
  SbBool keepVertices;

  void emitTriangle(SbTVertex *v);
  void cutTriangle(SbTVertex *t);
  void calcPolygonNormal(void);

  SbBool circleCenter(const SbVec3f &a, const SbVec3f &b,
                      const SbVec3f &c, float &cx, float &cy);
  float circleSize(const SbVec3f &a, const SbVec3f &b, const SbVec3f &c);
  float circleSize(SbTVertex *v);
  float dot2D(const SbVec3f &v1, const SbVec3f &v2);
  SbBool clippable(SbTVertex *v);
  SbBool isTriangle(SbTVertex *v);
  SbBool pointInTriangle(SbTVertex *p,SbTVertex *t);
  float area(SbTVertex *t);

  static float heap_evaluate(void *v);
};

#endif // COIN_SBTESSELATOR_H
