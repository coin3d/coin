#ifndef COIN_GLWRAPPER_H
#define COIN_GLWRAPPER_H

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


/* Under Win32, we need to make sure we use the correct calling method
   by using the APIENTRY define for the function signature types (or
   else we'll get weird stack errors). On other platforms, just define
   APIENTRY empty. */
#ifndef APIENTRY
#define APIENTRY
#endif /* !APIENTRY */

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/system/gl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Our own typedefs for OpenGL functions. These are copied from
   glext.h and prefixed with COIN_ to avoid namespace collisions. */
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);

typedef struct {
  /* OpenGL versioning. */
  struct {
    unsigned int major, minor, release;
  } version;

  /* OpenGL calls. Will be NULL if not available, otherwise they
     contain a valid function pointer into the OpenGL library. */
  COIN_PFNGLTEXIMAGE3DPROC glTexImage3D;
  COIN_PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
} GLWrapper_t;

const GLWrapper_t * GLWrapper(int contextid);

/*
  Returns 1 if the OpenGL implementation of the wrapper context is at
  least as "late" as what is given with the input arguments. Otherwise
  returns 0.
 */
int GLWrapper_versionMatchesAtLeast(GLWrapper_t * wrapper,
                                    unsigned int major,
                                    unsigned int minor,
                                    unsigned int release);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLWRAPPER_H */
