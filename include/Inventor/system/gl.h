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

/* At least necessary to include this header explicitly on Mac OS X. */
#ifdef HAVE_GL_GLEXT_H
#include <GL/glext.h>
#else
#ifdef HAVE_OPENGL_GLEXT_H
#include <OpenGL/glext.h>
#endif /* HAVE_OPENGL_GLEXT_H */
#endif /* !HAVE_GL_GLEXT_H */

/**********************************************************************/

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

/*** GL enums, end ****************************************************/
/**********************************************************************/

/* GLU enums we use are duplicated, because we should be able to
   compile without GLU and then pick it up and use it at run-time on
   some systems. */

/* gluGetString */
#ifndef GLU_VERSION
#define GLU_VERSION ((GLenum)100800)
#endif /* ! GLU_VERSION */
#ifndef GLU_EXTENSIONS
#define GLU_EXTENSIONS ((GLenum)100801)
#endif /* ! GLU_EXTENSIONS */

/* NurbsDisplay */
#ifndef GLU_OUTLINE_POLYGON
#define GLU_OUTLINE_POLYGON ((GLenum)100240)
#endif /* ! GLU_OUTLINE_POLYGON */
#ifndef GLU_OUTLINE_PATCH
#define GLU_OUTLINE_PATCH ((GLenum)100241)
#endif /* ! GLU_OUTLINE_PATCH */

/* NurbsCallback */
#ifndef GLU_NURBS_ERROR
#define GLU_NURBS_ERROR ((GLenum)100103)
#endif /* ! GLU_NURBS_ERROR */
#ifndef GLU_ERROR
#define GLU_ERROR ((GLenum)100103)
#endif /* ! GLU_ERROR */
#ifndef GLU_NURBS_BEGIN
#define GLU_NURBS_BEGIN ((GLenum)100164)
#endif /* ! GLU_NURBS_BEGIN */
#ifndef GLU_NURBS_VERTEX
#define GLU_NURBS_VERTEX ((GLenum)100165)
#endif /* ! GLU_NURBS_VERTEX */
#ifndef GLU_NURBS_NORMAL
#define GLU_NURBS_NORMAL ((GLenum)100166)
#endif /* ! GLU_NURBS_NORMAL */
#ifndef GLU_NURBS_COLOR
#define GLU_NURBS_COLOR ((GLenum)100167)
#endif /* ! GLU_NURBS_COLOR */
#ifndef GLU_NURBS_TEXTURE_COORD
#define GLU_NURBS_TEXTURE_COORD ((GLenum)100168)
#endif /* ! GLU_NURBS_TEXTURE_COORD */
#ifndef GLU_NURBS_END
#define GLU_NURBS_END ((GLenum)100169)
#endif /* ! GLU_NURBS_END */
#ifndef GLU_NURBS_BEGIN_DATA
#define GLU_NURBS_BEGIN_DATA ((GLenum)100170)
#endif /* ! GLU_NURBS_BEGIN_DATA */
#ifndef GLU_NURBS_VERTEX_DATA
#define GLU_NURBS_VERTEX_DATA ((GLenum)100171)
#endif /* ! GLU_NURBS_VERTEX_DATA */
#ifndef GLU_NURBS_NORMAL_DATA
#define GLU_NURBS_NORMAL_DATA ((GLenum)100172)
#endif /* ! GLU_NURBS_NORMAL_DATA */
#ifndef GLU_NURBS_COLOR_DATA
#define GLU_NURBS_COLOR_DATA ((GLenum)100173)
#endif /* ! GLU_NURBS_COLOR_DATA */
#ifndef GLU_NURBS_TEXTURE_COORD_DATA
#define GLU_NURBS_TEXTURE_COORD_DATA ((GLenum)100174)
#endif /* ! GLU_NURBS_TEXTURE_COORD_DATA */
#ifndef GLU_NURBS_END_DATA
#define GLU_NURBS_END_DATA ((GLenum)100175)
#endif /* ! GLU_NURBS_END_DATA */

