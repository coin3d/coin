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

#ifndef COIN_SIMAGEWRAPPER_H
#define COIN_SIMAGEWRAPPER_H

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

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

/* Typedefinitions of function signatures for simage calls we use. We
   need these for casting from the void-pointer return of dlsym().*/

  typedef void (APIENTRY *simage_version_t)(int *, int *, int *);
  typedef int (APIENTRY *simage_check_supported_t)(const char * filename);
  typedef unsigned char * (APIENTRY *simage_read_image_t)(const char * filename,
                                                          int * w, int * h,
                                                          int * numcomponents);
  typedef int (APIENTRY *simage_check_save_supported_t)(const char * filenameextension);
  typedef int (APIENTRY *simage_save_image_t)(const char * filename,
                                              const unsigned char * bytes,
                                              int w, int h, int numcomponents,
                                              const char * filenameextension);
  typedef const char * (APIENTRY *simage_get_last_error_t)(void);
  typedef unsigned char * (APIENTRY *simage_resize_t)(unsigned char * imagedata,
                                                      int width, int height,
                                                      int numcomponents,
                                                      int newwidth, int newheight);
  typedef void (APIENTRY *simage_free_image_t)(unsigned char * imagedata);
  typedef int (APIENTRY *simage_next_power_of_two_t)(int val);

  typedef int (APIENTRY *simage_get_num_savers_t)(void);
  typedef void * (APIENTRY *simage_get_saver_handle_t)(int idx);
  typedef const char * (APIENTRY *simage_get_saver_extensions_t)(void * handle);
  typedef const char * (APIENTRY *simage_get_saver_fullname_t)(void * handle);
  typedef const char * (APIENTRY *simage_get_saver_description_t)(void * handle);

  typedef struct {
    /* Is the simage library at all available? */
    int available;

    /* simage versioning. */
    struct {
      int major, minor, micro;
    } version;
    int (*versionMatchesAtLeast)(int major,
                                 int minor,
                                 int micro);

    simage_version_t simage_version;
    simage_check_supported_t simage_check_supported;
    simage_read_image_t simage_read_image;
    simage_check_save_supported_t simage_check_save_supported;
    simage_save_image_t simage_save_image;
    simage_get_last_error_t simage_get_last_error;
    simage_resize_t simage_resize;
    simage_free_image_t simage_free_image;
    simage_next_power_of_two_t simage_next_power_of_two;
    simage_get_num_savers_t simage_get_num_savers;
    simage_get_saver_handle_t simage_get_saver_handle;
    simage_get_saver_extensions_t simage_get_saver_extensions;
    simage_get_saver_fullname_t simage_get_saver_fullname;
    simage_get_saver_description_t simage_get_saver_description;
  } simage_wrapper_t;

  const simage_wrapper_t * simage_wrapper(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COIN_SIMAGEWRAPPER_H */
