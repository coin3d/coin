/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*
  Environment variable controls available:

  - COIN_GLXGLUE_NO_PBUFFERS: set to 1 to force software rendering of
    offscreen contexts.

  - COIN_GLXGLUE_NO_GLX13_PBUFFERS: don't use GLX 1.3 pbuffers support
    (will then attempt to use pbuffers through extensions).
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <Inventor/C/basic.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/glue/gl_glx.h>
#include <Inventor/C/tidbits.h>

/* ********************************************************************** */

#ifndef HAVE_GLX

void glxglue_init(cc_glglue * w)
{
  w->glx.version.major = -1;
  w->glx.version.minor = 0;
  w->glx.isdirect = 1;

  w->glx.serverversion = NULL;
  w->glx.servervendor = NULL;
  w->glx.serverextensions = NULL;
  w->glx.clientversion = NULL;
  w->glx.clientvendor = NULL;
  w->glx.clientextensions = NULL;
  w->glx.glxextensions = NULL;
}

void * glxglue_getprocaddress(const char * fname) { return NULL; }
int glxglue_ext_supported(const cc_glglue * w, const char * extension) { return 0; }

void * glxglue_context_create_offscreen(unsigned int width, unsigned int height) { assert(FALSE); return NULL; }
SbBool glxglue_context_make_current(void * ctx) { assert(FALSE); return FALSE; }
void glxglue_context_reinstate_previous(void * ctx) { assert(FALSE); }
void glxglue_context_destruct(void * ctx) { assert(FALSE); }

#else /* HAVE_GLX */

/* ********************************************************************** */

/*
 * GL/glx.h includes X11/Xmd.h which contains typedefs for BOOL and
 * INT32 that conflict with the definitions in windef.h (which is
 * included from windows.h, which may be included from
 * Inventor/system/gl.h).  To avoid this conflict, we rename the
 * typedefs done in X11/Xmd.h to use other names (tempbool and
 * tempint32), and try to clean up the hack after the header has been
 * parsed.  2003-06-25 larsa
 */
#ifndef BOOL
#define BOOL tempbool
#define COIN_DEFINED_BOOL
#endif /* !BOOL */
#ifndef INT32
#define INT32 tempint32
#define COIN_DEFINED_INT32
#endif /* !INT32 */

#include <GL/glx.h>

/*
 * This is the cleanup part of the X11/Xmd.h conflict fix hack set up
 * above.  2003-06-25 larsa
 */
#ifdef COIN_DEFINED_BOOL
#undef BOOL
#undef COIN_DEFINED_BOOL
#endif /* COIN_DEFINED_BOOL */
#ifdef COIN_DEFINED_INT32
#undef INT32
#undef COIN_DEFINED_INT32
#endif /* COIN_DEFINED_INT32 */

/* ********************************************************************** */

static int glxglue_screen = -1;
typedef void *(APIENTRY * COIN_PFNGLXGETPROCADDRESS)(const GLubyte *);
static COIN_PFNGLXGETPROCADDRESS glxglue_glXGetProcAddress = NULL;
static SbBool tried_bind_glXGetProcAddress = FALSE;

struct glxglue_contextdata;
static SbBool (* glxglue_context_create)(struct glxglue_contextdata * context) = NULL;

