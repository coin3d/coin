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
#include <cstring>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <utility>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/C/glue/gl.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec2f.h>
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
#include <Inventor/elements/SoViewVolumeElement.h>
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
#include "misc/SbHash.h" // Explicit legacy destructor specialization below.
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
  soshape_bump_program_error() : description(NULL), stage(NULL), error(GL_NO_ERROR), position(-1) {
    this->message[0] = '\0';
  }
  const char * description;
  const char * stage;
  GLenum error;
  GLint position;
  // Most contexts never fail. Retain a short diagnostic inline and allocate
  // the full driver message only after GL rollback has completed.
  enum { MESSAGE_CAPACITY = 512, INLINE_CAPACITY = 64 };
  struct LongMessage { char text[MESSAGE_CAPACITY]; };
  std::shared_ptr<LongMessage> longmessage;
  char message[INLINE_CAPACITY];
  static bool copyMessage(char * destination, size_t capacity, const char * text) {
    size_t i = 0;
    while (i < capacity - 1 && text && text[i]) {
      destination[i] = text[i];
      ++i;
    }
    destination[i] = '\0';
    const bool truncated = text && text[i];
    if (truncated) {
      destination[capacity - 4] = '.';
      destination[capacity - 3] = '.';
      destination[capacity - 2] = '.';
    }
    return truncated;
  }
  void setMessage(const char * text) {
    this->longmessage.reset();
    const bool truncated = copyMessage(this->message, INLINE_CAPACITY, text);
    if (truncated) {
      try {
        std::shared_ptr<LongMessage> full = std::make_shared<LongMessage>();
        copyMessage(full->text, MESSAGE_CAPACITY, text);
        this->longmessage = full;
      }
      catch (const std::bad_alloc &) {
        // Preserve the GL error and a bounded diagnostic under memory pressure.
      }
    }
  }
  const char * messageText() const {
    return this->longmessage ? this->longmessage->text : this->message;
  }
  void clearMessage() {
    this->longmessage.reset();
    this->message[0] = '\0';
  }
  void report() const {
    if (this->description) {
      SoDebugError::postWarning("soshape_bumprender::ensurePrograms",
                                "Error in %s during %s! (GL error: 0x%x, byte pos: %d) '%s'.\n",
                                this->description, this->stage, (unsigned int) this->error,
                                this->position, this->messageText());
    }
  }
};

