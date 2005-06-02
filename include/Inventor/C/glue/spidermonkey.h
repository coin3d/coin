#ifndef COIN_GLUE_SPIDERMONKEY_H
#define COIN_GLUE_SPIDERMONKEY_H

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

/*
  Internal Coin interface to the SpiderMonkey Javascript engine, from
  the Mozilla project.

  Coin can bind to the engine dynamically, i.e. by run-time linking,
  or at build-time.
*/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/system/inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

#define SPIDERMONKEY_RUNTIME_LINKING 0

#if !defined(HAVE_DYNAMIC_LINKING) && defined(HAVE_SPIDERMONKEY_LIB)

/* FIXME: static linking with SpiderMonkey library has not been tested
   yet. 20050125 mortene.*/

#define CROSS_COMPILE // FIXME: is this correct? 20050601 mortene.
#include <jsapi.h>

#else /* HAVE_DYNAMIC_LINKING || !HAVE_SPIDERMONKEY_LIB */

#undef SPIDERMONKEY_RUNTIME_LINKING
#define SPIDERMONKEY_RUNTIME_LINKING 1

/*
   Structs and defines.
*/

typedef int JSBool;
typedef long jsword;
typedef jsword jsval;
typedef unsigned int uintN;

typedef enum JSType {
  JSTYPE_VOID,
  JSTYPE_OBJECT,
  JSTYPE_FUNCTION,
  JSTYPE_STRING,
  JSTYPE_NUMBER,
  JSTYPE_BOOLEAN,
  JSTYPE_LIMIT
} JSType;

typedef enum JSAccessMode {
  JSACC_PROTO = 0,
  JSACC_PARENT = 1,
  JSACC_IMPORT = 2,
  JSACC_WATCH = 3,
  JSACC_READ = 4,
  JSACC_WRITE = 8,
  JSACC_LIMIT
} JSAccessMode;

typedef void JSRuntime;
typedef void JSContext;
typedef void JSObject;
typedef void JSObjectOps;
typedef void JSXDRState;
typedef void JSString;
typedef void JSErrorReport;
typedef struct JSClass JSClass;

#define JS_DLL_CALLBACK /* FIXME: set up this define properly. 20050601 mortene. */

typedef JSBool (* JS_DLL_CALLBACK JSPropertyOp)(JSContext *, JSObject *, jsval, jsval *);
typedef JSBool (* JS_DLL_CALLBACK JSEnumerateOp)(JSContext *, JSObject *);
typedef JSBool (* JS_DLL_CALLBACK JSResolveOp)(JSContext *, JSObject *, jsval);
typedef JSBool (* JS_DLL_CALLBACK JSConvertOp)(JSContext *, JSObject *, JSType, jsval *);
typedef void (* JS_DLL_CALLBACK JSFinalizeOp)(JSContext *, JSObject *);
typedef JSObjectOps * (* JS_DLL_CALLBACK JSGetObjectOps)(JSContext *, JSClass *);
typedef JSBool (* JS_DLL_CALLBACK JSCheckAccessOp)(JSContext *, JSObject *, jsval, JSAccessMode, jsval *);
typedef JSBool (* JS_DLL_CALLBACK JSNative)(JSContext *, JSObject *, uintN, jsval *, jsval *);
typedef JSBool (* JS_DLL_CALLBACK JSXDRObjectOp)(JSXDRState *, JSObject **);
typedef JSBool (* JS_DLL_CALLBACK JSHasInstanceOp)(JSContext *, JSObject *, jsval, JSBool *);
typedef uint32_t (* JS_DLL_CALLBACK JSMarkOp)(JSContext *, JSObject *, void *);
typedef void (* JS_DLL_CALLBACK JSErrorReporter)(JSContext *, const char *, JSErrorReport *);

