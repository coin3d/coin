#ifndef COIN_GLUE_GLP_H
#define COIN_GLUE_GLP_H

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

#ifndef COIN_INTERNAL
#error You have tried to use one of the private Coin header files
#endif /* ! COIN_INTERNAL */

/* NOTE: do *not* include this header file outside of the gl.c OpenGL
   wrapper -- even if you're inside Coin library implementation
   code. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_GLX
#include <GL/glx.h>
#endif /* HAVE_GLX */

#include <Inventor/system/gl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */



/* Under Win32, we need to make sure we use the correct calling method
   by using the APIENTRY define for the function signature types (or
   else we'll get weird stack errors). On other platforms, just define
   APIENTRY empty. */
#ifndef APIENTRY
#define APIENTRY
#endif /* !APIENTRY */

/* Our own typedefs for OpenGL functions. Prefixed with COIN_ to avoid
   namespace collisions. */
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DPROC)(GLenum target, 
                                                   GLint level, 
                                                   GLenum internalformat, 
                                                   GLsizei width, 
                                                   GLsizei height, 
                                                   GLsizei depth, 
                                                   GLint border, 
                                                   GLenum format, 
                                                   GLenum type, 
                                                   const GLvoid * pixels);

typedef void (APIENTRY * COIN_PFNGLTEXSUBIMAGE3DPROC)(GLenum target, 
                                                      GLint level, 
                                                      GLint xoffset, 
                                                      GLint yoffset, 
                                                      GLint zoffset, 
                                                      GLsizei width, 
                                                      GLsizei height, 
                                                      GLsizei depth, 
                                                      GLenum format, 
                                                      GLenum type, 
                                                      const GLvoid * pixels);

typedef void (APIENTRY * COIN_PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum target, 
                                                          GLint level, 
                                                          GLint xoffset, 
                                                          GLint yoffset, 
                                                          GLint zoffset, 
                                                          GLint x, 
                                                          GLint y, 
                                                          GLsizei width, 
                                                          GLsizei height);

typedef void (APIENTRY * COIN_PFNGLPOLYGONOFFSETPROC)(GLfloat factor, 
                                                      GLfloat bias);

typedef void (APIENTRY * COIN_PFNGLBINDTEXTUREPROC)(GLenum target, 
                                                    GLuint texture);

typedef void (APIENTRY * COIN_PFNGLDELETETEXTURESPROC)(GLsizei n, 
                                                       const GLuint * textures);

typedef void (APIENTRY * COIN_PFNGLGENTEXTURESPROC)(GLsizei n, 
                                                    GLuint *textures);

typedef void (APIENTRY * COIN_PFNGLTEXSUBIMAGE2DPROC)(GLenum target, 
                                                      GLint level, 
                                                      GLint xoffset, 
                                                      GLint yoffset, 
                                                      GLsizei width, 
                                                      GLsizei height, 
                                                      GLenum format, 
                                                      GLenum type, 
                                                      const GLvoid * pixels);

typedef void (APIENTRY * COIN_PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (APIENTRY * COIN_PFNGLMULTITEXCOORD2FPROC)(GLenum target,
                                                        GLfloat s,
                                                        GLfloat t);

/* Typedefs for GLX functions. */
typedef void *(APIENTRY * COIN_PFNGLXGETCURRENTDISPLAYPROC)(void);


/* Type specification for GLX info storage structure, embedded witin
   the main GL info structure below. */
struct cc_glxglue {
  struct {
    int major, minor;
  } version;

  SbBool isdirect;

  const char * serverversion;
  const char * servervendor;
  const char * serverextensions;

  const char * clientversion;
  const char * clientvendor;
  const char * clientextensions;

  const char * glxextensions;
};

/* GL info storage structure. An instance will be allocated and
   initialized for each new GL context id. */
struct cc_glglue {
  
  struct { /* OpenGL versioning. */
    unsigned int major, minor, release;
  } version;

  /* OpenGL calls. Will be NULL if not available, otherwise they
     contain a valid function pointer into the OpenGL library. */
  COIN_PFNGLPOLYGONOFFSETPROC glPolygonOffset;
  COIN_PFNGLPOLYGONOFFSETPROC glPolygonOffsetEXT;
  COIN_PFNGLGENTEXTURESPROC glGenTextures;
  COIN_PFNGLBINDTEXTUREPROC glBindTexture;
  COIN_PFNGLDELETETEXTURESPROC glDeleteTextures;
  COIN_PFNGLTEXIMAGE3DPROC glTexImage3D;
  COIN_PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
  COIN_PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
  COIN_PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
  COIN_PFNGLACTIVETEXTUREPROC glActiveTexture;
  COIN_PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
  COIN_PFNGLXGETCURRENTDISPLAYPROC glXGetCurrentDisplay;

  const char * vendorstr;
  SbBool vendor_is_SGI;
  const char * rendererstr;
  const char * extensionsstr;

  struct cc_glxglue glx;
};

/* Exported internally to gl_glx.c and gl_wgl.c. */
int coin_glglue_debug(void);
int coin_glglue_extension_available(const char * extensions, const char * ext);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COIN_GLUE_GLP_H */
