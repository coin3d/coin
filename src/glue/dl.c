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
#include <stdlib.h> /* atoi() */
#include <malloc.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/errors/debugerror.h>
#include <../tidbits.h>

struct cc_libhandle_struct {
  void * nativehnd;
};

/* Return value of COIN_DEBUG_DL environment variable. */
static int
cc_dl_debugging(void)
{
  static int d = -1;
  if (d == -1) {
    const char * val = coin_getenv("COIN_DEBUG_DL");
    d = val ? atoi(val) : 0;
  }
  return (d > 0) ? 1 : 0;
}


cc_libhandle
cc_dl_open(const char * filename)
{
  cc_libhandle h = (cc_libhandle) malloc(sizeof(struct cc_libhandle_struct));
  /* if (!h), FIXME: exception handling. 20020906 mortene. */
  h->nativehnd = NULL;

  /* FIXME: support HP-UX shn_load()?

     Some versions of HP-UX have dlopen() (at least 10.x and
     older). Although according to a discussion on the libtool
     mailinglist it has been buggy in an official release, needing a
     patch to function properly. This is of course a good reason to
     try to use shn_load() *first*, then dlopen() on HP-UX.

     Note also that it looks like dlopen() might reside in a library
     "svld" instead of "dl".

     20010626 mortene. */

#ifdef HAVE_DL_LIB

  h->nativehnd = dlopen(filename, RTLD_LAZY);
  /*
    If dlopen() fails for any reason than not being able to find the
    dynamic link-library given by "filename" on disk, we should really
    detect it and report an error, whether we're running in debug mode
    or release mode.

    The libdl interface doesn't provide any means to do that, though,
    so we'll just /assume/ that a NULL return means the library
    couldn't be found.

    But if a special debugging environment variable is found, we'll
    spit out the error message, which could prove useful for remote
    debugging:
  */

  if (cc_dl_debugging() && (h->nativehnd == NULL)) {
    const char * e = dlerror();
    if (e) {
      cc_debugerror_post("cc_dl_open", "dlopen(\"%s\") failed with: '%s'", filename, e);
    }
  }

#elif defined (HAVE_WINDLL_RUNTIME_BINDING)

  h->nativehnd = LoadLibrary(filename);
  /* FIXME: If the return value is NULL, we should call GetLastError() to 
     get extended error information and report this error. 20021015 thammer. */

#endif

  if (h->nativehnd == NULL) {
    free(h);
    h = NULL;
  }

  return h;
}

void *
cc_dl_sym(cc_libhandle handle, const char * symbolname)
{
  void * ptr = NULL;
  if ((handle == NULL) || (handle->nativehnd == NULL)) return NULL;

#ifdef HAVE_DL_LIB

  ptr = dlsym(handle->nativehnd, symbolname);

  if (cc_dl_debugging()) {
    const char * e = dlerror();
    if (e) { cc_debugerror_post("cc_dl_sym", "dlsym() failed with: '%s'", e); }
  }

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

  int result = dlclose(handle->nativehnd);

  if (cc_dl_debugging() && (result != 0)) {
    const char * e = dlerror();
    if (e) { cc_debugerror_post("cc_dl_close", "dlclose() failed with: '%s'", e); }
  }

#elif defined (HAVE_WINDLL_RUNTIME_BINDING)
  (void)FreeLibrary(handle->nativehnd);
  /* FIXME: If the return value is NULL, we should call GetLastError() to 
     get extended error information and report this error. 20021015 thammer. */
#endif /* HAVE_DL_LIB */
  
  free(handle);
}