typedef void * COIN_GLXFBConfig;
typedef COIN_GLXFBConfig * (APIENTRY * COIN_PFNGLXCHOOSEFBCONFIG)(Display * dpy, int screen, const int * attrib_list, int * nelements);
typedef GLXContext (APIENTRY * COIN_PFNGLXCREATENEWCONTEXT)(Display * dpy, COIN_GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
static COIN_PFNGLXCHOOSEFBCONFIG glxglue_glXChooseFBConfig;
static COIN_PFNGLXCREATENEWCONTEXT glxglue_glXCreateNewContext;

typedef XID COIN_GLXPbuffer;

typedef COIN_GLXPbuffer (APIENTRY * COIN_PFNGLXCREATEGLXPBUFFERSGIX)(Display * dpy,
                                                                     COIN_GLXFBConfig config,
                                                                     unsigned int width,
                                                                     unsigned int height,
                                                                     int * attrib_list);
typedef COIN_GLXPbuffer (APIENTRY * COIN_PFNGLXCREATEPBUFFER_GLX_1_3)(Display * dpy,
                                                                      COIN_GLXFBConfig config,
                                                                      const int * attrib_list);
typedef void (APIENTRY * COIN_PFNGLXDESTROYPBUFFER)(Display * dpy, COIN_GLXPbuffer pbuf);

static COIN_PFNGLXCREATEPBUFFER_GLX_1_3 glxglue_glXCreatePbuffer_GLX_1_3;
static COIN_PFNGLXCREATEGLXPBUFFERSGIX glxglue_glXCreateGLXPbufferSGIX;
static COIN_PFNGLXDESTROYPBUFFER glxglue_glXDestroyPbuffer;

/* ********************************************************************** */

/* Sanity checks for enum extension value assumed to be equal to the
 * final / "proper" / standard OpenGL enum values. (If not, we could
 * end up with hard-to-find bugs because of mismatches with the
 * compiled values versus the run-time values.)
 *
 * This doesn't really _fix_ anything, it is just meant as an aid to
 * smoke out platforms where we're getting unexpected enum values.
 */

#ifdef GLX_RENDER_TYPE_SGIX
#if GLX_RENDER_TYPE != GLX_RENDER_TYPE_SGIX
#error dangerous enum mismatch
#endif /* cmp */
#endif /* GLX_RENDER_TYPE_SGIX */

#ifdef GLX_DRAWABLE_TYPE_SGIX
#if GLX_DRAWABLE_TYPE != GLX_DRAWABLE_TYPE
#error dangerous enum mismatch
#endif /* cmp */
#endif /* GLX_DRAWABLE_TYPE_SGIX */

#ifdef GLX_RGBA_TYPE_SGIX
#if GLX_RGBA_TYPE != GLX_RGBA_TYPE
#error dangerous enum mismatch
#endif /* cmp */
#endif /* GLX_RGBA_TYPE_SGIX */

#ifdef GLX_RGBA_BIT_SGIX
#if GLX_RGBA_BIT != GLX_RGBA_BIT
#error dangerous enum mismatch
#endif /* cmp */
#endif /* GLX_RGBA_BIT_SGIX */

#ifdef GLX_PBUFFER_BIT_SGIX
#if GLX_PBUFFER_BIT != GLX_PBUFFER_BIT
#error dangerous enum mismatch
#endif /* cmp */
#endif /* GLX_PBUFFER_BIT_SGIX */

/* ********************************************************************** */


/* This function works in a "Singleton-like" manner. */
static Display *
glxglue_get_display(void)
{
  /* The Display resource is never deallocated explicitly (but of
   * course implicitly by the system on application close down). This
   * to work around some strange problems with the NVidia-driver 29.60
   * on XFree86 v4 when using XCloseDisplay() -- like doublebuffered
   * visuals coming up just blank.
   *
   *   XCloseDisplay(glxglue_display);
   */

  static SbBool failed = FALSE;
  static Display * glxglue_display = NULL;
  if ((glxglue_display == NULL) && !failed) {
    /* FIXME: should use the real display-setting. :-(  20020926 mortene. */
    glxglue_display = XOpenDisplay(NULL);
    if (glxglue_display == NULL) {
      cc_debugerror_post("glxglue_init",
                         "Couldn't open NULL display.");
      failed = TRUE;
    }
    else {
      /* FIXME: should use the real screen number. :-(  20020926 mortene. */
      glxglue_screen = XDefaultScreen(glxglue_display);

      if (coin_glglue_debug()) {
        cc_debugerror_postinfo("glxglue_get_display", "got Display*==%p",
                               glxglue_display);
      }
    }
  }
  return glxglue_display;
}

static void
glxglue_set_version(int * major, int * minor)
{
  Bool ok = False;

  *major = -1;
  *minor = 0;

  if (glxglue_get_display() == NULL) { return; }

  ok = glXQueryVersion(glxglue_get_display(), major, minor);

  if (!ok) {
    cc_debugerror_post("glxglue_version",
                       "Couldn't decide GLX version on your system!");
  }

  if (ok && coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_version",
                           "GLX version: %d.%d", *major, *minor);
  }
}

