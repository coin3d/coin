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

#include "GLWrapper.h"

#ifdef HAVE_AGL
#include <AGL/AGL.h>
#endif /* HAVE_AGL */

// FIXME: temporary fix awaiting configure-check for this feature
// (kintel 20011125)
#ifndef __APPLE__
#define COIN_OPENGL_DYNAMIC_BINDING
#endif

#include <Inventor/SbDict.h>

#include <../tidbits.h> // coin_atexit()
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/threads/threadsutilp.h>

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

/*
  Define the GETPROCADDRESS macro.
 */
#define GETPROCADDRESS(GLWHANDLE,FUNC) \
  GLWrapper_getProcAddress(GLWHANDLE, SO__QUOTE(FUNC))

/*
   Define GLWRAPPER_REGISTER_FUNC macro. Casting the type is
   necessary for this file to be compatible with C++ compilers.
*/
#define GLWRAPPER_REGISTER_FUNC(_wrapname_, _funcname_, _funcsig_) \
  gi->_wrapname_ = (_funcsig_)GETPROCADDRESS(gi, _funcname_)

void *
GLWrapper_getProcAddress(GLWrapper_t *gi, const char *fname)
{
#ifdef HAVE_WGL
  void * ptr = wglGetProcAddress(fname);
  if (ptr == NULL) {
    // check for function in opengl32.dll. 
    //
    // FIXME: Is it sufficient to just check opengl32.dll? pederb, 2002-07-12
    //
    // UPDATE 2002-07-12 kintel: I think not, as the function pointers
    // that should be used are dependant upon 1) the current
    // framebuffer configuration and 2) the ICD driver installed. You
    // might be lucky though...

    HINSTANCE h = GetModuleHandle("opengl32.dll");
    if (h) {
      ptr = (void*) GetProcAddress(h, fname);
    }
  }
  return ptr;
#else
  if (0) {
  }
  //FIXME: Bind this using dlsym() ? (kintel 20011123)
#ifdef GLX_VERSION_1_4           // Have glXGetProcAddress()
  else if (GLWrapper_glxVersionMatchesAtLeast(gi, 1,4)) {
      return (void *)glXGetProcAddress((const GLubyte *)fname);
  }
#endif
#ifdef HAVE_GLX
  else if (GLWrapper_glxVersionMatchesAtLeast(gi, 1,1) &&
           gi->glXGetProcAddressARB) {
    return (void *)gi->glXGetProcAddressARB((const GLubyte *)fname);
  }
#endif
#ifdef HAVE_DL_LIB
  else if (1) {
    if (!gi->libhandle) {
      gi->libhandle = dlopen(NULL, RTLD_LAZY);
      //      if (!gi->libhandle) FIXME: Error handling (kintel 20011121)
    }
    return dlsym(gi->libhandle, fname);
  }
#endif
  return NULL; //FIXME: Assert or run-time error? (kintel 20011121)
#endif
}

/*
 Returns a string describing the method used for dynamic binding
 */
const char *
GLWrapper_getProcAddressMethod(GLWrapper_t *gi)
{
#ifdef HAVE_WGL
  return "wglGetProcAddress() (MS Windows)";
#else
  if (0) {
  }
#ifdef GLX_VERSION_1_4           // Have glXGetProcAddress()
  else if (GLWrapper_glxVersionMatchesAtLeast(gi, 1,4)) {
      return "glXGetProcAddress() (GLX 1.4)"
  }
#endif
#ifdef HAVE_GLX
  else if (GLWrapper_glxVersionMatchesAtLeast(gi, 1,1) &&
           gi->glXGetProcAddressARB) {
    return "glXGetProcAddressARB()";
  }
#endif
#ifdef HAVE_DL_LIB
  else if (1) {
    return "dlopen()/dlsym()";
  }
#endif
  return "NULL: Error";
#endif
}

/* FIXME: support HP-UX 10.x? (Doesn't have dlopen().) 20010626 mortene. */

static GLWrapper_t * GL_instance = NULL;
static SbDict * gldict = NULL;

static void
free_GLWrapper_instance(unsigned long key, void * value)
{
  free(value);
}

/* Cleans up at exit. */
static void
GLWrapper_cleanup(void)
{
  // FIXME: clean up. 20011115 mortene.
  // if (GL_libhandle) { CLOSE_RUNTIME_BINDING(GL_libhandle); }
  gldict->applyToAll(free_GLWrapper_instance);
  delete gldict;
}

