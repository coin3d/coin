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

/*
  Useful resources:

   - The OpenGL Extension Registry:
     <URL:http://oss.sgi.com/projects/ogl-sample/registry/>

   - Brian Paul presentation "Using OpenGL Extensions" from SIGGRAPH '97:
     <URL:http://www.mesa3d.org/brianp/sig97/exten.htm>
*/

/* FIXME: should check if we're rendering to a remote context, and if
   so disable all extension..? Perhaps provide an envvar to enable
   them again? 20020917 mortene. */

/* FIXME: demand at least OpenGL 1.1 to get around the glGenTexture()
   problems Tingdahl is seeing? 20020917 mortene. */

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
   (kintel 20011125)

   UPDATE: fetch the define from include/Inventor/C/glue/dl.h (or
   dlp.h). 20020919 mortene.
*/
#ifndef __APPLE__
#define COIN_OPENGL_DYNAMIC_BINDING
#endif

#include <Inventor/C/base/hash.h>

#include <../tidbits.h> /*  coin_atexit() */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <Inventor/C/threads/threadsutilp.h>

#include "gl_wgl.h"
#include "gl_glx.h"

static cc_libhandle glglue_self_handle = NULL;
static SbBool glglue_tried_open_self = FALSE;

/* Resolve and return the integer value of an environment variable. */
static int
glglue_resolve_envvar(const char * txt)
{
  const char * val = coin_getenv(txt);
  return val ? atoi(val) : 0;
}

/* Return value of COIN_DEBUG_GLGLUE environment variable. */
int
coin_glglue_debug(void)
{
  static int d = -1;
  if (d == -1) { d = glglue_resolve_envvar("COIN_DEBUG_GLGLUE"); }
  return (d > 0) ? 1 : 0;
}

/* Return value of COIN_PREFER_GLPOLYGONOFFSET_EXT environment variable. */
static int
glglue_prefer_glPolygonOffsetEXT(void)
{
  static int d = -1;
  if (d == -1) { d = glglue_resolve_envvar("COIN_PREFER_GLPOLYGONOFFSET_EXT"); }
  return (d > 0) ? 1 : 0;
}

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
                           "parsed to major=='%d', minor=='%d', micro=='%d'",
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

#ifdef COIN_OPENGL_DYNAMIC_BINDING

#define PROC(_func_) glglue_getprocaddress(SO__QUOTE(_func_))

/* The OpenGL library which we dynamically pick up symbols from
   /could/ have all these defined. For the code below which tries to
   dynamically resolve the methods, we will assume that they are all
   defined. By doing this little "trick", can we use the same code
   below for resolving stuff dynamically as we need anyway to resolve
   in a static manner. */
#define GL_VERSION_1_1 1
#define GL_VERSION_1_2 1
#define GL_VERSION_1_3 1
#define GLX_VERSION_1_1 1
#define GLX_VERSION_1_2 1
#define GLX_VERSION_1_3 1
#define GL_EXT_polygon_offset 1
#define GL_EXT_texture_object 1
#define GL_EXT_subtexture 1
#define GL_EXT_texture3D 1
#define GL_ARB_multitexture 1

#else /* static binding */

#define PROC(_func_) (&_func_)

#endif /* static binding */


