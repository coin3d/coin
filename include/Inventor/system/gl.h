/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_GL_H
#define COIN_GL_H

/* This header file is supposed to take care of all operating system
 * dependent anomalies connected to the gl.h include file.  It is only
 * for use while building the Coin library, and should not be installed
 * with the rest of the header files (at least not yet).  20010913 larsa */

#ifndef COIN_CONFIGURE_BUILD
#error "you must include config.h before including Inventor/system/gl.h"
#endif /* !COIN_CONFIGURE_BUILD */

#ifdef HAVE_WINDOWS_H
/* on windows, headers do not include their dependencies */
#include <windows.h>
#endif /* HAVE_WINDOWS_H */

#ifdef HAVE_GL_GL_H
/* the preferred gl.h path */
#include <GL/gl.h>
#else
#ifdef HAVE_OPENGL_GL_H
/* how Mac OS X organizes things - should we now stuff Coin header in
   OpenInventor/?  (that *was* ironi) */
#include <OpenGL/gl.h>
#else
#error "don't know how to include gl.h header"
#endif /* !HAVE_GL_GL_H */
#endif /* !HAVE_OPENGL_GL_H */

#ifdef HAVE_SUPERGLU
#include <superglu/include/superglu.h>
#else
#ifdef HAVE_GL_GLU_H
#include <GL/glu.h>
#else
#ifdef HAVE_OPENGL_GLU_H
#include <OpenGL/glu.h>
#endif
#endif
#endif

/*
  MS Windows often has very old gl.h files, so we just define these
  value here if not defined. Run-time checks are used to determine
  which feature to use.
*/

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE         0x812F
#endif /* GL_CLAMP_TO_EDGE */

#ifndef GL_CLAMP_TO_EDGE_EXT
#define GL_CLAMP_TO_EDGE_EXT     0x812F
#endif /* GL_CLAMP_TO_EDGE_EXT */

#ifndef GL_CLAMP_TO_EDGE_SGIS
#define GL_CLAMP_TO_EDGE_SGIS    0x812F
#endif /* GL_CLAMP_TO_EDGE_SGIS */

#endif /* ! COIN_GL_H */
