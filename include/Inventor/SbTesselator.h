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

#ifndef __SBTESSELATOR_H__
#define __SBTESSELATOR_H__

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>
#include <stddef.h>

struct SbTVertex;
class SbHeap;
class SbVec3f;

class SbTesselator
{
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

#endif // __SBTESSELATOR_H_
