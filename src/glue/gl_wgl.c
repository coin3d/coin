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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>


void *
coin_wgl_getprocaddress(const char * fname)
{
#ifdef HAVE_WGL
  void * ptr = wglGetProcAddress(fname);
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
     * UPDATE 2002-09-06 mortene: couldn't we just get the executable
     * handle by passing NULL to GetModuleHandle()? Investigate.
     */

    HINSTANCE h = GetModuleHandle("opengl32.dll");
    if (h) { ptr = (void*) GetProcAddress(h, fname); }
  }
  return ptr;
#endif /* HAVE_WGL */
  return NULL;
}
