/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SbTesselator SbTesselator.h Inventor/SbTesselator.h
  \brief The SbTesselator class is used to tessellate polygons into triangles.
  \ingroup base

  SbTesselator is used within Coin to split polygons into
  triangles. It handles concave polygons, does Delaunay triangulation
  and avoids generating self-intersecting triangles.

  Here's a simple example which shows how to tessellate a quad polygon
  with corners in <0, 0, 0>, <1, 0, 0>, <1, 1, 0> and <0, 1, 0>.

  \code
// Callback function for the tessellator. Called once for each
// generated triangle with the vertices.
static void
tess_cb(void * v0, void * v1, void * v2, void * cbdata)
{
  SbVec3f * vtx0 = (SbVec3f *)v0;
  SbVec3f * vtx1 = (SbVec3f *)v1;
  SbVec3f * vtx2 = (SbVec3f *)v2;
  (void)fprintf(stdout, "triangle: <%f, %f, %f> <%f, %f, %f> <%f, %f, %f>\n",
                (*vtx0)[0], (*vtx0)[1], (*vtx0)[2],
                (*vtx1)[0], (*vtx1)[1], (*vtx1)[2],
                (*vtx2)[0], (*vtx2)[1], (*vtx2)[2]);

  // Do stuff with triangle here.
}

/// Testcode. ////////////////////////////

static SbVec3f vertices[] = {
  SbVec3f(1, 0, 0), SbVec3f(1, 1, 0),
  SbVec3f(0, 1, 0), SbVec3f(0, 0, 0)
};

SbTesselator mytessellator(tess_cb, NULL);
mytessellator.beginPolygon();
for (int i=0; i < 4; i++)
  mytessellator.addVertex(vertices[i], &vertices[i]);
