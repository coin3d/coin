/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/base/hash.h>
#include <Inventor/C/errors/debugerror.h>

#ifdef HAVE_AGL_AGL_H
#include <AGL/AGL.h>
#endif /* HAVE_AGL_AGL_H */

/* FIXME: temporary fix awaiting configure-check for this feature
   (kintel 20011125) */
#ifndef __APPLE__
#define COIN_OPENGL_DYNAMIC_BINDING
#endif

#include <Inventor/C/base/hash.h>

#include <../tidbits.h> /*  coin_atexit() */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <Inventor/C/threads/threadsutilp.h>

#ifdef HAVE_DLFCN_H
#include <dlfcn.h> /* Programming interface to libdl. */
#endif /* HAVE_DLFCN_H */

#include "gl_wgl.h"
#include "gl_glx.h"

static cc_libhandle glglue_self_handle = NULL;
static SbBool glglue_tried_open_self = FALSE;

/* Return value of COIN_DEBUG_GLGLUE environment variable. */
int
coin_glglue_debug(void)
{
  static int d = -1;
  if (d == -1) {
    const char * debuggl = coin_getenv("COIN_DEBUG_GLGLUE");
    d = debuggl ? atoi(debuggl) : 0;
  }

  return (d > 0) ? 1 : 0;
}

/*
   Define GLGLUE_REGISTER_FUNC macro. Casting the type is
   necessary for this file to be compatible with C++ compilers.
*/
#define GLGLUE_REGISTER_FUNC(_wrapname_, _funcname_, _funcsig_) \
  gi->_wrapname_ = (_funcsig_)glglue_getprocaddress(SO__QUOTE(_funcname_))

/*
  Returns address of the symbol (usually a function) named by
  "symname".
 */
static void *
glglue_getprocaddress(const char * symname)
{
  void * ptr = NULL;

  ptr = coin_wgl_getprocaddress(symname);
  if (ptr) goto returnpoint;

  ptr = coin_glx_getprocaddress(symname);
  if (ptr) goto returnpoint;

  if (glglue_self_handle) {
    ptr = cc_dl_sym(glglue_self_handle, symname);
    if (ptr) goto returnpoint;
  }

returnpoint:
  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glglue_getprocaddress", "%s==%p", symname, ptr);
  }
  return ptr;
}

/* Global dictionary which stores the mappings from the context IDs to
   actual cc_glglue instances. */
static cc_hash * gldict = NULL;

static void
free_glglue_instance(unsigned long key, void * value, void * closure)
{
  free(value);
}

/* Cleans up at exit. */
static void
glglue_cleanup(void)
{
  cc_hash_apply(gldict, free_glglue_instance, NULL);
  cc_hash_destruct(gldict);
  if (glglue_self_handle) cc_dl_close(glglue_self_handle);
}

/*
  Set the OpenGL version variables in the given cc_glglue struct
  instance.

  Note: this code has been copied from GLUWrapper.c, so if any changes
  are made, make sure they are propagated over if necessary.
*/
static void
glglue_set_glVersion(cc_glglue * wrapper)
{
  char buffer[256];
  char * dotptr;

  /* NB: if you are getting a crash here, it's because an attempt at
   * setting up a cc_glglue instance was made when there is no current
   * OpenGL context. */
  const char * versionstr = (const char *)glGetString(GL_VERSION);
  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glglue_set_glVersion",
                           "glGetString(GL_VERSION)=='%s'", versionstr);
  }

  wrapper->glVersion.major = 0;
  wrapper->glVersion.minor = 0;
  wrapper->glVersion.release = 0;

  (void)strncpy(buffer, (const char *)versionstr, 255);
  buffer[255] = '\0'; /* strncpy() will not null-terminate if strlen > 255 */
  dotptr = strchr(buffer, '.');
  if (dotptr) {
    char * spaceptr;
    char * start = buffer;
    *dotptr = '\0';
    wrapper->glVersion.major = atoi(start);
    start = ++dotptr;

    dotptr = strchr(start, '.');
    spaceptr = strchr(start, ' ');
    if (!dotptr && spaceptr) dotptr = spaceptr;
    if (dotptr && spaceptr && spaceptr < dotptr) dotptr = spaceptr;
    if (dotptr) {
      int terminate = *dotptr == ' ';
      *dotptr = '\0';
      wrapper->glVersion.minor = atoi(start);
      if (!terminate) {
        start = ++dotptr;
        dotptr = strchr(start, ' ');
        if (dotptr) *dotptr = '\0';
        wrapper->glVersion.release = atoi(start);
      }
    }
    else {
      wrapper->glVersion.minor = atoi(start);
    }
  }

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glglue_set_glVersion",
                           "parsed version nr: %d.%d.%d",
                           wrapper->glVersion.major,
                           wrapper->glVersion.minor,
                           wrapper->glVersion.release);
  }
}

