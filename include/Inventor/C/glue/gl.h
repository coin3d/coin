#ifndef COIN_GLUE_GL_H
#define COIN_GLUE_GL_H

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
  Returns the glue instance for the given context ID (defined
  by SoGLCacheContextElement).
*/
COIN_DLL_API const cc_glglue * cc_glglue_instance(int contextid);

/*
  Returns TRUE if the OpenGL implementation of the wrapper context is at
  least as "late" as what is given with the input arguments. Otherwise
  returns FALSE.
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

/*
  Returns TRUE if the gived GLX extension is supported by this GLX implementation,
  FALSE if not.
 */
COIN_DLL_API SbBool cc_glglue_glxext_supported(const cc_glglue * glue, const char * extname);

/* feature checking */
COIN_DLL_API SbBool cc_glglue_has_3d_textures(const cc_glglue * glue);
COIN_DLL_API SbBool cc_glglue_has_2d_proxy_textures(const cc_glglue * glue);
COIN_DLL_API SbBool cc_glglue_has_3d_proxy_textures(const cc_glglue * glue);
COIN_DLL_API SbBool cc_glglue_has_texture_edge_clamp(const cc_glglue * glue);
COIN_DLL_API SbBool cc_glglue_has_multitexture(const cc_glglue * glue);

/* wrappers */
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

COIN_DLL_API void cc_glglue_glPolygonOffset(const cc_glglue * glue,
                                            GLfloat factor, 
                                            GLfloat bias);

COIN_DLL_API void cc_glglue_glBindTexture(const cc_glglue * glue,
                                          GLenum target, 
                                          GLuint texture);

COIN_DLL_API void cc_glglue_glDeleteTextures(const cc_glglue * glue,
                                             GLsizei n, 
                                             const GLuint * textures);

COIN_DLL_API void cc_glglue_glGenTextures(const cc_glglue * glue,
                                          GLsizei n, 
                                          GLuint *textures);

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

COIN_DLL_API void cc_glglue_glActiveTexture(const cc_glglue * glue,
                                            GLenum texture);

COIN_DLL_API void cc_glglue_glMultiTexCoord2f(const cc_glglue * glue,
                                              GLenum target,
                                              GLfloat s,
                                              GLfloat t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_GL_H */