static SbBool
soshape_bumprender_load_program(const cc_glglue * glue,
                                const GLenum target,
                                const char * source,
                                const char * description,
                                GLuint & program,
                                soshape_bump_program_error & failure,
                                char (&message)[soshape_bump_program_error::MESSAGE_CAPACITY])
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
    soshape_bump_program_error::copyMessage(message, sizeof(message),
                                            (const char *) errorstring);
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
    Context() : specstatus(EMPTY), diffusestatus(EMPTY), specpooled(false), diffusepooled(false) {
      spec.fragment = spec.dirlight = spec.pointlight = 0;
      diffuse.dirlight = diffuse.pointlight = diffuse.normalrendering = 0;
    }
    InitStatus specstatus, diffusestatus;
    bool specpooled, diffusepooled;
    spec_programidx spec;
    diffuse_programidx diffuse;
    soshape_bump_program_error specerror, diffuseerror;
  };
  typedef std::map<uint32_t, Context> Contexts;
  // Only validated GL program names are shared. Renderer metadata and errors
  // remain in Context, with their original callback and sensor ownership.
  struct SharedPrograms {
    struct Entry {
      Entry() : references(0) {
        spec.fragment = spec.dirlight = spec.pointlight = 0;
        diffuse.dirlight = diffuse.pointlight = diffuse.normalrendering = 0;
      }
      size_t references;
      spec_programidx spec;
      diffuse_programidx diffuse;
    };
    typedef std::pair<uint32_t, bool> Key; // context id, diffuse family
    std::mutex mutex;
    std::map<Key, Entry> entries;
  };
  static SharedPrograms & sharedPrograms() {
    static SharedPrograms pool;
    return pool;
  }
  typedef std::shared_ptr<ProgramCache> Ptr;
  typedef std::map<uintptr_t, Ptr> Entries;
  struct Registry {
    Registry() : nexttoken(0), cleanupregistered(false), exhaustionreported(false) { }
    std::mutex mutex;
    Entries entries;
    uintptr_t nexttoken;
    bool cleanupregistered;
    bool exhaustionreported;
  };
  struct RedrawSensor : SoNodeSensor {
    explicit RedrawSensor(uintptr_t token) : SoNodeSensor(redraw_cb, (void *) token) {
      // Never notify priority-zero viewers from inside display-list traversal.
      this->setPriority(100);
    }
    void trigger() override {
      // A notification handler may destroy the renderer. Keep its sensor alive
      // until SoDataSensor::trigger() has finished its post-callback cleanup.
      const std::shared_ptr<RedrawSensor> sensor = this->self.lock();
      if (!sensor) return;
      SoNodeSensor::trigger();
    }
    std::weak_ptr<RedrawSensor> self;
  };
  typedef std::shared_ptr<RedrawSensor> RedrawPtr;
  typedef std::map<SoNode *, RedrawPtr> Redraws;
  ProgramCache() : token(0), alive(true), contextallocationfailed(false) { }
  std::mutex mutex;
  uintptr_t token;
  bool alive;
  bool contextallocationfailed;
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
      // Release our reference even if an application notification throws.
      // This only rolls back our ownership; it does not recover that handler.
      struct Unref {
        void operator()(SoNode * node) const { node->unref(); }
      };
      const std::unique_ptr<SoNode, Unref> reference(root);
      root->touch();
    }
  }

  static Registry & registry() {
    static Registry reg;
    return reg;
  }
  static Ptr create() {
    Registry * registry;
    try {
      // An empty std::map may allocate a sentinel (notably with MSVC).
      registry = &ProgramCache::registry();
    }
    catch (const std::bad_alloc &) {
      return Ptr(); // Failed static initialization can retry on the next call.
    }
    Registry & reg = *registry;
    std::unique_lock<std::mutex> lock(reg.mutex);
    // The low bit is reserved for the deferred initialization's program kind.
    if (reg.nexttoken >= (std::numeric_limits<uintptr_t>::max() >> 1)) {
      // Keep exhausted tokens unavailable in release builds too. Error
      // handlers may inspect the registry, so report after releasing its lock.
      // Record the diagnostic first, preventing recursive/repeated warnings.
      const bool report = !reg.exhaustionreported;
      reg.exhaustionreported = true;
      lock.unlock();
      if (report) {
        SoDebugError::postWarning("soshape_bumprender::ProgramCache::create",
                                  "Bump program callback tokens exhausted; program cache unavailable.");
      }
      return Ptr();
    }
    Ptr cache;
    try {
      cache = std::make_shared<ProgramCache>();
    }
    catch (const std::bad_alloc &) {
      return Ptr(); // No resource or callback has been published.
    }
    cache->token = ++reg.nexttoken;
    // Registration belongs to Coin, not to our host-allocation guard. Do it
    // before publishing an entry, so its exception cannot orphan our cache.
    if (!reg.cleanupregistered) {
      coin_atexit(cleanup_registry, CC_ATEXIT_NORMAL);
      reg.cleanupregistered = true;
    }
    try {
      reg.entries.insert(std::make_pair(cache->token, cache));
    }
    catch (const std::bad_alloc &) {
      return Ptr(); // Never roll back/reuse the consumed callback token.
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
    Registry & reg = registry();
    uintptr_t lasttoken;
    {
      std::lock_guard<std::mutex> lock(reg.mutex);
      lasttoken = reg.nexttoken;
      reg.cleanupregistered = false;
      // Do not reuse tokens or repeat exhaustion diagnostics after SoDB::init().
    }
    // Drain the old identities without constructing an allocating empty map.
    // New registrations made by a reentrant provider have larger tokens and
    // belong to the next cleanup registration, not to this snapshot.
    for (;;) {
      Ptr cache;
      {
        std::lock_guard<std::mutex> lock(reg.mutex);
        Entries::iterator it = reg.entries.begin();
        if (it == reg.entries.end() || it->first > lasttoken) break;
        cache = it->second;
        reg.entries.erase(it);
      }
      SoContextHandler::removeContextDestructionCallback(context_destruction_cb,
                                                         (void *) cache->token);
    }
  }
  // Caller holds mutex. The entry exists before allocating GL names, so
  // publishing a successfully uploaded set cannot allocate host memory.
  static void initializeUnshared(const cc_glglue * glue, Context & ctx, bool diffuse) {
    // Capture the driver's text before GL cleanup, then store it after cleanup.
    // No host allocation may interrupt rollback of generated program names.
    char message[soshape_bump_program_error::MESSAGE_CAPACITY];
    if (diffuse) {
      diffuse_programidx & p = ctx.diffuse;
      if (soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         diffusebumpdirlightvpprogram,
                                         "diffuse directional light vertex program", p.dirlight, ctx.diffuseerror, message) &&
          soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         normalrenderingvpprogram,
                                         "normal rendering vertex program", p.normalrendering, ctx.diffuseerror, message)) {
        ctx.diffusestatus = READY;
      }
      else {
        const GLuint ids[] = { p.dirlight, p.pointlight, p.normalrendering };
        soshape_bumprender_delete_programs(glue, ids, 3);
        p.dirlight = p.pointlight = p.normalrendering = 0;
        ctx.diffusestatus = FAILED;
        ctx.diffuseerror.setMessage(message);
      }
    }
    else {
      spec_programidx & p = ctx.spec;
      if (soshape_bumprender_load_program(glue, GL_FRAGMENT_PROGRAM_ARB,
                                         bumpspecfpprogram, "fragment program", p.fragment, ctx.specerror, message) &&
          soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         directionallightvpprogram,
                                         "directional light vertex program", p.dirlight, ctx.specerror, message) &&
          soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                         pointlightvpprogram,
                                         "point light vertex program", p.pointlight, ctx.specerror, message)) {
        ctx.specstatus = READY;
      }
      else {
        const GLuint ids[] = { p.fragment, p.dirlight, p.pointlight };
        soshape_bumprender_delete_programs(glue, ids, 3);
        p.fragment = p.dirlight = p.pointlight = 0;
        ctx.specstatus = FAILED;
        ctx.specerror.setMessage(message);
      }
    }
  }
  static void initialize(const cc_glglue * glue, Context & ctx,
                         bool diffuse, uint32_t contextid) {
    SharedPrograms * pool;
    try { pool = &sharedPrograms(); }
    catch (const std::bad_alloc &) {
      initializeUnshared(glue, ctx, diffuse);
      return;
    }
    std::unique_lock<std::mutex> lock(pool->mutex);
    const SharedPrograms::Key key(contextid, diffuse);
    std::map<SharedPrograms::Key, SharedPrograms::Entry>::iterator it = pool->entries.find(key);
    if (it != pool->entries.end()) {
      ++it->second.references;
      if (diffuse) {
        ctx.diffuse = it->second.diffuse;
        ctx.diffusestatus = READY;
        ctx.diffusepooled = true;
      }
      else {
        ctx.spec = it->second.spec;
        ctx.specstatus = READY;
        ctx.specpooled = true;
      }
      return;
    }
    try { it = pool->entries.insert(std::make_pair(key, SharedPrograms::Entry())).first; }
    catch (const std::bad_alloc &) {
      lock.unlock();
      initializeUnshared(glue, ctx, diffuse);
      return;
    }
    // Serialize only this first upload; the map node already exists, so
    // publishing the validated names cannot allocate after GL operations.
    try { initializeUnshared(glue, ctx, diffuse); }
    catch (...) { pool->entries.erase(it); throw; }
    if ((diffuse ? ctx.diffusestatus : ctx.specstatus) == READY) {
      it->second.references = 1;
      if (diffuse) { it->second.diffuse = ctx.diffuse; ctx.diffusepooled = true; }
      else { it->second.spec = ctx.spec; ctx.specpooled = true; }
    }
    else pool->entries.erase(it); // Do not share a failure or its diagnostic.
  }

  static void release(const cc_glglue * glue, uint32_t contextid,
                      bool diffuse, const Context & ctx) {
    const bool pooled = diffuse ? ctx.diffusepooled : ctx.specpooled;
    if (!pooled) {
      if (diffuse) {
        const GLuint ids[] = { ctx.diffuse.dirlight, ctx.diffuse.pointlight, ctx.diffuse.normalrendering };
        soshape_bumprender_delete_programs(glue, ids, 3);
      }
      else {
        const GLuint ids[] = { ctx.spec.fragment, ctx.spec.dirlight, ctx.spec.pointlight };
        soshape_bumprender_delete_programs(glue, ids, 3);
      }
      return;
    }
    SharedPrograms & pool = sharedPrograms();
    std::unique_lock<std::mutex> lock(pool.mutex);
    const SharedPrograms::Key key(contextid, diffuse);
    std::map<SharedPrograms::Key, SharedPrograms::Entry>::iterator it = pool.entries.find(key);
    // A stale cleanup callback may outlive the last owner of this context.
    if (it == pool.entries.end() || it->second.references == 0) return;
    if (--it->second.references != 0) return;
    const spec_programidx spec = it->second.spec;
    const diffuse_programidx diff = it->second.diffuse;
    pool.entries.erase(it);
    lock.unlock();
    if (diffuse) {
      const GLuint ids[] = { diff.dirlight, diff.pointlight, diff.normalrendering };
      soshape_bumprender_delete_programs(glue, ids, 3);
    }
    else {
      const GLuint ids[] = { spec.fragment, spec.dirlight, spec.pointlight };
      soshape_bumprender_delete_programs(glue, ids, 3);
    }
  }

  SbBool request(const cc_glglue * glue, SoState * state, uint32_t contextid,
                 bool diffuse, spec_programidx * spec, diffuse_programidx * diff) {
    std::unique_lock<std::mutex> lock(this->mutex);
    if (!this->alive) return FALSE;
    Contexts::iterator context = this->contexts.find(contextid);
    if (context == this->contexts.end()) {
      if (this->contextallocationfailed) return FALSE;
      try {
        context = this->contexts.insert(std::make_pair(contextid, Context())).first;
      }
      catch (const std::bad_alloc &) {
        // No GL operation has started. Keep existing contexts usable, but
        // refuse new metadata until a destruction notification allows retry.
        // The scalar latch cannot itself allocate and suppresses retry storms.
        this->contextallocationfailed = true;
        soshape_bump_program_error failure;
        failure.description = diffuse ? "diffuse program set" : "specular program set";
        failure.stage = "allocate context metadata";
        failure.error = GL_OUT_OF_MEMORY;
        lock.unlock();
        failure.report(); // External handler exceptions are not swallowed.
        return FALSE;
      }
    }
    Context & ctx = context->second;
    InitStatus & status = diffuse ? ctx.diffusestatus : ctx.specstatus;
    bool schedule = false;
    if (status == EMPTY) {
      GLint list = 0;
      glGetIntegerv(GL_LIST_INDEX, &list);
      if (list != 0) {
        status = PENDING;
        schedule = true;
      }
      else initialize(glue, ctx, diffuse, contextid);
    }
    const InitStatus result = status;
    if (result == READY) {
      if (spec) *spec = ctx.spec;
      if (diff) *diff = ctx.diffuse;
      return TRUE; // No diagnostic buffer is needed on the successful path.
    }
    soshape_bump_program_error failure;
    if (result == FAILED) {
      soshape_bump_program_error & cached = diffuse ? ctx.diffuseerror : ctx.specerror;
      failure = cached;
      cached.description = NULL; // Report at most once, on use by a renderer.
      cached.clearMessage();
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
        try {
          SoGLCacheContextElement::scheduleDeleteCallback(contextid,
                                                          initialize_program_cb,
                                                          (void *) closure);
        }
        catch (const std::bad_alloc &) {
          // No queued work may own this PENDING state. Fail this family closed
          // until context destruction, just like an allocation/upload failure.
          lock.lock();
          Contexts::iterator it = this->contexts.find(contextid);
          if (this->alive && it != this->contexts.end()) {
            InitStatus & current = diffuse ? it->second.diffusestatus : it->second.specstatus;
            if (current == PENDING) {
              current = FAILED;
              soshape_bump_program_error & cached = diffuse ? it->second.diffuseerror : it->second.specerror;
              cached.description = diffuse ? "diffuse program set" : "specular program set";
              cached.stage = "schedule initialization";
              cached.error = GL_OUT_OF_MEMORY;
              failure = cached;
              cached.description = NULL;
            }
          }
          lock.unlock();
          failure.report();
        }
      }
    }
    return FALSE;
  }
};

