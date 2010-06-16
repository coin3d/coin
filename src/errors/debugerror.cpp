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

/*! \file debugerror.h */
#include <Inventor/C/errors/debugerror.h>

#include <cstdlib>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "tidbitsp.h"

static cc_debugerror_cb * dbgerr_callback =
  reinterpret_cast<cc_debugerror_cb *>(cc_error_default_handler_cb);
static void * dbgerr_callback_data = NULL;
static SbBool dbgerr_cleanup_function_set = FALSE;

extern "C" {

static void
debugerror_cleanup(void)
{
  dbgerr_callback = reinterpret_cast<cc_debugerror_cb *>(cc_error_default_handler_cb);
  dbgerr_callback_data = NULL;
  dbgerr_cleanup_function_set = FALSE;
}

} // extern "C"

void
cc_debugerror_init(cc_debugerror * me)
{
  cc_error_init(reinterpret_cast<cc_error *>(me));
}

void
cc_debugerror_clean(cc_debugerror * me)
{
  cc_error_clean(reinterpret_cast<cc_error *>(me));
}


CC_DEBUGERROR_SEVERITY
cc_debugerror_get_severity(const cc_debugerror * me)
{
  return me->severity;
}

void
cc_debugerror_set_handler_callback(cc_debugerror_cb * function, void * data)
{
  dbgerr_callback = function;
  dbgerr_callback_data = data;

  if (!dbgerr_cleanup_function_set) {
    coin_atexit(debugerror_cleanup, CC_ATEXIT_MSG_SUBSYSTEM);
    dbgerr_cleanup_function_set = TRUE;
  }
}

cc_debugerror_cb *
cc_debugerror_get_handler_callback(void)
{
  return dbgerr_callback;
}

void *
cc_debugerror_get_handler_data(void)
{
  return dbgerr_callback_data;
}

cc_debugerror_cb *
cc_debugerror_get_handler(void ** data)
{
  *data = dbgerr_callback_data;
  return dbgerr_callback;
}

static void
cc_debugerror_internal_post(const char * source, cc_string * msg,
                            CC_DEBUGERROR_SEVERITY sev, const char * type)
{
  cc_debugerror deberr;

  cc_debugerror_init(&deberr);

  deberr.severity = sev;
  cc_error_set_debug_string(reinterpret_cast<cc_error *>(&deberr), "Coin ");
  cc_error_append_to_debug_string(reinterpret_cast<cc_error *>(&deberr), type);
  cc_error_append_to_debug_string(reinterpret_cast<cc_error *>(&deberr), " in ");
  cc_error_append_to_debug_string(reinterpret_cast<cc_error *>(&deberr), source);
  cc_error_append_to_debug_string(reinterpret_cast<cc_error *>(&deberr), "(): ");
  cc_error_append_to_debug_string(reinterpret_cast<cc_error *>(&deberr), cc_string_get_text(msg));

  if (dbgerr_callback != reinterpret_cast<cc_debugerror_cb *>(cc_error_default_handler_cb)) {
    dbgerr_callback(&deberr, dbgerr_callback_data);
  }
  else {
    cc_error_handle(reinterpret_cast<cc_error *>(&deberr));
  }

  /* FIXME: port to C. 20020524 mortene. */
  /* check_breakpoints(source);*/

  cc_debugerror_clean(&deberr);
}

#define CC_DEBUGERROR_POST(SEVERITY, TYPE) \
  cc_string s; \
  va_list args; \
 \
  va_start(args, format); \
  cc_string_construct(&s); \
  cc_string_vsprintf(&s, format, args); \
  va_end(args); \
 \
  cc_debugerror_internal_post(source, &s, SEVERITY, TYPE); \
  cc_string_clean(&s)

void
cc_debugerror_post(const char * source, const char * format, ...)
{
  CC_DEBUGERROR_POST(CC_DEBUGERROR_ERROR, "error");
}

void
cc_debugerror_postwarning(const char * source, const char * format, ...)
{
  CC_DEBUGERROR_POST(CC_DEBUGERROR_WARNING, "warning");
}

void
cc_debugerror_postinfo(const char * source, const char * format, ...)
{
  CC_DEBUGERROR_POST(CC_DEBUGERROR_INFO, "info");
}

#undef CC_DEBUGERROR_POST