static void
glglue_resolve_symbols(cc_glglue * w)
{
  /* Appeared in OpenGL v1.1. We store both the "real" function
     pointer and the extension pointer, in case we need to work around
     an SGI bug (see comments in cc_glglue_glPolygonOffset(). */
  w->glPolygonOffset = NULL;
  w->glPolygonOffsetEXT = NULL;
#ifdef GL_VERSION_1_1
  if (cc_glglue_glversion_matches_at_least(w, 1, 1, 0)) {
    w->glPolygonOffset = (COIN_PFNGLPOLYGONOFFSETPROC)PROC(glPolygonOffset);
  }
#endif /* GL_VERSION_1_1 */
#ifdef GL_EXT_polygon_offset
  if (cc_glglue_glext_supported(w, "GL_EXT_polygon_offset")) {
    w->glPolygonOffsetEXT = (COIN_PFNGLPOLYGONOFFSETPROC)PROC(glPolygonOffsetEXT);
  }
#endif /* GL_EXT_polygon_offset */



  /* Appeared in OpenGL v1.1. */
  w->glGenTextures = NULL;
  w->glBindTexture = NULL;
  w->glDeleteTextures = NULL;
#ifdef GL_VERSION_1_1
  if (cc_glglue_glversion_matches_at_least(w, 1, 1, 0)) {
    w->glGenTextures = (COIN_PFNGLGENTEXTURESPROC)PROC(glGenTextures);
    w->glBindTexture = (COIN_PFNGLBINDTEXTUREPROC)PROC(glBindTexture);
    w->glDeleteTextures = (COIN_PFNGLDELETETEXTURESPROC)PROC(glDeleteTextures);
  }
#endif /* GL_VERSION_1_1 */
#ifdef GL_EXT_texture_object
  if (!w->glGenTextures && cc_glglue_glext_supported(w,"GL_EXT_texture_object")) {
    w->glGenTextures = (COIN_PFNGLGENTEXTURESPROC)PROC(glGenTexturesEXT);
    w->glBindTexture = (COIN_PFNGLBINDTEXTUREPROC)PROC(glBindTextureEXT);
    w->glDeleteTextures = (COIN_PFNGLDELETETEXTURESPROC)PROC(glDeleteTexturesEXT);
  }
#endif /* GL_EXT_texture_object */

  /* Appeared in OpenGL v1.1. */
  w->glTexSubImage2D = NULL;
#ifdef GL_VERSION_1_1
  if (cc_glglue_glversion_matches_at_least(w, 1, 1, 0)) {
    w->glTexSubImage2D = (COIN_PFNGLTEXSUBIMAGE2DPROC)PROC(glTexSubImage2D);
  }
#endif /* GL_VERSION_1_1 */
#ifdef GL_EXT_subtexture
  if (!w->glTexSubImage2D && cc_glglue_glext_supported(w,"GL_EXT_subtexture")) {
    w->glTexSubImage2D = (COIN_PFNGLTEXSUBIMAGE2DPROC)PROC(glTexSubImage2DEXT);
  }
#endif /* GL_EXT_subtexture */

  /* These were introduced with OpenGL v1.2. */
  w->glTexImage3D = NULL;
  w->glCopyTexSubImage3D = NULL;
  w->glTexSubImage3D = NULL;
#ifdef GL_VERSION_1_2
  if (cc_glglue_glversion_matches_at_least(w, 1, 2, 0)) {
    w->glTexImage3D = (COIN_PFNGLTEXIMAGE3DPROC)PROC(glTexImage3D);
    w->glCopyTexSubImage3D = (COIN_PFNGLCOPYTEXSUBIMAGE3DPROC)PROC(glCopyTexSubImage3D);
    w->glTexSubImage3D = (COIN_PFNGLTEXSUBIMAGE3DPROC)PROC(glTexSubImage3D);
  }
#endif /* GL_VERSION_1_2 */
#ifdef GL_EXT_texture3D
  if (!w->glTexImage3D && cc_glglue_glext_supported(w,"GL_EXT_texture3D")) {
    w->glTexImage3D = (COIN_PFNGLTEXIMAGE3DPROC)PROC(glTexImage3DEXT);
    /* These are implicitly given if GL_EXT_texture3D is defined. */
    w->glCopyTexSubImage3D = (COIN_PFNGLCOPYTEXSUBIMAGE3DPROC)PROC(glCopyTexSubImage3DEXT);
    w->glTexSubImage3D = (COIN_PFNGLTEXSUBIMAGE3DPROC)PROC(glTexSubImage3DEXT);
  }
#endif /* GL_EXT_texture3D */


  /* Appeared in OpenGL v1.3. */
  w->glActiveTexture = NULL;
  w->glMultiTexCoord2f = NULL;
#ifdef GL_VERSION_1_3
  if (cc_glglue_glversion_matches_at_least(w, 1, 3, 0)) {
    w->glActiveTexture = (COIN_PFNGLACTIVETEXTUREPROC)PROC(glActiveTexture);
    w->glMultiTexCoord2f = (COIN_PFNGLMULTITEXCOORD2FPROC)PROC(glMultiTexCoord2f);
  }
#endif /* GL_VERSION_1_3 */
#ifdef GL_ARB_multitexture
  if (!w->glActiveTexture && cc_glglue_glext_supported(w,"GL_ARB_multitexture")) {
    w->glActiveTexture = (COIN_PFNGLACTIVETEXTUREPROC)PROC(glActiveTextureARB);
    w->glMultiTexCoord2f = (COIN_PFNGLMULTITEXCOORD2FPROC)PROC(glMultiTexCoord2fARB);
  }
#endif /* GL_ARB_multitexture */

  /* Appeared in GLX 1.3. */
  w->glXGetCurrentDisplay = NULL;
#ifdef GLX_VERSION_1_3
  if (cc_glglue_glxversion_matches_at_least(w, 1, 3)) {
    w->glXGetCurrentDisplay = (COIN_PFNGLXGETCURRENTDISPLAYPROC)PROC(glXGetCurrentDisplay);
  }
#endif /* GLX_VERSION_1_3 */
  /* From GLX_EXT_import_context. */
  /* FIXME: need to implement extension querying for GLX. 20020919 mortene. */
/*   if (!w->glXGetCurrentDisplay && cc_glglue_glxext_supported(w,"GLX_EXT_import_context")) { */
/*     w->glXGetCurrentDisplay = (COIN_PFNGLXGETCURRENTDISPLAYPROC)PROC(glXGetCurrentDisplayEXT); */
/*   } */
}