void *
glxglue_getprocaddress(const char * fname)
{
  void * ptr = NULL;

  if (!glxglue_glXGetProcAddress && !tried_bind_glXGetProcAddress) {
    cc_libhandle h = cc_dl_open(NULL);
    if (h) {
      glxglue_glXGetProcAddress = (COIN_PFNGLXGETPROCADDRESS)
        cc_dl_sym(h, "glXGetProcAddress");

      if (coin_glglue_debug()) {
        cc_debugerror_postinfo("glxglue_getprocaddress",
                               "%s glXGetProcAddress()",
                               glxglue_glXGetProcAddress ?
                               "picked up" : "can't use");
      }

      if (!glxglue_glXGetProcAddress) {
        glxglue_glXGetProcAddress = (COIN_PFNGLXGETPROCADDRESS)
          cc_dl_sym(h, "glXGetProcAddressARB");
      }

      if (coin_glglue_debug()) {
        cc_debugerror_postinfo("glxglue_getprocaddress",
                               "%s glXGetProcAddressARB()",
                               glxglue_glXGetProcAddress ?
                               "picked up" : "can't use");
      }


      cc_dl_close(h);
    }

    /* FIXME: use static binding if cc_dl_open() has no native method
       to use? 20020906 mortene. */

    tried_bind_glXGetProcAddress = TRUE;
  }

  if (glxglue_glXGetProcAddress) {
    ptr = (void *)glxglue_glXGetProcAddress((const GLubyte *)fname);
  }

  return ptr;
}

static SbBool
glxglue_isdirect(cc_glglue * w)
{
  GLXContext ctx = glXGetCurrentContext();

  if (!ctx) {
    cc_debugerror_postwarning("glxglue_isdirect", 
                              "Couldn't get current GLX context.");
    return TRUE;
  }

  if (!glxglue_get_display()) return TRUE;
  return glXIsDirect(glxglue_get_display(), ctx) ? TRUE : FALSE;
}

int
glxglue_ext_supported(const cc_glglue * w, const char * extension)
{
  return
    (w->glx.glxextensions != NULL) &&
    coin_glglue_extension_available(w->glx.glxextensions, extension);
}

#ifdef HAVE_DYNAMIC_LINKING

#define PROC(_func_) cc_glglue_getprocaddress(SO__QUOTE(_func_))

/* The OpenGL library's GLX part which we dynamically pick up symbols
   from /could/ have all these defined. For the code below which tries
   to dynamically resolve the methods, we will assume that they are
   all defined. By doing this little "trick", can we use the same code
   below for resolving stuff dynamically as we need anyway to resolve
   in a static manner. */

#define GLX_VERSION_1_1 1
#define GLX_VERSION_1_2 1
#define GLX_VERSION_1_3 1

#define GLX_EXT_import_context 1
#define GLX_SGIX_fbconfig 1
#define GLX_SGIX_pbuffer 1

#else /* static binding */

#define PROC(_func_) (&_func_)

#endif /* static binding */