mytessellator.endPolygon();
  \endcode

  The call to SbTesselator::endPolygon() triggers the SbTesselator to
  spring into action, calling the tess_cb() function for each triangle
  it generates.

  The reason we use 2 arguments to SbTesselator::addVertex() and passes
  void pointers for the vertices to the callback function is to make it
  possible to have more complex structures than just the coordinates
  themselves (as in the example above), like material information,
  lighting information or whatever other attributes your vertices have.

  This class is not part of the original Open Inventor API.

  (Another option for tessellating polygons is the tessellator of the
  GLU library. It has some features not part of SbTesselator (like
  handling hulls), but the GLU library is known to have bugs in various
  implementations and doesn't do Delaunay triangulation.)
*/


#include <Inventor/SbTesselator.h>
#include <Inventor/SbHeap.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <float.h>

struct SbTVertex {
  float weight;
  int dirtyweight;
  int heapidx;
  SbTVertex *prev;
  SbTesselator *thisp;
  SbVec3f v;
  void *data;
  SbTVertex *next;
};


float
SbTesselator::heap_evaluate(void *v)
{
  SbTVertex *vertex = (SbTVertex*)v;
  if (vertex->dirtyweight) {
    vertex->dirtyweight = 0;
    if (vertex->thisp->area(vertex) > FLT_EPSILON &&
        vertex->thisp->isTriangle(vertex) &&
        vertex->thisp->clippable(vertex)) {
#if 0 // testing code to avoid empty triangles
      vertex->weight = vertex->thisp->circleSize(vertex);
      SbTVertex *v2 = vertex->next;
      if (vertex->weight != FLT_MAX &&
          v2->thisp->keepVertices &&
          v2->thisp->numVerts > 4 &&
          fabs(v2->thisp->area(v2)) < FLT_EPSILON) {
        vertex->weight = 0.0f; // cut immediately!
      }

#else
      vertex->weight = vertex->thisp->circleSize(vertex);
#endif
    }
    else
      vertex->weight = FLT_MAX;
  }
  return vertex->weight;
}

static int
heap_get_index(void *v)
{
  return ((SbTVertex*)v)->heapidx;
}

static void
heap_set_index(void *v, int idx)
{
  ((SbTVertex*)v)->heapidx = idx;
}

//projection enums
enum {OXY,OXZ,OYZ};

/*!
  Initializes a tessellator. The \a callback argument specifies a
  function which will be called for each triangle returned by the
  tessellator. The callback function will get three pointers to each
  vertex and the \a userdata pointer. The vertex pointers are
  specified in the SbTesselator::addVertex() method.
*/
SbTesselator::SbTesselator(void (*callback)(void * v0, void * v1, void * v2,
                                            void * data),
                           void * userdata)
{
  this->setCallback(callback, userdata);
  this->headV = this->tailV = NULL;
  this->currVertex = 0;

  SbHeapFuncs hf;
  hf.eval_func = heap_evaluate;
  hf.get_index_func = heap_get_index;
  hf.set_index_func = heap_set_index;

  this->heap = new SbHeap(hf, 64);
}

/*!
  Destructor.
*/
SbTesselator::~SbTesselator()
{
  cleanUp();
  int i,n = this->vertexStorage.getLength();
  for (i = 0; i < n; i++) delete this->vertexStorage[i];
  delete this->heap;
}

/*!
  Initializes new polygon.

  You can explicitly set the polygon normal if you know what it
  is. Otherwise it will be calculated internally.

  If \a keepVerts is \c TRUE, all vertices will be included in the
  returned triangles, even though this might lead to triangles without
  area.
*/
void
SbTesselator::beginPolygon(SbBool keepVerts, const SbVec3f &normal)
{
  this->cleanUp();
  this->keepVertices = keepVerts;
  if (normal != SbVec3f(0.0f, 0.0f, 0.0f)) {
    this->polyNormal = normal;
    this->hasNormal = TRUE;
  }
  else {
    this->hasNormal = FALSE;
  }
  this->headV = this->tailV = NULL;
  this->numVerts=0;
}

/*!
  Adds a new vertex to the polygon. \a data will be returned as a
  vertex in the callback-function.
*/
void
SbTesselator::addVertex(const SbVec3f &v,void *data)
{
  SbTVertex *newv = this->newVertex();
  newv->v = v;
  newv->data = data;
  newv->next = NULL;
  newv->dirtyweight = 1;
  newv->weight = FLT_MAX;
  newv->heapidx = -1;
  newv->prev = this->tailV;
  newv->thisp = this;
  if (!this->headV) this->headV = newv;
  if (this->tailV) this->tailV->next = newv;
  this->tailV = newv;
  this->numVerts++;
}

/*!
  Signals the tessellator to begin tessellating. The callback function
  specified in the constructor (or set using the
  SbTesselator::setCallback() method) will be called for each triangle
  before returning.
*/
void
SbTesselator::endPolygon()
{
  SbTVertex *v;

  if (this->numVerts > 3) {
    calcPolygonNormal();

    // Find best projection plane
    int projection;
    if (fabs(polyNormal[0]) > fabs(polyNormal[1]))
      if (fabs(polyNormal[0]) > fabs(polyNormal[2]))
        projection=OYZ;
      else projection=OXY;
    else
      if (fabs(polyNormal[1]) > fabs(polyNormal[2]))
        projection=OXZ;
      else projection=OXY;

    switch (projection) {
    case OYZ:
      this->X=1;
      this->Y=2;
      polyDir=(int)(polyNormal[0]/fabs(polyNormal[0]));
      break;
    case OXY:
      this->X=0;
      this->Y=1;
      polyDir=(int)(polyNormal[2]/fabs(polyNormal[2]));
      break;
    case OXZ:
      this->X=2;
      this->Y=0;
      polyDir=(int)(polyNormal[1]/fabs(polyNormal[1]));
      break;
    }
    //Make loop
    this->tailV->next = this->headV;
    this->headV->prev = this->tailV;

    // add all vertices to heap.
    heap->emptyHeap();
    this->headV->heapidx = this->heap->add(this->headV);
    v = this->headV->next;
    while (v != this->headV) {
      v->heapidx = this->heap->add(v);
      v = v->next;
    }
    this->heap->buildHeap();

    while (this->numVerts > 4) {

      v = (SbTVertex*) this->heap->getMin();
      if (heap_evaluate(v) == FLT_MAX) break;
      this->heap->remove(v->next->heapidx);

      this->emitTriangle(v); // will remove v->next
      this->numVerts--;

      // update heap
      v->prev->dirtyweight = 1;
      v->dirtyweight = 1;
      this->heap->newWeight(v->prev, v->prev->heapidx);
      this->heap->newWeight(v, v->heapidx);
    }

    // remember that headV and tailV are not valid anymore!

    //
    // must handle special case when only four vertices remain
    //
    if (this->numVerts == 4) {
      float v0 = SbMax(heap_evaluate(v), heap_evaluate(v->next->next));
      float v1 = SbMax(heap_evaluate(v->next), heap_evaluate(v->prev));

      // abort if vertices should not be kept
      if (v0 == v1 && v0 == FLT_MAX && !this->keepVertices) return;

      if (v0 < v1) {
        this->emitTriangle(v);
        this->emitTriangle(v);
      }
      else {
        v = v->next;
        this->emitTriangle(v);
        this->emitTriangle(v);
      }
      this->numVerts -= 2;
    }

    // Emit the empty triangles that might lay around
    if (this->keepVertices) {
      while (numVerts>=3) {
        this->emitTriangle(v);
        this->numVerts--;
      }
    }
  }
  else if (this->numVerts==3) {   //only one triangle
    this->emitTriangle(headV);
  }
}

/*!
  Sets the callback function for this tessellator.
*/
void
SbTesselator::setCallback(void (*callback)(void *v0,
                                           void *v1,
                                           void *v2,
                                           void *data),
                          void *data)
{
  this->callback = callback;
  this->callbackData = data;
}


static SbBool
point_on_edge(const float x, const float y,
              const float * const v0, const float * const v1,
              const int X, const int Y)
{
  if (x < v0[X] && x < v1[X]) return FALSE;
  if (x > v0[X] && x > v1[X]) return FALSE;
  if (y < v0[Y] && y < v1[Y]) return FALSE;
  if (y > v0[Y] && y > v1[Y]) return FALSE;

  if (v0[X] == v1[X]) {
    if (fabs(x-v0[X]) <= FLT_EPSILON) return TRUE;
    return FALSE;
  }

  float ny = v0[Y] + (x-v0[X])*(v1[Y]-v0[Y])/(v1[X]-v0[X]);

  if (fabs(y-ny)<= FLT_EPSILON) {
    return TRUE;
  }
  return FALSE;
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
SbTesselator::pointInTriangle(SbTVertex *p, SbTVertex *t)
{
  float x,y;
  SbBool tst = FALSE;

  x = p->v[X];
  y = p->v[Y];

  const float *v1 = t->v.getValue();
  const float *v2 = t->next->next->v.getValue();

  if ((((v1[Y]<=y) && (y<v2[Y])) || ((v2[Y]<=y)  && (y<v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) /  (v2[Y] - v1[Y]) + v1[X]))
    tst = (tst==FALSE ? TRUE : FALSE);
  v2 = v1;
  v1 = t->next->v.getValue();
  if ((((v1[Y]<=y) && (y<v2[Y])) || ((v2[Y]<=y)  && (y<v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) /  (v2[Y] - v1[Y]) + v1[X]))
    tst = (tst==FALSE ? TRUE : FALSE);
  v2 = v1;
  v1 = t->next->next->v.getValue();
  if ((((v1[Y]<=y) && (y<v2[Y])) || ((v2[Y]<=y)  && (y<v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) /  (v2[Y] - v1[Y]) + v1[X]))
    tst = (tst==FALSE ? TRUE : FALSE);

#if 1
  if (this->keepVertices && tst == FALSE) {
    if (point_on_edge(x,y,t->v.getValue(),
                      t->next->v.getValue(),X,Y))
      return TRUE;
    if (point_on_edge(x,y,t->next->v.getValue(),
                      t->next->next->v.getValue(),X,Y))
      return TRUE;
    if (point_on_edge(x,y,t->next->next->v.getValue(),
                      t->v.getValue(),X,Y))
      return TRUE;
  }
#endif

  return tst;
}


//
// Check if v points to a legal triangle (normal is right way)
// (i.e convex or concave corner)
//
SbBool
SbTesselator::isTriangle(SbTVertex *v)
{
  if (((v->next->v[X]-v->v[X])*(v->next->next->v[Y]-v->v[Y])-
       (v->next->v[Y]-v->v[Y])*(v->next->next->v[X]-v->v[X]))*this->polyDir>0.0)
    return TRUE;
  else return FALSE;
}

//
// Check if there are no intersection to the triangle
// pointed to by v. (no other vertices are inside the triangle)
//
SbBool
SbTesselator::clippable(SbTVertex *v)
{
  SbTVertex *vtx=v->next->next->next;

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
SbTesselator::emitTriangle(SbTVertex *t)
{
  SbTVertex *tmp;

  assert(t);
  assert(t->next);
  assert(t->next->next);
  assert(this->callback);

  this->callback(t->data, t->next->data, t->next->next->data,
                 this->callbackData);
  tmp = t->next;
  t->next = t->next->next;
  tmp->next->prev = t;
}

//
// Throw away triangle
//
void
SbTesselator::cutTriangle(SbTVertex *t)
{
  SbTVertex *tmp;
  tmp=t->next;
  t->next=t->next->next;
  tmp->next->prev = t;
}

//
// Return the area of the triangle starting with v
//
float
SbTesselator::area(SbTVertex *v)
{
  return (float)fabs(((v->next->v[X]-v->v[X])*(v->next->next->v[Y]-v->v[Y])-
                     (v->next->v[Y]-v->v[Y])*(v->next->next->v[X]-v->v[X])));
}

//
// Returns the center of the circle through points a, b, c.
//
SbBool
SbTesselator::circleCenter(const SbVec3f &a, const SbVec3f &b,
                           const SbVec3f &c, float &cx, float &cy)
{
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
  tmp1 = a;
  tmp2 = b;
  tmp1 *= (c2+c3);
  tmp2 *= (c3+c1);
  tmp4 *= (c1+c2);
  tmp4 += tmp1;
  tmp4 += tmp2;

  float div = 2*(c1+c2+c3);
  if (fabs(div) > FLT_EPSILON) {
    float val = 1.0f / div;
    cx = tmp4[this->X] * val;
    cy = tmp4[this->Y] * val;
    return TRUE;
  }
  return FALSE;
}

//
// Returns the square of the radius of the circle through a, b, c
//
float
SbTesselator::circleSize(const SbVec3f &a, const SbVec3f &b, const SbVec3f &c)
{
  float cx, cy;
  if (circleCenter(a, b, c, cx, cy)) {
    float t1, t2;
    t1 = a[this->X] - cx;
    t2 = a[this->Y] - cy;
    return t1*t1+t2*t2;
  }
  return FLT_MAX;
}

float
SbTesselator::circleSize(SbTVertex *v)
{
  return circleSize(v->v, v->next->v, v->next->next->v);
}

float
SbTesselator::dot2D(const SbVec3f &v1, const SbVec3f &v2)
{
  return v1[this->X] * v2[this->X] + v1[this->Y] * v2[this->Y];
}

void
SbTesselator::calcPolygonNormal()
{
  assert(this->numVerts > 3);

  this->polyNormal.setValue(0.0f, 0.0f, 0.0f);
  SbVec3f vert1, vert2;
  SbTVertex *currvertex = this->headV;
  vert2 = currvertex->v;

  while (currvertex->next != NULL && currvertex != tailV) {
    vert1 = vert2;
    vert2 = currvertex->next->v;
    this->polyNormal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
    this->polyNormal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
    this->polyNormal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);
    currvertex = currvertex->next;
  }
  vert1 = vert2;
  vert2 = headV->v;
  polyNormal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
  polyNormal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
  polyNormal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);

  polyNormal.normalize();
}

//
// makes sure SbTVertexes are not allocated and deallocated
// all the time, by storing them in a growable array. This
// way, the SbTVertexes will not be deleted until the tessellator
// is destructed, and SbTVertexes can be reused.
//
struct SbTVertex *
SbTesselator::newVertex()
{
  assert(this->currVertex <= this->vertexStorage.getLength());
  if (this->currVertex == this->vertexStorage.getLength()) {
    struct SbTVertex *v = new SbTVertex;
    this->vertexStorage.append(v);
  }
  return this->vertexStorage[currVertex++];
}

void
SbTesselator::cleanUp()
{
  this->headV = this->tailV = NULL;
  this->currVertex = 0;
}
