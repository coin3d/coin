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

#include <GLUWrapper.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#if HAVE_CONFIG_H
#include <config.h>
#else /* No config.h? Hmm. Assume the GLU library is available for linking. */
#define GLUWRAPPER_ASSUME_GLU 1
#endif /* !HAVE_CONFIG_H */
#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */
#if HAVE_GLU /* In case we're _not_ doing runtime linking. */
#define GLUWRAPPER_ASSUME_GLU 1
#endif /* HAVE_GLU */

#if GLUWRAPPER_ASSUME_GLU
#include <GL/glu.h>
#endif /* GLUWRAPPER_ASSUME_GLU */


static GLUWrapper_t * GLU_instance = NULL;
static void * GLU_libhandle = NULL;
static int GLU_failed_to_load = 0;


/* Cleans up at exit. */
static void
GLUWrapper_cleanup(void)
{
#if GLU_RUNTIME_LINKING
  if (GLU_libhandle) (void)dlclose(GLU_libhandle);
#endif /* GLU_RUNTIME_LINKING */

  assert(GLU_instance);
  free(GLU_instance);
}


/* Set the GLU version variables in the global GLUWrapper_t. */
static void
GLUWrapper_set_version(const GLubyte * versionstr)
{
  char buffer[256];
  char * dotptr;

  GLU_instance->version.major = 0;
  GLU_instance->version.minor = 0;
  GLU_instance->version.release = 0;

#if 0 /* debug */
#if _WIN32
#error fprintf() in an MSWindows DLL is bad.
#endif /* _WIN32 */
  (void)fprintf(stderr, "GLU version string: \"%s\"\n", versionstr);
  (void)fflush(stderr);
#endif /* debug */
  (void)strncpy(buffer, (const char *)versionstr, 255);
  buffer[255] = '\0'; /* strncpy() will not null-terminate if strlen > 255 */
  dotptr = strchr(buffer, '.');
  if (dotptr) {
    char * spaceptr;
    char * start = buffer;
    *dotptr = '\0';
    GLU_instance->version.major = atoi(start);
    start = ++dotptr;

    dotptr = strchr(start, '.');
    spaceptr = strchr(start, ' ');
    if (!dotptr && spaceptr) dotptr = spaceptr;
    if (dotptr && spaceptr && spaceptr < dotptr) dotptr = spaceptr;
    if (dotptr) {
      int terminate = *dotptr == ' ';
      *dotptr = '\0';
      GLU_instance->version.minor = atoi(start);
      if (!terminate) {
        start = ++dotptr;
        dotptr = strchr(start, ' ');
        if (dotptr) *dotptr = '\0';
        GLU_instance->version.release = atoi(start);
      }
    }
    else {
      GLU_instance->version.minor = atoi(start);
    }
  }
#if 0 /* debug */
  else {
#if _WIN32
#error fprintf() in an MSWindows DLL is bad.
#endif /* _WIN32 */
    (void)fprintf(stderr, "Invalid GLU versionstring: \"%s\"\n", versionstr);
    (void)fflush(stderr);
  }
#endif /* debug */
#if 0 /* debug */
#if _WIN32
#error fprintf() in an MSWindows DLL is bad.
#endif /* _WIN32 */
  (void)fprintf(stderr, "GLU version: %d.%d.%d\n",
                GLU_instance->version.major,
                GLU_instance->version.minor,
                GLU_instance->version.release);
  (void)fflush(stderr);
#endif /* debug */
}

static int
GLUWrapper_versionMatchesAtLeast(unsigned int major,
                                 unsigned int minor,
                                 unsigned int release)
{
  if (GLUWrapper()->available == 0) return 0;
  if (GLUWrapper()->version.major < major) return 0;
  if (GLUWrapper()->version.minor < minor) return 0;
  if (GLUWrapper()->version.release < release) return 0;
  return 1;
}

/* Replacement function for gluGetString(). */
static const GLubyte * APIENTRY
GLUWrapper_gluGetString(GLenum name)
{
  static const GLubyte versionstring[] = "1.0.0";
  if (name == GLU_W_VERSION) return versionstring;
  return NULL;
}