static void
glxglue_resolve_symbols(cc_glglue * w)
{
  SbBool glx13pbuffer;
  const char * env;
  struct cc_glxglue * g = &(w->glx);

  /* SGI's glx.h header file shipped with the NVidia Linux drivers
     identifies glXGetCurrentDisplay() as a GLX 1.3 method, but Sun's
     GL man pages lists it as a GLX 1.2 function, ditto for HP's GL
     man pages, and ditto for AIX's man pages. (See top of this file
     for URL). So we will assume the man pages are correct.
  */
  g->glXGetCurrentDisplay = NULL;
#ifdef GLX_VERSION_1_2
  if (cc_glglue_glxversion_matches_at_least(w, 1, 2)) {
    g->glXGetCurrentDisplay = (COIN_PFNGLXGETCURRENTDISPLAYPROC)PROC(glXGetCurrentDisplay);
  }
#endif /* GLX_VERSION_1_2 */
#ifdef GLX_EXT_import_context
  if (!g->glXGetCurrentDisplay && glxglue_ext_supported(w, "GLX_EXT_import_context")) {
    g->glXGetCurrentDisplay = (COIN_PFNGLXGETCURRENTDISPLAYPROC)PROC(glXGetCurrentDisplayEXT);
  }
#endif /* GLX_EXT_import_context */

  glxglue_glXChooseFBConfig = NULL;
  glxglue_glXCreateNewContext = NULL;

  env = coin_getenv("COIN_GLXGLUE_NO_GLX13_PBUFFERS");
  glx13pbuffer = (env == NULL) || (atoi(env) < 1);

#ifdef GLX_VERSION_1_3
  if (glx13pbuffer && cc_glglue_glxversion_matches_at_least(w, 1, 3)) {
    glxglue_glXChooseFBConfig = (COIN_PFNGLXCHOOSEFBCONFIG)PROC(glXChooseFBConfig);
    glxglue_glXCreateNewContext = (COIN_PFNGLXCREATENEWCONTEXT)PROC(glXCreateNewContext);
  }
#endif /* GLX_VERSION_1_3 */
#ifdef GLX_SGIX_fbconfig
  if (!glxglue_glXChooseFBConfig && glxglue_ext_supported(w, "GLX_SGIX_fbconfig")) {
    glxglue_glXChooseFBConfig = (COIN_PFNGLXCHOOSEFBCONFIG)PROC(glXChooseFBConfigSGIX);
    glxglue_glXCreateNewContext = (COIN_PFNGLXCREATENEWCONTEXT)PROC(glXCreateContextWithConfigSGIX);
  }
#endif /* GLX_SGIX_fbconfig */

  glxglue_glXCreatePbuffer_GLX_1_3 = NULL;
  glxglue_glXCreateGLXPbufferSGIX = NULL;
  glxglue_glXDestroyPbuffer = NULL;

#ifdef GLX_VERSION_1_3
  if (glx13pbuffer && cc_glglue_glxversion_matches_at_least(w, 1, 3)) {
    glxglue_glXCreatePbuffer_GLX_1_3 = (COIN_PFNGLXCREATEPBUFFER_GLX_1_3)PROC(glXCreatePbuffer);
    glxglue_glXDestroyPbuffer = (COIN_PFNGLXDESTROYPBUFFER)PROC(glXDestroyPbuffer);
  }
#endif /* GLX_VERSION_1_3 */

#ifdef GLX_SGIX_pbuffer
  if (!glxglue_glXCreatePbuffer_GLX_1_3 && glxglue_ext_supported(w, "GLX_SGIX_pbuffer")) {
    glxglue_glXCreateGLXPbufferSGIX = (COIN_PFNGLXCREATEGLXPBUFFERSGIX)PROC(glXCreateGLXPbufferSGIX);
    glxglue_glXDestroyPbuffer = (COIN_PFNGLXDESTROYPBUFFER)PROC(glXDestroyGLXPbufferSGIX);
  }
#endif /* GLX_SGIX_pbuffer */
}

static SbBool
glxglue_has_pbuffer_support(void)
{
  /* Make it possible to turn off pbuffers support completely. Mostly
     relevant for debugging purposes. */
  const char * env = coin_getenv("COIN_GLXGLUE_NO_PBUFFERS");
  if (env && atoi(env) > 0) { return FALSE; }

  /* Dummy statement to invoke the glxglue_init() function (which
     attempts to bind the below functions related to pbuffer
     support). */
  (void)cc_glglue_instance_from_context_ptr(glxglue_has_pbuffer_support);

  return
    glxglue_glXChooseFBConfig && glxglue_glXCreateNewContext &&
    (glxglue_glXCreatePbuffer_GLX_1_3 || glxglue_glXCreateGLXPbufferSGIX) &&
    glxglue_glXDestroyPbuffer;
}