void
cc_glglue_glversion(const cc_glglue * w,
                    unsigned int * major,
                    unsigned int * minor,
                    unsigned int * release)
{
  *major = w->glVersion.major;
  *minor = w->glVersion.minor;
  *release = w->glVersion.release;
}


SbBool
cc_glglue_glversion_matches_at_least(const cc_glglue * w,
                                     unsigned int major,
                                     unsigned int minor,
                                     unsigned int revision)
{
  if (w->glVersion.major < major) return FALSE;
  else if (w->glVersion.major > major) return TRUE;
  if (w->glVersion.minor < minor) return FALSE;
  else if (w->glVersion.minor > minor) return TRUE;
  if (w->glVersion.release < revision) return FALSE;
  return TRUE;
}

SbBool
cc_glglue_glxversion_matches_at_least(const cc_glglue * w,
                                      int major,
                                      int minor)
{
  if (w->glxVersion.major < major) return FALSE;
  else if (w->glxVersion.major > major) return TRUE;
  if (w->glxVersion.minor < minor) return FALSE;
  return TRUE;
}

/* Sanity checks for enum _EXT value assumed to be equal to the final
 * / "proper" / standard OpenGL enum values. (If not, we could end up
 * with hard-to-find bugs because of mismatches with the compiled
 * values versus the run-time values.)
 *
 * This doesn't really _fix_ anything, it is just meant as an aid to
 * smoke out platforms where we're getting unexpected enum values. */