/*
  Set the OpenGL version variables in the given GLWrapper_t struct
  instance.

  Note: this code has been copied from GLUWrapper.c, so if any changes
  are made, make sure they are propagated over if necessary.
*/
static void
GLWrapper_set_glVersion(GLWrapper_t * wrapper)
{
  char buffer[256];
  char * dotptr;

  // NB: if you are getting a crash here, it's because an attempt at
  // setting up a GLWrapper instance was made when there is no current
  // OpenGL context.
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
  Set the GLX version variables in the given GLWrapper_t struct
  instance. Returns 1 on success, 0 on failure (which means the variables
  are not set.
*/
static int
GLWrapper_set_glxVersion(GLWrapper_t * gi)
{
#ifdef HAVE_GLX
  int major, minor;

  Display * display = XOpenDisplay(NULL);
  Bool ok = False;

  if (display) {
    ok = glXQueryVersion(display, &major, &minor);
    XCloseDisplay(display);
  }

  if (!ok) {
    SoDebugError::post("GLWrapper_set_glxVersion",
                       "couldn't decide GLX version on your system -- ai!%s",
                       display == NULL ? " (couldn't open NULL display)" : "");
  }

  gi->glxVersion.major = major;
  gi->glxVersion.minor = minor;

  return ok ? 1 : 0;
#else
  return 0;
#endif
}

int
GLWrapper_glVersionMatchesAtLeast(const GLWrapper_t * w,
                                  unsigned int major,
                                  unsigned int minor,
                                  unsigned int revision)
{
  if (w->glVersion.major < major) return 0;
  else if (w->glVersion.major > major) return 1;
  if (w->glVersion.minor < minor) return 0;
  else if (w->glVersion.minor > minor) return 1;
  if (w->glVersion.release < revision) return 0;
  return 1;
}

int
GLWrapper_glxVersionMatchesAtLeast(const GLWrapper_t * w,
                                   int major,
                                   int minor)
{
#ifdef HAVE_GLX
  if (w->glxVersion.major < major) return 0;
  else if (w->glxVersion.major > major) return 1;
  if (w->glxVersion.minor < minor) return 0;
  return 1;
#else
  return 0;
#endif
}

int
GLWrapper_glEXTSupported(GLWrapper_t * wrapper, const char * extension)
{
  assert(extension);

  static const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

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
GLWrapper_glxEXTSupported(GLWrapper_t * wrapper, const char * extension)
{
#ifdef HAVE_GLX
  assert(extension);

  static const char *extensions = NULL;
  const char *start;
  char *where, *terminator;

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
#endif
  return 0;
}

const GLWrapper_t *
GLWrapper(int contextid)
{
  CC_SYNC_BEGIN(GLWrapper);

  if (!gldict) {  /* First invocation, do initializations. */
    gldict = new SbDict;
    coin_atexit((coin_atexit_f *)GLWrapper_cleanup);
  }

  int found;
  void * ptr;
  found = (int)gldict->find(contextid, ptr);
  GLWrapper_t * gi = NULL;

  if (!found) {

    // Internal consistency checking.
#ifdef HAVE_GLX
    assert((glXGetCurrentContext() != NULL) && "must have a current GL context when instantiating GLWrapper");
#endif // HAVE_GLX
#ifdef HAVE_WGL
    assert((wglGetCurrentContext() != NULL) && "must have a current GL context when instantiating GLWrapper");
#endif // HAVE_WGL
#ifdef HAVE_AGL
    assert((aglGetCurrentContext() != NULL) && "must have a current GL context when instantiating GLWrapper");
#endif // HAVE_AGL

    gi = (GLWrapper_t *)malloc(sizeof(GLWrapper_t));
    /* FIXME: handle out-of-memory on malloc(). 20000928 mortene. */

    ptr = gi;
    gldict->enter(contextid, ptr);

    gi->libhandle = NULL;
    GLWrapper_set_glVersion(gi);
#ifdef HAVE_GLX
    GLWrapper_set_glxVersion(gi);
#endif

#ifdef HAVE_GLX
#ifdef HAVE_DL_LIB
    void *handle = dlopen(NULL, RTLD_LAZY);
    //FIXME: Error handling (kintel 20011123)
    gi->glXGetProcAddressARB = (COIN_PFNGLXGETPROCADDRESSARB)dlsym(handle, "glXGetProcAddressARB");
    dlclose(handle);
#elif HAVE_GLXGETPROCADDRESSARB
    gi->glXGetProcAddressARB = &glXGetProcAddressARB;
#endif
#endif

    if (COIN_DEBUG && 0) {
      SoDebugError::postInfo("GLWrapper",
                             "Using %s for dynamic binding.\n",
                             GLWrapper_getProcAddressMethod(gi));
    }

    // Initialize everything to zero.

    gi->has3DTextures = FALSE;
    gi->has2DProxyTextures = FALSE;
    gi->has3DProxyTextures = FALSE;
    gi->hasTextureEdgeClamp = FALSE;

    gi->glTexImage3D = NULL;
    gi->glCopyTexSubImage3D = NULL;
    gi->glTexSubImage3D = NULL;
    gi->glBindTexture = NULL;
    gi->glDeleteTextures = NULL;
    gi->glGenTextures = NULL;
    gi->glTexSubImage2D = NULL;

#ifdef HAVE_GLX
    gi->glXGetCurrentDisplay = NULL;
#endif

    // Sanity checks for enum _EXT value assumed to be equal to the
    // final / "proper" / standard OpenGL enum values. (If not, we
    // could end up with hard-to-find bugs because of mismatches with
    // the compiled values versus the run-time values.)
    //
    // This doesn't really _fix_ anything, it is just meant as an aid
    // to smoke out platforms where we're getting unexpected enum
    // values.
    SbBool enumsok = TRUE;
#ifdef GL_CLAMP_TO_EDGE_EXT
    enumsok = enumsok && (GL_CLAMP_TO_EDGE == GL_CLAMP_TO_EDGE_EXT);
#endif // GL_CLAMP_TO_EDGE_EXT
#ifdef GL_CLAMP_TO_EDGE_SGIS // Sanity check
    enumsok = enumsok && (GL_CLAMP_TO_EDGE == GL_CLAMP_TO_EDGE_SGIS);
#endif // GL_CLAMP_TO_EDGE_SGIS
#ifdef GL_MAX_3D_TEXTURE_SIZE_EXT
    enumsok = enumsok && (GL_MAX_3D_TEXTURE_SIZE == GL_MAX_3D_TEXTURE_SIZE_EXT);
#endif // GL_MAX_3D_TEXTURE_SIZE_EXT
#ifdef GL_PACK_IMAGE_HEIGHT_EXT
    enumsok = enumsok && (GL_PACK_IMAGE_HEIGHT == GL_PACK_IMAGE_HEIGHT_EXT);
#endif // GL_PACK_IMAGE_HEIGHT_EXT
#ifdef GL_PACK_SKIP_IMAGES_EXT
    enumsok = enumsok && (GL_PACK_SKIP_IMAGES == GL_PACK_SKIP_IMAGES_EXT);
#endif // GL_PACK_SKIP_IMAGES_EXT
#ifdef GL_PROXY_TEXTURE_2D_EXT
    enumsok = enumsok && (GL_PROXY_TEXTURE_2D == GL_PROXY_TEXTURE_2D_EXT);
#endif // GL_PROXY_TEXTURE_2D_EXT
#ifdef GL_PROXY_TEXTURE_3D_EXT
    enumsok = enumsok && (GL_PROXY_TEXTURE_3D == GL_PROXY_TEXTURE_3D_EXT);
#endif // GL_PROXY_TEXTURE_3D_EXT
#ifdef GL_TEXTURE_3D_EXT
    enumsok = enumsok && (GL_TEXTURE_3D == GL_TEXTURE_3D_EXT);
#endif // GL_TEXTURE_3D_EXT
#ifdef GL_TEXTURE_DEPTH_EXT
    enumsok = enumsok && (GL_TEXTURE_DEPTH == GL_TEXTURE_DEPTH_EXT);
#endif // GL_TEXTURE_DEPTH_EXT
#ifdef GL_TEXTURE_WRAP_R_EXT
    enumsok = enumsok && (GL_TEXTURE_WRAP_R == GL_TEXTURE_WRAP_R_EXT);
#endif // GL_TEXTURE_WRAP_R_EXT
#ifdef GL_UNPACK_IMAGE_HEIGHT_EXT
    enumsok = enumsok && (GL_UNPACK_IMAGE_HEIGHT == GL_UNPACK_IMAGE_HEIGHT_EXT);
#endif // GL_UNPACK_IMAGE_HEIGHT_EXT
#ifdef GL_UNPACK_SKIP_IMAGES_EXT
    enumsok = enumsok && (GL_UNPACK_SKIP_IMAGES == GL_UNPACK_SKIP_IMAGES_EXT);
#endif // GL_UNPACK_SKIP_IMAGES_EXT

    assert(enumsok && "OpenGL enum value assumption(s) failed!");


    // Enums and functions are resolved separately because of better
    // readability in case of static binding.

    // Resolve requests for supported features.
    if (GLWrapper_glVersionMatchesAtLeast(gi,1,2,0)) {
      gi->has3DTextures = TRUE;
      gi->has3DProxyTextures = TRUE;
    }
    else if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0) &&
             GLWrapper_glEXTSupported(gi,"GL_EXT_texture3D")) {
      gi->has3DTextures = TRUE;
      gi->has3DProxyTextures = TRUE;
    }

    if (GLWrapper_glVersionMatchesAtLeast(gi,1,2,0)) {
      gi->hasTextureEdgeClamp = TRUE;
    }
    else if (GLWrapper_glEXTSupported(gi, "GL_SGIS_texture_edge_clamp")) {
      gi->hasTextureEdgeClamp = TRUE;
    }
    // this test was for some reason disabled a while ago, which
    // caused big problems for me.  _Never_ disable this test again,
    // please! pederb, 2002-03-27
    else if (GLWrapper_glEXTSupported(gi, "GL_EXT_texture_edge_clamp")) {
      gi->hasTextureEdgeClamp = TRUE;
    }

    if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0)) {
      gi->has2DProxyTextures = TRUE;
    }
    else if (GLWrapper_glEXTSupported(gi, "GL_EXT_texture")) {
      gi->has2DProxyTextures = TRUE;
    }

    // Resolve our functions
#ifdef COIN_OPENGL_DYNAMIC_BINDING
    if (GLWrapper_glVersionMatchesAtLeast(gi,1,2,0)) {
      GLWRAPPER_REGISTER_FUNC(glTexImage3D, glTexImage3D,
                              COIN_PFNGLTEXIMAGE3DPROC);
      GLWRAPPER_REGISTER_FUNC(glCopyTexSubImage3D, glCopyTexSubImage3D,
                              COIN_PFNGLCOPYTEXSUBIMAGE3DPROC);
      GLWRAPPER_REGISTER_FUNC(glTexSubImage3D, glTexSubImage3D,
                              COIN_PFNGLTEXSUBIMAGE3DPROC);
    }
    else if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0) &&
             GLWrapper_glEXTSupported(gi,"GL_EXT_texture3D")) {
      GLWRAPPER_REGISTER_FUNC(glTexImage3D, glTexImage3DEXT,
                              COIN_PFNGLTEXIMAGE3DPROC);
      if (GLWrapper_glEXTSupported(gi, "GL_EXT_copy_texture"))
        GLWRAPPER_REGISTER_FUNC(glCopyTexSubImage3D, glCopyTexSubImage3DEXT,
                                COIN_PFNGLCOPYTEXSUBIMAGE3DPROC);
      if (GLWrapper_glEXTSupported(gi, "GL_EXT_subtexture"))
        GLWRAPPER_REGISTER_FUNC(glTexSubImage3D, glTexSubImage3DEXT,
                                COIN_PFNGLTEXSUBIMAGE3DPROC);
    }

    /* Some SGI OpenGL implementations report OpenGL 1.1 without supporting
       glPolygonOffset 100%. The do support glPolygonOffsetEXT though, so we
       hide the checking here.
       FIXME: We may want to check this during configure instead. */
    /* FIXME: Reintroduce the env variable check? */
    if (0) {
    }
