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

#include "shapenodes/soshape_bumprender.h"

#include <cassert>
#include <limits>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/C/glue/gl.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/elements/SoBumpMapElement.h>
#include <Inventor/elements/SoBumpMapMatrixElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoMultiTextureMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoMultiTextureMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/caches/SoPrimitiveVertexCache.h>

// For coin_apply_normalization_cube_map().
#include "glue/glp.h"
#include "rendering/SoGL.h"
#include "tidbitsp.h"
#include <Inventor/sensors/SoNodeSensor.h>

// *************************************************************************

// Fragment program for bumpmapping
static const char * bumpspecfpprogram =
"!!ARBfp1.0\n"
"PARAM u0 = program.env[0];\n" // Specular color (3 floats)
"PARAM u1 = program.env[1];\n" // Shininess color (1 float)
"PARAM c0 = {2, 0.5, 0, 0};\n"
"TEMP R0;\n"
"TEMP R1;\n"
" TEX R0.xyz, fragment.texcoord[0], texture[0], 2D;\n"
" ADD R0.xyz, R0, -c0.y;\n"
" MUL R0.xyz, R0, c0.x;\n"
" MOV R1.xyz, fragment.texcoord[2];\n"
" ADD R1.xyz, fragment.texcoord[1], R1;\n"
" DP3 R0.w, R1, R1;\n"
" RSQ R0.w, R0.w;\n"
" MUL R1.xyz, R0.w, R1;\n"
" TEX R1.xyz, R1, texture[1], CUBE;\n"
" ADD R1.xyz, R1, -c0.y;\n"
" MUL R1.xyz, R1, c0.x;\n"
" DP3_SAT R0.x, R0, R1;\n"
" POW R0.x, R0.x, u1.x;\n"
" MUL result.color, u0, R0.x;\n"
"END\n";

// Vertex program for directional lights
static const char * directionallightvpprogram =
"!!ARBvp1.0\n"
"TEMP R0;\n"
"ATTRIB v26 = vertex.texcoord[2];\n"
"ATTRIB v25 = vertex.texcoord[1];\n"
"ATTRIB v24 = vertex.texcoord[0];\n"
"ATTRIB v18 = vertex.normal;\n"
"ATTRIB v16 = vertex.position;\n"
"PARAM c1 = program.env[1];\n"
"PARAM c0 = program.env[0];\n"
"PARAM c6[4] = { state.matrix.texture[0] };\n"
"PARAM c2[4] = { state.matrix.mvp };\n"
" DPH result.position.x, v16.xyzz, c2[0];\n"
" DPH result.position.y, v16.xyzz, c2[1];\n"
" DPH result.position.z, v16.xyzz, c2[2];\n"
" DPH result.position.w, v16.xyzz, c2[3];\n"
" MUL R0.xy, c6[0].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].x, R0.x, R0.y;\n"
" MUL R0.xy, c6[1].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].y, R0.x, R0.y;\n"
" DP3 result.texcoord[1].x, v25.xyzx, c0.xyzx;\n"
" DP3 result.texcoord[1].y, v26.xyzx, c0.xyzx;\n"
" DP3 result.texcoord[1].z, v18.xyzx, c0.xyzx;\n"
" ADD R0.yzw, c1.xxyz, -v16.xxyz;\n"
" DP3 R0.x, R0.yzwy, R0.yzwy;\n"
" RSQ R0.x, R0.x;\n"
" MUL R0.xyz, R0.x, R0.yzwy;\n"
" DP3 result.texcoord[2].x, v25.xyzx, R0.xyzx;\n"
" DP3 result.texcoord[2].y, v26.xyzx, R0.xyzx;\n"
" DP3 result.texcoord[2].z, v18.xyzx, R0.xyzx;\n"
"END\n";


// Vertex program for point lights
static const char * pointlightvpprogram =
"!!ARBvp1.0\n"
"TEMP R0;\n"
"ATTRIB v26 = vertex.texcoord[2];\n"
"ATTRIB v25 = vertex.texcoord[1];\n"
"ATTRIB v24 = vertex.texcoord[0];\n"
"ATTRIB v18 = vertex.normal;\n"
"ATTRIB v16 = vertex.position;\n"
"PARAM c1 = program.env[1];\n" // Light position
"PARAM c0 = program.env[0];\n" // Eye position
"PARAM c2[4] = { state.matrix.mvp };\n"
"PARAM c6[4] = { state.matrix.texture[0] };\n"
" DPH result.position.x, v16.xyzz, c2[0];\n"
" DPH result.position.y, v16.xyzz, c2[1];\n"
" DPH result.position.z, v16.xyzz, c2[2];\n"
" DPH result.position.w, v16.xyzz, c2[3];\n"
" MUL R0.xy, c6[0].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].x, R0.x, R0.y;\n"
" MUL R0.xy, c6[1].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].y, R0.x, R0.y;\n"
" ADD R0.yzw, c0.xxyz, -v16.xxyz;\n"
" DP3 R0.x, R0.yzwy, R0.yzwy;\n"
" RSQ R0.x, R0.x;\n"
" MUL R0.xyz, R0.x, R0.yzwy;\n"
" DP3 result.texcoord[1].x, v25.xyzx, R0.xyzx;\n"
" DP3 result.texcoord[1].y, v26.xyzx, R0.xyzx;\n"
" DP3 result.texcoord[1].z, v18.xyzx, R0.xyzx;\n"
" ADD R0.yzw, c1.xxyz, -v16.xxyz;\n"
" DP3 R0.x, R0.yzwy, R0.yzwy;\n"
" RSQ R0.x, R0.x;\n"
" MUL R0.xyz, R0.x, R0.yzwy;\n"
" DP3 result.texcoord[2].x, v25.xyzx, R0.xyzx;\n"
" DP3 result.texcoord[2].y, v26.xyzx, R0.xyzx;\n"
" DP3 result.texcoord[2].z, v18.xyzx, R0.xyzx;\n"
"END\n";

