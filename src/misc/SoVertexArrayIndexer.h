#ifndef COIN_VERTEXARRAYINDEXER_H
#define COIN_VERTEXARRAYINDEXER_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/lists/SbList.h>
#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>
#include <stdlib.h>

class SoVertexArrayIndexer {
public:
  SoVertexArrayIndexer(GLenum target);
  ~SoVertexArrayIndexer();
  
  void setNext(SoVertexArrayIndexer * next);
  SoVertexArrayIndexer * getNext(void) const;
  GLenum getTarget(void) const;
  
  void addTriangle(const int32_t v0,
                   const int32_t v1,
                   const int32_t v2);

  void addQuad(const int32_t v0,
               const int32_t v1,
               const int32_t v2,
               const int32_t v3);
  
  void beginTarget(void);
  void targetVertex(const int32_t v);
  void endTarget(void);

  void close(void);
  void render(const cc_glglue * glue);

  // for debugging
  int getNumVertices(void) {
    return this->indexarray.getLength();
  }

private:
  GLenum target;
  SoVertexArrayIndexer * next;

  int targetcounter;
  SbList <GLsizei> countarray;
  SbList <const int32_t *> ciarray;
  SbList <int32_t> indexarray;
};

#endif // COIN_VERTEXARRAYINDEXER_H