static void
glglue_sanity_check_enums(void)
{
  SbBool enumsok = TRUE;

#ifdef GL_CLAMP_TO_EDGE_EXT
  enumsok = enumsok && (GL_CLAMP_TO_EDGE == GL_CLAMP_TO_EDGE_EXT);
#endif /* GL_CLAMP_TO_EDGE_EXT */
#ifdef GL_CLAMP_TO_EDGE_SGIS /* Sanity check */
  enumsok = enumsok && (GL_CLAMP_TO_EDGE == GL_CLAMP_TO_EDGE_SGIS);
#endif /* GL_CLAMP_TO_EDGE_SGIS */
#ifdef GL_MAX_3D_TEXTURE_SIZE_EXT
  enumsok = enumsok && (GL_MAX_3D_TEXTURE_SIZE == GL_MAX_3D_TEXTURE_SIZE_EXT);
#endif /* GL_MAX_3D_TEXTURE_SIZE_EXT */
#ifdef GL_PACK_IMAGE_HEIGHT_EXT
  enumsok = enumsok && (GL_PACK_IMAGE_HEIGHT == GL_PACK_IMAGE_HEIGHT_EXT);
#endif /* GL_PACK_IMAGE_HEIGHT_EXT */
#ifdef GL_PACK_SKIP_IMAGES_EXT
  enumsok = enumsok && (GL_PACK_SKIP_IMAGES == GL_PACK_SKIP_IMAGES_EXT);
#endif /* GL_PACK_SKIP_IMAGES_EXT */
#ifdef GL_PROXY_TEXTURE_2D_EXT
  enumsok = enumsok && (GL_PROXY_TEXTURE_2D == GL_PROXY_TEXTURE_2D_EXT);
#endif /* GL_PROXY_TEXTURE_2D_EXT */
#ifdef GL_PROXY_TEXTURE_3D_EXT
  enumsok = enumsok && (GL_PROXY_TEXTURE_3D == GL_PROXY_TEXTURE_3D_EXT);
#endif /* GL_PROXY_TEXTURE_3D_EXT */
#ifdef GL_TEXTURE_3D_EXT
  enumsok = enumsok && (GL_TEXTURE_3D == GL_TEXTURE_3D_EXT);
#endif /* GL_TEXTURE_3D_EXT */
#ifdef GL_TEXTURE_DEPTH_EXT
  enumsok = enumsok && (GL_TEXTURE_DEPTH == GL_TEXTURE_DEPTH_EXT);
#endif /* GL_TEXTURE_DEPTH_EXT */
#ifdef GL_TEXTURE_WRAP_R_EXT
  enumsok = enumsok && (GL_TEXTURE_WRAP_R == GL_TEXTURE_WRAP_R_EXT);
#endif /* GL_TEXTURE_WRAP_R_EXT */
#ifdef GL_UNPACK_IMAGE_HEIGHT_EXT
  enumsok = enumsok && (GL_UNPACK_IMAGE_HEIGHT == GL_UNPACK_IMAGE_HEIGHT_EXT);
#endif /* GL_UNPACK_IMAGE_HEIGHT_EXT */
#ifdef GL_UNPACK_SKIP_IMAGES_EXT
  enumsok = enumsok && (GL_UNPACK_SKIP_IMAGES == GL_UNPACK_SKIP_IMAGES_EXT);
#endif /* GL_UNPACK_SKIP_IMAGES_EXT */

  assert(enumsok && "OpenGL enum value assumption(s) failed!");
}

int
cc_glglue_glext_supported(const cc_glglue * wrapper, const char * extension)
{
  const GLubyte * start;
  GLubyte * where, * terminator;
  static const GLubyte * extensions = NULL;

  assert(extension && "NULL string");
  assert((extension[0] != '\0') && "empty string");
  assert((strchr(extension, ' ') == NULL) && "extension name can't have spaces");

  if (!extensions) { extensions = glGetString(GL_EXTENSIONS); }

  start = extensions;
  for (;;) {
    where = (GLubyte *) strstr((const char *)start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ') {
      if (*terminator == ' ' || *terminator == '\0') {
        return 1;
      }
    }
    start = terminator;
  }
  return 0;
}

