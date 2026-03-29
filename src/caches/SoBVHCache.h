#ifndef COIN_SOBVHCACHE_H
#define COIN_SOBVHCACHE_H

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

#include <Inventor/caches/SoCache.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/system/gl.h>

class SoRayPickAction;
class SoShape;
class SoBVHCacheP;

class SoBVHCache : public SoCache {
  typedef SoCache inherited;
public:
  SoBVHCache(SoState * state);
  virtual ~SoBVHCache();

  void build(const SbVec3f * vertices,
             const SbVec3f * normals,
             const SbVec4f * texcoords,
             const uint8_t * colors,
             int numVertices,
             const GLint * indices,
             int numTriangleIndices);

  void buildFlat(const SbVec3f * triVertices,
                 const SbVec3f * triNormals,
                 int numTriangles);

  SbBool rayPick(SoRayPickAction * action, SoShape * shape) const;

  int getNumTriangles(void) const;
  SbBool isBuilt(void) const;

private:
  SoBVHCacheP * pimpl;

  SoBVHCache(const SoBVHCache & rhs);
  SoBVHCache & operator = (const SoBVHCache & rhs);
};

#endif // COIN_SOBVHCACHE_H
