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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoTessellator include/Inventor/misc/SoTessellator.h
  The SoTessellator class is used to tessellate polygons into triangles.
  
  
*/

#define TESS_EPSILON 1e-6   //epsilon for compares

#define NODELAUNEY // until heap is added...

#include <Inventor/misc/SoTessellator.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#ifdef _WIN32
#include "float.h"
#endif


//projection enums
enum {OXY,OXZ,OYZ};

/*!
  Constructor with \a callback being the triangle callback, and 
  \a data being the user data which is supplied with each callback.
*/
SoTessellator::SoTessellator(void (*callback)(void *v0, 
					      void *v1, 
					      void *v2, 
					      void *data),
			     void *userdata)
{
  setCallback(callback, userdata);
  this->headV = this->tailV = NULL;
  this->currVertex = 0;
}

/*!
  Destructor
*/
SoTessellator::~SoTessellator()
{
  cleanUp();
  int i,n = this->vertexStorage.getLength();
  for (i = 0; i < n; i++) delete this->vertexStorage[i];
}

void
SoTessellator::cleanUp()
{
  currVertex = 0;
  headV = tailV = NULL;
}

/*!
  Sets the triangle callback.
*/
void 
SoTessellator::setCallback(void (*callback)(void *v0,
					    void *v1,
					    void *v2,
					    void *data),
			   void *userdata)
{
  this->callback=callback;
  this->callbackData=userdata;
}

/*!
  Called to signal a new polygon. \a normal is the polygon normal,
  and \a keepVerts should be \e TRUE if all vertices should be
  included in the final tessellation.

  \sa SoTessellator::addVertex()
  \sa SoTessellator::endPolygon()
*/
void
SoTessellator::beginPolygon(const SbVec3f &normal, SbBool keepVerts)
{
  cleanUp(); // in case previous polygon was not ended properly
  this->keepVertices=keepVerts;
  this->Normal=normal;
  this->hasNormal=TRUE;
  this->nverts=0;
}

/*!
  \overload
*/
void
SoTessellator::beginPolygon(SbBool keepVerts)
{
  cleanUp(); // in case previous polygon was not ended properly
  this->keepVertices=keepVerts;
  this->hasNormal=FALSE;
  this->nverts=0;
}

/*!
  Adds a new vertex to the polygon. \a data is the vertex data that
  will be supplied in the triangle callback.
  \sa SoTessellator::beginPolygon()
  \sa SoTessellator::endPolygon()
  \sa SoTessellator::setCallback()
*/
void
SoTessellator::addVertex(const SbVec3f &v,void *data)
{
  SoTessVertex *newv = newVertex();
  newv->v=v;
  newv->data=data;
  newv->next=NULL;
  if (!this->headV) this->headV=newv;
  if (this->tailV) this->tailV->next=newv;
  this->tailV=newv;
  this->nverts++;
}