static void
glglue_resolve_dynamic(cc_glglue * w)
{
#define proc glglue_getprocaddress

  /* These were introduced with OpenGL v1.2. */
  w->glTexImage3D = (COIN_PFNGLTEXIMAGE3DPROC)proc("glTexImage3D");
  w->glCopyTexSubImage3D = (COIN_PFNGLCOPYTEXSUBIMAGE3DPROC)proc("glCopyTexSubImage3D");
  w->glTexSubImage3D = (COIN_PFNGLTEXSUBIMAGE3DPROC)proc("glTexSubImage3D");
  /* Should be available if GL_EXT_texture3D is in the extension list. */
  if (!w->glTexImage3D) {
    w->glTexImage3D = (COIN_PFNGLTEXIMAGE3DPROC)proc("glTexImage3DEXT");
    w->glCopyTexSubImage3D = (COIN_PFNGLCOPYTEXSUBIMAGE3DPROC)proc("glCopyTexSubImage3DEXT");
    w->glTexSubImage3D = (COIN_PFNGLTEXSUBIMAGE3DPROC)proc("glTexSubImage3DEXT");
  }

  /* Appeared in OpenGL v1.1. */
  w->glPolygonOffset = (COIN_PFNGLPOLYGONOFFSETPROC)proc("glPolygonOffset");
  if (!w->glPolygonOffset) {
    w->glPolygonOffset = (COIN_PFNGLPOLYGONOFFSETPROC)proc("glPolygonOffsetEXT");
  }

  /* Appeared in OpenGL v1.1. */
  w->glBindTexture = (COIN_PFNGLBINDTEXTUREPROC)proc("glBindTexture");
  w->glDeleteTextures = (COIN_PFNGLDELETETEXTURESPROC)proc("glDeleteTextures");
  w->glGenTextures = (COIN_PFNGLGENTEXTURESPROC)proc("glGenTextures");
  /* From GL_EXT_texture_object. */
  if (!w->glBindTexture) {
    w->glBindTexture = (COIN_PFNGLBINDTEXTUREPROC)proc("glBindTextureEXT");
    w->glDeleteTextures = (COIN_PFNGLDELETETEXTURESPROC)proc("glDeleteTexturesEXT");
    w->glGenTextures = (COIN_PFNGLGENTEXTURESPROC)proc("glGenTexturesEXT");
  }

  /* Appeared in OpenGL v1.1. */
  w->glTexSubImage2D = (COIN_PFNGLTEXSUBIMAGE2DPROC)proc("glTexSubImage2D");
  /* From GL_EXT_subtexture. */
  if (!w->glTexSubImage2D) {
    w->glTexSubImage2D = (COIN_PFNGLTEXSUBIMAGE2DPROC)proc("glTexSubImage2DEXT");
  }

  /* Appeared in OpenGL v1.3. */
  w->glActiveTexture = (COIN_PFNGLACTIVETEXTUREPROC)proc("glActiveTexture");
  w->glMultiTexCoord2f = (COIN_PFNGLMULTITEXCOORD2FPROC)proc("glMultiTexCoord2f");
  /* From GL_ARB_multitexture. */
  if (!w->glActiveTexture) {
    w->glActiveTexture = (COIN_PFNGLACTIVETEXTUREPROC)proc("glActiveTextureARB");
    w->glMultiTexCoord2f = (COIN_PFNGLMULTITEXCOORD2FPROC)proc("glMultiTexCoord2fARB");
  }

  /* Appeared in GLX 1.3. */
  w->glXGetCurrentDisplay = (COIN_PFNGLXGETCURRENTDISPLAYPROC)proc("glXGetCurrentDisplay");
  /* From GLX_EXT_import_context. */
  if (!w->glXGetCurrentDisplay) {
    w->glXGetCurrentDisplay = (COIN_PFNGLXGETCURRENTDISPLAYPROC)proc("glXGetCurrentDisplayEXT");
  }

#undef proc
}

/* Resolve requests for supported features. */
static void
glglue_init_feature_indicators(cc_glglue * w)
{
  w->has3DTextures = FALSE;
  w->has3DProxyTextures = FALSE;

  if (cc_glglue_glversion_matches_at_least(w,1,2,0)) {
    w->has3DTextures = TRUE;
    w->has3DProxyTextures = TRUE;
  }
  else if (cc_glglue_glversion_matches_at_least(w,1,1,0) &&
           cc_glglue_glext_supported(w,"GL_EXT_texture3D")) {
    w->has3DTextures = TRUE;
    w->has3DProxyTextures = TRUE;
  }


  w->hasTextureEdgeClamp = FALSE;

  if (cc_glglue_glversion_matches_at_least(w,1,2,0)) {
    w->hasTextureEdgeClamp = TRUE;
  }
  else if (cc_glglue_glext_supported(w, "GL_SGIS_texture_edge_clamp")) {
    w->hasTextureEdgeClamp = TRUE;
  }
  /* this test was for some reason disabled a while ago, which caused
   * big problems for me.  _Never_ disable this test again, please!
   * pederb, 2002-03-27 */
  else if (cc_glglue_glext_supported(w, "GL_EXT_texture_edge_clamp")) {
    w->hasTextureEdgeClamp = TRUE;
  }


  w->has2DProxyTextures = FALSE;

  if (cc_glglue_glversion_matches_at_least(w,1,1,0)) {
    w->has2DProxyTextures = TRUE;
  }
  else if (cc_glglue_glext_supported(w, "GL_EXT_texture")) {
    w->has2DProxyTextures = TRUE;
  }


  w->hasMultitexture = FALSE;

  if (cc_glglue_glversion_matches_at_least(w,1,3,0)) {
    w->hasMultitexture = TRUE;
  }
  else if (cc_glglue_glext_supported(w, "GL_ARB_multitexture")) {
    w->hasMultitexture = TRUE;
  }
}

