#ifndef COIN_VBO_H
#define COIN_VBO_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>

#include "misc/SbHash.h"

class SoState;

class SoVBO {
 public:
  SoVBO(const GLenum target = GL_ARRAY_BUFFER,
        const GLenum usage = GL_STATIC_DRAW);
  ~SoVBO();

  static void init(void);

  void setBufferData(const GLvoid * data, intptr_t size, uint32_t dataid = 0);
  void * allocBufferData(intptr_t size, uint32_t dataid = 0);
  uint32_t getBufferDataId(void) const;
  void getBufferData(const GLvoid *& data, intptr_t & size);
  void bindBuffer(uint32_t contextid);

  static void setVertexCountLimits(const int minlimit, const int maxlimit);
  static int getVertexCountMinLimit(void);
  static int getVertexCountMaxLimit(void);

  static void testGLPerformance(const uint32_t contextid);
  static SbBool shouldCreateVBO(SoState * state, const uint32_t contextid, const int numdata);
  static SbBool shouldRenderAsVertexArrays(SoState * statea,
                                           const uint32_t contextid,
                                           const int numdata);

 private:
  static void context_created(const uint32_t contextid, void * closure);
  static SbBool isVBOFast(const uint32_t contextid);
  static void context_destruction_cb(uint32_t context, void * userdata);
  friend struct vbo_schedule;
  static void vbo_delete(void * closure, uint32_t contextid);

  GLenum target;
  GLenum usage;
  const GLvoid * data;
  intptr_t datasize;
  uint32_t dataid;
  SbBool didalloc;

  SbHash<uint32_t, GLuint> vbohash;
};

#endif // COIN_VERTEXARRAYINDEXER_H