#if defined(__sgi)
    else if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0) &&
             !GLWrapper_glVersionMatchesAtLeast(gi,1,2,0)) {
      GLWRAPPER_REGISTER_FUNC(glPolygonOffset, glPolygonOffsetEXT,
                              COIN_PFNGLPOLYGONOFFSETPROC);
    }
#endif
    else {
      GLWRAPPER_REGISTER_FUNC(glPolygonOffset, glPolygonOffset,
                              COIN_PFNGLPOLYGONOFFSETPROC);
    }

    if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0)) {
      GLWRAPPER_REGISTER_FUNC(glBindTexture, glBindTexture,
                              COIN_PFNGLBINDTEXTUREPROC);
      GLWRAPPER_REGISTER_FUNC(glDeleteTextures, glDeleteTextures,
                              COIN_PFNGLDELETETEXTURESPROC);
      GLWRAPPER_REGISTER_FUNC(glGenTextures, glGenTextures,
                              COIN_PFNGLGENTEXTURESPROC);
    }
    else if (GLWrapper_glEXTSupported(gi, "GL_EXT_texture_object")) {
      GLWRAPPER_REGISTER_FUNC(glBindTexture, glBindTextureEXT,
                              COIN_PFNGLBINDTEXTUREPROC);
      GLWRAPPER_REGISTER_FUNC(glDeleteTextures, glDeleteTexturesEXT,
                              COIN_PFNGLDELETETEXTURESPROC);
      GLWRAPPER_REGISTER_FUNC(glGenTextures, glGenTexturesEXT,
                              COIN_PFNGLGENTEXTURESPROC);
    }

    if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0)) {
      GLWRAPPER_REGISTER_FUNC(glTexSubImage2D, glTexSubImage2D,
                              COIN_PFNGLTEXSUBIMAGE2DPROC);
    }
    else if (GLWrapper_glEXTSupported(gi, "GL_EXT_subtexture")) {
      GLWRAPPER_REGISTER_FUNC(glTexSubImage2D, glTexSubImage2DEXT,
                              COIN_PFNGLTEXSUBIMAGE2DPROC);
    }

