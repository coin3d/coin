#ifndef COIN_GLUE_GLP_H
#define COIN_GLUE_GLP_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

/* NOTE: do *not* include this header file outside of the gl.c OpenGL
   wrapper -- even if you're inside Coin library implementation
   code. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

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

/* typedefs for texture compression */

typedef void (APIENTRY * COIN_PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum target,
                                                             GLint level,
                                                             GLenum internalformat,
                                                             GLsizei width,
                                                             GLsizei height,
                                                             GLsizei depth,
                                                             GLint border,
                                                             GLsizei imageSize,
                                                             const GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target,
                                                             GLint level,
                                                             GLenum internalformat,
                                                             GLsizei width,
                                                             GLsizei height,
                                                             GLint border,
                                                             GLsizei imageSize,
                                                             const GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLCOMPRESSEDTEXIMAGE1DPROC)(GLenum target,
                                                             GLint level,
                                                             GLenum internalformat,
                                                             GLsizei width,
                                                             GLint border,
                                                             GLsizei imageSize,
                                                             const GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum target,
                                                                GLint level,
                                                                GLint xoffset,
                                                                GLint yoffset,
                                                                GLint zoffset,
                                                                GLsizei width,
                                                                GLsizei height,
                                                                GLsizei depth,
                                                                GLenum format,
                                                                GLsizei imageSize,
                                                                const GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target,
                                                                GLint level,
                                                                GLint xoffset,
                                                                GLint yoffset,
                                                                GLsizei width,
                                                                GLsizei height,
                                                                GLenum format,
                                                                GLsizei imageSize,
                                                                const GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum target,
                                                                GLint level,
                                                                GLint xoffset,
                                                                GLsizei width,
                                                                GLenum format,
                                                                GLsizei imageSize,
                                                                const GLvoid *data);
typedef void (APIENTRY * COIN_PFNGLGETCOMPRESSEDTEXIMAGEPROC)(GLenum target,
                                                              GLint level,
                                                              void * img);


/* typedefs for palette tetures */
typedef void (APIENTRY * COIN_PFNGLCOLORTABLEPROC)(GLenum target,
                                                   GLenum internalFormat,
                                                   GLsizei width,
                                                   GLenum format,
                                                   GLenum type,
                                                   const GLvoid * table);
typedef void (APIENTRY * COIN_PFNGLCOLORSUBTABLEPROC)(GLenum target,
                                                      GLsizei start,
                                                      GLsizei count,
                                                      GLenum format,
                                                      GLenum type,
                                                      const GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLGETCOLORTABLEPROC)(GLenum target,
                                                      GLenum format,
                                                      GLenum type,
                                                      GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLGETCOLORTABLEPARAMETERIVPROC)(GLenum target,
                                                                 GLenum pname,
                                                                 GLint *params);
typedef void (APIENTRY * COIN_PFNGLGETCOLORTABLEPARAMETERFVPROC)(GLenum target,
                                                                 GLenum pname,
                                                                 GLfloat * params);

/* Typedefs for glBlendEquation[EXT]. */
typedef void *(APIENTRY * COIN_PFNGLBLENDEQUATIONPROC)(GLenum);

