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

// Here's a posting I made on Usenet 2003-02-19 with queries about
// increasing the robustness of the symbol binding process on MSWin
// DLLs. I'm keeping it here for reference, just so we remember that
// there are potential problems:
//
// ------8<------[snip]------8<------[snip]------8<------[snip]----
//    Newsgroups: comp.os.ms-windows.programmer.win32
//    Subject: Possible to do calling convention query?
//    Gcc: nnfolder+archive:out-news
//    From: Morten Eriksen <mortene@sim.no>
//    Organization: The Underpant Gnomes
//    --text follows this line--
//    Hi,
//    
//    I have a delicate problem with robustness which I would like to try to
//    solve. The outline of my problem is as follows:
//    
//      * I'm using LoadLibrary() to bind to the symbols of a DLL at
//        run-time.
//    
//      * This DLL was generated outside of my control.
//    
//      * I bind to function pointer symbols of the DLL with something like
//        the following snippet of code:
//    
//          typedef void (__stdcall * nurbsProperty_t)(void *, int, float);
//          nurbsProperty_t nurbsProperty =
//               (nurbsProperty_t)GetProcAddress(<dllhnd>, "nurbsProperty");
//    
//          // Now the function can be used like any other, e.g. like this:
//          //
//          // nurbsProperty(NULL, 0, 0.0f);
//    
//      * Now, notice the "__stdcall" part of the function signature
//        typedef. As you all probably know, this specifies the _assumed_
//        calling convention of the function, i.e. how arguments are pushed
//        on the stack, and if it's the caller's or the callee's
//        responsibility to clean up the stack afterwards.
//    
//    Now here's the problem I would like to solve: if the "nurbsProperty()"
//    function of the DLL was built with a different calling convention than
//    __stdcall (say, __cdecl), my subsequent invocation of the method will
//    lead to either a corrupted set of input arguments, a corrupted stack
//    upon return, or both. Therefore, I would like to *detect* whether the
//    DLL function symbols actually matches the calling convention I expect.
//    
//    Can this be done through the Win32 API somehow? I.e. getting to know
//    what calling convention an exported function in a DLL was built with?
//    
//    (I guess it can be done by loading the DLL file into memory "raw", and
//    parsing it's structures of the DLL / PE file, but that is _really_ the
//    last resort.)
//    
//    Regards,
//    Morten
//    -- 
//    Ees a sad an' beautiful world
// ------8<------[snip]------8<------[snip]------8<------[snip]----
//
// 20030219 mortene.

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_DLFCN_H
#include <dlfcn.h> /* Programming interface to libdl. */
#endif /* HAVE_DLFCN_H */

#if HAVE_WINDOWS_H
#include <windows.h>
#endif /* HAVE_WINDOWS_H */

#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/tidbits.h>
#include <assert.h>
#include <stddef.h> /* NULL definition. */
#include <stdlib.h> /* atoi() */

struct cc_libhandle_struct {
  void * nativehnd;
  cc_string libname;
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

#ifdef HAVE_WINDLL_RUNTIME_BINDING
/* Returns the string and error code describing the cause of an
   internal Win32 API error. */
static void
cc_dl_get_win32_err(DWORD * lasterr, cc_string * str)
{
  LPTSTR buffer;
  BOOL result;

  *lasterr = GetLastError();
  result = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                         FORMAT_MESSAGE_FROM_SYSTEM |
                         FORMAT_MESSAGE_IGNORE_INSERTS,
                         NULL,
                         *lasterr,
                         0,
                         (LPTSTR)&buffer,
                         0,
                         NULL);

  if (!result) {
    cc_debugerror_post("cc_dl_get_Win32_err", "FormatMessage() failed!?");
  }
  else {
    cc_string_set_text(str, buffer);
    (void)LocalFree(buffer);
  }
}
#endif /* HAVE_WINDLL_RUNTIME_BINDING */