/* We're basically using the Singleton pattern to instantiate and
   return OpenGL-glue "object structs". We're constructing one
   instance for each OpenGL context, though.  */
const cc_glglue *
cc_glglue_instance(int contextid)
{
  SbBool found;
  void * ptr;
  void * handle;
  cc_glglue * gi = NULL;

  CC_SYNC_BEGIN(cc_glglue_instance);

  if (!gldict) {  /* First invocation, do initializations. */
    gldict = cc_hash_construct(16, 0.75f);
    coin_atexit((coin_atexit_f *)glglue_cleanup);
  }

  found = cc_hash_get(gldict, (unsigned long) contextid, &ptr);

  if (!found) {
    /* Internal consistency checking. */
#ifdef HAVE_GLX
    /* Disabled this assert because GLX in Mesa version 3.4.2
       (GL_VENDOR "VA Linux Systems, Inc", GL_RENDERER "Mesa GLX
       Indirect", GL_VERSION "1.2 Mesa 3.4.2") returns NULL even
       though there really is a current context set up. (Reported by
       kintel.) */
/*     assert((glXGetCurrentContext() != NULL) && "must have a current GL context when instantiating cc_glglue"); */
#endif /* HAVE_GLX */
#ifdef HAVE_WGL
    assert((wglGetCurrentContext() != NULL) && "must have a current GL context when instantiating cc_glglue");
#endif /* HAVE_WGL */
#ifdef HAVE_AGL
    assert((aglGetCurrentContext() != NULL) && "must have a current GL context when instantiating cc_glglue");
#endif /* HAVE_AGL */

    gi = (cc_glglue*)malloc(sizeof(cc_glglue));
    /* FIXME: handle out-of-memory on malloc(). 20000928 mortene. */

    ptr = gi;
    cc_hash_put(gldict, (unsigned long) contextid, ptr);

    if (!glglue_self_handle && !glglue_tried_open_self) {
      glglue_self_handle = cc_dl_open(NULL);
      glglue_tried_open_self = TRUE;
    }

    glglue_set_glVersion(gi);
    coin_glx_version(&(gi->glxVersion.major), &(gi->glxVersion.minor));

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("cc_glglue_instance",
                             "glGetString(GL_VENDOR)=='%s'",
                             glGetString(GL_VENDOR));
      cc_debugerror_postinfo("cc_glglue_instance",
                             "glGetString(GL_RENDERER)=='%s'",
                             glGetString(GL_RENDERER));
      cc_debugerror_postinfo("cc_glglue_instance",
                             "glGetString(GL_EXTENSIONS)=='%s'",
                             glGetString(GL_EXTENSIONS));
    }


    /* Initialize function pointers to NULL pointers. */
    gi->glTexImage3D = NULL;
    gi->glCopyTexSubImage3D = NULL;
    gi->glTexSubImage3D = NULL;
    gi->glBindTexture = NULL;
    gi->glDeleteTextures = NULL;
    gi->glGenTextures = NULL;
    gi->glTexSubImage2D = NULL;
    gi->glActiveTexture = NULL;
    gi->glMultiTexCoord2f = NULL;
    gi->glXGetCurrentDisplay = NULL;

    glglue_sanity_check_enums();

    /* Resolve our functions */
#ifdef COIN_OPENGL_DYNAMIC_BINDING
    glglue_resolve_dynamic(gi);
#else /* Static binding */
    if (0) {
    }
#if GL_VERSION_1_2
    else if (cc_glglue_glversion_matches_at_least(gi,1,2,0)) {
      gi->glTexImage3D = &glTexImage3D;
      gi->glCopyTexSubImage3D = &glCopyTexSubImage3D;
      gi->glTexSubImage3D = &glTexSubImage3D;
    }
#endif
#if GL_VERSION_1_1
#ifdef GL_EXT_texture3D
    else if (cc_glglue_glversion_matches_at_least(gi,1,1,0) &&
             glglue_glext_supported(gi,"GL_EXT_texture3D")) {
      gi->glTexImage3D = (COIN_PFNGLTEXIMAGE3DPROC)&glTexImage3DEXT;
      /* FIXME: #ifdef these extensions as well or is that implicitly given? (kintel 20011123) */
      if (cc_glglue_glext_supported(gi, "GL_EXT_copy_texture"))
        gi->glCopyTexSubImage3D = (COIN_PFNGLCOPYTEXSUBIMAGE3DPROC)&glCopyTexSubImage3DEXT;
      if (cc_glglue_glext_supported(gi, "GL_EXT_subtexture"))
        gi->glTexSubImage3D = (COIN_PFNGLTEXSUBIMAGE3DPROC)&glTexSubImage3DEXT;
    }
#endif
#endif

    /* Some SGI OpenGL implementations report OpenGL 1.1 without supporting
       glPolygonOffset 100%. They do support glPolygonOffsetEXT though, so we
       hide the checking here.
       FIXME: We may want to check this during configure instead. */
    /* FIXME: Reintroduce the env variable check? */
    if (0) {
    }
#if defined(__sgi)
    else if (cc_glglue_glversion_matches_at_least(gi,1,1,0) &&
             !glglue_glversion_matches_at_least(gi,1,2,0)) {
      gi->glPolygonOffset = (COIN_PFNGLPOLYGONOFFSETPROC)&glPolygonOffsetEXT;
    }
#endif /* ! __sgi */
    else {
      gi->glPolygonOffset = (COIN_PFNGLPOLYGONOFFSETPROC)&glPolygonOffset;
    }

#if GL_VERSION_1_1
    gi->glBindTexture =  &glBindTexture;
    gi->glDeleteTextures = &glDeleteTextures;
    gi->glGenTextures = &glGenTextures;
#elif GL_EXT_texture_object
    if (cc_glglue_glext_supported(gi, "GL_EXT_texture_object")) {
      gi->glBindTexture =  &glBindTextureEXT;
      gi->glDeleteTextures = &glDeleteTexturesEXT;
      gi->glGenTextures = &glGenTexturesEXT;
    }
#endif /* !GL_VERSION_1_1 */

#if GL_VERSION_1_1
    gi->glTexSubImage2D = &glTexSubImage2D;
#elif GL_EXT_subtexture
    if (cc_glglue_glext_supported(gi, "GL_EXT_subtexture")) {
      gi->glTexSubImage2D = &glTexSubImage2DEXT;
    }
#endif /* !GL_VERSION_1_1 */

#if GL_VERSION_1_3
    gi->glActiveTexture = &glActiveTexture;
    gi->glMultiTexCoord2f = &glMultiTexCoord2f;
#elif GL_ARB_multitexture
    gi->glActiveTexture = &glActiveTextureARB;
    gi->glMultiTexCoord2f = &glMultiTexCoord2fARB;
#endif /* !GL_VERSION_1_3 */

#endif /* !COIN_OPENGL_DYNAMIC_BINDING */

  }
  else {
    gi = (cc_glglue *)ptr;
  }

  /* Set up all the "has-this-and-that" feature flags. */
  glglue_init_feature_indicators(gi);

  CC_SYNC_END(cc_glglue_instance);
  return gi;
}