#undef PROC


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

    glglue_sanity_check_enums();

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

    gi->isdirect = coin_glx_isdirect();
    gi->vendor_is_SGI = strcmp((const char *) glGetString(GL_VENDOR), "SGI") == 0;

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("cc_glglue_instance",
                             "glGetString(GL_VENDOR)=='%s' (=> vendor_is_SGI==%s)",
                             glGetString(GL_VENDOR),
                             gi->vendor_is_SGI ? "TRUE" : "FALSE");
      cc_debugerror_postinfo("cc_glglue_instance",
                             "glGetString(GL_RENDERER)=='%s'",
                             glGetString(GL_RENDERER));
      cc_debugerror_postinfo("cc_glglue_instance",
                             "glGetString(GL_EXTENSIONS)=='%s'",
                             glGetString(GL_EXTENSIONS));

      cc_debugerror_postinfo("cc_glglue_instance",
                             "Rendering is %sdirect.",
                             gi->isdirect ? "" : "in");
    }

    /* Resolve our function pointers. */
    glglue_resolve_symbols(gi);
  }
  else {
    gi = (cc_glglue *)ptr;
  }

  CC_SYNC_END(cc_glglue_instance);
  return gi;
}

/*!
  Whether glPolygonOffset() is availble or not: either we're on OpenGL
  1.1 or the GL_EXT_polygon_offset extension is available.

  Method then available for use:
  \li cc_glglue_glPolygonOffset
*/
SbBool
cc_glglue_has_polygon_offset(const cc_glglue * w)
{
  return (w->glPolygonOffset || w->glPolygonOffsetEXT) ? TRUE : FALSE;
}

/* Returns the glPolygonOffset() we're actually going to use. */
static COIN_PFNGLPOLYGONOFFSETPROC
glglue_glPolygonOffset(const cc_glglue * w)
{
  COIN_PFNGLPOLYGONOFFSETPROC poff = NULL;

  assert(w->glPolygonOffset ||  w->glPolygonOffsetEXT);

  poff = w->glPolygonOffset;

  /* Some SGI OpenGL 1.1 driver(s) seems to have a buggy
     implementation of glPolygonOffset(), according to pederb after
     some debugging he did for Fedem. These drivers'
     glPolygonOffsetEXT() actually seems to work better, so we prefer
     that if available. */
  if (w->vendor_is_SGI && w->glPolygonOffsetEXT &&
      cc_glglue_glversion_matches_at_least(w, 1, 1, 0) &&
      !cc_glglue_glversion_matches_at_least(w, 1, 2, 0)) {
    poff = w->glPolygonOffsetEXT;
  }

  /* Since we know glPolygonOffset() can be problematic, we also
     provide a way to prefer the EXT function instead through an
     environment variable "COIN_PREFER_GLPOLYGONOFFSET_EXT" (which
     could be handy for help debugging remote systems, at least). */
  if (w->glPolygonOffsetEXT && glglue_prefer_glPolygonOffsetEXT()) {
    poff = w->glPolygonOffsetEXT;
  }

  /* If glPolygonOffset() is not available (and the function pointer
     was not set by any of the bug workaround if-checks above), fall
     back on extension. */
  if (poff == NULL) { poff = w->glPolygonOffsetEXT; }

  return poff;
}

