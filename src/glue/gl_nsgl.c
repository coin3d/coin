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

#include "gl_nsgl.h"
#include <stdlib.h>
#include <string.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/C/errors/debugerror.h>

// FIXME: We probably should use a configure-time check for the
// availability of the NSSymbol functions instead of a simple platform
// check. Since these functions are part of the core Mac OS X system, this 
// should be okay for the time being, though. kyrah 20030115
#ifdef __APPLE__

#include <OpenGL/gl.h>
#include <mach-o/dyld.h>

/* There is no native aglGetProcAddress() or NSGLGetProcAddress(), but
 * you can use NSLookupAndBindSymbol() to get the address of an
 * OpenGL entry point. Note that the NSSymbol functions are part of
 * the System framework and *not* Cocoa (despite their misleading
 * names.
 */
void * NSGLGetProcAddress(const char * name) 
{
  NSSymbol symbol;
  char *symbolName;
  symbolName = malloc(strlen(name) + 2);
  strcpy(symbolName + 1, name);
  symbolName[0] = '_';
  symbol = NULL;
  if (NSIsSymbolNameDefined(symbolName)) {
    symbol = NSLookupAndBindSymbol(symbolName);
  }
  free (symbolName);
  return symbol ? NSAddressOfSymbol(symbol) : NULL;
}

#endif // __APPLE__

void *
coin_nsgl_getprocaddress(const char * fname)
{

#ifdef __APPLE__
  void * ptr = NSGLGetProcAddress(fname);
  return ptr;
#endif // __APPLE__

  return NULL;
}