/* Replacement function for gluScaleImage(). */
static GLint APIENTRY
GLUWrapper_gluScaleImage(GLenum a, GLsizei b, GLsizei c, GLenum d, const void * e, GLsizei f, GLsizei g, GLenum h, GLvoid * i)
{
  /* Just a void function (gluScaleImage() should normally be
     present). */

  /* 0 indicates success. */
  return 0;
}

/* Replacement function for gluBuild2DMipmaps(). */
static GLint APIENTRY
GLUWrapper_gluBuild2DMipmaps(GLenum a, GLint b, GLsizei c, GLsizei d, GLenum e, GLenum f, const void * g)
{
  /* Just a void function (gluBuild2DMipmaps() should normally be
     present). */

  /* 0 indicates success. */
  return 0;
}

/* Implemented by using the singleton pattern. */
const GLUWrapper_t *
GLUWrapper(void)
{
  if (!GLU_instance && !GLU_failed_to_load) {
    /* First invocation, do initializations. */
    GLU_instance = (GLUWrapper_t *)malloc(sizeof(GLUWrapper_t));
    /* FIXME: handle out-of-memory on malloc(). 20000928 mortene. */
    (void)atexit(GLUWrapper_cleanup);

    GLU_instance->versionMatchesAtLeast = GLUWrapper_versionMatchesAtLeast;

    /* The common case is that GLU is either available from the
       linking process or we're successfully going to link it in. */
    GLU_instance->available = 1;

#if GLU_RUNTIME_LINKING
    {
      const char * possiblelibnames[] = {
        /* FIXME: should we get the system shared library name from an
           Autoconf check? 20000930 mortene. */
        "GLU", "MesaGLU",
        "libGLU", "libMesaGLU",
        "libGLU.so", "libMesaGLU.so",
        /* FIXME: this hits on HP-UX? 20000930 mortene. */
        "libGLU.sl", "libMesaGLU.sl",
        NULL
      };
      /* FIXME: implement same functionality on MSWindows. 20000930 mortene. */
      int idx = 0;
      while (!GLU_libhandle && possiblelibnames[idx]) {
        GLU_libhandle = dlopen(possiblelibnames[idx], RTLD_LAZY);
#if 0 /* debug */
        if (!GLU_libhandle) {
          (void)fprintf(stderr,
                        "GLU wrapper debug: couldn't open '%s': '%s'\n",
                        possiblelibnames[idx],
                        dlerror());
          (void)fflush(stderr);
        }
#endif /* debug */
        idx++;
      }

      if (!GLU_libhandle) {
        GLU_instance->available = 0;
        GLU_failed_to_load = 1;
      }
    }

    /* Define GLUWRAPPER_REGISTER_FUNC macro. Casting the type is
       necessary for this file to be compatible with C++ compilers. */
#if HAVE_HASH_QUOTING
#define GLUWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    GLU_instance->_funcname_ = (_funcsig_)dlsym(GLU_libhandle, #_funcname_)
#elif HAVE_APOSTROPHES_QUOTING
#define GLUWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    GLU_instance->_funcname_ = (_funcsig_)dlsym(GLU_libhandle, "_funcname_")
#else
#error Unknown quoting.
#endif

#elif GLUWRAPPER_ASSUME_GLU /* !GLU_RUNTIME_LINKING */

    /* Define GLUWRAPPER_REGISTER_FUNC macro. */
#define GLUWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    GLU_instance->_funcname_ = (_funcsig_)_funcname_

#else /* !GLUWRAPPER_ASSUME_GLU */
    GLU_instance->available = 0;
    /* Define GLUWRAPPER_REGISTER_FUNC macro. */
#define GLUWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    GLU_instance->_funcname_ = NULL

#endif /* !GLUWRAPPER_ASSUME_GLU */

    GLUWRAPPER_REGISTER_FUNC(gluBuild2DMipmaps, gluBuild2DMipmaps_t);
    GLUWRAPPER_REGISTER_FUNC(gluScaleImage, gluScaleImage_t);
    GLUWRAPPER_REGISTER_FUNC(gluGetString, gluGetString_t);
    GLUWRAPPER_REGISTER_FUNC(gluNewNurbsRenderer, gluNewNurbsRenderer_t);
    GLUWRAPPER_REGISTER_FUNC(gluDeleteNurbsRenderer, gluDeleteNurbsRenderer_t);
    GLUWRAPPER_REGISTER_FUNC(gluNurbsProperty, gluNurbsProperty_t);
    GLUWRAPPER_REGISTER_FUNC(gluLoadSamplingMatrices, gluLoadSamplingMatrices_t);
    GLUWRAPPER_REGISTER_FUNC(gluBeginSurface, gluBeginSurface_t);
    GLUWRAPPER_REGISTER_FUNC(gluEndSurface, gluEndSurface_t);
    GLUWRAPPER_REGISTER_FUNC(gluNurbsSurface, gluNurbsSurface_t);
    GLUWRAPPER_REGISTER_FUNC(gluBeginTrim, gluBeginTrim_t);
    GLUWRAPPER_REGISTER_FUNC(gluEndTrim, gluEndTrim_t);
    GLUWRAPPER_REGISTER_FUNC(gluBeginCurve, gluBeginCurve_t);
    GLUWRAPPER_REGISTER_FUNC(gluEndCurve, gluEndCurve_t);
    GLUWRAPPER_REGISTER_FUNC(gluNurbsCurve, gluNurbsCurve_t);
    GLUWRAPPER_REGISTER_FUNC(gluPwlCurve, gluPwlCurve_t);
    GLUWRAPPER_REGISTER_FUNC(gluNurbsCallback, gluNurbsCallback_t);
#if GLU_VERSION_1_3 || GLU_RUNTIME_LINKING
    GLUWRAPPER_REGISTER_FUNC(gluNurbsCallbackData, gluNurbsCallbackData_t);
#else /* !gluNurbsCallbackData */
    GLU_instance->gluNurbsCallbackData = NULL;
#endif /* !gluNurbsCallbackData */

    /* "Backup" functions, makes it easier to be robust even when no
       GLU library can be loaded. */
    if (GLU_instance->gluScaleImage == NULL)
      GLU_instance->gluScaleImage = GLUWrapper_gluScaleImage;
    if (GLU_instance->gluBuild2DMipmaps == NULL)
      GLU_instance->gluBuild2DMipmaps = GLUWrapper_gluBuild2DMipmaps;
    if (GLU_instance->gluGetString == NULL) /* Was missing in GLU v1.0. */
      GLU_instance->gluGetString = GLUWrapper_gluGetString;

#if COIN_DEBUG && 0 // debug
    /* Test code for the GLUWrapper_set_version() parsing. */
    GLUWrapper_set_version("1.2");
    (void)fprintf(stdout, "%d.%d.%d\n",
                  GLU_instance->version.major,
                  GLU_instance->version.minor,
                  GLU_instance->version.release);
    GLUWrapper_set_version("1.2 ");
    (void)fprintf(stdout, "%d.%d.%d\n",
                  GLU_instance->version.major,
                  GLU_instance->version.minor,
                  GLU_instance->version.release);
    GLUWrapper_set_version("1.2.3");
    (void)fprintf(stdout, "%d.%d.%d\n",
                  GLU_instance->version.major,
                  GLU_instance->version.minor,
                  GLU_instance->version.release);
    GLUWrapper_set_version("1.2.3 ");
    (void)fprintf(stdout, "%d.%d.%d\n",
                  GLU_instance->version.major,
                  GLU_instance->version.minor,
                  GLU_instance->version.release);
    GLUWrapper_set_version("1.2.3 hepp");
    (void)fprintf(stdout, "%d.%d.%d\n",
                  GLU_instance->version.major,
                  GLU_instance->version.minor,
                  GLU_instance->version.release);
    (void)fflush(stdout);
#endif /* debug */

    /* Parse the version string once and expose the version numbers
       through the GLUWrapper API. */
    GLUWrapper_set_version(GLU_instance->gluGetString(GLU_W_VERSION));
  }

  return GLU_instance;
}
