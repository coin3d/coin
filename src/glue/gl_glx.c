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

#ifndef HAVE_GLX

void glxglue_init(cc_glglue * w) {
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

static int glxglue_screen = -1;
typedef void *(APIENTRY * COIN_PFNGLXGETPROCADDRESS)(const GLubyte *);
static COIN_PFNGLXGETPROCADDRESS glxglue_glXGetProcAddress = NULL;
static SbBool tried_bind_glXGetProcAddress = FALSE;

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
};

static void
glxglue_contextdata_init(struct glxglue_contextdata * c)
{
  c->visinfo = NULL;
  c->glxcontext = NULL;
  c->pixmap = 0;
  c->glxpixmap = 0;

  c->storeddisplay = NULL;
  c->storeddrawable = 0;
  c->storedcontext = NULL;
}

static void
glxglue_contextdata_cleanup(struct glxglue_contextdata * c)
{
  if (c->glxcontext) glXDestroyContext(glxglue_get_display(), c->glxcontext);
  if (c->glxpixmap) glXDestroyGLXPixmap(glxglue_get_display(), c->glxpixmap);
  if (c->pixmap) XFreePixmap(glxglue_get_display(), c->pixmap);
  if (c->visinfo) XFree(c->visinfo);
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
  const int ARRAYSIZE = 32;
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
}

void *
glxglue_context_create_offscreen(unsigned int width, unsigned int height)
{
  /* FIXME: on SGI boxes with p-buffer support, that should be used
     instead of a standard offscreen GLX context, as it would render
     much faster (due to hardware acceleration). 20020503 mortene. */

  struct glxglue_contextdata context, * retctx;
  glxglue_contextdata_init(&context);

  context.width = width;
  context.height = height;

  context.visinfo = glxglue_find_gl_visual();
  if (context.visinfo == NULL) { return NULL; }

  /* Note that the value of the last argument (which indicates whether
     or not we're asking for a DRI-capable context) is "False" on
     purpose, as the man pages for glXCreateContext() says:
    
          [...] direct rendering contexts [...] may be unable to
          render to GLX pixmaps [...]
    
     Rendering to a GLX pixmap is of course exactly what we want to be
     able to do. */
  context.glxcontext = glXCreateContext(glxglue_get_display(), context.visinfo, 0,
                                        False);
  if (context.glxcontext == NULL) {
    cc_debugerror_postwarning("glxglue_create_offscreen_context",
                              "Couldn't create GLX context.");
    glxglue_contextdata_cleanup(&context);
    return NULL;
  }

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_create_offscreen_context",
                           "made new offscreen context == %p",
                           context.glxcontext);
  }

  context.pixmap = XCreatePixmap(glxglue_get_display(),
                                 DefaultRootWindow(glxglue_get_display()),
                                 width, height, context.visinfo->depth);
  if (context.pixmap == 0) {
    cc_debugerror_postwarning("glxglue_create_offscreen_context",
                              "Couldn't create %dx%dx%d X11 Pixmap.",
                              width, height, context.visinfo->depth);
    glxglue_contextdata_cleanup(&context);
    return NULL;
  }

  context.glxpixmap = glXCreateGLXPixmap(glxglue_get_display(),
                                         context.visinfo, context.pixmap);
  if (context.glxpixmap == 0) {
    cc_debugerror_postwarning("glxglue_create_offscreen_context",
                              "Couldn't create GLX Pixmap.");
    glxglue_contextdata_cleanup(&context);
    return NULL;
  }

  retctx = (struct glxglue_contextdata *)malloc(sizeof(struct glxglue_contextdata));
  (void)memcpy(retctx, &context, sizeof(struct glxglue_contextdata));
  return retctx;
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
  struct glxglue_contextdata * context = (struct glxglue_contextdata *)ctx;

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_context_destruct",
                           "destroy context %p", context->glxcontext);
  }

  glxglue_contextdata_cleanup(context);
  free(context);
}

#endif /* HAVE_GLX */