/*!
  Signals end of polygon. The tessellation will be performed, and
  the triangle callback will be called for each triangle.
  
  \sa SoTessellator::beginPolygon()
  \sa SoTessellator::endPolygon()
*/
void
SoTessellator::endPolygon()
{
  SoTessVertex *v,*tmpv,*stv;
  int count;

  if (this->nverts > 3) {
    if (!this->hasNormal) calcPolygonNormal();

    //Find best projection plane
    int projection;
    if (fabs(Normal[0])>fabs(Normal[1]))
      if (fabs(Normal[0])>fabs(Normal[2]))
	projection=OYZ;
      else projection=OXY;
    else
      if (fabs(Normal[1])>fabs(Normal[2]))
	projection=OXZ;
      else projection=OXY;
    
    switch (projection) {
    case OYZ:
      this->X=1;
      this->Y=2;
      this->Dir=(int)(Normal[0]/fabs(Normal[0]));
      break;
    case OXY:
      this->X=0;
      this->Y=1;
      this->Dir=(int)(Normal[2]/fabs(Normal[2]));
      break;
    case OXZ:
      this->X=2;
      this->Y=0;
      this->Dir=(int)(Normal[1]/fabs(Normal[1]));
      break;
    }
    
    //Make loop
    this->tailV->next = this->headV;
    
    //tesselation:
    v = this->headV;
    float smallestval;
    SoTessVertex *smallesttri;
    
    while (nverts>3) {
      count=0;
      smallestval = FLT_MAX;
      smallesttri = NULL;
      while(count < nverts && nverts >= 3) {
	//Triangle has area==0 => remove
	if (area(v) < TESS_EPSILON) {
	  if (!this->keepVertices) {
	    cutTriangle(v);
	    this->nverts--;
	    count=0;
	    smallestval = FLT_MAX;
	    smallesttri = NULL;
	  }
	  else {
	    count++;
	    v = v->next;
	  }
	}
	// if the 3 coordinates starting with v, is a triangle and
	// the triangle is clippable => calculate Delaunay value.
	else if (isTriangle(v) && clippable(v)) {
#ifdef NODELAUNEY
	  smallesttri = v;
	  break;
#endif
	  float val = circleSize(v);
	  if (val < smallestval) {
	    smallestval = val;
	    smallesttri = v;
	  }
	  count++;
	  v = v->next;
	}
	else {
	  count++;
	  v = v->next;
	}
      }
      if (smallesttri) {
	v = smallesttri;
	emitTriangle(smallesttri);
	this->nverts--;
      }
      else break;   // could not cut any triangle
    }
    if (smallesttri) {
      if (nverts==3) emitTriangle(v);   //Emit last triangle
    }
    // Emit the empty triangles that might lay around
    else if (this->keepVertices) { 
      while (this->nverts >= 3) {
	emitTriangle(v);
	this->nverts--;
      }
    }    
  }
  else if (this->nverts==3) {   //only one triangle
    emitTriangle(headV);
  }
  headV = tailV = NULL;
}


//
// PRIVATE FUNCTIONS:
//