/*!
  Enable or disable z-buffer offsetting for the given primitive types.
*/
void
cc_glglue_glPolygonOffsetEnable(const cc_glglue * w,
                                SbBool enable, int m)
{
  COIN_PFNGLPOLYGONOFFSETPROC poff = glglue_glPolygonOffset(w);

  if (enable) {
    if (poff == w->glPolygonOffset) {
      if (m & cc_glglue_FILLED) glEnable(GL_POLYGON_OFFSET_FILL);
      else glDisable(GL_POLYGON_OFFSET_FILL);
      if (m & cc_glglue_LINES) glEnable(GL_POLYGON_OFFSET_LINE);
      else glDisable(GL_POLYGON_OFFSET_LINE);        
      if (m & cc_glglue_POINTS) glEnable(GL_POLYGON_OFFSET_POINT);
      else glDisable(GL_POLYGON_OFFSET_POINT);
    }
    else { /* using glPolygonOffsetEXT() */
      /* The old pre-1.1 extension only supports filled polygon
         offsetting. */
      if (m & cc_glglue_FILLED) glEnable(GL_POLYGON_OFFSET_EXT);
      else glDisable(GL_POLYGON_OFFSET_EXT);

      if (coin_glglue_debug() && (m != cc_glglue_FILLED)) {
        static SbBool first = TRUE;
        if (first) {
          cc_debugerror_postwarning("cc_glglue_glPolygonOffsetEnable",
                                    "using EXT_polygon_offset, which only "
                                    "supports filled-polygon offsetting");
          first = FALSE;
        }
      }
    }
  }
  else { /* disable */
    if (poff == w->glPolygonOffset) {
      if (m & cc_glglue_FILLED) glDisable(GL_POLYGON_OFFSET_FILL);
      if (m & cc_glglue_LINES) glDisable(GL_POLYGON_OFFSET_LINE);
      if (m & cc_glglue_POINTS) glDisable(GL_POLYGON_OFFSET_POINT);
    }
    else { /* using glPolygonOffsetEXT() */
      if (m & cc_glglue_FILLED) glDisable(GL_POLYGON_OFFSET_EXT);
      /* Pre-1.1 glPolygonOffset extension only supported filled primitives.*/
    }
  }
}

void
cc_glglue_glPolygonOffset(const cc_glglue * w,
                          GLfloat factor,
                          GLfloat units)
{
  COIN_PFNGLPOLYGONOFFSETPROC poff = glglue_glPolygonOffset(w);

  if (poff == w->glPolygonOffsetEXT) {
    /* Try to detect if user actually attempted to specify a valid
       bias value, like the old glPolygonOffsetEXT() extension
       needs. If not, assume that the "units" argument was set up for
       the "real" glPolygonOffset() function, and use a default value
       that should work fairly ok under most circumstances. */
    SbBool isbias = (units > 0.0f) && (units < 0.01f);
    if (!isbias) units = 0.000001f;

    /* FIXME: shouldn't there be an attempt to convert the other way
       around too? Ie, if it *is* a "bias" value and we're using the
       "real" 1.1 glPolygonOffset() function, try to convert it into a
       valid "units" value? 20020919 mortene. */
  }

  poff(factor, units);
}

/*!
  Whether 3D texture objects are available or not: either we're on OpenGL
  1.1, or the GL_EXT_texture_object extension is available.

  Methods then available for use:

  \li cc_glglue_glGenTextures
  \li cc_glglue_glBindTexture
  \li cc_glglue_glDeleteTextures
*/
SbBool
cc_glglue_has_texture_objects(const cc_glglue * w)
{
  return w->glGenTextures && w->glBindTexture && w->glDeleteTextures;
}

