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

#include <simage_wrapper.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#if HAVE_CONFIG_H
#include <config.h>
#else /* No config.h? Hmm. Assume the simage library is available for linking. */
#define SIMAGEWRAPPER_ASSUME_SIMAGE 1
#endif /* !HAVE_CONFIG_H */
#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#ifdef HAVE_LIBSIMAGE /* In case we're _not_ doing runtime linking. */
#define SIMAGEWRAPPER_ASSUME_SIMAGE 1
#endif /* HAVE_LIBSIMAGE */

#if SIMAGEWRAPPER_ASSUME_SIMAGE
#include <simage.h>
#endif /* SIMAGEWRAPPER_ASSUME_SIMAGE */


#ifdef HAVE_DL_LIB

  /* This should work on Linux and IRIX platforms, at least. Probably
     some other UNIX-based systems aswell. */

  #define LIBHANDLE_T void*
  #define OPEN_RUNTIME_BINDING(LIBNAME) dlopen(LIBNAME, RTLD_LAZY)
  #define CLOSE_RUNTIME_BINDING(RBHANDLE)  (void)dlclose(RBHANDLE)
  #define GET_RUNTIME_SYMBOL(RBHANDLE, FUNCNAME) dlsym(RBHANDLE, FUNCNAME)

#elif defined (HAVE_WINDLL_RUNTIME_BINDING)

  /* This should work on all MSWindows systems. */

  #define LIBHANDLE_T HINSTANCE
  #define OPEN_RUNTIME_BINDING(LIBNAME) LoadLibrary(LIBNAME)
  #define CLOSE_RUNTIME_BINDING(RBHANDLE)  (void)FreeLibrary(RBHANDLE)
  #define GET_RUNTIME_SYMBOL(RBHANDLE, FUNCNAME) GetProcAddress(RBHANDLE, FUNCNAME)

#else /* static binding */

  /* To avoid compiler error on the LIBHANDLE_T type. */
  #define LIBHANDLE_T void*

#endif  /* static binding */

/* FIXME: support HP-UX? (Doesn't have dlopen().) 20010626 mortene. */

static simage_wrapper_t * simage_instance = NULL;
static LIBHANDLE_T simage_libhandle = NULL;
static int simage_failed_to_load = 0;


/* Cleans up at exit. */
static void
simage_wrapper_cleanup(void)
{
#ifdef SIMAGE_RUNTIME_LINKING
  if (simage_libhandle) { CLOSE_RUNTIME_BINDING(simage_libhandle); }
#endif /* SIMAGE_RUNTIME_LINKING */

  assert(simage_instance);
  free(simage_instance);
}

/* backup-functions. More robust when simage is an old version, or not
   available */

static int
simage_wrapper_versionMatchesAtLeast(int major,
                                     int minor,
                                     int micro)
{
  if (simage_wrapper()->available == 0) return 0;
  if (simage_wrapper()->version.major < major) return 0;
  else if (simage_wrapper()->version.major > major) return 1;
  if (simage_wrapper()->version.minor < minor) return 0;
  else if (simage_wrapper()->version.minor > minor) return 1;
  if (simage_wrapper()->version.micro < micro) return 0;
  return 1;
}

static int APIENTRY
simage_wrapper_get_num_savers(void)
{
  return 0;
}

static void * APIENTRY
simage_wrapper_get_saver_handle(int jada)
{
  return NULL;
}

static int APIENTRY
simage_wrapper_check_save_supported(const char * jada)
{
  return 0;
}

static int APIENTRY
simage_wrapper_save_image(const char * jada,
                          const unsigned char * jada2,
                          int jada3, int jada4, int jada5,
                          const char * jada6)
{
  return 0;
}

const char * APIENTRY
simage_wrapper_get_saver_extensions(void * handle)
{
  return "";
}

const char * APIENTRY
simage_wrapper_get_saver_fullname(void * handle)
{
  return NULL;
}

const char * APIENTRY
simage_wrapper_get_saver_description(void * handle)
{
  return NULL;
}