#else // Static binding
    if (0) {
    }
#if GL_VERSION_1_2
    else if (GLWrapper_glVersionMatchesAtLeast(gi,1,2,0)) {
      gi->glTexImage3D = &glTexImage3D;
      gi->glCopyTexSubImage3D = &glCopyTexSubImage3D;
      gi->glTexSubImage3D = &glTexSubImage3D;
    }
#endif
#if GL_VERSION_1_1
#ifdef GL_EXT_texture3D
    else if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0) &&
             GLWrapper_glEXTSupported(gi,"GL_EXT_texture3D")) {
      gi->glTexImage3D = (COIN_PFNGLTEXIMAGE3DPROC)&glTexImage3DEXT;
      //FIXME: #ifdef these extensions as well or is that implicitly given? (kintel 20011123)
      if (GLWrapper_glEXTSupported(gi, "GL_EXT_copy_texture"))
        gi->glCopyTexSubImage3D = (COIN_PFNGLCOPYTEXSUBIMAGE3DPROC)&glCopyTexSubImage3DEXT;
      if (GLWrapper_glEXTSupported(gi, "GL_EXT_subtexture"))
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
    else if (GLWrapper_glVersionMatchesAtLeast(gi,1,1,0) &&
             !GLWrapper_glVersionMatchesAtLeast(gi,1,2,0)) {
      gi->glPolygonOffset = (COIN_PFNGLPOLYGONOFFSETPROC)&glPolygonOffsetEXT;
    }
#endif
    else {
      gi->glPolygonOffset = (COIN_PFNGLPOLYGONOFFSETPROC)&glPolygonOffset;
    }

#if GL_VERSION_1_1
    gi->glBindTexture =  &glBindTexture;
    gi->glDeleteTextures = &glDeleteTextures;
    gi->glGenTextures = &glGenTextures;
#elif GL_EXT_texture_object
    if (GLWrapper_glEXTSupported(gi, "GL_EXT_texture_object")) {
      gi->glBindTexture =  &glBindTextureEXT;
      gi->glDeleteTextures = &glDeleteTexturesEXT;
      gi->glGenTextures = &glGenTexturesEXT;
    }
#endif

#if GL_VERSION_1_1
    gi->glTexSubImage2D = &glTexSubImage2D;
#elif GL_EXT_subtexture
    if (GLWrapper_glEXTSupported(gi, "GL_EXT_subtexture")) {
      gi->glTexSubImage2D = &glTexSubImage2DEXT;
    }
#endif

#endif /* !COIN_OPENGL_DYNAMIC_BINDING */

#ifdef HAVE_GLX
    //FIXME: Check if this function exists as an extension as well.
    if (GLWrapper_glxVersionMatchesAtLeast(gi,1,2)) {
      GLWRAPPER_REGISTER_FUNC(glXGetCurrentDisplay, glXGetCurrentDisplay,
                              COIN_PFNGLXGETCURRENTDISPLAYPROC);
    }
#endif
  }
  else {
    gi = (GLWrapper_t *)ptr;
  }

  CC_SYNC_END(GLWrapper);
  return gi;
}
