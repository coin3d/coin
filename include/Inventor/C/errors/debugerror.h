#ifndef CC_DEBUGERROR_H
#define CC_DEBUGERROR_H

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

#include <Inventor/C/basic.h>
#include <Inventor/C/errors/error.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

typedef enum CC_DEBUGERROR_SEVERITY {
  CC_DEBUGERROR_ERROR,
  CC_DEBUGERROR_WARNING,
  CC_DEBUGERROR_INFO
} CC_DEBUGERROR_SEVERITY;

typedef struct cc_debugerror {
  cc_error super; /* make struct is-A cc_error */

  CC_DEBUGERROR_SEVERITY severity;
} cc_debugerror;

/* ********************************************************************** */

/* FIXME: missing stuff from SoDebugError: type-system,
   COIN_DEBUG_BREAK handling, ... 20020524 mortene. */

/* ********************************************************************** */

void cc_debugerror_post(const char * source, const char * format, ...);
void cc_debugerror_postwarning(const char * source, const char * format, ...);
void cc_debugerror_postinfo(const char * source, const char * format, ...);


void cc_debugerror_init(cc_debugerror * me);
void cc_debugerror_clean(cc_debugerror * me);

CC_DEBUGERROR_SEVERITY cc_debugerror_get_severity(const cc_debugerror * me);

void cc_debugerror_set_handler_callback(cc_error_cb * function, void * data);
cc_error_cb * cc_debugerror_get_handler_callback(void);
void * cc_debugerror_get_handler_data(void);

cc_error_cb * cc_debugerror_get_handler(void ** data);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_DEBUGERROR_H */