//
// Checks if the point p lies inside the triangle
// starting with t.
// Algorithm from comp.graphics.algorithms FAQ
//
SbBool
SoTessellator::pointInTriangle(SoTessVertex *p,SoTessVertex *t)
{
  SbVec3f v1, v2;
  double x,y;
  SbBool tst=FALSE;
  
  x=p->v[X];
  y=p->v[Y];

  v1=t->v;
  v2=t->next->next->v;
  if ((((v1[Y]<=y) && (y<v2[Y])) || ((v2[Y]<=y)  && (y<v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) /  (v2[Y] - v1[Y]) + v1[X]))
    tst = (tst==FALSE ? TRUE : FALSE);
  v2=v1;
  v1=t->next->v;
  if ((((v1[Y]<=y) && (y<v2[Y])) || ((v2[Y]<=y)  && (y<v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) /  (v2[Y] - v1[Y]) + v1[X]))
    tst = (tst==FALSE ? TRUE : FALSE);
  v2=v1;
  v1=t->next->next->v;
  if ((((v1[Y]<=y) && (y<v2[Y])) || ((v2[Y]<=y)  && (y<v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) /  (v2[Y] - v1[Y]) + v1[X]))
    tst = (tst==FALSE ? TRUE : FALSE);

  return tst;
}

//
// Check if v points to a legal triangle (normal is right way)
// (i.e convex or concave corner)
//
SbBool
SoTessellator::isTriangle(SoTessVertex *v)
{
  if (((v->next->v[X]-v->v[X])*(v->next->next->v[Y]-v->v[Y])-
       (v->next->v[Y]-v->v[Y])*(v->next->next->v[X]-v->v[X]))*Dir>0.0)
    return TRUE;
  else return FALSE;
}

//
// Check if there are no intersection to the triangle
// pointed to by v. (no other vertices are inside the triangle)
//
SbBool
SoTessellator::clippable(SoTessVertex *v)
{
  SoTessVertex *vtx=v->next->next->next;

  do {
    if (vtx!=v && vtx!=v->next && vtx!=v->next->next &&
	vtx->v!=v->v && vtx->v!=v->next->v && vtx->v!=v->next->next->v)
      if (pointInTriangle(vtx,v))
	return FALSE;
    vtx=vtx->next;
  } while (vtx!=v);
  return TRUE;
}

//
// Call the callback-function for the triangle starting with t
//
void
SoTessellator::emitTriangle(SoTessVertex *t)
{
  SoTessVertex *tmp;
  callback(t->data,t->next->data,t->next->next->data,callbackData);
  tmp=t->next;
  t->next=t->next->next;
}

//
// Throw away triangle
//
void
SoTessellator::cutTriangle(SoTessVertex *t)
{
  SoTessVertex *tmp;
  tmp=t->next;
  t->next=t->next->next;
}

//
// Return the area of the triangle starting with v
//
double
SoTessellator::area(SoTessVertex *v)
{
  return fabs(((v->next->v[X]-v->v[X])*(v->next->next->v[Y]-v->v[Y])-
	       (v->next->v[Y]-v->v[Y])*(v->next->next->v[X]-v->v[X])));
}


SbBool 
SoTessellator::circleCenter(const SbVec3f &a, const SbVec3f &b, 
			    const SbVec3f &c, float &cx, float &cy)
  // Returns the center of the circle through points a, b, c.
  // From Graphics Gems I, p.22
{
#ifdef NO_DELAUNEY
  return FALSE;
#else
  float d1, d2, d3, c1, c2, c3;
  SbVec3f tmp1, tmp2;

  tmp1 = b - a;
  tmp2 = c - a;
  d1 = dot2D(tmp1, tmp2);

  tmp1 = b - c;
  tmp2 = a - c;
  d2 = dot2D(tmp1, tmp2);
  
  tmp1 = a - b;
  tmp2 = c - b;
  d3 = dot2D(tmp1, tmp2);

  c1 = d2 * d3;
  c2 = d3 * d1;
  c3 = d1 * d2;

  SbVec3f tmp4(c);
  tmp1=a;
  tmp2=b;
  tmp1*=(c2+c3);
  tmp2*=(c3+c1);
  tmp4*=(c1+c2);
  tmp4+=tmp1;
  tmp4+=tmp2;

  float div = 2*(c1+c2+c3);
  if (div != 0.0f) {
    float val = 1.0f / div;
    cx = tmp4[this->X] * val;
    cy = tmp4[this->Y] * val;
    return TRUE;
  }
  return FALSE;
#endif // 
}

// Returns the square of the radius of the circle through a, b, c
float 
SoTessellator::circleSize(const SbVec3f &a, 
			  const SbVec3f &b, 
			  const SbVec3f &c)
{
#ifndef NO_DELAUNEY
  float cx, cy;
  if (circleCenter(a, b, c, cx, cy)) {  
    float t1, t2;
    t1 = a[this->X] - cx;
    t2 = a[this->Y] - cy;
    return t1*t1+t2*t2;
  }
#endif // !NO_DELAUNEY
  return FLT_MAX;
}   

float 
SoTessellator::circleSize(SoTessVertex *v)
{
  return circleSize(v->v, v->next->v, v->next->next->v);
}

float 
SoTessellator::dot2D(const SbVec3f &v1, const SbVec3f &v2)
{
  return v1[this->X] * v2[this->X] + v1[this->Y] * v2[this->Y]; 
}

void 
SoTessellator::calcPolygonNormal()
{
  assert(nverts > 3);

  // more than 3 vertices
  // use Newell's method to calculate normal vector
  // first three vertices may be collinear and need not be a convex "ear"
  // see Graphics Gems, Vol. III, V.5, p. 231
  
  Normal.setValue(0.0f, 0.0f, 0.0f);
  SbVec3f vert1, vert2;
  SoTessVertex *currvertex = headV;
  vert2 = currvertex->v;
  
  while (currvertex->next != NULL && currvertex != tailV) {
    vert1 = vert2;
    vert2 = currvertex->next->v;
    Normal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
    Normal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
    Normal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);
    currvertex = currvertex->next;
  }
  vert1 = vert2;  // last edge (back to v0)
  vert2 = headV->v;
  Normal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
  Normal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
  Normal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);      
  
  Normal.normalize();
}

//
// added 990609, pederb. Avoid lots of new/delete by keeping
// allocated vertices in an array.
//
SoTessellator::SoTessVertex *
SoTessellator::newVertex()
{
  assert(this->currVertex <= this->vertexStorage.getLength());
  if (this->currVertex == this->vertexStorage.getLength()) {
    SoTessVertex *v = new SoTessVertex;
    this->vertexStorage.append(v);
  }
  return this->vertexStorage[currVertex++];
}