/* typedefs for OpenGL vertex arrays */
typedef void (APIENTRY * COIN_PFNGLVERTEXPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (APIENTRY * COIN_PFNGLTEXCOORDPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (APIENTRY * COIN_PFNGLNORMALPOINTERPROC)(GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * COIN_PNFGLCOLORPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (APIENTRY * COIN_PFNGLINDEXPOINTERPROC)(GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (APIENTRY * COIN_PFNGLENABLECLIENTSTATEPROC)(GLenum array);
typedef void (APIENTRY * COIN_PFNGLDISABLECLIENTSTATEPROC)(GLenum array);
typedef void (APIENTRY * COIN_PFNGLINTERLEAVEDARRAYSPROC)(GLenum format, GLsizei stride, const GLvoid * pointer);
typedef void (APIENTRY * COIN_PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY * COIN_PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
typedef void (APIENTRY * COIN_PFNGLARRAYELEMENTPROC)(GLint i);

typedef void (APIENTRY * COIN_PFNGLMULTIDRAWARRAYSPROC)(GLenum mode, const GLint * first, 
                                                        const GLsizei * count, GLsizei primcount);
typedef void (APIENTRY * COIN_PFNGLMULTIDRAWELEMENTSPROC)(GLenum mode, const GLsizei * count, 
                                                          GLenum type, const GLvoid ** indices, GLsizei primcount);

/* Typedefs for NV_vertex_array_range */
typedef void (APIENTRY * COIN_PFNGLFLUSHVERTEXARRAYRANGENVPROC)(void);
typedef void (APIENTRY * COIN_PFNGLVERTEXARRAYRANGENVPROC)(GLsizei size, const GLvoid * pointer);
typedef void * (APIENTRY * COIN_PFNGLALLOCATEMEMORYNVPROC)(GLsizei size, GLfloat readfreq,
                                                           GLfloat writefreq, GLfloat priority);
typedef void (APIENTRY * COIN_PFNGLFREEMEMORYNVPROC)(GLvoid * buffer);


/* typedefs for GL_ARB_vertex_buffer_object */
typedef void (APIENTRY * COIN_PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRY * COIN_PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint * buffers);
typedef void (APIENTRY * COIN_PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRY * COIN_PFNGLISBUFFERPROC)(GLuint buffer);
typedef void (APIENTRY * COIN_PFNGLBUFFERDATAPROC)(GLenum target,
                                                   intptr_t size, /* 64 bit on 64 bit systems */
                                                   const GLvoid *data,
                                                   GLenum usage);
typedef void (APIENTRY * COIN_PFNGLBUFFERSUBDATAPROC)(GLenum target,
                                                      intptr_t offset, /* 64 bit */
                                                      intptr_t size, /* 64 bit */
                                                      const GLvoid * data);
typedef void (APIENTRY * COIN_PFNGLGETBUFFERSUBDATAPROC)(GLenum target,
                                                         intptr_t offset, /* 64 bit */
                                                         intptr_t size, /* 64 bit */
                                                         GLvoid *data);
typedef GLvoid * (APIENTRY * COIN_PNFGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef GLboolean (APIENTRY * COIN_PFNGLUNMAPBUFFERPROC)(GLenum target);
typedef void (APIENTRY * COIN_PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target,
                                                             GLenum pname,
                                                             GLint * params);
typedef void (APIENTRY * COIN_PFNGLGETBUFFERPOINTERVPROC)(GLenum target,
                                                          GLenum pname,
                                                          GLvoid ** params);

/* Typedefs for GLX functions. */
typedef void *(APIENTRY * COIN_PFNGLXGETCURRENTDISPLAYPROC)(void);

/* Type specification for GLX info storage structure, embedded within
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

  COIN_PFNGLXGETCURRENTDISPLAYPROC glXGetCurrentDisplay;
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

  COIN_PFNGLCOLORTABLEPROC glColorTable;
  COIN_PFNGLCOLORSUBTABLEPROC glColorSubTable;
  COIN_PFNGLGETCOLORTABLEPROC glGetColorTable;
  COIN_PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
  COIN_PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;

  SbBool supportsPalettedTextures;

  COIN_PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
  COIN_PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
  COIN_PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
  COIN_PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
  COIN_PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
  COIN_PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
  COIN_PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;

  COIN_PFNGLBLENDEQUATIONPROC glBlendEquation;
  COIN_PFNGLBLENDEQUATIONPROC glBlendEquationEXT;

  COIN_PFNGLVERTEXPOINTERPROC glVertexPointer;
  COIN_PFNGLTEXCOORDPOINTERPROC glTexCoordPointer;
  COIN_PFNGLNORMALPOINTERPROC glNormalPointer;
  COIN_PNFGLCOLORPOINTERPROC glColorPointer;
  COIN_PFNGLINDEXPOINTERPROC glIndexPointer;
  COIN_PFNGLENABLECLIENTSTATEPROC glEnableClientState;
  COIN_PFNGLDISABLECLIENTSTATEPROC glDisableClientState;
  COIN_PFNGLINTERLEAVEDARRAYSPROC glInterleavedArrays;
  COIN_PFNGLDRAWARRAYSPROC glDrawArrays;
  COIN_PFNGLDRAWELEMENTSPROC glDrawElements;
  COIN_PFNGLARRAYELEMENTPROC glArrayElement;
  
  COIN_PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
  COIN_PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
  
  COIN_PFNGLVERTEXARRAYRANGENVPROC glVertexArrayRangeNV;
  COIN_PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV;
  COIN_PFNGLALLOCATEMEMORYNVPROC glAllocateMemoryNV;
  COIN_PFNGLFREEMEMORYNVPROC glFreeMemoryNV;

  COIN_PFNGLBINDBUFFERPROC glBindBuffer;
  COIN_PFNGLDELETEBUFFERSPROC glDeleteBuffers;
  COIN_PFNGLGENBUFFERSPROC glGenBuffers;
  COIN_PFNGLISBUFFERPROC glIsBuffer;
  COIN_PFNGLBUFFERDATAPROC glBufferData;
  COIN_PFNGLBUFFERSUBDATAPROC glBufferSubData;
  COIN_PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData;
  COIN_PNFGLMAPBUFFERPROC glMapBuffer;
  COIN_PFNGLUNMAPBUFFERPROC glUnmapBuffer;
  COIN_PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
  COIN_PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;

  const char * versionstr;
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