// vertex program for bumpmapping (calculate tsb coordinates for
// texture unit 1)
static const char * diffusebumpdirlightvpprogram =
"!!ARBvp1.0\n"
"TEMP R0;\n"
"PARAM c5 = { 1, 0, 2, 0 };\n"
"PARAM color = { 1, 1, 1, 1 };\n"
"ATTRIB v19 = vertex.color;\n"
"ATTRIB v25 = vertex.texcoord[1];\n"
"ATTRIB v24 = vertex.texcoord[0];\n"
"ATTRIB v18 = vertex.normal;\n"
"ATTRIB v16 = vertex.position;\n"
"PARAM c0 = program.env[0];\n"
"PARAM c1[4] = { state.matrix.mvp };\n"
"PARAM c6[4] = { state.matrix.texture[0] };\n"
" MUL R0.xy, c6[0].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].x, R0.x, R0.y;\n"
" MUL R0.xy, c6[1].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].y, R0.x, R0.y;\n"
" MOV result.color, color;\n"
" DPH result.position.x, v16.xyzz, c1[0];\n"
" DPH result.position.y, v16.xyzz, c1[1];\n"
" DPH result.position.z, v16.xyzz, c1[2];\n"
" DPH result.position.w, v16.xyzz, c1[3];\n"
" DP3 result.texcoord[1].x, v25.xyzx, c0.xyzx;\n"
" DP3 result.texcoord[1].y, v19.xyzx, c0.xyzx;\n"
" DP3 result.texcoord[1].z, v18.xyzx, c0.xyzx;\n"
"END\n";

// vertex program for normal rendering. Needed to get exactly the same
// z-buffer value for each vertex
static const char * normalrenderingvpprogram =
"!!ARBvp1.0\n"
"TEMP R0;\n"
"ATTRIB v19 = vertex.color;\n"
"ATTRIB v16 = vertex.position;\n"
"ATTRIB v25 = vertex.texcoord[1];\n"
"ATTRIB v24 = vertex.texcoord[0];\n"
"PARAM c6[4] = { state.matrix.texture[0] };\n"
"PARAM c7[4] = { state.matrix.texture[1] };\n"
"PARAM c1[4] = { state.matrix.mvp };\n"
" DPH result.position.x, v16.xyzz, c1[0];\n"
" DPH result.position.y, v16.xyzz, c1[1];\n"
" DPH result.position.z, v16.xyzz, c1[2];\n"
" DPH result.position.w, v16.xyzz, c1[3];\n"
" MUL R0.xy, c6[0].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].x, R0.x, R0.y;\n"
" MUL R0.xy, c6[1].xyxx, v24.xyxx;\n"
" ADD result.texcoord[0].y, R0.x, R0.y;\n"
" MUL R0.xy, c7[0].xyxx, v25.xyxx;\n"
" ADD result.texcoord[1].x, R0.x, R0.y;\n"
" MUL R0.xy, c7[1].xyxx, v25.xyxx;\n"
" ADD result.texcoord[1].y, R0.x, R0.y;\n"
" MOV result.color, v19;\n"
"END\n";

// *************************************************************************

SbBool bumphack = TRUE;

// *************************************************************************

static void
soshape_bumprender_delete_programs(const cc_glglue * glue,
                                   const GLuint * programs,
                                   const int numprograms)
{
  assert(numprograms <= 3);
  GLuint validprograms[3];
  int numvalid = 0;
  for (int i = 0; i < numprograms; i++) {
    if (programs[i] != 0) validprograms[numvalid++] = programs[i];
  }
  if (numvalid > 0) {
    cc_glglue_glDeletePrograms(glue, (GLsizei) numvalid, validprograms);
  }
}

struct soshape_bump_program_error {
  soshape_bump_program_error() : description(NULL), stage(NULL), error(GL_NO_ERROR), position(-1) { }
  const char * description;
  const char * stage;
  GLenum error;
  GLint position;
  std::string message;
  void report() const {
    if (this->description) {
      SoDebugError::postWarning("soshape_bumprender::ensurePrograms",
                                "Error in %s during %s! (GL error: 0x%x, byte pos: %d) '%s'.\n",
                                this->description, this->stage, (unsigned int) this->error,
                                this->position, this->message.c_str());
    }
  }
};

static SbBool
soshape_bumprender_load_program(const cc_glglue * glue,
                                const GLenum target,
                                const char * source,
                                const char * description,
                                GLuint & program,
                                soshape_bump_program_error & failure)
{
  while (glGetError() != GL_NO_ERROR) { }
  program = 0;
  GLint previousprogram = 0;
  cc_glglue_glGetProgramiv(glue, target, GL_PROGRAM_BINDING_ARB, &previousprogram);
  GLenum err = glGetError();
  const char * stage = "save binding";
  GLint errorpos = -1;
  GLint programlength = 0;
  const GLubyte * errorstring = NULL;
  if (err == GL_NO_ERROR) {
    stage = "generate name";
    cc_glglue_glGenPrograms(glue, 1, &program);
    err = glGetError();
    if (err == GL_NO_ERROR && program != 0) {
      stage = "bind new program";
      cc_glglue_glBindProgram(glue, target, program);
      err = glGetError();
      if (err == GL_NO_ERROR) {
        stage = "upload";
        cc_glglue_glProgramString(glue, target, GL_PROGRAM_FORMAT_ASCII_ARB,
                                  (GLsizei) strlen(source), source);
        err = glGetError();
        if (err != GL_NO_ERROR) {
          glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorpos);
          errorstring = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
          (void) glGetError();
        }
        else {
          stage = "query uploaded length";
          cc_glglue_glGetProgramiv(glue, target, GL_PROGRAM_LENGTH_ARB, &programlength);
          err = glGetError();
        }
      }
      // Never upload after a failed bind. Restore only a binding obtained
      // successfully; a failed save must not change the caller's GL state.
      cc_glglue_glBindProgram(glue, target, (GLuint) previousprogram);
      const GLenum restoreerr = glGetError();
      if (err == GL_NO_ERROR && restoreerr != GL_NO_ERROR) {
        stage = "restore binding";
        err = restoreerr;
      }
    }
  }
  if (err != GL_NO_ERROR || program == 0 || programlength <= 0) {
    failure.description = description; failure.stage = stage;
    failure.error = err; failure.position = errorpos;
    failure.message = errorstring ? (const char *) errorstring : "";
    return FALSE;
  }
  return TRUE;
}

