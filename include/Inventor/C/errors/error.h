#ifndef CC_ERROR_H
#define CC_ERROR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
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

void cc_error_init(cc_error * me);
void cc_error_clean(cc_error * me);

  /*   const SbString & getDebugString(void) const; */
const cc_string * cc_error_get_debug_string(const cc_error * me);

/*   static void setHandlerCallback(SoErrorCB * const func, void * const data); */

void cc_error_set_handler_callback(cc_error_cb * func, void * data);

/*   static SoErrorCB * getHandlerCallback(void); */

cc_error_cb * cc_error_get_handler_callback(void);

/*   static void * getHandlerData(void); */

void * cc_error_get_handler_data(void);

/*   static void post(const char * const format, ...); */
void cc_error_post(const char * format, ...);

  /* protected: */
  /*   void setDebugString(const char * const str); */

void cc_error_set_debug_string(cc_error * me, const char * str);

  /* protected: */
  /*   void appendToDebugString(const char * const str); */

void cc_error_append_to_debug_string(cc_error * me, const char * str);

  /* protected: */
  /*   void handleError(void); */

void cc_error_handle(cc_error * me);

  /* protected: */
  /*   virtual SoErrorCB * getHandler(void * & data) const; */
cc_error_cb * cc_error_get_handler(void ** data);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !CC_ERROR_H */
