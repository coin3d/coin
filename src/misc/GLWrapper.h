#ifndef COIN_GLWRAPPER_H
#define COIN_GLWRAPPER_H

// FIXME: sooffscreenrenderer
// FIXME: soglpolygonoffsetelement

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

#ifdef COIN_INTERNAL
#include <Inventor/system/gl.h>
#include <Inventor/SbDict.h>
#else
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

#ifdef HAVE_GLX
#include <GL/glx.h>
#endif /* HAVE_GLX */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Our own enum definitions */
#define GL_PACK_SKIP_IMAGES               0x806B
#define GL_PACK_SKIP_IMAGES_EXT           0x806B
#define GL_PACK_IMAGE_HEIGHT              0x806C
#define GL_PACK_IMAGE_HEIGHT_EXT          0x806C
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_SKIP_IMAGES_EXT         0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_UNPACK_IMAGE_HEIGHT_EXT        0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_TEXTURE_3D_EXT                 0x806F
#define GL_PROXY_TEXTURE_3D               0x8070
#define GL_PROXY_TEXTURE_3D_EXT           0x8070
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_DEPTH_EXT              0x8071
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_TEXTURE_WRAP_R_EXT             0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_MAX_3D_TEXTURE_SIZE_EXT        0x8073
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_CLAMP_TO_EDGE_SGIS             0x812F
//  #define GL_TEXTURE_GEN_R
//  #define GL_TEXTURE_GEN_Q

/* Our own typedefs for OpenGL functions. These are copied from
   glext.h and prefixed with COIN_ to avoid namespace collisions. */
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * COIN_PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * COIN_PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * COIN_PFNGLPOLYGONOFFSETPROC) (GLfloat factor, GLfloat bias);
typedef void (APIENTRY * COIN_PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (APIENTRY * COIN_PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRY * COIN_PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);

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

  void *libhandle; // Only used in case of dlopen()/dlsym() binding

  /* Our own "enums". Will be 0 if not available, otherwise they
     contain a valid GLenum. */
  GLenum COIN_GL_TEXTURE_3D;
  GLenum COIN_GL_PROXY_TEXTURE_3D;
  GLenum COIN_GL_TEXTURE_WRAP_R;
  GLenum COIN_GL_TEXTURE_DEPTH;
  GLenum COIN_GL_MAX_3D_TEXTURE_SIZE;
  GLenum COIN_GL_PACK_IMAGE_HEIGHT;
  GLenum COIN_GL_UNPACK_IMAGE_HEIGHT;
  GLenum COIN_GL_PACK_SKIP_IMAGES;
  GLenum COIN_GL_UNPACK_SKIP_IMAGES;
  //FIXME: Check OpenGL 1.1 availability of these
  GLenum COIN_GL_TEXTURE_GEN_R;
  GLenum COIN_GL_TEXTURE_GEN_Q;

  GLenum COIN_GL_CLAMP_TO_EDGE;

  /* OpenGL calls. Will be NULL if not available, otherwise they
     contain a valid function pointer into the OpenGL library. */
  COIN_PFNGLTEXIMAGE3DPROC glTexImage3D;
  COIN_PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
  COIN_PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D; 
  COIN_PFNGLPOLYGONOFFSETPROC glPolygonOffset;
  COIN_PFNGLBINDTEXTUREPROC glBindTexture;
  COIN_PFNGLDELETETEXTURESPROC glDeleteTextures;
  COIN_PFNGLGENTEXTURESPROC glGenTextures;

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