void
glxglue_init(cc_glglue * w)
{
  glxglue_set_version(&w->glx.version.major, &w->glx.version.minor);
  w->glx.isdirect = glxglue_isdirect(w);


  w->glx.serverversion = NULL;
  w->glx.servervendor = NULL;
  w->glx.serverextensions = NULL;
  w->glx.clientversion = NULL;
  w->glx.clientvendor = NULL;
  w->glx.clientextensions = NULL;
  w->glx.glxextensions = NULL;

  if (glxglue_get_display()) {

    /* Note: be aware that glXQueryServerString(),
       glXGetClientString() and glXQueryExtensionsString() are all
       from GLX 1.1 -- just in case there are ever compile-time,
       link-time or run-time problems with this.  */

    Display * d = glxglue_get_display();
    w->glx.serverversion = glXQueryServerString(d, glxglue_screen, GLX_VERSION);
    w->glx.servervendor = glXQueryServerString(d, glxglue_screen, GLX_VENDOR);
    w->glx.serverextensions = glXQueryServerString(d, glxglue_screen, GLX_EXTENSIONS);

    w->glx.clientversion = glXGetClientString(d, GLX_VERSION);
    w->glx.clientvendor = glXGetClientString(d, GLX_VENDOR);
    w->glx.clientextensions = glXGetClientString(d, GLX_EXTENSIONS);

    w->glx.glxextensions = glXQueryExtensionsString(d, glxglue_screen);

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("glxglue_init",
                             "glXQueryServerString(GLX_VERSION)=='%s'",
                             w->glx.serverversion);
      cc_debugerror_postinfo("glxglue_init",
                             "glXQueryServerString(GLX_VENDOR)=='%s'",
                             w->glx.servervendor);
      cc_debugerror_postinfo("glxglue_init",
                             "glXQueryServerString(GLX_EXTENSIONS)=='%s'",
                             w->glx.serverextensions);

      cc_debugerror_postinfo("glxglue_init",
                             "glXClientString(GLX_VERSION)=='%s'",
                             w->glx.clientversion);
      cc_debugerror_postinfo("glxglue_init",
                             "glXClientString(GLX_VENDOR)=='%s'",
                             w->glx.clientvendor);
      cc_debugerror_postinfo("glxglue_init",
                             "glXClientString(GLX_EXTENSIONS)=='%s'",
                             w->glx.clientextensions);

      cc_debugerror_postinfo("glxglue_init",
                             "glXQueryExtensionsString()=='%s'",
                             w->glx.glxextensions);
    }
  }

  glxglue_resolve_symbols(w);
}

/* NOTE: the strategy applied here for iterating through OpenGL canvas
   settings is exactly the same as the one applied in
   SoXt/src/Inventor/Xt/SoXtGLWidget.cpp. So if you make any fixes or
   other improvements here, migrate your changes. */
static int
glxglue_build_GL_attrs(int * attrs, int trynum)
{
  int pos = 0;
  attrs[pos++] = GLX_RGBA;
  attrs[pos++] = GLX_DEPTH_SIZE;
  attrs[pos++] = 1;

  /* FIXME: An accumulator buffer should be added if numpasses >
     1. Mesa returns a GL_INVALID_OPERATION error if the accumulator
     buffer is missing, while NVIDIA automatically creates
     one. 20021127 handegar. */

  if (! (trynum & 0x04)) {
    attrs[pos++] = GLX_STENCIL_SIZE;
    attrs[pos++] = 1;
  }
  if (! (trynum & 0x02)) {
    attrs[pos++] = GLX_DOUBLEBUFFER;
  }
  if (! (trynum & 0x01)) {
    attrs[pos++] = GLX_RED_SIZE;
    attrs[pos++] = 4;
    attrs[pos++] = GLX_GREEN_SIZE;
    attrs[pos++] = 4;
    attrs[pos++] = GLX_BLUE_SIZE;
    attrs[pos++] = 4;
    /* FIXME: won't get an alpha channel in the context unless we also
       request a particular ALPHA bitsize. 20020605 mortene. */
  }
  attrs[pos++] = None;
  return pos;
}

