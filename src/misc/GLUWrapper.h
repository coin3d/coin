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

#ifndef COIN_GLUWRAPPER_H
#define COIN_GLUWRAPPER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#if HAVE_WINDOWS_H
#include <windows.h> /* to pick up the APIENTRY define */
#endif /* HAVE_WINDOWS_H */

#include <Inventor/system/gl.h>

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

/* Callback func type. */
typedef void (APIENTRY* gluNurbsCallback_cb_t)(void *, ...);

/* Typedefinitions of function signatures for GLU calls we use. We
   need these for casting from the void-pointer return of dlsym().*/
typedef const GLubyte * (APIENTRY *gluGetString_t)(GLenum);
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

  /* GLU calls which might be used. Note that any of these can be NULL
     pointers if the function is not available, unless marked as being
     always available. (That is, as long as GLU itself is available.)
     */
  gluGetString_t gluGetString;
  gluScaleImage_t gluScaleImage; /* always present */
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
