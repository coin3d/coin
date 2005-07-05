/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* !HAVE_CONFIG_H */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <Inventor/C/glue/spidermonkey.h>

#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ******************************************************************** */

static SpiderMonkey_t * spidermonkey_instance = NULL;
static cc_libhandle spidermonkey_libhandle = NULL;
static SbBool spidermonkey_failed_to_load = FALSE;

/* ******************************************************************** */

static SbBool
spidermonkey_debug(void)
{
  static int dbg = -1;
  if (dbg == -1) {
    const char * env = coin_getenv("COIN_DEBUG_SPIDERMONKEY_BINDING");
    dbg = (env && (atoi(env) > 0)) ? 1 : 0;
  }
  return dbg;
}

/* ******************************************************************** */

/* Cleans up at exit. */
static void
spidermonkey_cleanup(void)
{
#ifdef SPIDERMONKEY_RUNTIME_LINKING
  if (spidermonkey_libhandle) { cc_dl_close(spidermonkey_libhandle); }
#endif /* SPIDERMONKEY_RUNTIME_LINKING */

  assert(spidermonkey_instance);
  free(spidermonkey_instance);
}

/* ******************************************************************** */

/* Implemented by using the singleton pattern. */
const SpiderMonkey_t *
spidermonkey(void)
{
  SpiderMonkey_t * sm;

  CC_SYNC_BEGIN(spidermonkey);

  if (spidermonkey_instance || spidermonkey_failed_to_load) { goto wrapperexit; }

  /* Detect recursive calls. */
  {
    static int is_initializing = 0;
    assert(is_initializing == 0);
    is_initializing = 1;
  }


  /* First invocation, do initializations. */
  spidermonkey_instance = sm = (SpiderMonkey_t *)malloc(sizeof(SpiderMonkey_t));
  /* FIXME: handle out-of-memory on malloc(). 20000928 mortene. */
  (void)coin_atexit((coin_atexit_f*) spidermonkey_cleanup, 0);

  /* Be optimistic. */
  sm->available = 1;

#if SPIDERMONKEY_RUNTIME_LINKING

  {
    unsigned int idx;

    /* FIXME: there's a configure mortene. */
    const char * possiblelibnames[] = {
      NULL, /* is set below */
      /* (assumed) MSWindows DLL name */
      "js",

      /* UNIX-style names */
      "js", "libjs", "libjs.so",

      /* terminator */
      NULL
    };
    possiblelibnames[0] = coin_getenv("COIN_SPIDERMONKEY_LIBNAME");
    idx = possiblelibnames[0] ? 0 : 1;

    while (!spidermonkey_libhandle && possiblelibnames[idx]) {
      spidermonkey_libhandle = cc_dl_open(possiblelibnames[idx]);
      idx++;
    }

    if (!spidermonkey_libhandle) {
      sm->available = 0;
      spidermonkey_failed_to_load = 1;
      goto wrapperexit;
    }

    if (spidermonkey_debug()) {
      if (spidermonkey_failed_to_load) {
        cc_debugerror_postinfo("spidermonkey", "Found no SpiderMonkey library on system.");
      }
      else {
        cc_debugerror_postinfo("spidermonkey",
                               "Dynamically loaded SpiderMonkey library as '%s'.",
                               possiblelibnames[idx-1]);
      }
    }
  }


  /* Define macro for grabbing function symbols. Casting the type is
     necessary for this file to be compatible with C++ compilers. */
  #define REGISTER_FUNC(_funcname_, _funcsig_) \
          sm->_funcname_ = (_funcsig_)cc_dl_sym(spidermonkey_libhandle, SO__QUOTE(_funcname_)); \
          assert(sm->_funcname_)

  /* Some functions in SpiderMonkey may have a symbol name different
     from the API name. */
  #define REGISTER_FUNC_ALTERNATE(_funcname_, _altname_, _funcsig_) \
          sm->_funcname_ = (_funcsig_)cc_dl_sym(spidermonkey_libhandle, SO__QUOTE(_funcname_)); \
          if (sm->_funcname_ == NULL) { sm->_funcname_ = (_funcsig_)cc_dl_sym(spidermonkey_libhandle, SO__QUOTE(_altname_)); } \
          assert(sm->_funcname_)

#elif defined(HAVE_SPIDERMONKEY_LIB) /* static linking */

  #define REGISTER_FUNC(_funcname_, _funcsig_) \
          sm->_funcname_ = (_funcsig_)_funcname_; \
          assert(sm->_funcname_)

  #define REGISTER_FUNC_ALTERNATE(_funcname_, _altname_, _funcsig_) \
          REGISTER_FUNC(_funcname_, _funcsig_)

#else /* neither dynamic nor static linking */

  sm->available = 0;

  #define REGISTER_FUNC(_funcname_, _funcsig_) \
          sm->_funcname_ = NULL

  #define REGISTER_FUNC_ALTERNATE(_funcname_, _altname_, _funcsig_) \
          REGISTER_FUNC(_funcname_, _funcsig_)

#endif /* done setting up REGISTER_FUNC */

  REGISTER_FUNC(JS_GetImplementationVersion, JS_GetImplementationVersion_t);
  if (spidermonkey_debug() && sm->JS_GetImplementationVersion) {
    const char * version = sm->JS_GetImplementationVersion();
    cc_debugerror_postinfo("spidermonkey", "%s", version);
  }

  REGISTER_FUNC(JS_EvaluateScript, JS_EvaluateScript_t);
  REGISTER_FUNC(JS_ValueToString, JS_ValueToString_t);
  REGISTER_FUNC(JS_GetStringBytes, JS_GetStringBytes_t);
  REGISTER_FUNC(JS_SetProperty, JS_SetProperty_t);
  REGISTER_FUNC(JS_GetProperty, JS_GetProperty_t);
  REGISTER_FUNC(JS_CallFunctionName, JS_CallFunctionName_t);
  REGISTER_FUNC_ALTERNATE(JS_NewRuntime, JS_Init, JS_NewRuntime_t);
  REGISTER_FUNC_ALTERNATE(JS_DestroyRuntime, JS_Finish, JS_DestroyRuntime_t);
  REGISTER_FUNC(JS_NewContext, JS_NewContext_t);
  REGISTER_FUNC(JS_DestroyContext, JS_DestroyContext_t);
  REGISTER_FUNC(JS_ShutDown, JS_ShutDown_t);
  REGISTER_FUNC(JS_SetErrorReporter, JS_SetErrorReporter_t);
  REGISTER_FUNC(JS_PropertyStub, JS_PropertyStub_t);
  REGISTER_FUNC(JS_EnumerateStub, JS_EnumerateStub_t);
  REGISTER_FUNC(JS_ResolveStub, JS_ResolveStub_t);
  REGISTER_FUNC(JS_ConvertStub, JS_ConvertStub_t);
  REGISTER_FUNC(JS_FinalizeStub, JS_FinalizeStub_t);
  REGISTER_FUNC(JS_NewObject, JS_NewObject_t);
  REGISTER_FUNC(JS_InitStandardClasses, JS_InitStandardClasses_t);

wrapperexit:
  CC_SYNC_END(spidermonkey);
  return spidermonkey_instance;
}

#undef REGISTER_FUNC

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