static XVisualInfo *
glxglue_find_gl_visual(void)
{
  int trynum = 0;
  /* This used to be an "const int", but not all C compilers can
     handle arrays declaration with const int sizes. (We had a report
     of this failing for some compiler under HP-UX 10.20.) */
#define ARRAYSIZE 32
  int attrs[ARRAYSIZE];
  XVisualInfo * visinfo = NULL;

  if (glxglue_get_display() == NULL) { return NULL; }

  while (visinfo == NULL && trynum < 8) {
    int arraysize = glxglue_build_GL_attrs(attrs, trynum);
    assert(arraysize < ARRAYSIZE);
    visinfo = glXChooseVisual(glxglue_get_display(), DefaultScreen(glxglue_get_display()),
                              attrs);
    trynum++;
  }

  if (!visinfo) {
    cc_debugerror_postwarning("glxglue_find_gl_visual",
                              "Couldn't get any OpenGL-capable RGBA X11 visual.");
    return NULL;
  }

  return visinfo;
#undef ARRAYSIZE
}

/*** GLX offscreen contexts **************************************************/

struct glxglue_contextdata {
  XVisualInfo * visinfo;
  GLXContext glxcontext;
  unsigned int width, height;
  Pixmap pixmap;
  GLXPixmap glxpixmap;

  Display * storeddisplay;
  GLXDrawable storeddrawable;
  GLXContext storedcontext;
  SbBool pbuffer;
};

static struct glxglue_contextdata *
glxglue_contextdata_init(unsigned int width, unsigned int height)
{
  struct glxglue_contextdata * ctx;

  XVisualInfo * vi = glxglue_find_gl_visual();
  if (vi == NULL) { return NULL; }

  ctx = (struct glxglue_contextdata *)malloc(sizeof(struct glxglue_contextdata));

  ctx->visinfo = vi;
  ctx->glxcontext = NULL;
  ctx->width = width;
  ctx->height = height;

  ctx->pixmap = 0;
  ctx->glxpixmap = 0;

  ctx->storeddisplay = NULL;
  ctx->storeddrawable = 0;
  ctx->storedcontext = NULL;
  ctx->pbuffer = FALSE;

  return ctx;
}

static void
glxglue_contextdata_cleanup(struct glxglue_contextdata * c)
{
  if (c->glxcontext) glXDestroyContext(glxglue_get_display(), c->glxcontext);
  if (c->glxpixmap) {
    c->pbuffer ? glXDestroyPbuffer(glxglue_get_display(), c->glxpixmap) :
                 glXDestroyGLXPixmap(glxglue_get_display(), c->glxpixmap);
  }
  if (c->pixmap) XFreePixmap(glxglue_get_display(), c->pixmap);
  if (c->visinfo) XFree(c->visinfo);

  free(c);
}

static SbBool
glxglue_context_create_software(struct glxglue_contextdata * context)
{
  /* Note that the value of the last argument (which indicates whether
     or not we're asking for a DRI-capable context) is "False" on
     purpose, as the man pages for glXCreateContext() says:
    
          [...] direct rendering contexts [...] may be unable to
          render to GLX pixmaps [...]
    
     Rendering to a GLX pixmap is of course exactly what we want to be
     able to do. */
  context->glxcontext = glXCreateContext(glxglue_get_display(), context->visinfo, 0,
                                         False);

  if (context->glxcontext == NULL) {
    cc_debugerror_postwarning("glxglue_context_create_software",
                              "Couldn't create GLX context.");
    glxglue_contextdata_cleanup(context);
    return FALSE;
  }
  
  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_context_create_software",
                           "made new offscreen context == %p",
                           context->glxcontext);
  }

  context->pixmap = XCreatePixmap(glxglue_get_display(),
                                  DefaultRootWindow(glxglue_get_display()),
                                  context->width, context->height, context->visinfo->depth);
  if (context->pixmap == 0) {
    cc_debugerror_postwarning("glxglue_context_create_software",
                              "Couldn't create %dx%dx%d X11 Pixmap.",
                              context->width, context->height, context->visinfo->depth);
    glxglue_contextdata_cleanup(context);
    return FALSE;
  }
  
  context->glxpixmap = glXCreateGLXPixmap(glxglue_get_display(),
                                          context->visinfo, context->pixmap);
  if (context->glxpixmap == 0) {
    cc_debugerror_postwarning("glxglue_context_create_software",
                              "Couldn't create GLX Pixmap.");
    glxglue_contextdata_cleanup(context);
    return FALSE;
  }

  return TRUE;
}

// XXX
#include <stdio.h>

