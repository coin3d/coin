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
#include <Inventor/C/errors/debugerror.h>


void *
coin_wgl_getprocaddress(const char * fname)
{
#ifdef HAVE_WGL
  void * ptr = wglGetProcAddress(fname);

  /* wglGetProcAddress() seems to only be able to fetch
     function-addresses for *extension* functions, not "proper" OpenGL
     (1.1+) functions. */

  if (ptr == NULL) {
    /* check for function in opengl32.dll.
     *
     * FIXME: Is it sufficient to just check opengl32.dll? pederb, 2002-07-12
     *
     * UPDATE 2002-07-12 kintel: I think not, as the function pointers
     * that should be used are dependent upon 1) the current
     * framebuffer configuration and 2) the ICD driver installed. You
     * might be lucky though...
     *
     * UPDATE 2002-09-11 mortene: should at least replace
     * "opengl32.dll" with the OpenGL library name found by the
     * configure script.
     */

    /* If this worked like dlopen() on UNIX-like systems with libdl,
     * we could just get the executable handle by passing NULL to
     * GetModuleHandle(), and then get any symbol from that. That
     * doesn't work, though, as GetProcAddress() will *only* resolve
     * symbols from the specific DLL module handle.
     */
    HINSTANCE glhandle = GetModuleHandle("opengl32.dll");

    if (!glhandle && coin_glglue_debug()) {
      cc_debugerror_postwarning("coin_wgl_getprocaddress",
                                "couldn't get hold of module handle for "
                                "\"opengl32.dll\"");
    }

    if (glhandle) {
      ptr = (void *)GetProcAddress(glhandle, fname);

      if (ptr && coin_glglue_debug()) {
        cc_debugerror_postinfo("coin_wgl_getprocaddress",
                               "wglGetProcAddress() missed \"%s\", "
                               "but found with GetProcAddress()", fname);
      }
    }
  }
  return ptr;
#endif /* HAVE_WGL */
  return NULL;
}