/* NurbsProperty */
#ifndef GLU_AUTO_LOAD_MATRIX
#define GLU_AUTO_LOAD_MATRIX ((GLenum)100200)
#endif /* ! GLU_AUTO_LOAD_MATRIX */
#ifndef GLU_CULLING
#define GLU_CULLING ((GLenum)100201)
#endif /* ! GLU_CULLING */
#ifndef GLU_SAMPLING_TOLERANCE
#define GLU_SAMPLING_TOLERANCE ((GLenum)100203)
#endif /* ! GLU_SAMPLING_TOLERANCE */
#ifndef GLU_DISPLAY_MODE
#define GLU_DISPLAY_MODE ((GLenum)100204)
#endif /* ! GLU_DISPLAY_MODE */
#ifndef GLU_PARAMETRIC_TOLERANCE
#define GLU_PARAMETRIC_TOLERANCE ((GLenum)100202)
#endif /* ! GLU_PARAMETRIC_TOLERANCE */
#ifndef GLU_SAMPLING_METHOD
#define GLU_SAMPLING_METHOD ((GLenum)100205)
#endif /* ! GLU_SAMPLING_METHOD */
#ifndef GLU_U_STEP
#define GLU_U_STEP ((GLenum)100206)
#endif /* ! GLU_U_STEP */
#ifndef GLU_V_STEP
#define GLU_V_STEP ((GLenum)100207)
#endif /* ! GLU_V_STEP */
#ifndef GLU_NURBS_MODE
#define GLU_NURBS_MODE ((GLenum)100160)
#endif /* ! GLU_NURBS_MODE */
#ifndef GLU_NURBS_TESSELLATOR
#define GLU_NURBS_TESSELLATOR ((GLenum)100161)
#endif /* ! GLU_NURBS_TESSELLATOR */
#ifndef GLU_NURBS_RENDERER
#define GLU_NURBS_RENDERER ((GLenum)100162)
#endif /* ! GLU_NURBS_RENDERER */

/* NurbsSampling */
#ifndef GLU_OBJECT_PARAMETRIC_ERROR
#define GLU_OBJECT_PARAMETRIC_ERROR ((GLenum)100208)
#endif /* ! GLU_OBJECT_PARAMETRIC_ERROR */
#ifndef GLU_OBJECT_PATH_LENGTH
#define GLU_OBJECT_PATH_LENGTH ((GLenum)100209)
#endif /* ! GLU_OBJECT_PATH_LENGTH */
#ifndef GLU_PATH_LENGTH
#define GLU_PATH_LENGTH ((GLenum)100215)
#endif /* ! GLU_PATH_LENGTH */
#ifndef GLU_PARAMETRIC_ERROR
#define GLU_PARAMETRIC_ERROR ((GLenum)100216)
#endif /* ! GLU_PARAMETRIC_ERROR */
#ifndef GLU_DOMAIN_DISTANCE
#define GLU_DOMAIN_DISTANCE ((GLenum)100217)
#endif /* ! GLU_DOMAIN_DISTANCE */

/* NurbsTrim */
#ifndef GLU_MAP1_TRIM_2
#define GLU_MAP1_TRIM_2 ((GLenum)100210)
#endif /* ! GLU_MAP1_TRIM_2 */
#ifndef GLU_MAP1_TRIM_3
#define GLU_MAP1_TRIM_3 ((GLenum)100211)
#endif /* ! GLU_MAP1_TRIM_3 */

/* QuadricDrawStyle */
#ifndef GLU_POINT
#define GLU_POINT ((GLenum)100010)
#endif /* ! GLU_POINT */
#ifndef GLU_LINE
#define GLU_LINE ((GLenum)100011)
#endif /* ! GLU_LINE */
#ifndef GLU_FILL
#define GLU_FILL ((GLenum)100012)
#endif /* ! GLU_FILL */
#ifndef GLU_SILHOUETTE
#define GLU_SILHOUETTE ((GLenum)10001)
#endif /* ! GLU_SILHOUETTE */

/*** GLU enums, end ***************************************************/
/**********************************************************************/

#endif /* ! COIN_GL_H */
