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

#include "GLWrapper.h"

#include <Inventor/SbDict.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#if HAVE_CONFIG_H
#include "config.h"
#endif /* !HAVE_CONFIG_H */

#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#ifdef HAVE_GLX
#include <GL/glx.h>
#endif /* HAVE_GLX */

#if   defined(HAVE_WGL)
  #define LIBHANDLE_T void*
  #define OPEN_RUNTIME_BINDING(LIBNAME) (LIBHANDLE_T)1
  #define GETPROCADDRESS(DLHANDLE,FUNC)   wglGetProcAddress(FUNC)
#elif defined(GLX_VERSION_1_4)           // Have glXGetProcAddress()
  #define LIBHANDLE_T void*
  #define OPEN_RUNTIME_BINDING(LIBNAME) (LIBHANDLE_T)1
  #define GETPROCADDRESS(DLHANDLE,FUNC)   glXGetProcAddress((const GLubyte *)FUNC)
#elif defined(GLX_ARB_get_proc_address)
  #define LIBHANDLE_T void*
  #define OPEN_RUNTIME_BINDING(LIBNAME) (LIBHANDLE_T)1
  #define GETPROCADDRESS(DLHANDLE,FUNC)   glXGetProcAddressARB((const GLubyte *)FUNC)
#elif defined(HAVE_DL_LIB)
  #define LIBHANDLE_T void*
  #define OPEN_RUNTIME_BINDING(LIBNAME)   dlopen(LIBNAME, RTLD_LAZY)
  #define GETPROCADDRESS(DLHANDLE,FUNC)   dlsym(DLHANDLE,FUNC)
#else // This platform has no run-time binding capabilities
  #define LIBHANDLE_T void*
  #define OPEN_RUNTIME_BINDING(LIBNAME) (LIBHANDLE_T)1
  #define GETPROCADDRESS(DLHANDLE,FUNC)   (&FUNC)
#endif

/* FIXME: support HP-UX? (Doesn't have dlopen().) 20010626 mortene. */


LIBHANDLE_T GL_libhandle = NULL;
static GLWrapper_t * GL_instance = NULL;
static SbDict * gldict = NULL;


static void
free_GLWrapper_instance(unsigned long key, void * value)
{
  free(value);
}

/* Cleans up at exit. */
static void
GLWrapper_cleanup(void)
{
  // FIXME: clean up. 20011115 mortene.
  // if (GL_libhandle) { CLOSE_RUNTIME_BINDING(GL_libhandle); }

  gldict->applyToAll(free_GLWrapper_instance);
  delete gldict;
}

const GLWrapper_t *
GLWrapper(int contextid)
{
  if (!gldict) {  /* First invocation, do initializations. */
    (void)atexit(GLWrapper_cleanup);

    gldict = new SbDict;

    GL_libhandle = OPEN_RUNTIME_BINDING(NULL);
    if (!GL_libhandle) { } // FIXME: Error handling. 200111xx kintel.
  }

  void * ptr;
  SbBool found = gldict->find(contextid, ptr);

  if (!found) {
    GLWrapper_t * gi = (GLWrapper_t *)malloc(sizeof(GLWrapper_t));
    /* FIXME: handle out-of-memory on malloc(). 20000928 mortene. */
    ptr = gi;
    gldict->enter(contextid, ptr);

    // Define GLWRAPPER_REGISTER_FUNC macro. Casting the type is
    // necessary for this file to be compatible with C++ compilers. 
#ifdef HAVE_HASH_QUOTING
  #define GLWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
  gi->_funcname_ = (_funcsig_)GETPROCADDRESS(GL_libhandle, #_funcname_)
#elif defined(HAVE_APOSTROPHES_QUOTING)
  #define GLWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
  gi->_funcname_ = (_funcsig_)GETPROCADDRESS(GL_libhandle, "_funcname_")
#else
  #error Unknown quoting.
#endif

    // Resolve our functions 
    GLWRAPPER_REGISTER_FUNC(glTexImage3D, COIN_PFNGLTEXIMAGE3DPROC);
    GLWRAPPER_REGISTER_FUNC(glTexImage3DEXT, COIN_PFNGLTEXIMAGE3DEXTPROC);
  }

  return (GLWrapper_t *)ptr;
}
