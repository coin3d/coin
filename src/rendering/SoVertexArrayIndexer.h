#ifndef COIN_VERTEXARRAYINDEXER_H
#define COIN_VERTEXARRAYINDEXER_H

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

#include <Inventor/lists/SbList.h>
#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>
#include <stdlib.h>

class SoVBO;

class SoVertexArrayIndexer {
public:
  SoVertexArrayIndexer(void);
  ~SoVertexArrayIndexer();

  void addTriangle(const int32_t v0,
                   const int32_t v1,
                   const int32_t v2);
  void addLine(const int32_t v0,
               const int32_t v1);
  void addPoint(const int32_t v0);


  void addQuad(const int32_t v0,
               const int32_t v1,
               const int32_t v2,
               const int32_t v3);

  void beginTarget(GLenum target);
  void targetVertex(GLenum target, const int32_t v);
  void endTarget(GLenum target);

  void close(void);
  void render(const cc_glglue * glue, const SbBool renderasvbo, const uint32_t vbocontextid);

  int getNumVertices(void);
  int getNumIndices(void) const;
  const GLint * getIndices(void) const;
  GLint * getWriteableIndices(void);

private:
  void addIndex(int32_t i);
  void sort_triangles(void);
  void sort_lines(void);
  SoVertexArrayIndexer * getNext(void);

  GLenum target;
  SoVertexArrayIndexer * next;

  int targetcounter;
  SbList <GLsizei> countarray;
  SbList <const GLint *> ciarray;
  SbList <GLint> indexarray;
  SoVBO * vbo;
  SbBool use_shorts;
};

#endif // COIN_VERTEXARRAYINDEXER_H
