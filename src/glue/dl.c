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

/* FIXME: support HP-UX 10.x? (Doesn't have dlopen().) 20010626 mortene. */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_DLFCN_H
#include <dlfcn.h> /* Programming interface to libdl. */
#endif /* HAVE_DLFCN_H */

#if HAVE_WINDOWS_H
#include <windows.h>
#endif /* HAVE_WINDOWS_H */

#include <stddef.h> /* NULL definition. */
#include <malloc.h>
#include <Inventor/C/glue/dl.h>

struct cc_libhandle_struct {
  void * nativehnd;
};

cc_libhandle
cc_dl_open(const char * filename)
{
#ifdef HAVE_DL_LIB
  cc_libhandle h = (cc_libhandle) malloc(sizeof(struct cc_libhandle_struct));
  /* if (!h), FIXME: error handling. 20020906 mortene. */
  h->nativehnd = dlopen(filename, RTLD_LAZY);
  /* FIXME: Error handling (kintel 20011121) */
  return h;
#elif defined (HAVE_WINDLL_RUNTIME_BINDING)
  cc_libhandle h = (cc_libhandle) malloc(sizeof(struct cc_libhandle_struct));
  /* if (!h), FIXME: error handling. 20020906 mortene. */
  h->nativehnd = LoadLibrary(filename);
  /* FIXME: If the return value is NULL, we should call GetLastError() to 
     get extended error information and report this error. 20021015 thammer. */
  return h;
#endif
  return NULL;
}

void *
cc_dl_sym(cc_libhandle handle, const char * symbolname)
{
  void * ptr = NULL;
  if ((handle == NULL) || (handle->nativehnd == NULL)) return NULL;

#ifdef HAVE_DL_LIB
  ptr = dlsym(handle->nativehnd, symbolname);
#elif defined (HAVE_WINDLL_RUNTIME_BINDING)
  ptr = GetProcAddress(handle->nativehnd, symbolname);
  /* FIXME: If the return value is NULL, we should call GetLastError() to 
     get extended error information and report this error. 20021015 thammer. */
#endif /* HAVE_DL_LIB */

  return ptr;
}

void
cc_dl_close(cc_libhandle handle)
{
#ifdef HAVE_DL_LIB
  (void)dlclose(handle->nativehnd);
#elif defined (HAVE_WINDLL_RUNTIME_BINDING)
  (void)FreeLibrary(handle->nativehnd);
  /* FIXME: If the return value is NULL, we should call GetLastError() to 
     get extended error information and report this error. 20021015 thammer. */
#endif /* HAVE_DL_LIB */
  
  free(handle);
}
