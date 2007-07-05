#ifndef COIN_SBTESSELATOR_H
#define COIN_SBTESSELATOR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>
#include <stddef.h>

struct SbTVertex;
class SbHeap;
class SbVec3f;

typedef void SbTesselatorCB(void * v0, void * v1, void * v2, void * data);

class COIN_DLL_API SbTesselator {
public:
  SbTesselator(SbTesselatorCB * func = NULL, void * data = NULL);
  ~SbTesselator(void);

  void beginPolygon(SbBool keepVertices = FALSE,
                    const SbVec3f & normal = SbVec3f(0.0f, 0.0f, 0.0f));
  void addVertex(const SbVec3f &v, void * data);
  void endPolygon(void);
  void setCallback(SbTesselatorCB * func, void * data);

private:
  struct SbTVertex * newVertex(void);
  void cleanUp(void);

  int currVertex;
  SbList <struct SbTVertex*> vertexStorage;
  SbHeap * heap;

  SbTVertex * headV, * tailV;
  int numVerts;
  SbVec3f polyNormal;
  int X,Y;
  int polyDir;
  void (*callback)(void * v0,void * v1,void * v2,void * data);
  void * callbackData;
  SbBool hasNormal;
  SbBool keepVertices;

  void emitTriangle(SbTVertex * v);
  void cutTriangle(SbTVertex * t);
  void calcPolygonNormal(void);

  SbBool circleCenter(const SbVec3f &a, const SbVec3f &b,
                      const SbVec3f &c, float &cx, float &cy);
  float circleSize(const SbVec3f &a, const SbVec3f &b, const SbVec3f &c);
  float circleSize(SbTVertex * v);
  float dot2D(const SbVec3f &v1, const SbVec3f &v2);
  SbBool clippable(SbTVertex * v);
  SbBool isTriangle(SbTVertex * v);
  SbBool pointInTriangle(SbTVertex * p, SbTVertex * t);
  float area(SbTVertex * t);

  static float heap_evaluate(void * v);
  static int heap_compare(void * v0, void * v1);
};

#endif // !COIN_SBTESSELATOR_H
