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
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/C/errors/debugerror.h>

typedef void *(APIENTRY * COIN_PFNGLXGETPROCADDRESS)(const GLubyte *);
static COIN_PFNGLXGETPROCADDRESS coin_glx_glXGetProcAddress = NULL;
static SbBool tried_bind_glXGetProcAddress = FALSE;

void *
coin_glx_getprocaddress(const char * fname)
{
  void * ptr = NULL;

  if (!coin_glx_glXGetProcAddress && !tried_bind_glXGetProcAddress) {
    cc_libhandle h = cc_dl_open(NULL);
    if (h) {
      coin_glx_glXGetProcAddress = (COIN_PFNGLXGETPROCADDRESS)
        cc_dl_sym(h, "glXGetProcAddress");

      if (coin_glglue_debug()) {
        cc_debugerror_postinfo("coin_glx_getprocaddress",
                               "%s glXGetProcAddress()",
                               coin_glx_glXGetProcAddress ?
                               "picked up" : "can't use");
      }

      if (!coin_glx_glXGetProcAddress) {
        coin_glx_glXGetProcAddress = (COIN_PFNGLXGETPROCADDRESS)
          cc_dl_sym(h, "glXGetProcAddressARB");
      }

      if (coin_glglue_debug()) {
        cc_debugerror_postinfo("coin_glx_getprocaddress",
                               "%s glXGetProcAddressARB()",
                               coin_glx_glXGetProcAddress ?
                               "picked up" : "can't use");
      }


      cc_dl_close(h);
    }

    /* FIXME: use static binding if cc_dl_open() has no native method
       to use? 20020906 mortene. */

    tried_bind_glXGetProcAddress = TRUE;
  }

  if (coin_glx_glXGetProcAddress) {
    ptr = (void *)coin_glx_glXGetProcAddress((const GLubyte *)fname);
  }

  return ptr;
}


/*
  Find GLX version.
*/
void
coin_glx_version(int * major, int * minor)
{
#ifndef HAVE_GLX
  *major = -1;
  *minor = 0;
#else /* HAVE_GLX */
  Display * display = XOpenDisplay(NULL);
  Bool ok = False;

  static SbBool first = TRUE;
  if (!first) { return; }
  first = FALSE;

  if (display) {
    ok = glXQueryVersion(display, major, minor);
    /* The Display resources is never deallocated explicitly (but of
     * course implicitly by the system on application close
     * down). This to work around some strange problems with the
     * NVidia-driver 29.60 on XFree86 v4 when using XCloseDisplay() --
     * like doublebuffered visuals not working correctly.
     *
     *   XCloseDisplay(display); */
  }

  if (!ok) {
    cc_debugerror_post("coin_glx_version",
                       "couldn't decide GLX version on your system -- ai!%s",
                       display == NULL ? " (couldn't open NULL display)" : "");
  }

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("coin_glx_version",
                           "GLX version: %d.%d", *major, *minor);
  }
#endif /* HAVE_GLX */
}