// A callback never points at a shape/renderer. It resolves a non-reused token
// to shared resource state, so a copied callback remains safe after removal.
// After the shape dies, this state owns its GL objects until a current-context
// cleanup or context destruction drains them. All cache access is serialized.
struct soshape_bumprender::ProgramCache {
  enum InitStatus { EMPTY, PENDING, READY, FAILED };
  struct Context {
    Context() : specstatus(EMPTY), diffusestatus(EMPTY) {
      spec.fragment = spec.dirlight = spec.pointlight = 0;
      diffuse.dirlight = diffuse.pointlight = diffuse.normalrendering = 0;
    }
    InitStatus specstatus, diffusestatus;
    spec_programidx spec;
    diffuse_programidx diffuse;
    soshape_bump_program_error specerror, diffuseerror;
  };
  typedef std::map<uint32_t, Context> Contexts;
  typedef std::shared_ptr<ProgramCache> Ptr;
  typedef std::map<uintptr_t, Ptr> Entries;
  struct Registry {
    Registry() : nexttoken(0), cleanupregistered(false) { }
    std::mutex mutex;
    Entries entries;
    uintptr_t nexttoken;
    bool cleanupregistered;
  };
  struct RedrawSensor : SoNodeSensor, std::enable_shared_from_this<RedrawSensor> {
    explicit RedrawSensor(uintptr_t token) : SoNodeSensor(redraw_cb, (void *) token) {
      // Never notify priority-zero viewers from inside display-list traversal.
      this->setPriority(100);
    }
    void trigger() override {
      // A notification handler may destroy the renderer. Keep its sensor alive
      // until SoDataSensor::trigger() has finished its post-callback cleanup.
      const std::shared_ptr<RedrawSensor> sensor = this->shared_from_this();
      SoNodeSensor::trigger();
    }
  };
  typedef std::shared_ptr<RedrawSensor> RedrawPtr;
  typedef std::map<SoNode *, RedrawPtr> Redraws;
  ProgramCache() : token(0), alive(true) { }
  std::mutex mutex;
  uintptr_t token;
  bool alive;
  Contexts contexts;
  Redraws redraws;

  static void redraw_cb(void * closure, SoSensor * sensor) {
    const Ptr cache = lookup((uintptr_t) closure);
    if (!cache) return;
    SoNode * root = NULL;
    {
      std::lock_guard<std::mutex> lock(cache->mutex);
      SoNodeSensor * node = static_cast<SoNodeSensor *>(sensor);
      if (cache->alive) root = node->getAttachedNode();
      if (root) root->ref();
      node->detach();
    }
    // Node deletion detaches the sensor. Keep a surviving root alive through
    // user notification handlers, with no cache/render lock held.
    if (root) {
      root->touch();
      root->unref();
    }
  }