SbBool
cc_glglue_has_3d_textures(const cc_glglue * glue)
{
  return glue->has3DTextures;
}

SbBool
cc_glglue_has_2d_proxy_textures(const cc_glglue * glue)
{
  return glue->has2DProxyTextures;
}

SbBool
cc_glglue_has_3d_proxy_textures(const cc_glglue * glue)
{
  return glue->has3DProxyTextures;
}

SbBool
cc_glglue_has_texture_edge_clamp(const cc_glglue * glue)
{
  return glue->hasTextureEdgeClamp;
}

SbBool
cc_glglue_has_multitexture(const cc_glglue * glue)
{
  return glue->hasMultitexture;
}

void
cc_glglue_glTexImage3D(const cc_glglue * glue,
                       GLenum target,
                       GLint level,
                       GLenum internalformat,
                       GLsizei width,
                       GLsizei height,
                       GLsizei depth,
                       GLint border,
                       GLenum format,
                       GLenum type,
                       const GLvoid *pixels)
{
  assert(glue->glTexImage3D);
  if (glue->glTexImage3D) {
    glue->glTexImage3D(target, level, internalformat,
                       width, height, depth, border,
                       format, type, pixels);
  }
}

void
cc_glglue_glTexSubImage3D(const cc_glglue * glue,
                          GLenum target,
                          GLint level,
                          GLint xoffset,
                          GLint yoffset,
                          GLint zoffset,
                          GLsizei width,
                          GLsizei height,
                          GLsizei depth,
                          GLenum format,
                          GLenum type,
                          const GLvoid * pixels)
{
  assert(glue->glTexSubImage3D);
  if (glue->glTexSubImage3D) {
    glue->glTexSubImage3D(target, level, xoffset, yoffset,
                          zoffset, width, height, depth, format,
                          type, pixels);
  }
}

