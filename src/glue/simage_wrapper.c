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

#include <Inventor/C/basic.h>
#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#else /* No config.h? Hmm. Assume the simage library is available for linking. */
#define SIMAGEWRAPPER_ASSUME_SIMAGE 1
#endif /* !HAVE_CONFIG_H */

#ifdef HAVE_LIBSIMAGE /* In case we're _not_ doing runtime linking. */
#define SIMAGEWRAPPER_ASSUME_SIMAGE 1
#endif /* HAVE_LIBSIMAGE */

#if SIMAGEWRAPPER_ASSUME_SIMAGE
#include <simage.h>
#endif /* SIMAGEWRAPPER_ASSUME_SIMAGE */

#include <simage_wrapper.h>

static simage_wrapper_t * simage_instance = NULL;
static cc_libhandle simage_libhandle = NULL;
static int simage_failed_to_load = 0;


/* Cleans up at exit. */
static void
simage_wrapper_cleanup(void)
{
#ifdef SIMAGE_RUNTIME_LINKING
  if (simage_libhandle) { cc_dl_close(simage_libhandle); }
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

static int
simage_wrapper_get_num_savers(void)
{
  return 0;
}

static void *
simage_wrapper_get_saver_handle(int jada)
{
  return NULL;
}

static int
simage_wrapper_check_save_supported(const char * jada)
{
  return 0;
}

static int
simage_wrapper_save_image(const char * jada,
                          const unsigned char * jada2,
                          int jada3, int jada4, int jada5,
                          const char * jada6)
{
  return 0;
}

const char *
simage_wrapper_get_saver_extensions(void * handle)
{
  return "";
}

const char *
simage_wrapper_get_saver_fullname(void * handle)
{
  return NULL;
}

const char *
simage_wrapper_get_saver_description(void * handle)
{
  return NULL;
}

unsigned char *
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

void
simage_wrapper_s_params_set(s_params * params, ...)
{
  return;
}

int
simage_wrapper_s_params_get(s_params * params, ...)
{
  return 0;
}

s_stream *
simage_wrapper_s_stream_open(const char * filename,
              s_params * params /* | NULL */)
{
  return NULL;
}

s_stream *
simage_wrapper_s_stream_create(const char * filename,
                s_params * params /* | NULL */)
{
  return NULL;
}

void *
simage_wrapper_s_stream_get_buffer(s_stream * stream,
                    void * prealloc /* | NULL */,
                    int *size /* | NULL */,
                    s_params * params /* | NULL */)
{
  return NULL;
}

int
simage_wrapper_s_stream_put_buffer(s_stream * stream, void * buffer,
                    int size, s_params * params /* | NULL */)
{
  return 0;
}

void
simage_wrapper_s_stream_close(s_stream * stream)
{
  return;
}

void
simage_wrapper_s_stream_destroy(s_stream * stream)
{
  return;
}

s_params *
simage_wrapper_s_stream_params(s_stream * stream)
{
  return NULL;
}


/* Implemented by using the singleton pattern. */
const simage_wrapper_t *
simage_wrapper(void)
{
  CC_SYNC_BEGIN(simage_wrapper);

  /* FIXME: we're not thread-safe, due to the "get_last_error" design
     of simage. Should keep a single entry-lock here to work around
     this. 20020628 mortene. */

  if (!simage_instance && !simage_failed_to_load) {

    /* First invocation, do initializations. */
    simage_wrapper_t * si = (simage_wrapper_t *)malloc(sizeof(simage_wrapper_t));
    (void)coin_atexit((coin_atexit_f *)simage_wrapper_cleanup);

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
           a better strategy. Perhaps it'd be a good idea to use an
           application programmer controlled environment variable?
           20010626 mortene. */
        "simage6", "simage5", "simage4", "simage3", "simage2", "simage1",
        /* FIXME: not checking for simageXd.dll names (ie debug
           versions of the library). 20010926 mortene. */
        NULL
      };

      int idx = 0;
      while (!simage_libhandle && possiblelibnames[idx]) {
        simage_libhandle = cc_dl_open(possiblelibnames[idx]);
        idx++;
      }

      if (!simage_libhandle) {
        si->available = 0;
        simage_failed_to_load = 1;
      }
    }
    /* Define SIMAGEWRAPPER_REGISTER_FUNC macro. Casting the type is
       necessary for this file to be compatible with C++ compilers. */
#define SIMAGEWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    si->_funcname_ = (_funcsig_)cc_dl_sym(simage_libhandle, SO__QUOTE(_funcname_))

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
      cc_debugerror_post("simage_wrapper",
                         "Loaded simage DLL ok, but couldn't resolve symbol "
                         "simage_version().");
      si->available = 0;
      simage_failed_to_load = 1;

      simage_instance = si;
    }
    else {
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

      if (simage_wrapper_versionMatchesAtLeast(1,4,0)) {
#if !defined(HAVE_LIBSIMAGE) || defined(SIMAGE_VERSION_1_4)
        SIMAGEWRAPPER_REGISTER_FUNC(simage_resize3d, simage_resize3d_t);

        SIMAGEWRAPPER_REGISTER_FUNC(s_params_set, s_params_set_t);
        SIMAGEWRAPPER_REGISTER_FUNC(s_params_get, s_params_get_t);
        SIMAGEWRAPPER_REGISTER_FUNC(s_stream_open, s_stream_open_t);
        SIMAGEWRAPPER_REGISTER_FUNC(s_stream_get_buffer, s_stream_get_buffer_t);
        SIMAGEWRAPPER_REGISTER_FUNC(s_stream_close, s_stream_close_t);
        SIMAGEWRAPPER_REGISTER_FUNC(s_stream_destroy, s_stream_destroy_t);
        SIMAGEWRAPPER_REGISTER_FUNC(s_stream_params, s_stream_params_t);
#endif
      }
      else {
#if 0
        // 20021018 thammer. I might want to use these later instead
        // of setting all of them to NULL
        si->s_params_set = simage_wrapper_s_params_set;
        si->s_params_get = simage_wrapper_s_params_get;
        si->s_stream_open = simage_wrapper_s_stream_open;
        si->s_stream_get_buffer = simage_wrapper_s_stream_get_buffer;
        si->s_stream_close = simage_wrapper_s_stream_close;
        si->s_stream_destroy = simage_wrapper_s_stream_destroy;
        si->s_stream_params = simage_wrapper_s_stream_params;
#endif
        si->s_params_set = NULL;
        si->s_params_get = NULL;
        si->s_stream_open = NULL;
        si->s_stream_get_buffer = NULL;
        si->s_stream_close = NULL;
        si->s_stream_destroy = NULL;
        si->s_stream_params = NULL;
      }
    }
  }
  CC_SYNC_END(simage_wrapper);
  return simage_instance;
}