  static Registry & registry() {
    static Registry reg;
    return reg;
  }
  static Ptr create() {
    Ptr cache(new ProgramCache);
    Registry & reg = registry();
    std::lock_guard<std::mutex> lock(reg.mutex);
    // The low bit is reserved for the deferred initialization's program kind.
    if (reg.nexttoken == (std::numeric_limits<uintptr_t>::max() >> 1)) {
      throw std::overflow_error("bump program callback tokens exhausted");
    }
    cache->token = ++reg.nexttoken;
    reg.entries.insert(std::make_pair(cache->token, cache));
    if (!reg.cleanupregistered) {
      coin_atexit(cleanup_registry, CC_ATEXIT_NORMAL);
      reg.cleanupregistered = true;
    }
    return cache;
  }
  static Ptr lookup(uintptr_t token) {
    Registry & reg = registry();
    std::lock_guard<std::mutex> lock(reg.mutex);
    Entries::const_iterator it = reg.entries.find(token);
    return it == reg.entries.end() ? Ptr() : it->second;
  }
  static void retire(const Ptr & cache) {
    bool removed = false;
    {
      std::lock_guard<std::mutex> lock(cache->mutex);
      if (cache->alive || !cache->contexts.empty()) return;
      Registry & reg = registry();
      std::lock_guard<std::mutex> registrylock(reg.mutex);
      removed = reg.entries.erase(cache->token) != 0;
    }
    if (removed) {
      SoContextHandler::removeContextDestructionCallback(context_destruction_cb,
                                                         (void *) cache->token);
    }
  }
  static void cleanup_registry() {
    Entries entries;
    Registry & reg = registry();
    {
      std::lock_guard<std::mutex> lock(reg.mutex);
      entries.swap(reg.entries);
      reg.cleanupregistered = false;
      // Do not reuse tokens across SoDB::finish()/init() either.
    }
    for (Entries::const_iterator it = entries.begin(); it != entries.end(); ++it) {
      SoContextHandler::removeContextDestructionCallback(context_destruction_cb,
                                                         (void *) it->first);
    }
  }
  // Caller holds mutex. The entry exists before allocating GL names, so
  // publishing a successfully uploaded set cannot allocate host memory.
  static void initialize(const cc_glglue * glue, Context & ctx, bool diffuse) {
    if (diffuse) {
      diffuse_programidx & p = ctx.diffuse;
      if (soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         diffusebumpdirlightvpprogram,
                                         "diffuse directional light vertex program", p.dirlight, ctx.diffuseerror) &&
          soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         normalrenderingvpprogram,
                                         "normal rendering vertex program", p.normalrendering, ctx.diffuseerror)) {
        ctx.diffusestatus = READY;
      }
      else {
        const GLuint ids[] = { p.dirlight, p.pointlight, p.normalrendering };
        soshape_bumprender_delete_programs(glue, ids, 3);
        p.dirlight = p.pointlight = p.normalrendering = 0;
        ctx.diffusestatus = FAILED;
      }
    }
    else {
      spec_programidx & p = ctx.spec;
      if (soshape_bumprender_load_program(glue, GL_FRAGMENT_PROGRAM_ARB,
                                         bumpspecfpprogram, "fragment program", p.fragment, ctx.specerror) &&
          soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         directionallightvpprogram,
                                         "directional light vertex program", p.dirlight, ctx.specerror) &&
          soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         pointlightvpprogram,
                                         "point light vertex program", p.pointlight, ctx.specerror)) {
        ctx.specstatus = READY;
      }
      else {
        const GLuint ids[] = { p.fragment, p.dirlight, p.pointlight };
        soshape_bumprender_delete_programs(glue, ids, 3);
        p.fragment = p.dirlight = p.pointlight = 0;
        ctx.specstatus = FAILED;
      }
    }
  }
  SbBool request(const cc_glglue * glue, SoState * state, uint32_t contextid,
                 bool diffuse, spec_programidx * spec, diffuse_programidx * diff) {
    soshape_bump_program_error failure;
    std::unique_lock<std::mutex> lock(this->mutex);
    if (!this->alive) return FALSE;
    Context & ctx = this->contexts[contextid];
    InitStatus & status = diffuse ? ctx.diffusestatus : ctx.specstatus;
    bool schedule = false;
    if (status == EMPTY) {
      GLint list = 0;
      glGetIntegerv(GL_LIST_INDEX, &list);
      if (list != 0) {
        status = PENDING;
        schedule = true;
      }
      else initialize(glue, ctx, diffuse);
    }
    const InitStatus result = status;
    if (result == READY) {
      if (spec) *spec = ctx.spec;
      if (diff) *diff = ctx.diffuse;
    }
    if (result == FAILED) {
      soshape_bump_program_error & cached = diffuse ? ctx.diffuseerror : ctx.specerror;
      failure = std::move(cached);
      cached.description = NULL; // Report at most once, on use by a renderer.
    }
    lock.unlock();
    // Error handlers are user callbacks. Do not invoke them under our mutex,
    // or from the scheduler (which also holds a lock during deferred uploads).
    failure.report();
    if (result == PENDING) {
      // Queries are immediate in GL_COMPILE, but Bind/ProgramString are not.
      // Invalidate the incomplete list and initialize before the next
      // traversal opens a cache, including when renderCaching is forced ON.
      SoCacheElement::invalidate(state);
      SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DONT_AUTO_CACHE);
      if (schedule) {
        const uintptr_t closure = (this->token << 1) | (diffuse ? 1 : 0);
        SoGLCacheContextElement::scheduleDeleteCallback(contextid,
                                                        initialize_program_cb,
                                                        (void *) closure);
      }
    }
    return result == READY;
  }
};

soshape_bumprender::soshape_bumprender(void)
  : programcache(ProgramCache::create())
{
  SoContextHandler::addContextDestructionCallback(context_destruction_cb,
                                                   (void *) this->programcache->token);
}

soshape_bumprender::~soshape_bumprender()
{
  const ProgramCache::Ptr cache = this->programcache;
  std::vector<uint32_t> contexts;
  ProgramCache::Redraws redraws;
  {
    std::lock_guard<std::mutex> lock(cache->mutex);
    cache->alive = false;
    redraws.swap(cache->redraws);
    for (ProgramCache::Contexts::iterator it = cache->contexts.begin();
         it != cache->contexts.end();) {
      if (it->second.specstatus == ProgramCache::READY ||
          it->second.diffusestatus == ProgramCache::READY) {
        contexts.push_back(it->first);
        ++it;
      }
      else cache->contexts.erase(it++);
    }
  }
  // Sensor queue changes can invoke application callbacks; never cancel under
  // the cache mutex, and keep every sensor alive through reentrant handlers.
  for (ProgramCache::Redraws::const_iterator it = redraws.begin(); it != redraws.end(); ++it) {
    if (it->second->isScheduled()) it->second->unschedule();
    it->second->detach();
  }
  // Never take the scheduler's mutex while holding the cache mutex: cleanup
  // invokes deferred callbacks under its own lock. Queued callbacks contain
  // tokens, not program names or pointers to this dead renderer.
  for (size_t i = 0; i < contexts.size(); ++i) {
    SoGLCacheContextElement::scheduleDeleteCallback(contexts[i], cleanup_program_cb,
                                                    (void *) cache->token);
  }
  ProgramCache::retire(cache);
}

// Retained for the private ABI. New deletion callbacks use resource tokens.
void
soshape_bumprender::delete_program_cb(void * closure, uint32_t contextid)
{
  const GLuint program = (GLuint) ((uintptr_t) closure);
  const cc_glglue * glue = cc_glglue_instance((int) contextid);
  cc_glglue_glDeletePrograms(glue, 1, &program);
}

