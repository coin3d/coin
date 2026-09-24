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

static SbBool
soshape_bumprender_load_program(const cc_glglue * glue,
                                const GLenum target,
                                const char * source,
                                const char * description,
                                GLuint & program)
{
  // Isolate errors from this upload. The old implementation consumed one
  // error after ProgramString too, but using a sticky error as a publication
  // gate would otherwise reject a valid program because of an earlier call.
  while (glGetError() != GL_NO_ERROR) { }

  GLint previousprogram = 0;
  cc_glglue_glGetProgramiv(glue, target, GL_PROGRAM_BINDING_ARB,
                           &previousprogram);
  const GLenum bindingqueryerr = glGetError();

  program = 0;
  cc_glglue_glGenPrograms(glue, 1, &program);
  cc_glglue_glBindProgram(glue, target, program);
  cc_glglue_glProgramString(glue, target, GL_PROGRAM_FORMAT_ASCII_ARB,
                            (GLsizei) strlen(source), source);

  const GLenum uploaderr = glGetError();
  GLint errorpos = -1;
  GLint programlength = 0;
  glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorpos);
  cc_glglue_glGetProgramiv(glue, target, GL_PROGRAM_LENGTH_ARB, &programlength);
  const GLenum queryerr = glGetError();

  // Program creation is transactional with respect to both the cache and the
  // caller's GL state. In particular, rollback should not discard a program
  // that was bound before this helper was entered.
  cc_glglue_glBindProgram(glue, target, (GLuint) previousprogram);
  const GLenum restoreerr = glGetError();

  const GLenum err = bindingqueryerr != GL_NO_ERROR ? bindingqueryerr :
    (uploaderr != GL_NO_ERROR ? uploaderr :
     (queryerr != GL_NO_ERROR ? queryerr : restoreerr));

  if (err != GL_NO_ERROR || program == 0 || programlength <= 0) {
    const GLubyte * errorstring = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
    SoDebugError::postWarning("soshape_bumprender::ensurePrograms",
                              "Error in %s! (GL error: 0x%x, byte pos: %d) '%s'.\n",
                              description, (unsigned int) err, errorpos,
                              errorstring ? (const char *) errorstring : "unknown error");
    return FALSE;
  }
  return TRUE;
}

soshape_bumprender::soshape_bumprender(void)
{
  SoContextHandler::addContextDestructionCallback(context_destruction_cb, this);
}

soshape_bumprender::~soshape_bumprender()
{
  SoContextHandler::removeContextDestructionCallback(context_destruction_cb, this);

  for (ContextId2SpecStruct::const_iterator iter =
         this->specularprogramdict.const_begin();
       iter != this->specularprogramdict.const_end(); ++iter) {
    const GLuint programs[] = {
      iter->obj.fragment, iter->obj.dirlight, iter->obj.pointlight
    };
    for (int i = 0; i < 3; i++) {
      if (programs[i] != 0) {
        void * closure = (void *) ((uintptr_t) programs[i]);
        SoGLCacheContextElement::scheduleDeleteCallback(iter->key,
                                                        delete_program_cb,
                                                        closure);
      }
    }
  }
  this->specularprogramdict.clear();

  for (ContextId2DiffuseStruct::const_iterator iter =
         this->diffuseprogramdict.const_begin();
       iter != this->diffuseprogramdict.const_end(); ++iter) {
    const GLuint programs[] = {
      iter->obj.dirlight, iter->obj.pointlight, iter->obj.normalrendering
    };
    for (int i = 0; i < 3; i++) {
      if (programs[i] != 0) {
        void * closure = (void *) ((uintptr_t) programs[i]);
        SoGLCacheContextElement::scheduleDeleteCallback(iter->key,
                                                        delete_program_cb,
                                                        closure);
      }
    }
  }
  this->diffuseprogramdict.clear();
}

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
  soshape_bumprender * thisp = (soshape_bumprender *) userdata;
  spec_programidx specularprograms;
  diffuse_programidx diffuseprograms;
  const SbBool havespecular =
    thisp->specularprogramdict.get(contextid, specularprograms);
  const SbBool havediffuse =
    thisp->diffuseprogramdict.get(contextid, diffuseprograms);

  if (!havespecular && !havediffuse) return;

  const cc_glglue * glue = cc_glglue_instance((int) contextid);
  if (havespecular) {
    const GLuint programs[] = {
      specularprograms.fragment,
      specularprograms.dirlight,
      specularprograms.pointlight
    };
    soshape_bumprender_delete_programs(glue, programs, 3);
    (void) thisp->specularprogramdict.erase(contextid);
  }
  if (havediffuse) {
    const GLuint programs[] = {
      diffuseprograms.dirlight,
      diffuseprograms.pointlight,
      diffuseprograms.normalrendering
    };
    soshape_bumprender_delete_programs(glue, programs, 3);
    (void) thisp->diffuseprogramdict.erase(contextid);
  }
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

SbBool
soshape_bumprender::ensureDiffusePrograms(const cc_glglue * glue,
                                          SoState * state,
                                          diffuse_programidx & programs)
{
  const uint32_t contextid =
    (uint32_t) SoGLCacheContextElement::get(state);
  if (this->diffuseprogramdict.get(contextid, programs)) return TRUE;

  programs.pointlight = 0; // Pointlight diffuse rendering is not implemented.
  programs.dirlight = 0;
  programs.normalrendering = 0;

  if (!soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                       diffusebumpdirlightvpprogram,
                                       "diffuse directional light vertex program",
                                       programs.dirlight) ||
      !soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                       normalrenderingvpprogram,
                                       "normal rendering vertex program",
                                       programs.normalrendering)) {
    const GLuint ids[] = {
      programs.dirlight, programs.pointlight, programs.normalrendering
    };
    soshape_bumprender_delete_programs(glue, ids, 3);
    programs.dirlight = programs.pointlight = programs.normalrendering = 0;
    return FALSE;
  }

  (void) this->diffuseprogramdict.put(contextid, programs);
  return TRUE;
}

SbBool
soshape_bumprender::ensurePrograms(const cc_glglue * glue,
                                   SoState * state,
                                   spec_programidx & programs)
{
  const uint32_t contextid =
    (uint32_t) SoGLCacheContextElement::get(state);
  if (this->specularprogramdict.get(contextid, programs)) return TRUE;

  programs.fragment = 0;
  programs.dirlight = 0;
  programs.pointlight = 0;

  if (!soshape_bumprender_load_program(glue, GL_FRAGMENT_PROGRAM_ARB,
                                       bumpspecfpprogram,
                                       "fragment program",
                                       programs.fragment) ||
      !soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                       directionallightvpprogram,
                                       "directional light vertex program",
                                       programs.dirlight) ||
      !soshape_bumprender_load_program(glue, GL_VERTEX_PROGRAM_ARB,
                                       pointlightvpprogram,
                                       "point light vertex program",
                                       programs.pointlight)) {
    const GLuint ids[] = {
      programs.fragment, programs.dirlight, programs.pointlight
    };
    soshape_bumprender_delete_programs(glue, ids, 3);
    programs.fragment = programs.dirlight = programs.pointlight = 0;
    return FALSE;
  }

  (void) this->specularprogramdict.put(contextid, programs);
  return TRUE;
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
