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

#include <Inventor/C/basic.h>
#include "gl_glx.h"
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/errors/debugerror.h>

static void * glxglue_display = NULL;
static int glxglue_screen = -1;
typedef void *(APIENTRY * COIN_PFNGLXGETPROCADDRESS)(const GLubyte *);
static COIN_PFNGLXGETPROCADDRESS glxglue_glXGetProcAddress = NULL;
static SbBool tried_bind_glXGetProcAddress = FALSE;


static void
glxglue_set_version(int * major, int * minor)
{
#ifdef HAVE_GLX
  Bool ok = False;
#endif /* HAVE_GLX */

  *major = -1;
  *minor = 0;

  if (glxglue_display == NULL) { return; }

#ifdef HAVE_GLX
  ok = glXQueryVersion((Display *)glxglue_display, major, minor);

  if (!ok) {
    cc_debugerror_post("glxglue_version",
                       "Couldn't decide GLX version on your system!");
  }

  if (ok && coin_glglue_debug()) {
    cc_debugerror_postinfo("glxglue_version",
                           "GLX version: %d.%d", *major, *minor);
  }
#endif /* HAVE_GLX */
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
#ifdef HAVE_GLX
  GLXContext ctx = glXGetCurrentContext();

  if (!ctx) {
    cc_debugerror_postwarning("glxglue_isdirect", 
                              "Couldn't get current GLX context.");
    return TRUE;
  }

  if (!glxglue_display) return TRUE;
  return glXIsDirect((Display *)glxglue_display, ctx) ? TRUE : FALSE;
#else /* ! HAVE_GLX */
  return TRUE;
#endif /* ! HAVE_GLX */
}

int
glxglue_ext_supported(const cc_glglue * w, const char * extension)
{
  return coin_glglue_extension_available(w->glx.glxextensions, extension);
}

void
glxglue_init(cc_glglue * w)
{
#ifdef HAVE_GLX
  if (glxglue_display == NULL) {
    /* FIXME: should use the real display-setting. :-(  20020926 mortene. */
    glxglue_display = XOpenDisplay(NULL);
    if (glxglue_display == NULL) {
      cc_debugerror_post("glxglue_init",
                         "Couldn't open NULL display.");
    }
    else {
      /* FIXME: should use the real screen number. :-(  20020926 mortene. */
      glxglue_screen = XDefaultScreen((Display *)glxglue_display);
    }
  }
#endif /* HAVE_GLX */

  /* The Display resource is never deallocated explicitly (but of
   * course implicitly by the system on application close down). This
   * to work around some strange problems with the NVidia-driver 29.60
   * on XFree86 v4 when using XCloseDisplay() -- like doublebuffered
   * visuals coming up just blank.
   *
   *   XCloseDisplay((Display *)glxglue_display);
   */

  glxglue_set_version(&w->glx.version.major, &w->glx.version.minor);
  w->glx.isdirect = glxglue_isdirect(w);


  w->glx.serverversion = NULL;
  w->glx.servervendor = NULL;
  w->glx.serverextensions = NULL;
  w->glx.clientversion = NULL;
  w->glx.clientvendor = NULL;
  w->glx.clientextensions = NULL;
  w->glx.glxextensions = NULL;
#ifdef HAVE_GLX
  if (glxglue_display) {

    /* Note: be aware that glXQueryServerString(),
       glXGetClientString() and glXQueryExtensionsString() are all
       from GLX 1.1 -- just in case there are ever compile-time,
       link-time or run-time problems with this.  */

    Display * d = (Display *)glxglue_display;
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
#endif /* HAVE_GLX */
}