void
soshape_bumprender::context_destruction_cb(uint32_t contextid, void * userdata)
{
  const ProgramCache::Ptr cache = ProgramCache::lookup((uintptr_t) userdata);
  if (!cache) return;
  ProgramCache::Context ctx;
  {
    std::lock_guard<std::mutex> lock(cache->mutex);
    ProgramCache::Contexts::iterator it = cache->contexts.find(contextid);
    if (it == cache->contexts.end()) return;
    ctx = it->second;
    cache->contexts.erase(it);
  }
  // No renderer access, and no cache lock across GL/glue callbacks.
  if (ctx.specstatus == ProgramCache::READY || ctx.diffusestatus == ProgramCache::READY) {
    const cc_glglue * glue = cc_glglue_instance((int) contextid);
    const GLuint spec[] = { ctx.spec.fragment, ctx.spec.dirlight, ctx.spec.pointlight };
    const GLuint diffuse[] = { ctx.diffuse.dirlight, ctx.diffuse.pointlight, ctx.diffuse.normalrendering };
    soshape_bumprender_delete_programs(glue, spec, 3);
    soshape_bumprender_delete_programs(glue, diffuse, 3);
  }
  ProgramCache::retire(cache);
}

void
soshape_bumprender::cleanup_program_cb(void * closure, uint32_t contextid)
{
  context_destruction_cb(contextid, closure);
}

void
soshape_bumprender::initialize_program_cb(void * closure, uint32_t contextid)
{
  const uintptr_t encoded = (uintptr_t) closure;
  const bool diffuse = (encoded & 1) != 0;
  const ProgramCache::Ptr cache = ProgramCache::lookup(encoded >> 1);
  if (!cache) return;
  GLint list = 0;
  glGetIntegerv(GL_LIST_INDEX, &list);
  const cc_glglue * glue = list == 0 ? cc_glglue_instance((int) contextid) : NULL;
  std::lock_guard<std::mutex> lock(cache->mutex);
  ProgramCache::Contexts::iterator it = cache->contexts.find(contextid);
  if (!cache->alive || it == cache->contexts.end()) return;
  ProgramCache::InitStatus & status = diffuse ? it->second.diffusestatus : it->second.specstatus;
  if (status != ProgramCache::PENDING) return;
  if (list != 0) status = ProgramCache::EMPTY; // Retry scheduling, never upload in a list.
  else ProgramCache::initialize(glue, it->second, diffuse);
}

// to avoid warnings from SbVec3f::normalize()
inline void NORMALIZE(SbVec3f &v)
{
  float len = v.length();
  if (len) {
    len = 1.0f / len;
    v[0] *= len;
    v[1] *= len;
    v[2] *= len;
  }
}

void
soshape_bumprender::scheduleRedraw(SoState * state, SoNode * root)
{
  const ProgramCache::Ptr cache = this->programcache;
  ProgramCache::RedrawPtr sensor;
  {
    std::lock_guard<std::mutex> lock(cache->mutex);
    for (ProgramCache::Redraws::iterator it = cache->redraws.begin(); it != cache->redraws.end();) {
      if (!it->second->isScheduled() && !it->second->getAttachedNode()) cache->redraws.erase(it++);
      else ++it;
    }
    ProgramCache::Contexts::const_iterator it =
      cache->contexts.find((uint32_t) SoGLCacheContextElement::get(state));
    if (!cache->alive || it == cache->contexts.end() ||
        (it->second.specstatus != ProgramCache::PENDING &&
         it->second.diffusestatus != ProgramCache::PENDING)) return;
    // A shared shape may belong to multiple viewers. Notify every root, not
    // just the most recently traversed one. Sensors do not keep roots alive.
    ProgramCache::RedrawPtr & entry = cache->redraws[root];
    if (!entry) entry.reset(new ProgramCache::RedrawSensor(cache->token));
    sensor = entry;
    if (sensor->getAttachedNode() != root) {
      sensor->detach();
      sensor->attach(root);
    }
  }
  sensor->schedule(); // notifyChanged() is an application callback.
  bool alive;
  {
    std::lock_guard<std::mutex> lock(cache->mutex);
    alive = cache->alive;
  }
  // Cancellation may have run reentrantly while the queue was changing.
  if (!alive) {
    if (sensor->isScheduled()) sensor->unschedule();
    sensor->detach();
  }
}

SbBool
soshape_bumprender::ensureDiffusePrograms(const cc_glglue * glue,
                                          SoState * state,
                                          diffuse_programidx & programs)
{
  const ProgramCache::Ptr cache = this->programcache;
  return cache->request(glue, state, (uint32_t) SoGLCacheContextElement::get(state),
                         true, NULL, &programs);
}

SbBool
soshape_bumprender::ensurePrograms(const cc_glglue * glue,
                                   SoState * state,
                                   spec_programidx & programs)
{
  const ProgramCache::Ptr cache = this->programcache;
  return cache->request(glue, state, (uint32_t) SoGLCacheContextElement::get(state),
                         false, &programs, NULL);
}

// These wrappers retain private symbols exported by earlier Coin releases.
// Rendering uses the result-returning helpers above so failed program uploads
// are never published or used.
void
soshape_bumprender::initPrograms(const cc_glglue * glue, SoState * state)
{
  spec_programidx programs;
  (void) this->ensurePrograms(glue, state, programs);
}

void
soshape_bumprender::initDiffusePrograms(const cc_glglue * glue, SoState * state)
{
  diffuse_programidx programs;
  (void) this->ensureDiffusePrograms(glue, state, programs);
}

// Preserve the destructor symbol for the old private cache specialization.
// The live cache stores values and does not incur the legacy allocation cost.
template SbHash<int, soshape_bumprender::diffuse_programidx *>::~SbHash();

