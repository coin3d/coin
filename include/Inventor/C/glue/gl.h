#ifndef COIN_GLUE_GL_H
#define COIN_GLUE_GL_H

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

#include <Inventor/system/gl.h>
#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

typedef struct cc_glglue cc_glglue;

/*
  Returns the glue instance for the given context ID.

  The context ID can be any number chosen to match the current OpenGL
  context in a _unique_ manner (this is important!).

  (Note: internally in Coin, we use the context ids defined by
  SoGLCacheContextElement. Make sure context ids from external code
  doesn't crash with those.)
*/
COIN_DLL_API const cc_glglue * cc_glglue_instance(int contextid);

/*
  Fetch version number information for the underlying OpenGL
  implementation.
*/
COIN_DLL_API void cc_glglue_glversion(const cc_glglue * glue,
                                      unsigned int * major,
                                      unsigned int * minor,
                                      unsigned int * release);

/*
  Returns TRUE if the OpenGL implementation of the wrapper context is
  at least as "late" as what is given with the input
  arguments. Otherwise returns FALSE.
*/
COIN_DLL_API SbBool cc_glglue_glversion_matches_at_least(const cc_glglue * glue,
                                                         unsigned int major,
                                                         unsigned int minor,
                                                         unsigned int release);

/*
  Returns TRUE if the GLX implementation of the wrapper context is at
  least as "late" as what is given with the input arguments. Otherwise
  returns FALSE.
*/
COIN_DLL_API SbBool cc_glglue_glxversion_matches_at_least(const cc_glglue * glue,
                                                          int major,
                                                          int minor);
/*
  Returns TRUE if the given extension is supported by this context,
  FALSE if not.
*/
COIN_DLL_API SbBool cc_glglue_glext_supported(const cc_glglue * glue, const char * extname);

/* Returns TRUE if rendering is done directly on the display (ie not
   through any software indirection layer over GLX). */
COIN_DLL_API SbBool cc_glglue_isdirect(const cc_glglue * w);

/*** Wrapped OpenGL 1.1+ features and extensions. *********************/

COIN_DLL_API SbBool cc_glglue_has_polygon_offset(const cc_glglue * glue);
/* Bitflags for the last argument of cc_glglue_glPolygonOffsetEnable(). */
enum cc_glglue_Primitives { cc_glglue_FILLED = 1 << 0,
                            cc_glglue_LINES  = 1 << 1,
                            cc_glglue_POINTS = 1 << 2 };
COIN_DLL_API void cc_glglue_glPolygonOffsetEnable(const cc_glglue * glue,
                                                  SbBool enable, int m);
COIN_DLL_API void cc_glglue_glPolygonOffset(const cc_glglue * glue,
                                            GLfloat factor,
                                            GLfloat units);

COIN_DLL_API SbBool cc_glglue_has_texture_objects(const cc_glglue * glue);
COIN_DLL_API void cc_glglue_glGenTextures(const cc_glglue * glue,
                                          GLsizei n,
                                          GLuint *textures);
COIN_DLL_API void cc_glglue_glBindTexture(const cc_glglue * glue,
                                          GLenum target,
                                          GLuint texture);
COIN_DLL_API void cc_glglue_glDeleteTextures(const cc_glglue * glue,
                                             GLsizei n,
                                             const GLuint * textures);


COIN_DLL_API SbBool cc_glglue_has_3d_textures(const cc_glglue * glue);
COIN_DLL_API void cc_glglue_glTexImage3D(const cc_glglue * glue,
                                         GLenum target,
                                         GLint level,
                                         GLenum internalformat,
                                         GLsizei width,
                                         GLsizei height,
                                         GLsizei depth,
                                         GLint border,
                                         GLenum format,
                                         GLenum type,
                                         const GLvoid *pixels);
