/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/SbClip.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbPlane.h>
#include <stdio.h>
#include <assert.h>

/*!
  \class SbClip SbClip.h include/Inventor/base/SbClip.h
  \brief The SbClip class is a generic polygon clipper class.
  \ingroup base

  It is used by first adding all vertices in the polygon, and then
  clipping against any number of planes. If you need to supply
  additional information per vertex (e.g. texture coordinates), you
  should supply a callback in the constructor, and a pointer to your
  vertex structure in addVertex(). For every new vertex created, the
  callback is called with the line being clipped, including the
  pointers to your vertex structures and the position of the new
  (clipped against some plane) vertex. You should then create a new
  vertex structure, calculate your data (e.g. a new texture
  coordinate) and return a pointer to this structure.  

  Be aware that this class is an extension for Coin, and it is not
  available in the original SGI Open Inventor v2.1 API.

  \since 2002-01-20
*/
// FIXME: a small usage example would do wonders for the class
// doc. 20020120 mortene.

/*!
  A constructor. Supply a callback if you need to handle additional
  data per vertex.
*/
SbClip::SbClip(SbClipCallback * callback, void * userdata)
  : callback(callback),
    cbdata(userdata),
    curr(0)
{
}

/*!
  Adds a polygon vertex. \a vdata could be a pointer to your
  vertex structure.
*/
void
SbClip::addVertex(const SbVec3f &v, void * vdata)
{
  this->array[this->curr].append(SbClipData(v, vdata));
}

/*!
  Resets the clipper. This should be called before adding any vertices.
*/
void
SbClip::reset(void)
{
  this->array[0].truncate(0);
  this->array[1].truncate(0);
}

//
// private method, inline only inside this class
//
inline void
SbClip::outputVertex(const SbVec3f & v, void * data)
{
  this->array[this->curr ^ 1].append(SbClipData(v, data));
}

/*!
  Clip polygon against \a plane. This might change the number of
  vertices in the polygon. For each time a new vertex is created, the
  callback supplied in the constructor (if != NULL) is called with the
  line being clipped and the new vertex calculated. The callback
  should return a new void pointer to be stored by the clipper.
*/
void
SbClip::clip(const SbPlane & plane)
{
  int n = this->array[this->curr].getLength();

  if (n == 0) return;

  // create loop
  SbClipData dummy = this->array[this->curr][0];
  this->array[this->curr].append(dummy);

  const SbVec3f & planeN = plane.getNormal();

  for (int i = 0; i < n; i++) {
    SbVec3f v0, v1;
    void * data0, *data1;
    this->array[this->curr][i].get(v0, data0);
    this->array[this->curr][i+1].get(v1, data1);

    float d0 = plane.getDistance(v0);
    float d1 = plane.getDistance(v1);

    if (d0 >= 0.0f && d1 < 0.0f) { // exit plane
      SbVec3f dir = v1-v0;
      dir.normalize();
      float dot = dir.dot(planeN);
      SbVec3f newvertex = v0 - dir * (d0/dot);
      void * newdata = NULL;
      if (this->callback) {
        newdata = this->callback(v0, data0, v1, data1, newvertex, this->cbdata);
      }
      outputVertex(newvertex, newdata);
    }
    else if (d0 < 0.0f && d1 >= 0.0f) { // enter plane
      SbVec3f dir = v1-v0;
      dir.normalize();
      float dot = dir.dot(planeN);
      SbVec3f newvertex = v0 - dir * (d0/dot);
      void * newdata = NULL;
      if (this->callback) {
        newdata = this->callback(v0, data0, v1, data1, newvertex, this->cbdata);
      }
      outputVertex(newvertex, newdata);
      outputVertex(v1, data1);
    }
    else if (d0 >= 0.0f && d1 >= 0.0f) { // in plane
      outputVertex(v1, data1);
    }
  }
  this->array[this->curr].truncate(0);
  this->curr ^= 1;
}

/*!
  Returns the number of vertices in the polygon.
  \sa SbClip::getVertex()
*/
int
SbClip::getNumVertices(void) const
{
  return this->array[this->curr].getLength();
}

/*!
  Returns the vertex at index \a idx.
  \sa SbClip::getNumVertices()
*/
void
SbClip::getVertex(const int idx, SbVec3f & v, void ** vdata) const
{
  v = this->array[this->curr][idx].vertex;
  if (vdata) *vdata = this->array[this->curr][idx].data;
}

/*!
  Return the vertex data at index \a idx.
*/
void *
SbClip::getVertexData(const int idx) const
{
  return this->array[this->curr][idx].data;
}