void
soshape_bumprender::renderBumpSpecular(SoState * state,
                                       const SoPrimitiveVertexCache * cache,
                                       SoLight * light, const SbMatrix & toobjectspace)
{

  //
  // A check for fragment- and vertex-program support has already
  // been done in SoShape::shouldGLRender().
  //
  const int n = cache->getNumTriangleIndices();
  if (n == 0) return;

  const cc_glglue * glue = sogl_glue_instance(state);
  const SbColor spec = SoLazyElement::getSpecular(state);
  float shininess = SoLazyElement::getShininess(state);

  spec_programidx programs;
  if (!this->ensurePrograms(glue, state, programs)) return;

  this->initLight(light, toobjectspace);

  const SbMatrix & oldtexture0matrix = SoMultiTextureMatrixElement::get(state, 0);
  const SbMatrix & oldtexture1matrix = SoMultiTextureMatrixElement::get(state, 1);
  const SbMatrix & oldtexture2matrix = SoMultiTextureMatrixElement::get(state, 2);
  const SbMatrix & bumpmapmatrix = SoBumpMapMatrixElement::get(state);

  int lastenabled;
  const SbBool * enabled = 
    SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled); 

  state->push();
  SoMultiTextureEnabledElement::disableAll(state);
  SoGLMultiTextureEnabledElement::set(state, NULL, 0, TRUE); // enable GL_TEXTURE_2D
  
  SoGLImage * bumpimage = SoBumpMapElement::get(state);
  assert(bumpimage);
  // set up textures
  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);

  if (bumpmapmatrix != oldtexture0matrix) {
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(bumpmapmatrix[0]);
    glMatrixMode(GL_MODELVIEW);
  }
  
  bumpimage->getGLDisplayList(state)->call(state);

  // FRAGMENT: Setting up spec. colour and shininess for the fragment program
  glEnable(GL_FRAGMENT_PROGRAM_ARB);
  cc_glglue_glBindProgram(glue, GL_FRAGMENT_PROGRAM_ARB, programs.fragment);
  cc_glglue_glProgramEnvParameter4f(glue, GL_FRAGMENT_PROGRAM_ARB, 0,
                                    spec[0], spec[1], spec[2], 1.0f);

  cc_glglue_glProgramEnvParameter4f(glue, GL_FRAGMENT_PROGRAM_ARB, 1,
                                    shininess * 64, 0.0f, 0.0f, 1.0f);

  const SbViewVolume & vv = SoViewVolumeElement::get(state);
  //const SbMatrix & vm = SoViewingMatrixElement::get(state);

  SbVec3f eyepos = vv.getProjectionPoint();
  SoModelMatrixElement::get(state).inverse().multVecMatrix(eyepos, eyepos);

  // VERTEX: Setting up lightprograms
  glEnable(GL_VERTEX_PROGRAM_ARB);
  if (!this->ispointlight) {
    cc_glglue_glBindProgram(glue, GL_VERTEX_PROGRAM_ARB, programs.dirlight);
  }
  else {
    cc_glglue_glBindProgram(glue, GL_VERTEX_PROGRAM_ARB, programs.pointlight);
  }

  cc_glglue_glProgramEnvParameter4f(glue, GL_VERTEX_PROGRAM_ARB, 0,
                                    this->lightvec[0],
                                    this->lightvec[1],
                                    this->lightvec[2], 1);

  cc_glglue_glProgramEnvParameter4f(glue, GL_VERTEX_PROGRAM_ARB, 1,
                                    eyepos[0],
                                    eyepos[1],
                                    eyepos[2], 1);

  if (oldtexture2matrix != SbMatrix::identity()) {
    cc_glglue_glActiveTexture(glue, GL_TEXTURE2);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity(); // load identity texture matrix
    glMatrixMode(GL_MODELVIEW);
  }

  cc_glglue_glActiveTexture(glue, GL_TEXTURE1);
  if (oldtexture1matrix != SbMatrix::identity()) {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity(); // load identity texture matrix
    glMatrixMode(GL_MODELVIEW);
  }
  coin_apply_normalization_cube_map(glue);
  glEnable(GL_TEXTURE_CUBE_MAP);

  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);

  //const SbVec3f * cmptr = this->cubemaplist.getArrayPtr();
  const SbVec3f * tptr = this->tangentlist.getArrayPtr();
  
  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, 0,
                            cache->getVertexArray());
  cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);

  cc_glglue_glTexCoordPointer(glue, 2, GL_FLOAT, 0,
                              cache->getBumpCoordArray());
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glNormalPointer(glue, GL_FLOAT, 0,
                           cache->getNormalArray());
  cc_glglue_glEnableClientState(glue, GL_NORMAL_ARRAY);

  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE1);
  cc_glglue_glTexCoordPointer(glue, 3, GL_FLOAT, 6*sizeof(float), tptr);
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE2);
  cc_glglue_glTexCoordPointer(glue, 3, GL_FLOAT, 6*sizeof(float), (tptr + 1));
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glDrawElements(glue, GL_TRIANGLES, n, GL_UNSIGNED_INT,
                           (const GLvoid*) cache->getTriangleIndices());

  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE1);
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0);
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_NORMAL_ARRAY);

  glDisable(GL_FRAGMENT_PROGRAM_ARB);
  glDisable(GL_VERTEX_PROGRAM_ARB);
  glDisable(GL_TEXTURE_CUBE_MAP); // unit 1

  if (lastenabled >= 1 && enabled[1]) {
    // restore blend mode for texture unit 1
    SoGLMultiTextureImageElement::restore(state, 1);
  }

  if (oldtexture2matrix != SbMatrix::identity()) {
    cc_glglue_glActiveTexture(glue, GL_TEXTURE2);
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(oldtexture2matrix[0]);
    glMatrixMode(GL_MODELVIEW);
  }

  cc_glglue_glActiveTexture(glue, GL_TEXTURE1);
  glDisable(GL_TEXTURE_CUBE_MAP);
  if (oldtexture1matrix != SbMatrix::identity()) {
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(oldtexture1matrix[0]);
    glMatrixMode(GL_MODELVIEW);
  }

  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);

  if (bumpmapmatrix != oldtexture0matrix) {
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(oldtexture0matrix[0]);
    glMatrixMode(GL_MODELVIEW);
  }

  state->pop();
}


