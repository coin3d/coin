/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef _SO_TESSELLATOR_H_
#define _SO_TESSELLATOR_H_

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/lists/SbList.h>

class SoTessellator
{
public:
  struct SoTessVertex {
    SbVec3f v;
    void *data;
    SoTessVertex *next;
  };
private:  
  SoTessVertex *newVertex();
  int currVertex;
  SbList <SoTessVertex*> vertexStorage;

  SoTessVertex *headV,*tailV;  // list of vertices
  int nverts;              // nr. of vertices
  SbVec3f Normal;              // Normal-vector
  int X,Y;                  // Defines projection
  int Dir;                  // Direction of projected polygon
  void (*callback)(void *v0,void *v1,void *v2,void *data);
  void *callbackData;
  SbBool hasNormal;
  SbBool keepVertices;

  SbBool clippable(SoTessVertex *v);
  SbBool isTriangle(SoTessVertex *v);
  void emitTriangle(SoTessVertex *v);
  SbBool pointInTriangle(SoTessVertex *p,SoTessVertex *t);
  double area(SoTessVertex *t);
  void cutTriangle(SoTessVertex *t);
  void calcPolygonNormal();

  SbBool circleCenter(const SbVec3f &a, const SbVec3f &b, 
		      const SbVec3f &c, float &cx, float &cy);
  float circleSize(const SbVec3f &a, const SbVec3f &b, const SbVec3f &c);
  float circleSize(SoTessVertex *v);
  float dot2D(const SbVec3f &v1, const SbVec3f &v2);
  void cleanUp();
  
public:
  SoTessellator(void (*callback)(void *v0, void *v1, void *v2, void *data),
		void *userdata);
  ~SoTessellator();
  void beginPolygon(SbBool keepVertices=FALSE);
  void beginPolygon(const SbVec3f &normal,SbBool keepVertices=FALSE);
  void addVertex(const SbVec3f &v,void *data);
  void endPolygon();
  void setCallback(void (*callback)(void *v0,void *v1,void *v2,void *data),
		   void *userdata);
};

#endif // _SO_TESSELLATOR_H_