static COIN_GLXPbuffer
glxglue_glXCreatePbuffer(Display * dpy, COIN_GLXFBConfig config, int width, int height)
{
  if (glxglue_glXCreatePbuffer_GLX_1_3) {
    const int attrs[] = {
      GLX_PBUFFER_WIDTH, width,
      GLX_PBUFFER_HEIGHT, height,
      None
    };

    return glxglue_glXCreatePbuffer_GLX_1_3(dpy, config, attrs);
  }

  assert(glxglue_glXCreateGLXPbufferSGIX);
  return glxglue_glXCreateGLXPbufferSGIX(dpy, config, width, height, NULL);
}

static SbBool
glxglue_context_create_pbuffer(struct glxglue_contextdata * context)
{
  COIN_GLXPbuffer pb;
  COIN_GLXFBConfig * fbc;
  Display * dpy;

  /* number of FBConfigs returned */
  int fbc_cnt;

  /* set frame buffer attributes */
  const int attrs[] = {
    /* FIXME: if I enable any of these attributes on my system, the
       glXChooseFBConfig() call fails (i.e. produces no valid
       configs). 20030808 mortene. */
/*     GLX_RGBA_BIT, */
/*     GLX_RED_SIZE,   8, */
/*     GLX_GREEN_SIZE, 8, */
/*     GLX_BLUE_SIZE,  8, */
/*     GLX_ALPHA_SIZE, 8, */
    GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
/*     GLX_DEPTH_SIZE, 24, */
    None
  };

  dpy = glxglue_get_display();
  if (!dpy) { return FALSE; }

  /* get frame buffer configuration */
  fbc = glxglue_glXChooseFBConfig(dpy, DefaultScreen(dpy), attrs, &fbc_cnt);
  assert(fbc_cnt >= 0);
  if ((fbc_cnt == 0) || (fbc == NULL)) {
    cc_debugerror_postwarning("glxglue_context_create_pbuffer",
                              "glXChooseFBConfig() gave no valid configs");
    return FALSE;
  }

  /* FIXME: always config[0]? Why? 20030808 mortene. */
  pb = glxglue_glXCreatePbuffer(dpy, fbc[0], context->width, context->height);

  if (pb == 0) {
    cc_debugerror_postwarning("glxglue_context_create_pbuffer",
                              "glXCreatePbuffer(..., ..., %d, %d) failed",
                              context->width, context->height);
    return FALSE;
  }

  /* FIXME: shouldn't we error check the return value of the next
     call? Find function docs and investigate. 20030807 mortene. */

  /* direct rendering graphic context creation == Hardware use */

  context->glxcontext = glxglue_glXCreateNewContext(dpy, fbc[0],
                                                    GLX_RGBA_TYPE, NULL, TRUE);
  
  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_context_create_pbuffer",
                           "made new pbuffer offscreen context == %p",
                           context->glxcontext);
  }

  /* assign our pbuffer to glxpixmap */
  context->glxpixmap = pb;
  context->pbuffer = TRUE;

  return TRUE;
}