void
soshape_bumprender::renderBump(SoState * state,
                               const SoPrimitiveVertexCache * cache,
                               SoLight * light, const SbMatrix & toobjectspace)
{
  const int n = cache->getNumTriangleIndices();
  if (n == 0) return;

  this->initLight(light, toobjectspace);

  const cc_glglue * glue = sogl_glue_instance(state);
  const SbMatrix & oldtexture0matrix = SoMultiTextureMatrixElement::get(state, 0);
  const SbMatrix & oldtexture1matrix = SoMultiTextureMatrixElement::get(state, 1);
  const SbMatrix & bumpmapmatrix = SoBumpMapMatrixElement::get(state);
  
  int lastenabled;
  const SbBool * enabled = 
    SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled); 

  state->push();
  // only use vertex program if two texture units (or less) are used
  // (only two units supported in the vertex program)
  SbBool use_vertex_program = lastenabled <= 1 && SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_VERTEX_PROGRAM);
  use_vertex_program = FALSE; // FIXME: disabled until vertex program
                              // for point lights is implemented
  diffuse_programidx diffuseprograms;
  if (use_vertex_program &&
      !this->ensureDiffusePrograms(glue, state, diffuseprograms)) {
    use_vertex_program = FALSE;
  }
  if (!use_vertex_program) {
    // need to calculate tsb coordinates manually
    this->calcTSBCoords(cache, light);
  }
  SoMultiTextureEnabledElement::disableAll(state);
  SoMultiTextureEnabledElement::set(state, NULL, 0, TRUE);

  SoGLImage * bumpimage = SoBumpMapElement::get(state);
  assert(bumpimage);

  // set up textures
  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);

  if (bumpmapmatrix != oldtexture0matrix) {
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(bumpmapmatrix[0]);
    glMatrixMode(GL_MODELVIEW);
  }

  bumpimage->getGLDisplayList(state)->call(state);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

  cc_glglue_glActiveTexture(glue, GL_TEXTURE1);

  if (oldtexture1matrix != SbMatrix::identity()) {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity(); // load identity texture matrix
    glMatrixMode(GL_MODELVIEW);
  }
  coin_apply_normalization_cube_map(glue);
  glEnable(GL_TEXTURE_CUBE_MAP);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);

  const SbVec3f * cmptr = this->cubemaplist.getArrayPtr();
  const SbVec3f * tsptr = this->tangentlist.getArrayPtr();

  if (!SoGLDriverDatabase::isSupported(glue, SO_GL_VBO_IN_DISPLAYLIST)) {
    SoCacheElement::invalidate(state);
    SoGLCacheContextElement::shouldAutoCache(state, 
                                             SoGLCacheContextElement::DONT_AUTO_CACHE);
  }

  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, 0,
                            cache->getVertexArray());
  cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);
  cc_glglue_glTexCoordPointer(glue, 2, GL_FLOAT, 0,
                              cache->getBumpCoordArray());
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE1);
  if (use_vertex_program) {
    cc_glglue_glColorPointer(glue, 3, GL_FLOAT, 6*sizeof(float),
                             (tsptr + 1));
    cc_glglue_glEnableClientState(glue, GL_COLOR_ARRAY);
    cc_glglue_glTexCoordPointer(glue, 3, GL_FLOAT, 6*sizeof(float),
                                tsptr);
    cc_glglue_glNormalPointer(glue, GL_FLOAT, 0,
                              cache->getNormalArray());
    cc_glglue_glEnableClientState(glue, GL_NORMAL_ARRAY);
  }
  else {
    cc_glglue_glTexCoordPointer(glue, 3, GL_FLOAT, 0,
                                cmptr);
  }
  cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);

  if (use_vertex_program) {
    glEnable(GL_VERTEX_PROGRAM_ARB);
    if (!this->ispointlight) {
      cc_glglue_glBindProgram(glue, GL_VERTEX_PROGRAM_ARB,
                              diffuseprograms.dirlight);
    }
    else {
      assert(0);
    }
    cc_glglue_glProgramEnvParameter4f(glue, GL_VERTEX_PROGRAM_ARB, 0,
                                      this->lightvec[0],
                                      this->lightvec[1],
                                      this->lightvec[2], 1);
  }

  cc_glglue_glDrawElements(glue, GL_TRIANGLES, n, GL_UNSIGNED_INT,
                           (const GLvoid*) cache->getTriangleIndices());

  if (use_vertex_program) {
    cc_glglue_glDisableClientState(glue, GL_NORMAL_ARRAY);
    cc_glglue_glDisableClientState(glue, GL_COLOR_ARRAY);
    glDisable(GL_VERTEX_PROGRAM_ARB);
  }
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0);
  cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);

  glDisable(GL_TEXTURE_CUBE_MAP); // unit 1

  if (lastenabled >= 1 && enabled[1]) {
    // restore blend mode for texture unit 1
    SoGLMultiTextureImageElement::restore(state, 1);
  }

  if (oldtexture1matrix != SbMatrix::identity()) {
    cc_glglue_glActiveTexture(glue, GL_TEXTURE1);
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(oldtexture1matrix[0]);
    glMatrixMode(GL_MODELVIEW);
  }

  cc_glglue_glActiveTexture(glue, GL_TEXTURE0);

  if (bumpmapmatrix != oldtexture0matrix) {
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(oldtexture0matrix[0]);
    glMatrixMode(GL_MODELVIEW);
  }
  state->pop();
}

