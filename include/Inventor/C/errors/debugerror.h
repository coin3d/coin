#ifndef CC_DEBUGERROR_H
#define CC_DEBUGERROR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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

typedef void cc_debugerror_cb(const cc_debugerror * err, void * data);

/* ********************************************************************** */

/* FIXME: missing stuff from SoDebugError: type-system,
   COIN_DEBUG_BREAK handling, ... 20020524 mortene. */

/* ********************************************************************** */

COIN_DLL_API void cc_debugerror_post(const char * source, const char * format, ...);
COIN_DLL_API void cc_debugerror_postwarning(const char * source, const char * format, ...);
COIN_DLL_API void cc_debugerror_postinfo(const char * source, const char * format, ...);


COIN_DLL_API void cc_debugerror_init(cc_debugerror * me);
COIN_DLL_API void cc_debugerror_clean(cc_debugerror * me);

COIN_DLL_API CC_DEBUGERROR_SEVERITY cc_debugerror_get_severity(const cc_debugerror * me);

COIN_DLL_API void cc_debugerror_set_handler_callback(cc_debugerror_cb * function, void * data);
COIN_DLL_API cc_debugerror_cb * cc_debugerror_get_handler_callback(void);
COIN_DLL_API void * cc_debugerror_get_handler_data(void);

COIN_DLL_API cc_debugerror_cb * cc_debugerror_get_handler(void ** data);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_DEBUGERROR_H */