void
cc_glglue_glCopyTexSubImage3D(const cc_glglue * glue,
                              GLenum target,
                              GLint level,
                              GLint xoffset,
                              GLint yoffset,
                              GLint zoffset,
                              GLint x,
                              GLint y,
                              GLsizei width,
                              GLsizei height)
{
  assert(glue->glCopyTexSubImage3D);
  if (glue->glCopyTexSubImage3D) {
    glue->glCopyTexSubImage3D(target,
                              level,
                              xoffset,
                              yoffset,
                              zoffset,
                              x,
                              y,
                              width,
                              height);
  }
}

void
cc_glglue_glPolygonOffset(const cc_glglue * glue,
                          GLfloat factor,
                          GLfloat bias)
{
  assert(glue->glPolygonOffset);
  if (glue->glPolygonOffset) {
    glue->glPolygonOffset(factor, bias);
  }
}

void
cc_glglue_glBindTexture(const cc_glglue * glue,
                        GLenum target,
                        GLuint texture)
{
  assert(glue->glBindTexture);
  if (glue->glBindTexture) {
    glue->glBindTexture(target, texture);
  }
}

void
cc_glglue_glDeleteTextures(const cc_glglue * glue,
                           GLsizei n,
                           const GLuint * textures)
{
  assert(glue->glDeleteTextures);
  if (glue->glDeleteTextures) {
    glue->glDeleteTextures(n, textures);
  }
}

void
cc_glglue_glGenTextures(const cc_glglue * glue,
                        GLsizei n,
                        GLuint *textures)
{
  assert(glue->glGenTextures);
  if (glue->glGenTextures) {
    glue->glGenTextures(n, textures);
  }
}

void
cc_glglue_glTexSubImage2D(const cc_glglue * glue,
                          GLenum target,
                          GLint level,
                          GLint xoffset,
                          GLint yoffset,
                          GLsizei width,
                          GLsizei height,
                          GLenum format,
                          GLenum type,
                          const GLvoid * pixels)
{
  if (glue->glTexSubImage2D) {
    glue->glTexSubImage2D(target, level, xoffset, yoffset,
                          width, height, format, type, pixels);
  }
}

void
cc_glglue_glActiveTexture(const cc_glglue * glue,
                          GLenum texture)
{
  assert(glue->glActiveTexture);
  if (glue->glActiveTexture) {
    glue->glActiveTexture(texture);
  }
}

void
cc_glglue_glMultiTexCoord2f(const cc_glglue * glue,
                            GLenum target,
                            GLfloat s,
                            GLfloat t)
{
  assert(glue->glMultiTexCoord2f);
  if (glue->glMultiTexCoord2f) {
    glue->glMultiTexCoord2f(target, s, t);
  }
}
