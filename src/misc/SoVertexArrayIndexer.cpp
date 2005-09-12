/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include "SoVertexArrayIndexer.h"
#include "SoVBO.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

SoVertexArrayIndexer::SoVertexArrayIndexer(void)
  : target(0),
    next(NULL),
    vbo(NULL)
{
}

SoVertexArrayIndexer::~SoVertexArrayIndexer() 
{
  delete this->vbo;
  delete this->next;
}

void 
SoVertexArrayIndexer::addTriangle(const int32_t v0,
                                  const int32_t v1,
                                  const int32_t v2)
{
  if (this->target == 0) this->target = GL_TRIANGLES;
  if (this->target == GL_TRIANGLES) {
    this->indexarray.append(v0);
    this->indexarray.append(v1);
    this->indexarray.append(v2);
  }
  else {
    this->getNext()->addTriangle(v0,v1,v2);
  }
}

void 
SoVertexArrayIndexer::addQuad(const int32_t v0,
                              const int32_t v1,
                              const int32_t v2,
                              const int32_t v3)
{
  if (this->target == 0) this->target = GL_QUADS;
  if (this->target == GL_QUADS) {
    this->indexarray.append(v0);
    this->indexarray.append(v1);
    this->indexarray.append(v2);
    this->indexarray.append(v3);
  }
  else {
    this->getNext()->addQuad(v0,v1,v2,v3);
  }
}

void 
SoVertexArrayIndexer::beginTarget(GLenum targetin)
{
  if (this->target == 0) this->target = targetin;
  if (this->target == targetin) {
    this->targetcounter = 0;
  }
  else {
    this->getNext()->beginTarget(targetin);
  }
}
 
void 
SoVertexArrayIndexer::targetVertex(GLenum targetin, const int32_t v)
{
  assert(this->target != 0);
  if (this->target == targetin) {
    this->targetcounter++;
    this->indexarray.append(v);
  }
  else {
    this->getNext()->targetVertex(targetin, v);
  }
}

void 
SoVertexArrayIndexer::endTarget(GLenum targetin)
{
  assert(this->target != 0);
  if (this->target == targetin) {
    this->countarray.append(this->targetcounter);
  }
  else {
    this->getNext()->endTarget(targetin);
  }
}

void 
SoVertexArrayIndexer::close(void)
{
  this->indexarray.fit();
  this->countarray.fit();
  this->ciarray.truncate(0);

  if (this->target != GL_TRIANGLES && this->target != GL_QUADS) {
    const int32_t * ptr = this->indexarray.getArrayPtr();
    for (int i = 0; i < this->countarray.getLength(); i++) {
      this->ciarray.append(ptr);
      ptr += (int) this->countarray[i];
    }
  }
  if (this->target == GL_TRIANGLES) {
    this->sort_triangles();
  }
  if (this->next) this->next->close();
}

void 
SoVertexArrayIndexer::render(const cc_glglue * glue, const SbBool renderasvbo, const uint32_t contextid)
{
  SbBool ismac = FALSE;
#ifdef __APPLE__
  ismac = TRUE;
#endif
  switch (this->target) {
  case GL_TRIANGLES:
  case GL_QUADS:
    // common case
    if (renderasvbo) {
      if (this->vbo == NULL) {
        this->vbo = new SoVBO(GL_ELEMENT_ARRAY_BUFFER);
        this->vbo->setBufferData(this->indexarray.getArrayPtr(),
                                 this->indexarray.getLength()*sizeof(int32_t));
      }
      this->vbo->bindBuffer(contextid);
      cc_glglue_glDrawElements(glue, 
                               this->target, 
                               this->indexarray.getLength(), 
                               GL_UNSIGNED_INT, NULL);
      cc_glglue_glBindBuffer(glue, GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else {
      cc_glglue_glDrawElements(glue,
                               this->target,
                               this->indexarray.getLength(),
                               GL_UNSIGNED_INT,
                               (const GLvoid*) this->indexarray.getArrayPtr());
    }
    break;
  default:
    if (cc_glglue_has_multidraw_vertex_arrays(glue) && !ismac) {
      cc_glglue_glMultiDrawElements(glue,
                                    this->target,
                                    (GLsizei*) this->countarray.getArrayPtr(),
                                    GL_UNSIGNED_INT,
                                    (const GLvoid**) this->ciarray.getArrayPtr(),
                                    this->countarray.getLength());
    }
    else {
      for (int i = 0; i < this->countarray.getLength(); i++) {
        const int32_t * ptr = this->ciarray[i];
        GLsizei cnt = this->countarray[i];
        cc_glglue_glDrawElements(glue,
                                 this->target,
                                 cnt,
                                 GL_UNSIGNED_INT,
                                 (const GLvoid*) ptr);
      }
    }
    break;
  }

  if (this->next) this->next->render(glue, renderasvbo, contextid);
}

int 
SoVertexArrayIndexer::getNumVertices(void)
{
  int count = this->indexarray.getLength();
  if (this->next) count += this->next->getNumVertices();
  return count;
}

SoVertexArrayIndexer *
SoVertexArrayIndexer::getNext(void)
{
  if (this->next == NULL) {
    this->next = new SoVertexArrayIndexer;
  }
  return this->next;
}

// sort an array of three integers
static void sort3(int32_t * arr)
{
  // simple bubble-sort
  int32_t tmp;
  if (arr[1] < arr[0]) {
    tmp = arr[0];
    arr[0] = arr[1];
    arr[1] = tmp;
  }
  if (arr[2] < arr[1]) {
    tmp = arr[1];
    arr[1] = arr[2];
    arr[2] = tmp;
  }
  if (arr[1] < arr[0]) {
    tmp = arr[0];
    arr[0] = arr[1];
    arr[1] = tmp;
  }
}

// qsort callback used for sorting triangles based on vertex indices
extern "C" {
static int
compare_triangle(const void * v0, const void * v1)
{
  int i;
  int32_t * t0 = (int*) v0;
  int32_t * t1 = (int*) v1;

  int32_t ti0[3];
  int32_t ti1[3];
  for (i = 0; i < 3; i++) {
    ti0[i] = t0[i];
    ti1[i] = t1[i];
  }
  sort3(ti0);
  sort3(ti1);

  for (i = 0; i < 3; i++) {
    int32_t diff = ti0[i] - ti1[i];
    if (diff != 0) return diff;
  }
  return 0;
}
}

void 
SoVertexArrayIndexer::sort_triangles(void)
{
  // sort triangles based on vertex indices to get more hits in the
  // GPU vertex cache. Not the optimal solution, but should work
  // pretty well. Example: bunny.iv (~70000 triangles) went from 238
  // fps with no sorting to 380 fps with sorting.
  if (this->indexarray.getLength()) {
    qsort((void*) this->indexarray.getArrayPtr(),
          this->indexarray.getLength() / 3,
          sizeof(int32_t) * 3,
          compare_triangle);
  }
}
