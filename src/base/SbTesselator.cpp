/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    (void) fprintf(stdout, "triangle: <%f, %f, %f> <%f, %f, %f> <%f, %f, %f>\n",
      (*vtx0)[0], (*vtx0)[1], (*vtx0)[2],
      (*vtx1)[0], (*vtx1)[1], (*vtx1)[2],
      (*vtx2)[0], (*vtx2)[1], (*vtx2)[2]);

    // Do stuff with triangle here.
  }

  static SbVec3f vertices[] = {
    SbVec3f(1, 0, 0), SbVec3f(1, 1, 0),
    SbVec3f(0, 1, 0), SbVec3f(0, 0, 0)
  };

  SbTesselator mytessellator(tess_cb, NULL);
  mytessellator.beginPolygon();
  for (int i=0; i < 4; i++) {
    mytessellator.addVertex(vertices[i], &vertices[i]);
  }
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


  Another option for tessellating polygons is the tessellator of the
  GLU library. It has some features not part of SbTesselator (like
  handling hulls), but the GLU library is known to have bugs in
  various implementations and doesn't do Delaunay triangulation. If
  you however still prefer to use the GLU tessellator instead of this
  one, that can be forced by setting an environment variable:

  \code
  (void) coin_setenv("COIN_PREFER_GLU_TESSELLATOR", "1", 1);
  \endcode
*/

// *************************************************************************

/*! \file SbTesselator.h */
#include <Inventor/SbTesselator.h>

#include <cstdio>
#include <climits>
#include <cassert>
#include <cfloat>

#include <Inventor/C/base/heap.h>
#include <Inventor/SbBSPTree.h>
#include <Inventor/SbSphere.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

/*!
  \typedef void SbTesselatorCB(void * v0, void * v1, void * v2, void * data)

  The type definition of the callback function which is called for each triangle
  returned by the tesselator.
*/

// *************************************************************************

class SbTesselator::PImpl {
public:

#if 1 // enable this to do double-precision internal calculations
  typedef double tessfloat_t;
#else // or else do single-precision calculations.
  // FIXME: we should normally want do to double-precision, but this
  // is kept around for now since there are suspected bugs in the
  // tessellation code which are easier to reproduce with
  // single-precision calculations.  -mortene.
  typedef float tessfloat_t;
#endif

  // this type is for internal real values which is forced by external
  // factors to be single-precision floats. (e.g. coming into the api
  // as an SbVec3f.)
  typedef float forcedfloat_t;

  struct Vertex {
    SbTesselator::PImpl * thisp;

    SbVec3f v;
    void * data;

    tessfloat_t weight;
    int dirtyweight;

    Vertex * prev, * next;
  };

  PImpl(void) : bsptree(256) { }
  cc_heap * heap;
  SbBSPTree bsptree;
  SbList <int> clippablelist;
  tessfloat_t epsilon;
  SbBox3f bbox;

  Vertex * newVertex(void);
  void cleanUp(void);

  int currVertex;
  SbList <struct Vertex *> vertexStorage;

  Vertex * headV, * tailV;
  int numVerts;
  SbVec3f polyNormal;
  int X, Y;
  int polyDir;
  void (*callback)(void * v0, void * v1, void * v2, void * data);
  void * callbackData;
  SbBool hasNormal;
  SbBool keepVertices;

  void emitTriangle(Vertex * v);
  void cutTriangle(Vertex * t);
  void calcPolygonNormal(void);

  tessfloat_t circleSize(Vertex * v);
  SbBool clippable(Vertex * v);
  SbBool isTriangle(Vertex * v);
  SbBool pointInTriangle(Vertex * p, Vertex * t);
  tessfloat_t signedArea(Vertex * t);

  static tessfloat_t heap_evaluate(void * v);
  static int heap_compare(void * v0, void * v1);

  tessfloat_t squaredCircleRadius(const forcedfloat_t * a, const forcedfloat_t * b, const forcedfloat_t * c);
  SbBool pointInTriangle(const forcedfloat_t * p,
                         const forcedfloat_t * a, const forcedfloat_t * b, const forcedfloat_t * c);
  SbBool pointOnEdge(tessfloat_t x, tessfloat_t y,
                     const forcedfloat_t * v0, const forcedfloat_t * v1);
};

#define PRIVATE(obj) ((obj)->pimpl)

// *************************************************************************

int
SbTesselator::PImpl::heap_compare(void * h0, void * h1)
{
  if (heap_evaluate(h0) > heap_evaluate(h1)) return -1;
  return 1;
}

SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::heap_evaluate(void * v)
{
  Vertex * vertex = static_cast<Vertex *>(v);
  if (vertex->dirtyweight) {
    vertex->dirtyweight = 0;
    if ((fabs(vertex->thisp->signedArea(vertex)) > vertex->thisp->epsilon) &&
        vertex->thisp->isTriangle(vertex) &&
        vertex->thisp->clippable(vertex)) {
#if 0 // testing code to avoid empty triangles
      vertex->weight = vertex->thisp->circleSize(vertex);
      Vertex *v2 = vertex->next;
      if (vertex->weight != FLT_MAX &&
          v2->thisp->keepVertices &&
          v2->thisp->numVerts > 4 &&
          fabs(v2->thisp->signed_area(v2)) < v2->thisp->epsilon) {
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

// *************************************************************************

/*!
  Initializes a tessellator. The \a callback argument specifies a
  function which will be called for each triangle returned by the
  tessellator. The callback function will get three pointers to each
  vertex and the \a userdata pointer. The vertex pointers are
  specified in the SbTesselator::addVertex() method.
*/
SbTesselator::SbTesselator(SbTesselatorCB * func, void * data)
{
  this->setCallback(func, data);
  PRIVATE(this)->headV = PRIVATE(this)->tailV = NULL;
  PRIVATE(this)->currVertex = 0;

  PRIVATE(this)->heap =
    cc_heap_construct(256, reinterpret_cast<cc_heap_compare_cb *>(PImpl::heap_compare), TRUE);
  PRIVATE(this)->epsilon = FLT_EPSILON;
}

/*!
  Destructor.
*/
SbTesselator::~SbTesselator()
{
  PRIVATE(this)->cleanUp();
  int i, n = PRIVATE(this)->vertexStorage.getLength();
  for (i = 0; i < n; i++) { delete PRIVATE(this)->vertexStorage[i]; }

  cc_heap_destruct(PRIVATE(this)->heap);
}

// *************************************************************************

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
  PRIVATE(this)->cleanUp();
  PRIVATE(this)->keepVertices = keepVerts;
  if (normal != SbVec3f(0.0f, 0.0f, 0.0f)) {
    PRIVATE(this)->polyNormal = normal;
    PRIVATE(this)->hasNormal = TRUE;
  }
  else {
    PRIVATE(this)->hasNormal = FALSE;
  }
  PRIVATE(this)->headV = PRIVATE(this)->tailV = NULL;
  PRIVATE(this)->numVerts = 0;
  PRIVATE(this)->bbox.makeEmpty();
}

/*!
  Adds a new vertex to the polygon. \a data will be returned as a
  vertex in the callback-function.
*/
void
SbTesselator::addVertex(const SbVec3f &v,void *data)
{
  if (PRIVATE(this)->tailV &&
      !PRIVATE(this)->keepVertices &&
      v == PRIVATE(this)->tailV->v)
    return;

  PRIVATE(this)->bbox.extendBy(v);

  PImpl::Vertex *newv = PRIVATE(this)->newVertex();
  newv->v = v;
  newv->data = data;
  newv->next = NULL;
  newv->dirtyweight = 1;
  newv->weight = FLT_MAX;
  newv->prev = PRIVATE(this)->tailV;
  newv->thisp = &(PRIVATE(this).get());
  if (!PRIVATE(this)->headV) PRIVATE(this)->headV = newv;
  if (PRIVATE(this)->tailV) PRIVATE(this)->tailV->next = newv;
  PRIVATE(this)->tailV = newv;
  PRIVATE(this)->numVerts++;
}

/*!
  Signals the tessellator to begin tessellating. The callback function
  specified in the constructor (or set using the
  SbTesselator::setCallback() method) will be called for each triangle
  before returning.
*/
void
SbTesselator::endPolygon(void)
{
  // projection enums
  enum { OXY, OXZ, OYZ };

  // check for special case when last point equals the first point
  if (!PRIVATE(this)->keepVertices && PRIVATE(this)->numVerts >= 3) {
    PImpl::Vertex * first = PRIVATE(this)->headV;
    PImpl::Vertex * last = PRIVATE(this)->tailV;
    if (first->v == last->v) {
      PImpl::Vertex * newlast = last->prev;
      newlast->next = NULL;
      // don't delete old tail. We have some special memory handling
      // in this class
      PRIVATE(this)->tailV = newlast;
      PRIVATE(this)->numVerts--;
    }
  }

  PImpl::Vertex *v;

  if (PRIVATE(this)->numVerts > 3) {
    PRIVATE(this)->calcPolygonNormal();

    // Find best projection plane
    int projection;
    if (fabs(PRIVATE(this)->polyNormal[0]) > fabs(PRIVATE(this)->polyNormal[1]))
      if (fabs(PRIVATE(this)->polyNormal[0]) > fabs(PRIVATE(this)->polyNormal[2]))
        projection=OYZ;
      else projection=OXY;
    else
      if (fabs(PRIVATE(this)->polyNormal[1]) > fabs(PRIVATE(this)->polyNormal[2]))
        projection=OXZ;
      else projection=OXY;

    switch (projection) {
    case OYZ:
      PRIVATE(this)->X=1;
      PRIVATE(this)->Y=2;
      PRIVATE(this)->polyDir=static_cast<int>(PRIVATE(this)->polyNormal[0]/fabs(PRIVATE(this)->polyNormal[0]));
      break;
    case OXY:
      PRIVATE(this)->X=0;
      PRIVATE(this)->Y=1;
      PRIVATE(this)->polyDir=static_cast<int>(PRIVATE(this)->polyNormal[2]/fabs(PRIVATE(this)->polyNormal[2]));
      break;
    case OXZ:
      PRIVATE(this)->X=2;
      PRIVATE(this)->Y=0;
      PRIVATE(this)->polyDir=static_cast<int>(PRIVATE(this)->polyNormal[1]/fabs(PRIVATE(this)->polyNormal[1]));
      break;
    }

    // find epsilon based on bbox
    SbVec3f d;
    PRIVATE(this)->bbox.getSize(d[0],d[1],d[2]);
    PRIVATE(this)->epsilon = SbMin(d[PRIVATE(this)->X], d[PRIVATE(this)->Y]) * FLT_EPSILON * FLT_EPSILON;

    //Make loop
    PRIVATE(this)->tailV->next = PRIVATE(this)->headV;
    PRIVATE(this)->headV->prev = PRIVATE(this)->tailV;

    // add all vertices to heap.
    cc_heap_clear(PRIVATE(this)->heap);
    PRIVATE(this)->bsptree.clear(256);

    // use two loops to add points to bsptree and heap, since the heap
    // requires that the bsptree is fully set up to evaluate
    // correctly.
    v = PRIVATE(this)->headV;
    do {
      PRIVATE(this)->bsptree.addPoint(SbVec3f(v->v[PRIVATE(this)->X],
                                              v->v[PRIVATE(this)->Y],
                                              0.0f), v);
      v = v->next;
    } while (v != PRIVATE(this)->headV);

    do {
      cc_heap_add(PRIVATE(this)->heap, v);
      v = v->next;
    } while (v != PRIVATE(this)->headV);

    while (PRIVATE(this)->numVerts > 4) {
      v = static_cast<PImpl::Vertex *>(cc_heap_get_top(PRIVATE(this)->heap));
      if (PImpl::heap_evaluate(v) == FLT_MAX)
        break;
      cc_heap_remove(PRIVATE(this)->heap, v->next);
      PRIVATE(this)->bsptree.removePoint(SbVec3f(v->next->v[PRIVATE(this)->X],
                                                 v->next->v[PRIVATE(this)->Y],
                                                 0.0f));
      PRIVATE(this)->emitTriangle(v); // will remove v->next
      PRIVATE(this)->numVerts--;

      v->prev->dirtyweight = 1;
      v->dirtyweight = 1;

      (void) cc_heap_remove(PRIVATE(this)->heap, v->prev);
      (void) cc_heap_remove(PRIVATE(this)->heap, v);

      cc_heap_add(PRIVATE(this)->heap, v->prev);
      cc_heap_add(PRIVATE(this)->heap, v);
    }

    // remember that headV and tailV are not valid anymore!

    //
    // must handle special case when only four vertices remain
    //
    if (PRIVATE(this)->numVerts == 4) {
      v->next->dirtyweight = 1;
      v->next->next->dirtyweight = 1;
      SbTesselator::PImpl::tessfloat_t v0 = SbMax(PImpl::heap_evaluate(v), PImpl::heap_evaluate(v->next->next));
      SbTesselator::PImpl::tessfloat_t v1 = SbMax(PImpl::heap_evaluate(v->next), PImpl::heap_evaluate(v->prev));

      // abort if vertices should not be kept
      if (v0 == v1 && v0 == FLT_MAX && !PRIVATE(this)->keepVertices) return;

      if (v0 < v1) {
        PRIVATE(this)->emitTriangle(v);
        PRIVATE(this)->emitTriangle(v);
      }
      else {
        v = v->next;
        PRIVATE(this)->emitTriangle(v);
        PRIVATE(this)->emitTriangle(v);
      }
      PRIVATE(this)->numVerts -= 2;
    }

    // Emit the empty triangles that might lay around
    if (PRIVATE(this)->keepVertices) {
      while (PRIVATE(this)->numVerts>=3) {
        PRIVATE(this)->emitTriangle(v);
        PRIVATE(this)->numVerts--;
      }
    }
  }
  else if (PRIVATE(this)->numVerts==3) {   //only one triangle
    PRIVATE(this)->emitTriangle(PRIVATE(this)->headV);
  }
}

// *************************************************************************

/*!
  Sets the callback function for this tessellator.
*/
void
SbTesselator::setCallback(SbTesselatorCB * func, void *data)
{
  PRIVATE(this)->callback = func;
  PRIVATE(this)->callbackData = data;
}

// *************************************************************************

//
// PRIVATE FUNCTIONS:
//

// Checks the distance of point p(x,y) to edge v0->v1.
// The point is on the edge if its distance is less than epsilon.
// Algorithm from comp.graphics.algorithms FAQ
SbBool
SbTesselator::PImpl::pointOnEdge(tessfloat_t x, tessfloat_t y,
                                 const forcedfloat_t *v0,
                                 const forcedfloat_t *v1)
{
  tessfloat_t C = v1[X] - v0[X];
  tessfloat_t D = v1[Y] - v0[Y];

  tessfloat_t dot = (x - v0[X]) * C + (y - v0[Y]) * D;
  tessfloat_t norm = C * C + D * D;
  tessfloat_t t = -1;
  if (norm != 0.0)
    t = dot / norm;

  if (t < 0.0)
    t = 0.0;
  if (t > 1.0)
    t = 1.0;

  tessfloat_t px = v0[X] + t * C;
  tessfloat_t py = v0[Y] + t * D;
  tessfloat_t dx = x - px;
  tessfloat_t dy = y - py;

  return fabs(dx * dx + dy * dy) < epsilon*epsilon;
}

// see: https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
// p is the testpoint, and the other points are corners in the triangle
SbBool SbTesselator::PImpl::pointInTriangle(const forcedfloat_t * p, const forcedfloat_t * a, const forcedfloat_t * b, const forcedfloat_t * c)
{
  tessfloat_t x = p[X] - a[X];
  tessfloat_t y = p[Y] - a[Y];
  bool s_ab = (b[X] - a[X]) * y - (b[Y] - a[Y]) * x > 0;
  if ((c[X] - a[X]) * y - (c[Y] - a[Y]) * x > 0 == s_ab)
    return false;
  if ((c[X] - b[X]) * (p[Y] - b[Y]) - (c[Y] - b[Y]) * (p[X] - b[X]) > 0 != s_ab)
    return false;
  return true;
}

SbBool SbTesselator::PImpl::pointInTriangle(Vertex* pt, Vertex* t)
{
  const forcedfloat_t * p = pt->v.getValue();
  const forcedfloat_t * a = t->v.getValue();
  const forcedfloat_t * b = t->next->v.getValue();
  const forcedfloat_t * c = t->next->next->v.getValue();

  if (pointInTriangle(p, a, b, c))
    return true;

  // The pointInTriangle test might fail for vertices that are on one
  // of the triangle edges. Do a pointOnEdge test for all three
  // edges to handle this case.
  if (pointOnEdge(p[X], p[Y], a, b))
    return true;
  if (pointOnEdge(p[X], p[Y], b, c))
    return true;
  if (pointOnEdge(p[X], p[Y], c, a))
    return true;

  return false;
}

//
// Check if v points to a legal triangle (normal is right way)
// (i.e convex or concave corner)
//
SbBool
SbTesselator::PImpl::isTriangle(Vertex *v)
{
  return (signedArea(v) * polyDir > 0.0) ? TRUE : FALSE;
}

//
// Check if there are no intersection to the triangle
// pointed to by t. (no other vertices are inside the triangle)
//
SbBool
SbTesselator::PImpl::clippable(Vertex *t)
{
  SbBox3f boundingbox;
  boundingbox.makeEmpty();
  boundingbox.extendBy(SbVec3f(t->v[X], t->v[Y], 0.0f));
  boundingbox.extendBy(SbVec3f(t->next->v[X], t->next->v[Y], 0.0f));
  boundingbox.extendBy(SbVec3f(t->next->next->v[X], t->next->next->v[Y], 0.0f));

  SbSphere sphere;
  sphere.circumscribe(boundingbox);

  SbList <int> & l = clippablelist;
  l.truncate(0);
  bsptree.findPoints(sphere, l);
  for (int i = 0; i < l.getLength(); i++) {
    Vertex * vtx = static_cast<Vertex*>(bsptree.getUserData(l[i]));
    if (vtx != t && vtx != t->next && vtx != t->next->next) {
      if (pointInTriangle(vtx, t))
        return FALSE;
    }
  }
  return TRUE;
}

//
// Call the callback-function for the triangle starting with t
//
void
SbTesselator::PImpl::emitTriangle(Vertex *t)
{
  assert(t);
  assert(t->next);
  assert(t->next->next);
  assert(callback);

  callback(t->data, t->next->data, t->next->next->data, callbackData);
  cutTriangle(t);
}

//
// Cuts t->next out of the triangle vertex list.
//
// FIXME: bad design, this should have been a method on
// Vertex. 20031007 mortene.
void
SbTesselator::PImpl::cutTriangle(Vertex *t)
{
  t->next->next->prev = t;
  t->next = t->next->next;
}

//
// Returns the signed area of the triangle starting with t
//
SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::signedArea(Vertex *t)
{
  return 0.5*((t->next->v[X] - t->v[X]) * (t->next->next->v[Y] - t->v[Y]) -
              (t->next->v[Y] - t->v[Y]) * (t->next->next->v[X] - t->v[X]));
}

//
// Returns the square of the radius of the circle through points a, b, c in projection plane.
// Algorithm for calulating the center of the circle from comp.graphics.algorithms FAQ
//
SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::squaredCircleRadius(const forcedfloat_t * a, const forcedfloat_t * b, const forcedfloat_t * c)
{
  tessfloat_t A = b[X] - a[X];
  tessfloat_t B = b[Y] - a[Y];
  tessfloat_t C = c[X] - a[X];
  tessfloat_t D = c[Y] - a[Y];

  tessfloat_t E = A * (a[X] + b[X]) + B * (a[Y] + b[Y]);
  tessfloat_t F = C * (a[X] + c[X]) + D * (a[Y] + c[Y]);

  tessfloat_t G = 2.0 * (A * (c[Y] - b[Y]) - B * (c[X] - b[X]));

  if (G != 0.0) {
    tessfloat_t val = 1.0 / G;
    tessfloat_t cx = (D * E - B * F) * val;
    tessfloat_t cy = (A * F - C * E) * val;
    tessfloat_t rx = a[X] - cx;
    tessfloat_t ry = a[Y] - cy;
    return (rx * rx + ry * ry);
  }
  return FLT_MAX;
}

SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::circleSize(Vertex *t)
{
  return squaredCircleRadius(t->v.getValue(), t->next->v.getValue(), t->next->next->v.getValue());
}

void
SbTesselator::PImpl::calcPolygonNormal()
{
  assert(numVerts > 3);

  polyNormal.setValue(0.0f, 0.0f, 0.0f);
  SbVec3f vert1, vert2;
  Vertex *currvertex = headV;
  vert2 = currvertex->v;

  while (currvertex->next != NULL && currvertex != tailV) {
    vert1 = vert2;
    vert2 = currvertex->next->v;
    polyNormal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
    polyNormal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
    polyNormal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);
    currvertex = currvertex->next;
  }
  vert1 = vert2;
  vert2 = headV->v;
  polyNormal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
  polyNormal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
  polyNormal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);

  if (polyNormal.normalize() == 0.0f) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbTesselator::calcPolygonNormal",
                              "Polygon has no normal.");
#endif // COIN_DEBUG
  }
}

//
// makes sure Vertexes are not allocated and deallocated
// all the time, by storing them in a growable array. This
// way, the Vertexes will not be deleted until the tessellator
// is destructed, and Vertexes can be reused.
//
SbTesselator::PImpl::Vertex *
SbTesselator::PImpl::newVertex()
{
  assert(currVertex <= vertexStorage.getLength());
  if (currVertex == vertexStorage.getLength()) {
    Vertex * v = new Vertex;
    vertexStorage.append(v);
  }
  return vertexStorage[currVertex++];
}

void
SbTesselator::PImpl::cleanUp()
{
  headV = tailV = NULL;
  currVertex = 0;
  numVerts = 0;
}

// *************************************************************************

#undef PRIVATE