/* Implemented by using the singleton pattern. */
const simage_wrapper_t *
simage_wrapper(void)
{
  if (!simage_instance && !simage_failed_to_load) {
    /* First invocation, do initializations. */
    simage_instance = (simage_wrapper_t *)malloc(sizeof(simage_wrapper_t));
    (void)atexit(simage_wrapper_cleanup);

    simage_instance->versionMatchesAtLeast = simage_wrapper_versionMatchesAtLeast;

    /* The common case is that simage is either available from the
       linking process or we're successfully going to link it in. */
    simage_instance->available = 1;

#ifdef SIMAGE_RUNTIME_LINKING
    {
      /* FIXME: should we get the system shared library name from an
         Autoconf check? 20000930 mortene. */
      const char * possiblelibnames[] = {
        "simage", "libsimage", "libsimage.so",

        /* MSWindows DLL names for the simage library */
        /* FIXME: a bit of a hack this, but it looks difficult to find
           a better strategy. 20010626 mortene. */
        "simage1", "simage2", "simage3", "simage4", "simage5", "simage6",
        NULL
      };

      int idx = 0;
      while (!simage_libhandle && possiblelibnames[idx]) {
        /*
         *  FIXME: Purify complains about Bad Function Parameter here
         *  for dlopen().  Everything seems to work ok though?
         *  pederb, 2001-02-07
         */
        simage_libhandle = OPEN_RUNTIME_BINDING(possiblelibnames[idx]);
        idx++;
      }

      if (!simage_libhandle) {
        simage_instance->available = 0;
        simage_failed_to_load = 1;
      }
    }
    /* Define SIMAGEWRAPPER_REGISTER_FUNC macro. Casting the type is
       necessary for this file to be compatible with C++ compilers. */
#ifdef HAVE_HASH_QUOTING
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    simage_instance->_funcname_ = (_funcsig_)GET_RUNTIME_SYMBOL(simage_libhandle, #_funcname_)
#elif defined(HAVE_APOSTROPHES_QUOTING)
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    simage_instance->_funcname_ = (_funcsig_)GET_RUNTIME_SYMBOL(simage_libhandle, "_funcname_")
#else
#error Unknown quoting.
#endif

#elif defined(SIMAGEWRAPPER_ASSUME_SIMAGE) /* !SIMAGE_RUNTIME_LINKING */

    /* Define SIMAGEWRAPPER_REGISTER_FUNC macro. */
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    simage_instance->_funcname_ = (_funcsig_)_funcname_

#else /* !SIMAGEWRAPPER_ASSUME_SIMAGE */
    simage_instance->available = 0;
    /* Define SIMAGEWRAPPER_REGISTER_FUNC macro. */
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    simage_instance->_funcname_ = NULL

#endif /* !SIMAGEWRAPPER_ASSUME_SIMAGE */

    SIMAGEWRAPPER_REGISTER_FUNC(simage_version, simage_version_t);

    if (simage_instance->available && !simage_instance->simage_version) {
      /* something is seriously wrong */
      simage_instance->available = 0;
      simage_failed_to_load = 1;
      return simage_instance;
    }

    /* get version */
    if (simage_instance->available) {
      simage_instance->simage_version(&simage_instance->version.major,
                                      &simage_instance->version.minor,
                                      &simage_instance->version.micro);
    }

    SIMAGEWRAPPER_REGISTER_FUNC(simage_check_supported, simage_check_supported_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_read_image, simage_read_image_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_get_last_error, simage_get_last_error_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_resize, simage_resize_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_free_image, simage_free_image_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_next_power_of_two, simage_next_power_of_two_t);

    if (simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
#if !defined(HAVE_LIBSIMAGE) || defined(SIMAGE_VERSION_1_1)
      SIMAGEWRAPPER_REGISTER_FUNC(simage_get_num_savers, simage_get_num_savers_t);
      SIMAGEWRAPPER_REGISTER_FUNC(simage_get_saver_handle, simage_get_saver_handle_t);
      SIMAGEWRAPPER_REGISTER_FUNC(simage_check_save_supported, simage_check_save_supported_t);
      SIMAGEWRAPPER_REGISTER_FUNC(simage_save_image, simage_save_image_t);
      SIMAGEWRAPPER_REGISTER_FUNC(simage_get_saver_extensions, simage_get_saver_extensions_t);
      SIMAGEWRAPPER_REGISTER_FUNC(simage_get_saver_fullname, simage_get_saver_fullname_t);
      SIMAGEWRAPPER_REGISTER_FUNC(simage_get_saver_description, simage_get_saver_description_t);
#endif /* !HAVE_LIBSIMAGE || SIMAGE_VERSION_1_1 */
    }
    else {
      simage_instance->simage_get_saver_handle = simage_wrapper_get_saver_handle;
      simage_instance->simage_get_num_savers = simage_wrapper_get_num_savers;
      simage_instance->simage_check_save_supported = simage_wrapper_check_save_supported;
      simage_instance->simage_save_image = simage_wrapper_save_image;
      simage_instance->simage_get_saver_extensions = simage_wrapper_get_saver_extensions;
      simage_instance->simage_get_saver_fullname = simage_wrapper_get_saver_fullname;
      simage_instance->simage_get_saver_description = simage_wrapper_get_saver_description;
    }
  }
  return simage_instance;
}