soshape_bumprender::soshape_bumprender(void)
  : programcache(ProgramCache::create())
{
  if (!this->programcache) return;
  try {
    SoContextHandler::addContextDestructionCallback(context_destruction_cb,
                                                     (void *) this->programcache->token);
  }
  catch (...) {
    // Roll back our entry, not the provider's possibly partial registration.
    // A retained callback can only look up the now-absent, non-reused token.
    ProgramCache::Registry & reg = ProgramCache::registry();
    std::lock_guard<std::mutex> lock(reg.mutex);
    reg.entries.erase(this->programcache->token);
    throw; // Provider failures are not reported as successful local recovery.
  }
}

soshape_bumprender::~soshape_bumprender()
{
  const ProgramCache::Ptr cache = this->programcache;
  if (!cache) return;
  {
    std::lock_guard<std::mutex> lock(cache->mutex);
    cache->alive = false;
    for (ProgramCache::Contexts::iterator it = cache->contexts.begin();
         it != cache->contexts.end();) {
      if (it->second.specstatus == ProgramCache::READY ||
          it->second.diffusestatus == ProgramCache::READY) ++it;
      else cache->contexts.erase(it++);
    }
  }
  // Sensor queue changes can invoke application callbacks; never cancel under
  // the cache mutex. Transfer one shared owner at a time: an empty temporary
  // std::map is not allocation-free on all STL implementations.
  for (;;) {
    ProgramCache::RedrawPtr sensor;
    {
      std::lock_guard<std::mutex> lock(cache->mutex);
      ProgramCache::Redraws::iterator it = cache->redraws.begin();
      if (it == cache->redraws.end()) break;
      sensor = it->second;
      cache->redraws.erase(it);
    }
    // This sensor, and all sensors still owned by the cache, remain alive
    // through any reentrant application queue-change handler.
    if (sensor->isScheduled()) sensor->unschedule();
    sensor->detach();
  }
  // Never take the scheduler's mutex while holding the cache mutex: cleanup
  // invokes deferred callbacks under its own lock. Queued callbacks contain
  // tokens, not program names or pointers to this dead renderer.
  // Visit numeric keys, not iterators retained across reentrant callbacks, and
  // do not allocate a temporary context list inside this noexcept destructor.
  bool first = true;
  uint32_t previous = 0;
  for (;;) {
    uint32_t contextid;
    {
      std::lock_guard<std::mutex> lock(cache->mutex);
      ProgramCache::Contexts::const_iterator it = first ? cache->contexts.begin() :
        cache->contexts.upper_bound(previous);
      if (it == cache->contexts.end()) break;
      contextid = it->first;
    }
    first = false;
    previous = contextid;
    try {
      SoGLCacheContextElement::scheduleDeleteCallback(contextid, cleanup_program_cb,
                                                      (void *) cache->token);
    }
    catch (const std::bad_alloc &) {
      // The registry and existing destruction callback still own these names.
      // Leave them there until this context is destroyed; never unwind here.
    }
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
    cache->contextallocationfailed = false;
    ProgramCache::Contexts::iterator it = cache->contexts.find(contextid);
    if (it == cache->contexts.end()) return;
    ctx = it->second;
    cache->contexts.erase(it);
  }
  // No renderer access, and no cache lock across GL/glue callbacks.
  if (ctx.specstatus == ProgramCache::READY || ctx.diffusestatus == ProgramCache::READY) {
    const cc_glglue * glue = cc_glglue_instance((int) contextid);
    if (ctx.specstatus == ProgramCache::READY) ProgramCache::release(glue, contextid, false, ctx);
    if (ctx.diffusestatus == ProgramCache::READY) ProgramCache::release(glue, contextid, true, ctx);
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
  else ProgramCache::initialize(glue, it->second, diffuse, contextid);
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
  if (!cache) return;
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
    ProgramCache::Redraws::iterator redraw = cache->redraws.find(root);
    if (redraw == cache->redraws.end()) {
      try {
        ProgramCache::RedrawPtr candidate =
          std::make_shared<ProgramCache::RedrawSensor>(cache->token);
        candidate->self = candidate; // weak lock cannot throw bad_weak_ptr.
        redraw = cache->redraws.insert(std::make_pair(root, candidate)).first;
      }
      catch (const std::bad_alloc &) {
        // Best-effort redraw only: no empty entry, attachment or queued sensor
        // remains. Deferred GL initialization still owns its separate callback;
        // a later traversal can retry notification or use the validated set.
        return;
      }
    }
    sensor = redraw->second;
    // These dependency operations are deliberately outside the allocation
    // guard: their queues/auditors and application callbacks own their contract.
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
  if (!cache) return FALSE;
  return cache->request(glue, state, (uint32_t) SoGLCacheContextElement::get(state),
                         true, NULL, &programs);
}

SbBool
soshape_bumprender::ensurePrograms(const cc_glglue * glue,
                                   SoState * state,
                                   spec_programidx & programs)
{
  const ProgramCache::Ptr cache = this->programcache;
  if (!cache) return FALSE;
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
  // Reserve at the list's usual doubling threshold. Exact reservation would
  // reallocate for every small increase in a changing vertex cache.
  int capacity = 1;
  while (capacity < numv) {
    if (capacity > std::numeric_limits<int>::max() / 2) throw std::bad_alloc();
    capacity *= 2;
  }
  this->cubemaplist.ensureCapacity(capacity);
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