COIN_DLL_API void cc_glglue_glTexSubImage3D(const cc_glglue * glue,
                                            GLenum target,
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
COIN_DLL_API void cc_glglue_glCopyTexSubImage3D(const cc_glglue * glue,
                                                GLenum target,
                                                GLint level,
                                                GLint xoffset,
                                                GLint yoffset,
                                                GLint zoffset,
                                                GLint x,
                                                GLint y,
                                                GLsizei width,
                                                GLsizei height);


COIN_DLL_API SbBool cc_glglue_has_multitexture(const cc_glglue * glue);
COIN_DLL_API void cc_glglue_glMultiTexCoord2f(const cc_glglue * glue,
                                              GLenum target,
                                              GLfloat s,
                                              GLfloat t);
COIN_DLL_API void cc_glglue_glActiveTexture(const cc_glglue * glue,
                                            GLenum texture);


COIN_DLL_API SbBool cc_glglue_has_texsubimage(const cc_glglue * glue);
COIN_DLL_API void cc_glglue_glTexSubImage2D(const cc_glglue * glue,
                                            GLenum target,
                                            GLint level,
                                            GLint xoffset,
                                            GLint yoffset,
                                            GLsizei width,
                                            GLsizei height,
                                            GLenum format,
                                            GLenum type,
                                            const GLvoid * pixels);

COIN_DLL_API SbBool cc_glglue_has_2d_proxy_textures(const cc_glglue * glue);

COIN_DLL_API SbBool cc_glglue_has_texture_edge_clamp(const cc_glglue * glue);

COIN_DLL_API SbBool cc_glue_has_texture_compression(const cc_glglue * glue);

COIN_DLL_API void cc_glglue_glCompressedTexImage3D(const cc_glglue * glue,
                                                   GLenum target, 
                                                   GLint level, 
                                                   GLenum internalformat, 
                                                   GLsizei width, 
                                                   GLsizei height, 
                                                   GLsizei depth, 
                                                   GLint border, 
                                                   GLsizei imageSize, 
                                                   const GLvoid * data);
COIN_DLL_API void cc_glglue_glCompressedTexImage2D(const cc_glglue * glue,
                                                   GLenum target, 
                                                   GLint level, 
                                                   GLenum internalformat, 
                                                   GLsizei width, 
                                                   GLsizei height, 
                                                   GLint border, 
                                                   GLsizei imageSize, 
                                                   const GLvoid *data);
COIN_DLL_API void cc_glglue_glCompressedTexImage1D(const cc_glglue * glue,
                                                   GLenum target, 
                                                   GLint level, 
                                                   GLenum internalformat, 
                                                   GLsizei width, 
                                                   GLint border, 
                                                   GLsizei imageSize, 
                                                   const GLvoid *data);
COIN_DLL_API void cc_glglue_glCompressedTexSubImage3D(const cc_glglue * glue,
                                                      GLenum target, 
                                                      GLint level, 
                                                      GLint xoffset, 
                                                      GLint yoffset, 
                                                      GLint zoffset, 
                                                      GLsizei width, 
                                                      GLsizei height, 
                                                      GLsizei depth, 
                                                      GLenum format, 
                                                      GLsizei imageSize, 
                                                      const GLvoid *data);
COIN_DLL_API void cc_glglue_glCompressedTexSubImage2D(const cc_glglue * glue,
                                                      GLenum target, 
                                                      GLint level, 
                                                      GLint xoffset, 
                                                      GLint yoffset, 
                                                      GLsizei width, 
                                                      GLsizei height, 
                                                      GLenum format, 
                                                      GLsizei imageSize, 
                                                      const GLvoid *data);
COIN_DLL_API void cc_glglue_glCompressedTexSubImage1D(const cc_glglue * glue,
                                                      GLenum target, 
                                                      GLint level, 
                                                      GLint xoffset, 
                                                      GLsizei width, 
                                                      GLenum format, 
                                                      GLsizei imageSize, 
                                                      const GLvoid *data);
COIN_DLL_API void cc_glglue_glGetCompressedTexImage(const cc_glglue * glue,
                                                    GLenum target, 
                                                    GLint level, 
                                                    void *img);

COIN_DLL_API SbBool cc_glglue_has_color_tables(const cc_glglue * glue);
COIN_DLL_API SbBool cc_glglue_has_color_subtables(const cc_glglue * glue);
/* TRUE from the next check also guarantees that the two color table
   checks above returns TRUE. */
COIN_DLL_API SbBool cc_glglue_has_paletted_textures(const cc_glglue * glue);

COIN_DLL_API void cc_glglue_glColorTable(const cc_glglue * glue,
                                         GLenum target, 
                                         GLenum internalFormat, 
                                         GLsizei width, 
                                         GLenum format, 
                                         GLenum type, 
                                         const GLvoid *table);
COIN_DLL_API void cc_glglue_glColorSubTable(const cc_glglue * glue,
                                            GLenum target,
                                            GLsizei start,
                                            GLsizei count,
                                            GLenum format,
                                            GLenum type,
                                            const GLvoid * data);
COIN_DLL_API void cc_glglue_glGetColorTable(const cc_glglue * glue,
                                            GLenum target, 
                                            GLenum format, 
                                            GLenum type, 
                                            GLvoid *data);
COIN_DLL_API void cc_glglue_glGetColorTableParameteriv(const cc_glglue * glue,
                                                       GLenum target, 
                                                       GLenum pname, 
                                                       GLint *params);
COIN_DLL_API void cc_glglue_glGetColorTableParameterfv(const cc_glglue * glue,
                                                       GLenum target, 
                                                       GLenum pname, 
                                                       GLfloat *params);

COIN_DLL_API SbBool cc_glglue_has_blendequation(const cc_glglue * glue);
COIN_DLL_API void cc_glglue_glBlendEquation(const cc_glglue * glue, GLenum mode);

COIN_DLL_API void * cc_glglue_glXGetCurrentDisplay(const cc_glglue * w);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_GL_H */
