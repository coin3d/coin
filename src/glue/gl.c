/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/C/base/hash.h>

#ifdef HAVE_AGL
#include <AGL/AGL.h>
#endif /* HAVE_AGL */

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
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

/*
  Define the GETPROCADDRESS macro.
 */
#define GETPROCADDRESS(GLWHANDLE, FUNC) \
  glglue_get_proc_address(GLWHANDLE, SO__QUOTE(FUNC))

/*
   Define GLGLUE_REGISTER_FUNC macro. Casting the type is
   necessary for this file to be compatible with C++ compilers.
*/
#define GLGLUE_REGISTER_FUNC(_wrapname_, _funcname_, _funcsig_) \
  gi->_wrapname_ = (_funcsig_)GETPROCADDRESS(gi, _funcname_)

static void *
glglue_get_proc_address(cc_glglue * gi, const char * fname)
{
#ifdef HAVE_WGL
  void * ptr = wglGetProcAddress(fname);
  if (ptr == NULL) {
    /* check for function in opengl32.dll.
     *
     * FIXME: Is it sufficient to just check opengl32.dll? pederb, 2002-07-12
     *
     * UPDATE 2002-07-12 kintel: I think not, as the function pointers
     * that should be used are dependant upon 1) the current
     * framebuffer configuration and 2) the ICD driver installed. You
     * might be lucky though... */

    HINSTANCE h = GetModuleHandle("opengl32.dll");
    if (h) {
      ptr = (void*) GetProcAddress(h, fname);
    }
  }
  return ptr;
#else
  if (0) {
  }
  /* FIXME: Bind this using dlsym() ? (kintel 20011123) */ 
#ifdef GLX_VERSION_1_4           /* Have glXGetProcAddress() */
  else if (cc_glglue_glxversion_matches_at_least(gi, 1,4)) {
    return (void *)glXGetProcAddress((const GLubyte *)fname);
  }
#endif /* GLX_VERSION_1_4 */
#ifdef HAVE_GLX
  else if (cc_glglue_glxversion_matches_at_least(gi, 1, 1) &&
           gi->glXGetProcAddressARB) {
    return (void *)gi->glXGetProcAddressARB((const GLubyte *)fname);
  }
#endif
#ifdef HAVE_DL_LIB
  else if (1) {
    if (!gi->libhandle) {
      gi->libhandle = dlopen(NULL, RTLD_LAZY);
      /*     if (!gi->libhandle) FIXME: Error handling (kintel 20011121) */
    }
    return dlsym(gi->libhandle, fname);
  }
#endif
  return NULL; /* FIXME: Assert or run-time error? (kintel 20011121) */
#endif
}

/*
 Returns a string describing the method used for dynamic binding
 */
static const char *
glglue_get_proc_address_method(cc_glglue * gi)
{
#ifdef HAVE_WGL
  return "wglGetProcAddress() (MS Windows)";
#else /* HAVE_WGL */
  if (0) {
  }
#ifdef GLX_VERSION_1_4           /* Have glXGetProcAddress() */
  else if (cc_glglue_glxversion_matches_at_least(gi, 1,4)) {
    return "glXGetProcAddress() (GLX 1.4)";
  }
#endif /* GLX_VERSION_1_4 */
#ifdef HAVE_GLX
  else if (cc_glglue_glxversion_matches_at_least(gi, 1, 1) &&
           gi->glXGetProcAddressARB) {
    return "glXGetProcAddressARB()";
  }
#endif /* HAVE_GLX */
#ifdef HAVE_DL_LIB
  else if (1) {
    return "dlopen()/dlsym()";
  }
#endif /* HAVE_DL_LIB */
  return "NULL: Error";
#endif /* !HAVE_WGL */
}

/* FIXME: support HP-UX 10.x? (Doesn't have dlopen().) 20010626 mortene. */

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
  /* FIXME: clean up. 20011115 mortene.
   * if (GL_libhandle) { CLOSE_RUNTIME_BINDING(GL_libhandle); } */
  cc_hash_apply(gldict, free_glglue_instance, NULL);
  cc_hash_destruct(gldict);
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
}