/* Create and return a handle to an offscreen OpenGL buffer.

   Where p-buffer support is available that will be used instead of a
   standard offscreen GLX context, as it should render much faster
   (due to hardware acceleration).

   See: http://www.oss.sgi.com/projects/ogl-sample/registry/SGIX/pbuffer.txt

   The initial pbuffer implementation was contributed by Tamer Fahmy
   and Hannes Kaufmann.
*/
void *
glxglue_context_create_offscreen(unsigned int width, unsigned int height)
{
  SbBool ok, pbuffer;
  struct glxglue_contextdata * swctx, * pbctx;

  swctx = glxglue_contextdata_init(width, height);
  if (swctx == NULL) { return NULL; }

  if (glxglue_context_create != NULL) {
    ok = glxglue_context_create(swctx);
    if (ok) { return swctx; }

    glxglue_contextdata_cleanup(swctx);
    return NULL;
  }

  /* As there could possibly be no valid glx context at this moment,
     we have to first make a context and set it current to be able
     to query pbuffer extension availability. */

  ok = glxglue_context_create_software(swctx);
  if (!ok || !glxglue_context_make_current(swctx)) {
    glxglue_contextdata_cleanup(swctx);
    return NULL;
  }

  /* ok, so we can at least use a non-pbuffer offscreen context */
  glxglue_context_create = glxglue_context_create_software;

  /* next, check if pbuffer support is available in the OpenGL
     library image */

  pbuffer = glxglue_has_pbuffer_support();

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_context_create_offscreen",
                           "PBuffer offscreen rendering is %ssupported "
                           "by the OpenGL driver", pbuffer ? "" : "NOT ");
  }

  glxglue_context_reinstate_previous(swctx);

  if (!pbuffer) { return swctx; }

  /* attempt to make a pbuffer, to make sure the system is actually
     set up with that capability (just having the GLX methods
     available doesn't really prove it) */

  pbctx = glxglue_contextdata_init(width, height);
  if (pbctx == NULL) { return swctx; }

  ok = glxglue_context_create_pbuffer(pbctx);

  if (!ok) {
    glxglue_contextdata_cleanup(pbctx);
    return swctx;
  }

  /* pbuffers are really supported, kill the software offscreen
     context and use the pbuffer-enabled one */

  glxglue_contextdata_cleanup(swctx);

  glxglue_context_create = glxglue_context_create_pbuffer;

  return pbctx;
}

SbBool
glxglue_context_make_current(void * ctx)
{
  struct glxglue_contextdata * context = (struct glxglue_contextdata *)ctx;
  Bool r;

  context->storedcontext = glXGetCurrentContext();
  if (context->storedcontext) {
    /* Must know for sure that there's a current context before
       instantiating a glglue, or we'll get a crash due to the OpenGL
       calls within GLWrapper(). */
    const cc_glglue * glw = cc_glglue_instance_from_context_ptr(context->storedcontext);
    context->storeddisplay = (Display *)cc_glglue_glXGetCurrentDisplay(glw);
    context->storeddrawable = glXGetCurrentDrawable();
  }

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_make_context_current",
                           "store current status first => context==%p, "
                           "drawable==%p, display==%p",
                           context->storedcontext,
                           context->storeddrawable,
                           context->storeddisplay);
  }

  r = glXMakeCurrent(glxglue_get_display(), context->glxpixmap, context->glxcontext);

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_make_context_current",
                           "%ssuccessfully made context %p current",
                           (r == True) ? "" : "un", context->glxcontext);
  }

  return (r == True) ? TRUE : FALSE;
}

void
glxglue_context_reinstate_previous(void * ctx)
{
  struct glxglue_contextdata * context = (struct glxglue_contextdata *)ctx;

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_context_reinstate_previous",
                           "releasing context (glxMakeCurrent(d, None, NULL))");
  }

  (void)glXMakeCurrent(glxglue_get_display(), None, NULL); /* release */
    
  /* The previous context is stored and reset to make it possible to
     use an SoOffscreenRenderer from for instance an SoCallback node
     callback during SoGLRenderAction traversal, without the need for
     any extra book-keeping on the application side. */
  
  if (context->storedcontext && context->storeddrawable && context->storeddisplay) {
    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("glxglue_context_reinstate_previous",
                             "restoring context %p to be current "
                             "(drawable==%p, display==%p)",
                             context->storedcontext,
                             context->storeddrawable,
                             context->storeddisplay);
    }

    /* FIXME: this causes a crash for the Mesa version 3.4.2 that
       comes with XFree86 v4, on the third invocation after two
       successful runs first. This is _bad_. 20020729 mortene.
      
       UPDATE: this might be our bug, and could be fixed now --
       test. 20020802 mortene. */
    (void)glXMakeCurrent(context->storeddisplay, context->storeddrawable,
                         context->storedcontext);
  }
}

void
glxglue_context_destruct(void * ctx)
{
  /* FIXME: needs to call into the (as of yet unimplemented)
     C wrapper around the SoContextHandler. 20030310 mortene. */

  struct glxglue_contextdata * context = (struct glxglue_contextdata *)ctx;

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_context_destruct",
                           "destroy context %p", context->glxcontext);
  }

  glxglue_contextdata_cleanup(context);
}

#endif /* HAVE_GLX */
