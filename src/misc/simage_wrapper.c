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

/* FIXME: support HP-UX shn_load()?

   Some versions of HP-UX have dlopen(). Although according to a
   discussion on the libtool mailinglist it has been buggy in an
   official release, needing a patch to function properly. This is of
   course a good reason to try to use shn_load() *first*, then
   dlopen() on HP-UX.

   Note also that it looks like dlopen() might reside in a library
   "svld" instead of "ld".

   20010626 mortene. */

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
simage_wrapper_versionMatchesAtLeast(int major, int minor, int micro)
{
  assert(simage_instance);
  if (simage_instance->available == 0) return 0;
  if (simage_instance->version.major < major) return 0;
  else if (simage_instance->version.major > major) return 1;
  if (simage_instance->version.minor < minor) return 0;
  else if (simage_instance->version.minor > minor) return 1;
  if (simage_instance->version.micro < micro) return 0;
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

unsigned char * APIENTRY 
simage_wrapper_resize3d(unsigned char * imagedata,
                        int width, int height,
                        int numcomponents,
                        int layers,
                        int newwidth, 
                        int newheight,
                        int newlayers)
{
  return NULL;
}

/* Implemented by using the singleton pattern. */
const simage_wrapper_t *
simage_wrapper(void)
{
  /* FIXME: we're not thread-safe, due to the "get_last_error" design
     of simage. Should keep a single entry-lock here to work around
     this. 20020628 mortene. */

  if (!simage_instance && !simage_failed_to_load) {
    /* First invocation, do initializations. */
    simage_wrapper_t * si = (simage_wrapper_t *)malloc(sizeof(simage_wrapper_t));
    (void)atexit(simage_wrapper_cleanup);

    /* Detect recursive calls. */
    {
      static int is_initializing = 0;
      assert(is_initializing == 0);
      is_initializing = 1;
    }

    si->versionMatchesAtLeast = simage_wrapper_versionMatchesAtLeast;

    /* The common case is that simage is either available from the
       linking process or we're successfully going to link it in. */
    si->available = 1;

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
        si->available = 0;
        simage_failed_to_load = 1;
      }
    }
    /* Define SIMAGEWRAPPER_REGISTER_FUNC macro. Casting the type is
       necessary for this file to be compatible with C++ compilers. */
#ifdef HAVE_HASH_QUOTING
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    si->_funcname_ = (_funcsig_)GET_RUNTIME_SYMBOL(simage_libhandle, #_funcname_)
#elif defined(HAVE_APOSTROPHES_QUOTING)
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    si->_funcname_ = (_funcsig_)GET_RUNTIME_SYMBOL(simage_libhandle, "_funcname_")
#else
#error Unknown quoting.
#endif

#elif defined(SIMAGEWRAPPER_ASSUME_SIMAGE) /* !SIMAGE_RUNTIME_LINKING */

    /* Define SIMAGEWRAPPER_REGISTER_FUNC macro. */
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    si->_funcname_ = (_funcsig_)_funcname_

#else /* !SIMAGEWRAPPER_ASSUME_SIMAGE */
    si->available = 0;
    /* Define SIMAGEWRAPPER_REGISTER_FUNC macro. */
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    si->_funcname_ = NULL

#endif /* !SIMAGEWRAPPER_ASSUME_SIMAGE */

    SIMAGEWRAPPER_REGISTER_FUNC(simage_version, simage_version_t);

    if (si->available && !si->simage_version) {
      /* something is seriously wrong */
      si->available = 0;
      simage_failed_to_load = 1;

      simage_instance = si;
      return simage_instance;
    }

    /* get version */
    if (si->available) {
      si->simage_version(&si->version.major,
                         &si->version.minor,
                         &si->version.micro);
    }

    SIMAGEWRAPPER_REGISTER_FUNC(simage_check_supported, simage_check_supported_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_read_image, simage_read_image_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_get_last_error, simage_get_last_error_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_resize, simage_resize_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_free_image, simage_free_image_t);
    SIMAGEWRAPPER_REGISTER_FUNC(simage_next_power_of_two, simage_next_power_of_two_t);

    /* Do this late, so we can detect recursive calls to this function. */
    simage_instance = si;

    if (simage_wrapper_versionMatchesAtLeast(1,1,0)) {
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
      si->simage_get_saver_handle = simage_wrapper_get_saver_handle;
      si->simage_get_num_savers = simage_wrapper_get_num_savers;
      si->simage_check_save_supported = simage_wrapper_check_save_supported;
      si->simage_save_image = simage_wrapper_save_image;
      si->simage_get_saver_extensions = simage_wrapper_get_saver_extensions;
      si->simage_get_saver_fullname = simage_wrapper_get_saver_fullname;
      si->simage_get_saver_description = simage_wrapper_get_saver_description;
    }

    if (simage_wrapper_versionMatchesAtLeast(1,3,0)) {
#if !defined(HAVE_LIBSIMAGE) || defined(SIMAGE_VERSION_1_3)
      SIMAGEWRAPPER_REGISTER_FUNC(simage_resize3d, simage_resize3d_t);
#endif
    }
    else si->simage_resize3d = NULL;
  }
  return simage_instance;
}