/*
  Set the GLX version variables in the given cc_glglue struct
  instance.   
*/
static void
glglue_set_glxVersion(cc_glglue * gi)
{
#ifdef HAVE_GLX
  int major, minor;
  Display * display = XOpenDisplay(NULL);
  Bool ok = False;

  static SbBool first = TRUE;
  if (!first) { return; }
  first = FALSE;

  gi->glxVersion.major = 0;
  gi->glxVersion.minor = 0;

  if (display) {
    ok = glXQueryVersion(display, &major, &minor);
    /* The Display resources is never deallocated explicitly (but of
     * course implicitly by the system on application close
     * down). This to work around some strange problems with the
     * NVidia-driver 29.60 on XFree86 v4 when using XCloseDisplay() --
     * like doublebuffered visuals not working correctly.
     *
     *   XCloseDisplay(display); */
  }

  if (!ok) {
#if 0
    SoDebugError::post("glglue_set_glxVersion",
                       "couldn't decide GLX version on your system -- ai!%s",
                       display == NULL ? " (couldn't open NULL display)" : "");
#endif
  }

  gi->glxVersion.major = major;
  gi->glxVersion.minor = minor;
#endif /* HAVE_GLX */
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
#ifdef HAVE_GLX
  if (w->glxVersion.major < major) return FALSE;
  else if (w->glxVersion.major > major) return TRUE;
  if (w->glxVersion.minor < minor) return FALSE;
  return TRUE;
#else /* HAVE_GLX */
  return FALSE;
#endif /* ! HAVE_GLX */
}