void
soshape_bumprender::renderNormal(SoState * state, const SoPrimitiveVertexCache * cache)
{
  const cc_glglue * glue = sogl_glue_instance(state);
  int lastenabled = -1;
  //const SbBool * enabled = SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);

  // only use vertex program if two texture units (or less) are used
  // (only two units supported in the vertex program)
  SbBool use_vertex_program = lastenabled <= 1 && SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_VERTEX_PROGRAM);
  use_vertex_program = FALSE; // FIXME: disabled until vertex program
                              // for point lights is implemented
  diffuse_programidx diffuseprograms;
  if (use_vertex_program &&
      !this->ensureDiffusePrograms(glue, state, diffuseprograms)) {
    use_vertex_program = FALSE;
  }
  if (use_vertex_program) {
    glEnable(GL_VERTEX_PROGRAM_ARB);
    cc_glglue_glBindProgram(glue, GL_VERTEX_PROGRAM_ARB,
                            diffuseprograms.normalrendering);
  }

  int arrays =
    SoPrimitiveVertexCache::TEXCOORD|
    SoPrimitiveVertexCache::COLOR;
  cache->renderTriangles(state, arrays);

  if (use_vertex_program) {
    glDisable(GL_VERTEX_PROGRAM_ARB);
  }
}

void
soshape_bumprender::calcTangentSpace(const SoPrimitiveVertexCache * cache)
{
  int i;
  const int numi = cache->getNumTriangleIndices();
  if (numi == 0) return;


  const int numv = cache->getNumVertices();
  const GLint * idxptr = cache->getTriangleIndices();
  const SbVec3f * vertices = cache->getVertexArray();
  //const SbVec3f * normals = cache->getNormalArray();
  const SbVec2f * bumpcoords = cache->getBumpCoordArray();

  this->tangentlist.truncate(0);
  this->tangentlist.ensureCapacity(numv * 2);

  for (i = 0; i < numv; i++) {
    this->tangentlist.append(SbVec3f(0.0f, 0.0f, 0.0f));
    this->tangentlist.append(SbVec3f(0.0f, 0.0f, 0.0f));
  }

  SbVec3f sTangent;
  SbVec3f tTangent;

  int idx[3];

  for (i = 0; i < numi; i += 3) {
    idx[0] = idxptr[i];
    idx[1] = idxptr[i+1];
    idx[2] = idxptr[i+2];

    SbVec3f side0 = vertices[idx[1]] - vertices[idx[0]];
    SbVec3f side1 = vertices[idx[2]] - vertices[idx[0]];

    float deltaT0 = bumpcoords[idx[1]][1] - bumpcoords[idx[0]][1];
    float deltaT1 = bumpcoords[idx[2]][1] - bumpcoords[idx[0]][1];
    sTangent = deltaT1 * side0 - deltaT0 * side1;
    NORMALIZE(sTangent);

    float deltaS0 = bumpcoords[idx[1]][0] - bumpcoords[idx[0]][0];
    float deltaS1 = bumpcoords[idx[2]][0] - bumpcoords[idx[0]][0];
    tTangent = deltaS1 * side0 - deltaS0 * side1;
    NORMALIZE(tTangent);

    for (int j = 0; j < 3; j++) {
      this->tangentlist[idx[j]*2] += sTangent;
      this->tangentlist[idx[j]*2+1] += tTangent;
    }
  }
  for (i = 0; i < numv; i++) {
    NORMALIZE(this->tangentlist[i*2]);
    NORMALIZE(this->tangentlist[i*2+1]);
  }
}

void
soshape_bumprender::calcTSBCoords(const SoPrimitiveVertexCache * cache, SoLight * COIN_UNUSED_ARG(light))
{
  SbVec3f thelightvec;
  SbVec3f tlightvec;

  const int numv = cache->getNumVertices();
  const SbVec3f * vertices = cache->getVertexArray();
  const SbVec3f * normals = cache->getNormalArray();

  this->cubemaplist.truncate(0);
  for (int i = 0; i < numv; i++) {
    SbVec3f sTangent = this->tangentlist[i*2];
    SbVec3f tTangent = this->tangentlist[i*2+1];
    thelightvec = this->getLightVec(vertices[i]);
    tlightvec = thelightvec;
#if 0 // FIXME: I don't think it's necessary to do this test. pederb, 2003-11-20
    SbVec3f tcross = tTangent.cross(sTangent);
    if (tcross.dot(normals[i]) < 0.0f) {
      tlightvec = -tlightvec;      
    }
#endif // disabled, probably not necessary
    this->cubemaplist.append(SbVec3f(sTangent.dot(tlightvec),
                                     tTangent.dot(tlightvec),
                                     normals[i].dot(thelightvec)));

  }
}

void
soshape_bumprender::initLight(SoLight * light, const SbMatrix & m)
{
  if (light->isOfType(SoPointLight::getClassTypeId())) {
    SoPointLight * pl = (SoPointLight*) light;
    this->lightvec = pl->location.getValue();
    m.multVecMatrix(this->lightvec, this->lightvec);
    this->ispointlight = TRUE;
  }
  else if (light->isOfType(SoDirectionalLight::getClassTypeId())) {
    SoDirectionalLight * dir = (SoDirectionalLight*)light;
    m.multDirMatrix(-(dir->direction.getValue()), this->lightvec);
    this->ispointlight = FALSE;
    NORMALIZE(this->lightvec);
  }
  else if (light->isOfType(SoSpotLight::getClassTypeId())) {
    SoSpotLight * pl = (SoSpotLight*) light;
    this->lightvec = pl->location.getValue();
    m.multVecMatrix(this->lightvec, this->lightvec);
    this->ispointlight = TRUE;

  }
  else {
    this->lightvec = SbVec3f(0.0f, 0.0f, 1.0f);
    this->ispointlight = FALSE;

  }
}


SbVec3f
soshape_bumprender::getLightVec(const SbVec3f & v) const
{
  if (this->ispointlight) {
    SbVec3f tmp = lightvec - v;
    NORMALIZE(tmp);
    return tmp;
  }
  else return this->lightvec;
}
