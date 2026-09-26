#ifndef COIN_SOSHAPE_BUMPRENDER_H
#define COIN_SOSHAPE_BUMPRENDER_H

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

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/C/glue/gl.h>

#include "misc/SbHash.h"
#include <memory>

// *************************************************************************

class SoState;
class SoNode;
class SoLight;
class SoGLImage;
class SbMatrix;
class SoPrimitiveVertexCache;

// *************************************************************************

// FIXME: inherit from SoCache to avoid regenerating everything every frame

class soshape_bumprender {
public:
  soshape_bumprender(void);
  ~soshape_bumprender();
  void scheduleRedraw(SoState * state, SoNode * root);

  void calcTangentSpace(const SoPrimitiveVertexCache * cache);
  void renderBump(SoState * state,
                  const SoPrimitiveVertexCache * cache,
                  SoLight * light, const SbMatrix & toobjectspace);
  void renderBumpSpecular(SoState * state,
                          const SoPrimitiveVertexCache * cache,
                          SoLight * light, const SbMatrix & toobjectspace);
  void renderNormal(SoState * state, const SoPrimitiveVertexCache * cache);

private:
  // A renderer owns one registration/resource lifetime, not a copyable handle.
  soshape_bumprender(const soshape_bumprender &) = delete;
  soshape_bumprender & operator=(const soshape_bumprender &) = delete;

  struct spec_programidx {
    GLuint dirlight;
    GLuint pointlight;
    GLuint fragment;
  };

  struct diffuse_programidx {
    GLuint pointlight; // Pointlight diffuse rendering not implemented as a program yet.
    GLuint dirlight;
    GLuint normalrendering;
  };

  void initLight(SoLight * light, const SbMatrix & m);
  void calcTSBCoords(const SoPrimitiveVertexCache * cache, SoLight * light);
  SbVec3f getLightVec(const SbVec3f & v) const;
  void initPrograms(const cc_glglue * glue, SoState * state);
  void initDiffusePrograms(const cc_glglue * glue, SoState * state);
  SbBool ensurePrograms(const cc_glglue * glue, SoState * state,
                        spec_programidx & programs);
  SbBool ensureDiffusePrograms(const cc_glglue * glue, SoState * state,
                               diffuse_programidx & programs);

  static void context_destruction_cb(uint32_t contextid, void * userdata);
  static void delete_program_cb(void * closure, uint32_t contextid);
  static void initialize_program_cb(void * closure, uint32_t contextid);
  static void cleanup_program_cb(void * closure, uint32_t contextid);

  SbList <SbVec3f> cubemaplist;
  SbList <SbVec3f> tangentlist;

  SbVec3f lightvec;
  SbBool ispointlight;

  typedef SbHash<uint32_t, diffuse_programidx> ContextId2DiffuseStruct;
  typedef SbHash<uint32_t, spec_programidx> ContextId2SpecStruct;
  struct ProgramCache;
  std::shared_ptr<ProgramCache> programcache;
};

#endif // COIN_SOSHAPE_BUMPRENDER