void
cc_glglue_glGenTextures(const cc_glglue * w, GLsizei n, GLuint * textures)
{
  assert(w->glGenTextures);
  w->glGenTextures(n, textures);
}

void
cc_glglue_glBindTexture(const cc_glglue * w, GLenum target, GLuint texture)
{
  assert(w->glBindTexture);
  w->glBindTexture(target, texture);
}

void
cc_glglue_glDeleteTextures(const cc_glglue * w, GLsizei n, const GLuint * textures)
{
  assert(w->glDeleteTextures);
  w->glDeleteTextures(n, textures);
}

/*!
  Whether sub-textures are supported: either we're on OpenGL 1.2, or
  the GL_EXT_texture3D extension is available.

  Methods then available for use:

  \li cc_glglue_glTexImage3D
  \li cc_glglue_glTexSubImage3D
  \li cc_glglue_glCopyTexSubImage3D
*/
SbBool
cc_glglue_has_texsubimage(const cc_glglue * w)
{
  return w->glTexSubImage2D ? TRUE : FALSE;
}

void
cc_glglue_glTexSubImage2D(const cc_glglue * w,
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
  assert(w->glTexSubImage2D);
  w->glTexSubImage2D(target, level, xoffset, yoffset,
                     width, height, format, type, pixels);
}

/*!
  Whether 3D textures are available or not: either we're on OpenGL
  1.2, or the GL_EXT_texture3D extension is available.

  Methods then available for use:

  \li cc_glglue_glTexImage3D
  \li cc_glglue_glTexSubImage3D
  \li cc_glglue_glCopyTexSubImage3D
*/
SbBool
cc_glglue_has_3d_textures(const cc_glglue * w)
{
  return
    w->glTexImage3D &&
    w->glCopyTexSubImage3D &&
    w->glTexSubImage3D;
}

SbBool
cc_glglue_has_2d_proxy_textures(const cc_glglue * w)
{
  return
    cc_glglue_glversion_matches_at_least(w, 1, 1, 0) ||
    cc_glglue_glext_supported(w, "GL_EXT_texture");
}

SbBool
cc_glglue_has_texture_edge_clamp(const cc_glglue * w)
{
  return
    cc_glglue_glversion_matches_at_least(w, 1, 2, 0) ||
    cc_glglue_glext_supported(w, "GL_EXT_texture_edge_clamp") ||
    cc_glglue_glext_supported(w, "GL_SGIS_texture_edge_clamp");
}

SbBool
cc_glglue_has_multitexture(const cc_glglue * w)
{
  return w->glMultiTexCoord2f && w->glActiveTexture;
}

void
cc_glglue_glTexImage3D(const cc_glglue * w,
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
  assert(w->glTexImage3D);
  w->glTexImage3D(target, level, internalformat,
                  width, height, depth, border,
                  format, type, pixels);
}

void
cc_glglue_glTexSubImage3D(const cc_glglue * w,
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
  assert(w->glTexSubImage3D);
  w->glTexSubImage3D(target, level, xoffset, yoffset,
                     zoffset, width, height, depth, format,
                     type, pixels);
}

void
cc_glglue_glCopyTexSubImage3D(const cc_glglue * w,
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
  assert(w->glCopyTexSubImage3D);
  w->glCopyTexSubImage3D(target,
                         level,
                         xoffset,
                         yoffset,
                         zoffset,
                         x,
                         y,
                         width,
                         height);
}

void
cc_glglue_glActiveTexture(const cc_glglue * w,
                          GLenum texture)
{
  assert(w->glActiveTexture);
  w->glActiveTexture(texture);
}

void
cc_glglue_glMultiTexCoord2f(const cc_glglue * w,
                            GLenum target,
                            GLfloat s,
                            GLfloat t)
{
  assert(w->glMultiTexCoord2f);
  w->glMultiTexCoord2f(target, s, t);
}

/*!
  Returns current X11 display the OpenGL context is in. If none, or if
  the glXGetCurrentDisplay() method is not available (it was
  introduced with GLX 1.3), returns \c NULL.
*/
void *
cc_glglue_glXGetCurrentDisplay(const cc_glglue * w)
{
  return w->glXGetCurrentDisplay ? w->glXGetCurrentDisplay() : NULL;
}