int
cc_glglue_glext_supported(const cc_glglue * wrapper, const char * extension)
{
  const GLubyte *start;
  GLubyte * where, * terminator;
  static const GLubyte * extensions = NULL;

  assert(extension);
  
  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return 0;

  if (!extensions)
    extensions = glGetString(GL_EXTENSIONS);
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

int
cc_glglue_glxext_supported(const cc_glglue * wrapper, const char * extension)
{
#ifdef HAVE_GLX
  const char * start;
  char *where, * terminator;
  static const char * extensions = NULL;

  assert(extension);

  /* Extension names should not have spaces. */
  where = (char *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return 0;

  if (!extensions) {
    extensions = NULL;
  }
  start = extensions;
  for (;;) {
    where = (char *) strstr((const char *)start, extension);
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
#endif /* HAVE_GLX */
  return 0;
}

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
    SbBool enumsok = TRUE;

    /* Internal consistency checking. */
#ifdef HAVE_GLX
    assert((glXGetCurrentContext() != NULL) && "must have a current GL context when instantiating cc_glglue");
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

    gi->libhandle = NULL;
    glglue_set_glVersion(gi);
#ifdef HAVE_GLX
    glglue_set_glxVersion(gi);
#endif /* HAVE_GLX */

#ifdef HAVE_GLX
#ifdef HAVE_DL_LIB
    handle = dlopen(NULL, RTLD_LAZY);
    /* FIXME: Error handling (kintel 20011123) */
    gi->glXGetProcAddressARB = (COIN_PFNGLXGETPROCADDRESSARB)dlsym(handle, "glXGetProcAddressARB");
    dlclose(handle);
#elif HAVE_GLXGETPROCADDRESSARB
    gi->glXGetProcAddressARB = &glXGetProcAddressARB;
#endif /* ! HAVE_DL_LIB */
#endif /* HAVE_GLX */

    if (COIN_DEBUG && 0) {
#if 0
      SoDebugError::postInfo("cc_glglue",
                             "Using %s for dynamic binding.\n",
                             glglue_get_proc_address_method(gi));
#endif
    }

    /* Initialize everything to zero. */

    gi->has3DTextures = FALSE;
    gi->has2DProxyTextures = FALSE;
    gi->has3DProxyTextures = FALSE;
    gi->hasTextureEdgeClamp = FALSE;
    gi->hasMultitexture = FALSE;

    gi->glTexImage3D = NULL;
    gi->glCopyTexSubImage3D = NULL;
    gi->glTexSubImage3D = NULL;
    gi->glBindTexture = NULL;
    gi->glDeleteTextures = NULL;
    gi->glGenTextures = NULL;
    gi->glTexSubImage2D = NULL;
    gi->glActiveTexture = NULL;
    gi->glMultiTexCoord2f = NULL;

#ifdef HAVE_GLX
    gi->glXGetCurrentDisplay = NULL;
#endif /* HAVE_GLX */

    /* Sanity checks for enum _EXT value assumed to be equal to the
     * final / "proper" / standard OpenGL enum values. (If not, we
     * could end up with hard-to-find bugs because of mismatches with
     * the compiled values versus the run-time values.)
     *
     * This doesn't really _fix_ anything, it is just meant as an aid
     * to smoke out platforms where we're getting unexpected enum
     * values. */
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

    /* Enums and functions are resolved separately because of better
     * readability in case of static binding.
     *
     * Resolve requests for supported features. */
    if (cc_glglue_glversion_matches_at_least(gi,1,2,0)) {
      gi->has3DTextures = TRUE;
      gi->has3DProxyTextures = TRUE;
    }
    else if (cc_glglue_glversion_matches_at_least(gi,1,1,0) &&
             cc_glglue_glext_supported(gi,"GL_EXT_texture3D")) {
      gi->has3DTextures = TRUE;
      gi->has3DProxyTextures = TRUE;
    }

    if (cc_glglue_glversion_matches_at_least(gi,1,2,0)) {
      gi->hasTextureEdgeClamp = TRUE;
    }
    else if (cc_glglue_glext_supported(gi, "GL_SGIS_texture_edge_clamp")) {
      gi->hasTextureEdgeClamp = TRUE;
    }
    /* this test was for some reason disabled a while ago, which
     * caused big problems for me.  _Never_ disable this test again,
     * please! pederb, 2002-03-27 */
    else if (cc_glglue_glext_supported(gi, "GL_EXT_texture_edge_clamp")) {
      gi->hasTextureEdgeClamp = TRUE;
    }

    if (cc_glglue_glversion_matches_at_least(gi,1,1,0)) {
      gi->has2DProxyTextures = TRUE;
    }
    else if (cc_glglue_glext_supported(gi, "GL_EXT_texture")) {
      gi->has2DProxyTextures = TRUE;
    }
    
    if (cc_glglue_glversion_matches_at_least(gi,1,3,0)) {
      gi->hasMultitexture = TRUE;
    }
    else if (cc_glglue_glext_supported(gi, "GL_ARB_multitexture")) {
      gi->hasMultitexture = TRUE;
    }

    /* Resolve our functions */
#ifdef COIN_OPENGL_DYNAMIC_BINDING
    if (cc_glglue_glversion_matches_at_least(gi,1,2,0)) {
      GLGLUE_REGISTER_FUNC(glTexImage3D, glTexImage3D,
                           COIN_PFNGLTEXIMAGE3DPROC);
      GLGLUE_REGISTER_FUNC(glCopyTexSubImage3D, glCopyTexSubImage3D,
                           COIN_PFNGLCOPYTEXSUBIMAGE3DPROC);
      GLGLUE_REGISTER_FUNC(glTexSubImage3D, glTexSubImage3D,
                           COIN_PFNGLTEXSUBIMAGE3DPROC);
    }
    else if (cc_glglue_glversion_matches_at_least(gi,1,1,0) &&
             cc_glglue_glext_supported(gi,"GL_EXT_texture3D")) {
      GLGLUE_REGISTER_FUNC(glTexImage3D, glTexImage3DEXT,
                           COIN_PFNGLTEXIMAGE3DPROC);
      if (cc_glglue_glext_supported(gi, "GL_EXT_copy_texture"))
        GLGLUE_REGISTER_FUNC(glCopyTexSubImage3D, glCopyTexSubImage3DEXT,
                             COIN_PFNGLCOPYTEXSUBIMAGE3DPROC);
      if (cc_glglue_glext_supported(gi, "GL_EXT_subtexture"))
        GLGLUE_REGISTER_FUNC(glTexSubImage3D, glTexSubImage3DEXT,
                             COIN_PFNGLTEXSUBIMAGE3DPROC);
    }
    
    /* Some SGI OpenGL implementations report OpenGL 1.1 without supporting
       glPolygonOffset 100%. They do support glPolygonOffsetEXT though, so we
       hide the checking here.
       FIXME: We may want to check this during configure instead. */
    /* FIXME: Reintroduce the env variable check? */
    if (0) {
    }
#if defined(__sgi)
    else if (cc_glglue_glversion_matches_at_least(gi,1,1,0) &&
             !cc_glglue_glversion_matches_at_least(gi,1,2,0)) {
      GLGLUE_REGISTER_FUNC(glPolygonOffset, glPolygonOffsetEXT,
                           COIN_PFNGLPOLYGONOFFSETPROC);
    }
#endif /* _sgi */
    else {
      GLGLUE_REGISTER_FUNC(glPolygonOffset, glPolygonOffset,
                              COIN_PFNGLPOLYGONOFFSETPROC);
    }

    if (cc_glglue_glversion_matches_at_least(gi,1,1,0)) {
      GLGLUE_REGISTER_FUNC(glBindTexture, glBindTexture,
                           COIN_PFNGLBINDTEXTUREPROC);
      GLGLUE_REGISTER_FUNC(glDeleteTextures, glDeleteTextures,
                           COIN_PFNGLDELETETEXTURESPROC);
      GLGLUE_REGISTER_FUNC(glGenTextures, glGenTextures,
                           COIN_PFNGLGENTEXTURESPROC);
    }
    else if (cc_glglue_glext_supported(gi, "GL_EXT_texture_object")) {
      GLGLUE_REGISTER_FUNC(glBindTexture, glBindTextureEXT,
                           COIN_PFNGLBINDTEXTUREPROC);
      GLGLUE_REGISTER_FUNC(glDeleteTextures, glDeleteTexturesEXT,
                           COIN_PFNGLDELETETEXTURESPROC);
      GLGLUE_REGISTER_FUNC(glGenTextures, glGenTexturesEXT,
                           COIN_PFNGLGENTEXTURESPROC);
    }
    
    if (cc_glglue_glversion_matches_at_least(gi,1,1,0)) {
      GLGLUE_REGISTER_FUNC(glTexSubImage2D, glTexSubImage2D,
                           COIN_PFNGLTEXSUBIMAGE2DPROC);
    }
    else if (cc_glglue_glext_supported(gi, "GL_EXT_subtexture")) {
      GLGLUE_REGISTER_FUNC(glTexSubImage2D, glTexSubImage2DEXT,
                           COIN_PFNGLTEXSUBIMAGE2DPROC);
    }
    if (cc_glglue_glversion_matches_at_least(gi,1,3,0)) {
      GLGLUE_REGISTER_FUNC(glActiveTexture, glActiveTexture,
                           COIN_PFNGLACTIVETEXTUREPROC);
      GLGLUE_REGISTER_FUNC(glMultiTexCoord2f, glMultiTexCoord2f,
                           COIN_PFNGLMULTITEXCOORD2FPROC);
    }
    else if (cc_glglue_glext_supported(gi, "GL_ARB_multitexture")) {
      GLGLUE_REGISTER_FUNC(glActiveTexture, glActiveTextureARB,
                           COIN_PFNGLACTIVETEXTUREPROC);
      GLGLUE_REGISTER_FUNC(glMultiTexCoord2f, glMultiTexCoord2fARB,
                           COIN_PFNGLMULTITEXCOORD2FPROC);
    }
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

#ifdef HAVE_GLX
    /* FIXME: Check if this function exists as an extension as well.*/
    if (cc_glglue_glxversion_matches_at_least(gi,1,2)) {
      GLGLUE_REGISTER_FUNC(glXGetCurrentDisplay, glXGetCurrentDisplay,
                           COIN_PFNGLXGETCURRENTDISPLAYPROC);
    }
#endif /* HAVE_GLX */
  }
  else {
    gi = (cc_glglue *)ptr;
  }

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

