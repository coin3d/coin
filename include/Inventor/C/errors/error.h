#ifndef CC_ERROR_H
#define CC_ERROR_H

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

/* FIXME: missing stuff from SoError: type-system. 20020526 mortene. */

/* ********************************************************************** */

#include <Inventor/C/base/string.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

typedef struct cc_error {
  cc_string debugstring;
} cc_error;

typedef void cc_error_cb(const cc_error * err, void * data);

/* ********************************************************************** */

COIN_DLL_API void cc_error_init(cc_error * me);
COIN_DLL_API void cc_error_clean(cc_error * me);
COIN_DLL_API void cc_error_copy(const cc_error * src, cc_error * dst);

  /*   const SbString & getDebugString(void) const; */
COIN_DLL_API const cc_string * cc_error_get_debug_string(const cc_error * me);

/*   static void setHandlerCallback(SoErrorCB * const func, void * const data); */

COIN_DLL_API void cc_error_set_handler_callback(cc_error_cb * func, void * data);

/*   static SoErrorCB * getHandlerCallback(void); */

COIN_DLL_API cc_error_cb * cc_error_get_handler_callback(void);

/*   static void * getHandlerData(void); */

COIN_DLL_API void * cc_error_get_handler_data(void);

/*   static void post(const char * const format, ...); */
COIN_DLL_API void cc_error_post(const char * format, ...);

  /* protected: */
  /*   void setDebugString(const char * const str); */

COIN_DLL_API void cc_error_set_debug_string(cc_error * me, const char * str);

  /* protected: */
  /*   void appendToDebugString(const char * const str); */

COIN_DLL_API void cc_error_append_to_debug_string(cc_error * me, const char * str);

  /* protected: */
  /*   void handleError(void); */

COIN_DLL_API void cc_error_handle(cc_error * me);

  /* protected: */
  /*   virtual SoErrorCB * getHandler(void * & data) const; */
COIN_DLL_API cc_error_cb * cc_error_get_handler(void ** data);

  /* protected: */
  /*   static void defaultHandlerCB(const SoError * error, void * userdata); */
COIN_DLL_API void cc_error_default_handler_cb(const cc_error * err, void * data);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !CC_ERROR_H */