struct JSClass {
  const char * name;
  uint32_t flags;
  JSPropertyOp addProperty;
  JSPropertyOp delProperty;
  JSPropertyOp getProperty;
  JSPropertyOp setProperty;
  JSEnumerateOp enumerate;
  JSResolveOp resolve;
  JSConvertOp convert;
  JSFinalizeOp finalize;
  JSGetObjectOps getObjectOps;
  JSCheckAccessOp checkAccess;
  JSNative call;
  JSNative construct;
  JSXDRObjectOp xdrObject;
  JSHasInstanceOp hasInstance;
  JSMarkOp mark;
  jsword spare;
};

#endif /* HAVE_DYNAMIC_LINKING */


/* Defines and macros. ************************************************** */

#define JSVAL_BOOLEAN 0x6

#define JSVAL_TAGBITS 3
#define JSVAL_SETTAG(v,t) ((v) | (t))

#define BOOLEAN_TO_JSVAL(b) JSVAL_SETTAG((jsval)(b) << JSVAL_TAGBITS, JSVAL_BOOLEAN)
#define JSVAL_TO_BOOLEAN(v) ((JSBool)((v) >> JSVAL_TAGBITS))

/* Function typedefs. *************************************************** */

typedef JSBool (* JS_EvaluateScript_t)(JSContext *, JSObject *, const char *, uintN, const char *, uintN, jsval *);
typedef JSString * (* JS_ValueToString_t)(JSContext *, jsval);
typedef char * (* JS_GetStringBytes_t)(JSString *);
typedef JSBool (* JS_SetProperty_t)(JSContext *, JSObject *, const char *, jsval *);
typedef JSBool (* JS_GetProperty_t)(JSContext *, JSObject *, const char *, jsval *);
typedef JSBool (* JS_CallFunctionName_t)(JSContext *, JSObject *, const char *, uintN, jsval *, jsval *);

typedef JSRuntime * (* JS_NewRuntime_t)(uint32_t);
typedef void (* JS_DestroyRuntime_t)(JSRuntime *);
typedef JSContext * (* JS_NewContext_t)(JSRuntime *, size_t);
typedef void (* JS_DestroyContext_t)(JSContext *);
typedef void (* JS_ShutDown_t)(void);

typedef JSObject * (* JS_NewObject_t)(JSContext *, JSClass *, JSObject *, JSObject *);
typedef JSBool (* JS_InitStandardClasses_t)(JSContext *, JSObject *);

typedef JSErrorReporter (* JS_SetErrorReporter_t)(JSContext *, JSErrorReporter);

typedef JSBool (* JS_PropertyStub_t)(JSContext *, JSObject *, jsval, jsval *);
typedef JSBool (* JS_EnumerateStub_t)(JSContext *, JSObject *);
typedef JSBool (* JS_ResolveStub_t)(JSContext *, JSObject *, jsval);
typedef JSBool (* JS_ConvertStub_t)(JSContext *, JSObject *, JSType, jsval *);
typedef void (* JS_FinalizeStub_t)(JSContext *, JSObject *);

/* Access interface. **************************************************** */

typedef struct {
  int available;

  JS_EvaluateScript_t JS_EvaluateScript;
  JS_ValueToString_t JS_ValueToString;
  JS_GetStringBytes_t JS_GetStringBytes;
  JS_SetProperty_t JS_SetProperty;
  JS_GetProperty_t JS_GetProperty;
  JS_CallFunctionName_t JS_CallFunctionName;

  JS_NewRuntime_t JS_NewRuntime;
  JS_DestroyRuntime_t JS_DestroyRuntime;
  JS_NewContext_t JS_NewContext;
  JS_DestroyContext_t JS_DestroyContext;
  JS_ShutDown_t JS_ShutDown;

  JS_SetErrorReporter_t JS_SetErrorReporter;

  JS_PropertyStub_t JS_PropertyStub;
  JS_EnumerateStub_t JS_EnumerateStub;
  JS_ResolveStub_t JS_ResolveStub;
  JS_ConvertStub_t JS_ConvertStub;
  JS_FinalizeStub_t JS_FinalizeStub;

  JS_NewObject_t JS_NewObject;
  JS_InitStandardClasses_t JS_InitStandardClasses;

} SpiderMonkey_t;

const SpiderMonkey_t * spidermonkey(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_SPIDERMONKEY_H */