cc_libhandle
cc_dl_open(const char * filename)
{
  cc_libhandle h = (cc_libhandle) malloc(sizeof(struct cc_libhandle_struct));
  /* if (!h), FIXME: exception handling. 20020906 mortene. */
  h->nativehnd = NULL;

  /* FIXME: support HP-UX shn_load()? (dlopen() is missing on older
     HP-UX versions.)

     Some versions of HP-UX has dlopen() (from version 11 and
     onwards?). Although according to a discussion on the libtool
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

  /* FIXME: if filename==NULL, could we use Module32First() and
     Module32Next() to cycle through the loaded modules, to "fake"
     what happens on dlopen(NULL) on UNIX-systems?

     That would still not work on NT4, which is missing the Tool Help
     API. Check in a Win32-API related Usenet group if there is any
     other way to resolve symbols in the current process
     image. 20021015 mortene. */


  /* Don't use GetModuleHandle(): LoadLibrary() will *not* load a new
     image if the module is already loaded, it will only inc the
     reference count.

     Also, GetModuleHandle() doesn't inc the reference count, so it is
     dangerous in the sense that the module could be free'd from
     somewhere else between us opening it, and until it is used for
     resolving symbols.
  */

  if (filename != NULL)
    h->nativehnd = LoadLibrary(filename);
  else
    return NULL;

  /*
    We don't want to call LoadLibrary(NULL) because this causes a 
    crash on some Windows platforms (Crashes on Windows2000 has 
    been reported). 20021101 thammer
   */

  if (cc_dl_debugging() && (h->nativehnd == NULL)) {
    DWORD lasterr;
    cc_string errstr;

    cc_string_construct(&errstr);
    cc_dl_get_win32_err(&lasterr, &errstr);
    cc_debugerror_post("cc_dl_open", "LoadLibrary(\"%s\") failed with: '%s'",
                       filename ? filename : "(null)",
                       cc_string_get_text(&errstr));
    cc_string_clean(&errstr);
  }

#endif

  if (h->nativehnd == NULL) {
    free(h);
    h = NULL;
  }
  else {
    cc_string_construct(&h->libname);
    cc_string_set_text(&h->libname, filename ? filename : "(null)");

    if (cc_dl_debugging()) {
#ifdef HAVE_WINDLL_RUNTIME_BINDING
      char libpath[512];
      DWORD retval = GetModuleFileName(h->nativehnd, libpath, sizeof(libpath));
      assert(retval > 0 && "GetModuleFileName() failed");
      libpath[sizeof(libpath) - 1] = 0;
      cc_debugerror_postinfo("cc_dl_open", "Opened library '%s'", libpath);
#elif defined (HAVE_DL_LIB)
      cc_debugerror_postinfo("cc_dl_open", "Opening library '%s'", cc_string_get_text(&h->libname));
#endif
    }
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
    if (e) {
      cc_debugerror_post("cc_dl_sym", "dlsym(\"%s\", \"%s\") failed with: '%s'",
                         cc_string_get_text(&handle->libname), symbolname, e);
    }
  }

#elif defined (HAVE_WINDLL_RUNTIME_BINDING)
  ptr = GetProcAddress(handle->nativehnd, symbolname);

  if (cc_dl_debugging() && (ptr == NULL)) {
    DWORD lasterr;
    cc_string errstr;

    cc_string_construct(&errstr);
    cc_dl_get_win32_err(&lasterr, &errstr);
    cc_debugerror_post("cc_dl_sym",
                       "GetProcAddress(\"%s\", \"%s\") failed with: '%s'",
                       cc_string_get_text(&handle->libname), symbolname,
                       cc_string_get_text(&errstr));
    cc_string_clean(&errstr);
  }

#endif

  return ptr;
}

void
cc_dl_close(cc_libhandle handle)
{
#ifdef HAVE_DL_LIB

  int result = dlclose(handle->nativehnd);

  if (result != 0) {
    const char * e = dlerror();
    if (e) {
      cc_debugerror_post("cc_dl_close", "dlclose(\"%s\") failed with: '%s'",
                         cc_string_get_text(&handle->libname), e);
    }
  }

#elif defined (HAVE_WINDLL_RUNTIME_BINDING)

  BOOL result = FreeLibrary(handle->nativehnd);

  if (!result) {
    DWORD lasterr;
    cc_string errstr;

    cc_string_construct(&errstr);
    cc_dl_get_win32_err(&lasterr, &errstr);
    cc_debugerror_post("cc_dl_close",
                       "FreeLibrary(\"%s\") failed with: '%s'",
                       cc_string_get_text(&handle->libname),
                       cc_string_get_text(&errstr));
    cc_string_clean(&errstr);
  }

#endif

  cc_string_clean(&handle->libname);
  free(handle);
}
