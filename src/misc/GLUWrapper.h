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

#ifndef COIN_GLUWRAPPER_H
#define COIN_GLUWRAPPER_H

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#if HAVE_WINDOWS_H
#include <windows.h> /* for GL and for APIENTRY */
#endif /* HAVE_WINDOWS_H */

#include <GL/gl.h>

/* Under Win32, we need to make sure we use the correct calling method
   by using the APIENTRY define for the function signature types (or
   else we'll get weird stack errors). On other platforms, just define
   APIENTRY empty. */
#ifndef APIENTRY
#define APIENTRY
#endif /* !APIENTRY */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Duplicated enum names. */
enum {
  /* gluGetString */
  GLU_W_VERSION = 100800,
  GLU_W_EXTENSIONS = 100801,

  /* NurbsDisplay */
  GLU_W_OUTLINE_POLYGON = 100240,
  GLU_W_OUTLINE_PATCH = 100241,

  /* NurbsCallback */
  GLU_W_NURBS_ERROR = 100103,
  GLU_W_ERROR = 100103,
  GLU_W_NURBS_BEGIN = 100164,
  GLU_W_NURBS_VERTEX = 100165,
  GLU_W_NURBS_NORMAL = 100166,
  GLU_W_NURBS_COLOR = 100167,
  GLU_W_NURBS_TEXTURE_COORD = 100168,
  GLU_W_NURBS_END = 100169,
  GLU_W_NURBS_BEGIN_DATA = 100170,
  GLU_W_NURBS_VERTEX_DATA = 100171,
  GLU_W_NURBS_NORMAL_DATA = 100172,
  GLU_W_NURBS_COLOR_DATA = 100173,
  GLU_W_NURBS_TEXTURE_COORD_DATA = 100174,
  GLU_W_NURBS_END_DATA = 100175,

  /* NurbsProperty */
  GLU_W_AUTO_LOAD_MATRIX = 100200,
  GLU_W_CULLING = 100201,
  GLU_W_SAMPLING_TOLERANCE = 100203,
  GLU_W_DISPLAY_MODE = 100204,
  GLU_W_PARAMETRIC_TOLERANCE = 100202,
  GLU_W_SAMPLING_METHOD = 100205,
  GLU_W_U_STEP = 100206,
  GLU_W_V_STEP = 100207,
  GLU_W_NURBS_MODE = 100160,
  GLU_W_NURBS_TESSELLATOR = 100161,
  GLU_W_NURBS_RENDERER = 100162,

  /* NurbsSampling */
  GLU_W_OBJECT_PARAMETRIC_ERROR = 100208,
  GLU_W_OBJECT_PATH_LENGTH = 100209,
  GLU_W_PATH_LENGTH = 100215,
  GLU_W_PARAMETRIC_ERROR = 100216,
  GLU_W_DOMAIN_DISTANCE = 100217,

  /* NurbsTrim */
  GLU_W_MAP1_TRIM_2 = 100210,
  GLU_W_MAP1_TRIM_3 = 100211,

  /* QuadricDrawStyle */
  GLU_W_POINT = 100010,
  GLU_W_LINE = 100011,
  GLU_W_FILL = 100012,
  GLU_W_SILHOUETTE = 100013
};


/* Callback func type. */
typedef void (*gluNurbsCallback_cb_t)(void *, ...);

/* Typedefinitions of function signatures for GLU calls we use. We
   need these for casting from the void-pointer return of dlsym().*/
typedef const GLubyte * (APIENTRY *gluGetString_t)(GLenum);
typedef GLint (APIENTRY *gluBuild2DMipmaps_t)(GLenum, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *);
typedef GLint (APIENTRY *gluScaleImage_t)(GLenum, GLsizei, GLsizei, GLenum, const void *, GLsizei, GLsizei, GLenum, GLvoid *);
/* The first argument for these methods is actually either GLUnurbs or
   GLUnurbsObj, depending on the GLU version (yes, they managed to
   change the API over version 1.x to 1.y, for some value of [0, 3]
   for x and y, where x < y). */
typedef void * (APIENTRY *gluNewNurbsRenderer_t)(void);
typedef void (APIENTRY *gluDeleteNurbsRenderer_t)(void *);
typedef void (APIENTRY *gluNurbsProperty_t)(void *, GLenum, GLfloat);
typedef void (APIENTRY *gluLoadSamplingMatrices_t)(void *, const GLfloat *, const GLfloat *, const GLint *);
typedef void (APIENTRY *gluBeginSurface_t)(void *);
typedef void (APIENTRY *gluEndSurface_t)(void *);
typedef void (APIENTRY *gluNurbsSurface_t)(void *, GLint, GLfloat *, GLint, GLfloat *, GLint, GLint, GLfloat *, GLint, GLint, GLenum);
typedef void (APIENTRY *gluBeginTrim_t)(void *);
typedef void (APIENTRY *gluEndTrim_t)(void *);
typedef void (APIENTRY *gluBeginCurve_t)(void *);
typedef void (APIENTRY *gluEndCurve_t)(void *);
typedef void (APIENTRY *gluNurbsCurve_t)(void *, GLint, GLfloat *, GLint, GLfloat *, GLint, GLenum);
typedef void (APIENTRY *gluPwlCurve_t)(void *, GLint, GLfloat *, GLint, GLenum);
typedef void (APIENTRY *gluNurbsCallback_t)(void *, GLenum, gluNurbsCallback_cb_t);
typedef void (APIENTRY *gluNurbsCallbackData_t)(void *, GLvoid *);


typedef struct {
  /* Is the GLU library at all available? */
  int available;

  /* GLU versioning. */
  struct {
    unsigned int major, minor, release;
  } version;
  int (*versionMatchesAtLeast)(unsigned int major,
                               unsigned int minor,
                               unsigned int release);

  /* GLU calls which might be used. */
  gluGetString_t gluGetString;
  gluBuild2DMipmaps_t gluBuild2DMipmaps;
  gluScaleImage_t gluScaleImage;
  gluNewNurbsRenderer_t gluNewNurbsRenderer;
  gluDeleteNurbsRenderer_t gluDeleteNurbsRenderer;
  gluNurbsProperty_t gluNurbsProperty;
  gluLoadSamplingMatrices_t gluLoadSamplingMatrices;
  gluBeginSurface_t gluBeginSurface;
  gluEndSurface_t gluEndSurface;
  gluNurbsSurface_t gluNurbsSurface;
  gluBeginTrim_t gluBeginTrim;
  gluEndTrim_t gluEndTrim;
  gluBeginCurve_t gluBeginCurve;
  gluEndCurve_t gluEndCurve;
  gluNurbsCurve_t gluNurbsCurve;
  gluPwlCurve_t gluPwlCurve;
  gluNurbsCallback_t gluNurbsCallback;
  gluNurbsCallbackData_t gluNurbsCallbackData;
} GLUWrapper_t;


const GLUWrapper_t * GLUWrapper(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COIN_GLUWRAPPER_H */
