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
#include <assert.h>
#include <string.h>

SoVertexArrayIndexer::SoVertexArrayIndexer(GLenum targetin)
  : target(targetin),
    next(NULL)
{
}

SoVertexArrayIndexer::~SoVertexArrayIndexer() 
{
  for (int i = 0; i < this->ciarray.getLength(); i++) {
    delete[] this->ciarray[i];
  }
  delete this->next;
}
  
GLenum 
SoVertexArrayIndexer::getTarget(void) const
{
  return this->target;
}

void 
SoVertexArrayIndexer::setNext(SoVertexArrayIndexer * nextin)
{
  this->next = nextin;
}

SoVertexArrayIndexer * 
SoVertexArrayIndexer::getNext(void) const
{
  return this->next;
}

void 
SoVertexArrayIndexer::addTriangle(const int32_t v0,
                                  const int32_t v1,
                                  const int32_t v2)
{
  assert(this->target == GL_TRIANGLES);
  this->indexarray.append(v0);
  this->indexarray.append(v1);
  this->indexarray.append(v2);
}

void 
SoVertexArrayIndexer::addQuad(const int32_t v0,
                              const int32_t v1,
                              const int32_t v2,
                              const int32_t v3)
{
  assert(this->target == GL_QUADS);
  this->indexarray.append(v0);
  this->indexarray.append(v1);
  this->indexarray.append(v2);
}
  
void 
SoVertexArrayIndexer::beginTarget(void)
{
  this->targetcounter = 0;
}
 
void 
SoVertexArrayIndexer::targetVertex(const int32_t v)
{
  this->targetcounter++;
  this->indexarray.append(v);
}

void 
SoVertexArrayIndexer::endTarget(void)
{
  this->countarray.append(this->targetcounter);
}

void 
SoVertexArrayIndexer::close(void)
{
  this->indexarray.fit();
  this->countarray.fit();
  this->ciarray.truncate(0);
  const int32_t * ptr = this->indexarray.getArrayPtr();
  for (int i = 0; i < this->countarray.getLength(); i++) {
    this->ciarray.append(ptr);
    ptr += (int) this->countarray[i];
  }
}

void 
SoVertexArrayIndexer::render(const cc_glglue * glue)
{
  switch (this->target) {
  case GL_TRIANGLES:
  case GL_QUADS:
    cc_glglue_glDrawElements(glue,
                             this->target,
                             this->indexarray.getLength(),
                             GL_UNSIGNED_INT,
                             (const GLvoid*) this->indexarray.getArrayPtr());

    break;
  default:
    if (cc_glglue_has_multidraw_vertex_arrays(glue)) {
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

  if (this->next) this->next->render(glue);
}
