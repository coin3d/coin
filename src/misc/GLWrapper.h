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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/system/gl.h>
#include <Inventor/SbBasic.h>

#ifdef HAVE_GLX
#include <GL/glx.h>
#endif /* HAVE_GLX */

/* Under Win32, we need to make sure we use the correct calling method
   by using the APIENTRY define for the function signature types (or
   else we'll get weird stack errors). On other platforms, just define
   APIENTRY empty. */
#ifndef APIENTRY
#define APIENTRY
#endif /* !APIENTRY */

#define GLWRAPPER_FROM_STATE(state) \
  GLWrapper(((SoGLRenderAction *)state->getAction())->getCacheContext())

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Our own typedefs for OpenGL functions. These are copied from
   glext.h and prefixed with COIN_ to avoid namespace collisions. */
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * COIN_PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * COIN_PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * COIN_PFNGLPOLYGONOFFSETPROC) (GLfloat factor, GLfloat bias);
typedef void (APIENTRY * COIN_PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (APIENTRY * COIN_PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRY * COIN_PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
typedef void (APIENTRY * COIN_PFNGLTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);


#ifdef HAVE_GLX
/* GLX functions */
typedef void *(APIENTRY * COIN_PFNGLXGETPROCADDRESSARB) (const GLubyte *procName);
typedef Display *(APIENTRY * COIN_PFNGLXGETCURRENTDISPLAYPROC) (void);
#endif

typedef struct {
  /* OpenGL versioning. */
  struct {
    unsigned int major, minor, release;
  } glVersion;

  struct {
    int major, minor;
  } glxVersion;

  void * libhandle; /* Only used in case of dlopen()/dlsym() binding */

  /* Capability flags for features of the underlying OpenGL implementation. */
  SbBool has3DTextures;
  SbBool has2DProxyTextures;
  SbBool has3DProxyTextures;
  SbBool hasTextureEdgeClamp;

  /* OpenGL calls. Will be NULL if not available, otherwise they
     contain a valid function pointer into the OpenGL library. */
  COIN_PFNGLTEXIMAGE3DPROC glTexImage3D;
  COIN_PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
  COIN_PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
  COIN_PFNGLPOLYGONOFFSETPROC glPolygonOffset;
  COIN_PFNGLBINDTEXTUREPROC glBindTexture;
  COIN_PFNGLDELETETEXTURESPROC glDeleteTextures;
  COIN_PFNGLGENTEXTURESPROC glGenTextures;
  COIN_PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;

#ifdef HAVE_GLX
  COIN_PFNGLXGETPROCADDRESSARB glXGetProcAddressARB;
  COIN_PFNGLXGETCURRENTDISPLAYPROC glXGetCurrentDisplay;
#endif

} GLWrapper_t;

/*
  Returns the wrapper instance for the given context ID (defined
  by SoGLCacheContextElement).
 */
const GLWrapper_t * GLWrapper(int contextid);

/*
  Returns 1 if the OpenGL implementation of the wrapper context is at
  least as "late" as what is given with the input arguments. Otherwise
  returns 0.
 */
int GLWrapper_glVersionMatchesAtLeast(const GLWrapper_t * wrapper,
                                      unsigned int major,
                                      unsigned int minor,
                                      unsigned int release);

/*
  Returns 1 if the GLX implementation of the wrapper context is at
  least as "late" as what is given with the input arguments. Otherwise
  returns 0.
 */
int GLWrapper_glxVersionMatchesAtLeast(const GLWrapper_t * wrapper,
                                       int major,
                                       int minor);

/*
  Returns 1 if the given extension is supported by this context,
  0 if not.
 */
int GLWrapper_glEXTSupported(GLWrapper_t * wrapper, const char * extname);

/*
  Returns 1 if the gived GLX extension is supported by this GLX implementation,
  0 if not.
 */
int GLWrapper_glxEXTSupported(GLWrapper_t * wrapper, const char * extname);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLWRAPPER_H */
