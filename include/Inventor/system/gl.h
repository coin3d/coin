#ifndef COIN_GL_H
#define COIN_GL_H

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
   OpenInventor/?  (that *was* irony) */
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
  Some systems have very old gl.h files, and other systems include
  extension enums which we want to use if we run-time detect the
  extension function calls to be available, so we just define these
  values here if not defined already.
*/

/* Note: GL_CLAMP_TO_EDGE_EXT and GL_CLAMP_TO_EDGE_SGIS have the same
   enum values as GL_CLAMP_TO_EDGE. We only use the "real" enum
   name. */

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE                  ((GLenum)0x812F)
#endif /* !GL_CLAMP_TO_EDGE */


/* Note: all following enums also have an *_EXT version with the same
   enum value as the "real" enum. We only use the "real" enum name in
   Coin code. */

#ifndef GL_MAX_3D_TEXTURE_SIZE
#define GL_MAX_3D_TEXTURE_SIZE            ((GLenum)0x8073)
#endif /* !GL_MAX_3D_TEXTURE_SIZE */
#ifndef GL_PACK_IMAGE_HEIGHT
#define GL_PACK_IMAGE_HEIGHT              ((GLenum)0x806C)
#endif /* !GL_PACK_IMAGE_HEIGHT */
#ifndef GL_PACK_SKIP_IMAGES
#define GL_PACK_SKIP_IMAGES               ((GLenum)0x806B)
#endif /* !GL_PACK_SKIP_IMAGES */
#ifndef GL_PROXY_TEXTURE_2D
#define GL_PROXY_TEXTURE_2D               ((GLenum)0x8064)
#endif /* !GL_PROXY_TEXTURE_2D */
#ifndef GL_PROXY_TEXTURE_3D
#define GL_PROXY_TEXTURE_3D               ((GLenum)0x8070)
#endif /* !GL_PROXY_TEXTURE_3D */
#ifndef GL_TEXTURE_3D
#define GL_TEXTURE_3D                     ((GLenum)0x806F)
#endif /* !GL_TEXTURE_3D */
#ifndef GL_TEXTURE_DEPTH
#define GL_TEXTURE_DEPTH                  ((GLenum)0x8071)
#endif /* !GL_TEXTURE_DEPTH */
#ifndef GL_TEXTURE_WRAP_R
#define GL_TEXTURE_WRAP_R                 ((GLenum)0x8072)
#endif /* !GL_TEXTURE_WRAP_R */
#ifndef GL_UNPACK_IMAGE_HEIGHT
#define GL_UNPACK_IMAGE_HEIGHT            ((GLenum)0x806E)
#endif /* !GL_UNPACK_IMAGE_HEIGHT */
#ifndef GL_UNPACK_SKIP_IMAGES
#define GL_UNPACK_SKIP_IMAGES             ((GLenum)0x806D)
#endif /* !GL_UNPACK_SKIP_IMAGES */

#endif /* ! COIN_GL_H */
