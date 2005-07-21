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
#endif // HAVE_CONFIG_H
  
#include <Inventor/misc/SoJavaScriptEngine.h>
#include <Inventor/C/glue/spidermonkey.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/tidbitsp.h>

JSRuntime * SoJavaScriptEngine::runtime = NULL;
uint32_t SoJavaScriptEngine::RUNTIME_MAXBYTES = 4 * 1024 * 1024; /* maxbytes allocated before GC runs */
size_t SoJavaScriptEngine::CONTEXT_STACK_CHUNK_SIZE = 8192; /* stack chunk size */

JSRuntime *
SoJavaScriptEngine::getRuntime(void)
{
  return SoJavaScriptEngine::runtime;
}

void
SoJavaScriptEngine::setRuntime(JSRuntime * runtime)
{
  SoJavaScriptEngine::runtime = runtime;
}

void
SoJavaScriptEngine::init(void)
{
  JSRuntime * rt =
    spidermonkey()->JS_NewRuntime(SoJavaScriptEngine::RUNTIME_MAXBYTES);

  if (!rt) {
    SoDebugError::postWarning("SoJavaScriptEngine::init",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to instantiate a JSRuntime!");
    return;
  }
  SoJavaScriptEngine::setRuntime(rt);
}

void
SoJavaScriptEngine::shutdown(void)
{
  JSRuntime * rt = SoJavaScriptEngine::getRuntime();
  if (rt != NULL) {
    spidermonkey()->JS_DestroyRuntime(rt);
  }

  spidermonkey()->JS_ShutDown();
  SoJavaScriptEngine::setRuntime(NULL);
}

SbBool
SoJavaScriptEngine::debug(void)
{
  static int d = -1;
  if (d == -1) {
    const char * env = coin_getenv("COIN_DEBUG_VRMLSCRIPT");
    d = (env && (atoi(env) > 0)) ? 1 : 0;

  }
  return d ? TRUE : FALSE;
}

// FIXME: imported from SquirrelMonkey/src/jsutils.cpp
// 20050719 erikgors.
/*!
  Prints a stacktrace for the pending exception.
  Does nothing if there aren't any pending exceptions.
*/
static void
printJSException(JSContext *cx)
{
  jsval val, stack;
  JSObject * obj;
  char * cstr;
  int len;
  JSString *s;

  if (!spidermonkey()->JS_GetPendingException(cx, &val)) return;
  if (!JSVAL_IS_OBJECT(val)) return;
  obj = JSVAL_TO_OBJECT(val);
  if (!spidermonkey()->JS_GetProperty(cx, obj, "stack", &stack)) return;

  /* print exception.stack */
  if (!(s=spidermonkey()->JS_ValueToString(cx, stack))) {
    SoDebugError::postWarning("printJSException", "could not convert exception to string");
    return;
  }
  /* root the string */
  if (!spidermonkey()->JS_AddRoot(cx, s)) {
    SoDebugError::postWarning("printJSException", "could not root string");
    return;
  }

  /* Todo: we loose unicode information here */
  cstr = spidermonkey()->JS_GetStringBytes(s);
  if (!cstr) {
    SoDebugError::postWarning("printJSException", "could not get string bytes");
    assert(spidermonkey()->JS_RemoveRoot(cx, s));
    return;
  }
  len = spidermonkey()->JS_GetStringLength(s);
  SoDebugError::postWarning("printJSException", "Stack:");
  /*
     Todo: somehow do nice indent
     Note: string might contain \0 => we don't use fputs
  */
  // FIXME: this looks ugly. 20050719 erikgors.
  fwrite(cstr, 1, spidermonkey()->JS_GetStringLength(s), stderr);
  fprintf(stderr, "\n");
  assert(spidermonkey()->JS_RemoveRoot(cx, s));
}

static void
SpiderMonkey_ErrorHandler(JSContext * cx, const char * message, JSErrorReport * report)
{                               
  SoDebugError::postWarning("SpiderMonkey_ErrorHandler",
                            "%s:%d: %s:\n  %s\n",
                            report->filename, report->lineno, message, report->linebuf);
  printJSException(cx);
}   

static JSBool JavascriptPrint(JSContext * cx, JSObject * obj, 
                    uintN argc, jsval * argv, jsval * rval)
{
  SbString out;

  if (argc > 0) {
    // " ".join(argv)
    uintN i;
    for (i=0; i<argc-1; ++i) {
      out += spidermonkey()->JS_GetStringBytes(spidermonkey()->JS_ValueToString(cx, argv[i]));
      out += " ";
    }
    out += spidermonkey()->JS_GetStringBytes(spidermonkey()->JS_ValueToString(cx, argv[i]));
  }
  
  SoDebugError::postInfo("JavascriptPrint", out.getString());
  return JS_TRUE;
}

SoJavaScriptEngine::SoJavaScriptEngine()
{
  JSContext * cx = this->context =
    spidermonkey()->JS_NewContext(SoJavaScriptEngine::getRuntime(), SoJavaScriptEngine::CONTEXT_STACK_CHUNK_SIZE);
  if (!cx) {
    SoDebugError::postWarning("SoJavaScriptEngine::SoJavaScriptEngine",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to set up a JSContext!");
    SoJavaScriptEngine::shutdown();
    return;
  }

  (void)spidermonkey()->JS_SetErrorReporter(cx, SpiderMonkey_ErrorHandler);

  static JSClass jclass = {
    "Coin SoJavaScriptEngine global object class", 0,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_EnumerateStub,
    spidermonkey()->JS_ResolveStub,
    spidermonkey()->JS_ConvertStub, 
    spidermonkey()->JS_FinalizeStub,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0
  };

  // FIXME: add global as an argument, so more than one context can share
  // the same global object? 20050719 erikgors.
  JSObject * global = this->global =
    spidermonkey()->JS_NewObject(cx, &jclass, NULL, NULL);
  if (!global) {
    SoDebugError::postWarning("SoJavaScriptEngine::SoJavaScriptEngine",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to set up a global JSObject!");
    SoJavaScriptEngine::shutdown();
    return;
  }

  // JS_InitStandardClasses also adds gc protection and sets cx's global
  // 20050719 erikgors.
  JSBool ok = spidermonkey()->JS_InitStandardClasses(cx, global);
  if (!ok) {
    SoDebugError::postWarning("SoJavaScriptEngine::SoJavaScriptEngine",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to init standard classes for "
                              "global JSObject!");
    SoJavaScriptEngine::shutdown();
    return;
  }

  // FIXME: maybe this should be optional? 20050719 erikgors.
  spidermonkey()->JS_DefineFunction(cx, global, "print", JavascriptPrint, 0, 0);

  // Make the engine accessable from within the context
  spidermonkey()->JS_SetContextPrivate(cx, this);
}

SoJavaScriptEngine::~SoJavaScriptEngine()
{
  spidermonkey()->JS_DestroyContext(this->context);
}

SbBool
SoJavaScriptEngine::executeScript(const SbName & name, const SbString & script) const
{
  if (SoJavaScriptEngine::debug()) {
    SoDebugError::postInfo("SoJavaScriptEngine::executeScript", "script=='%s'",
                           script.getString());
  }

  JSScript * jsscript =
    spidermonkey()->JS_CompileScript(this->context,
                                     this->global,
                                     script.getString(), script.getLength(),
                                     name.getString(), 1);

  return this->executeJSScript(jsscript);
}

SbBool
SoJavaScriptEngine::executeFile(const SbName & filename) const
{
  if (SoJavaScriptEngine::debug()) {
    SoDebugError::postInfo("SoJavaScriptEngine::executeFile", "filename=='%s'", filename.getString());
  }

  JSScript * script = spidermonkey()->JS_CompileFile(this->context, this->global, filename);

  return this->executeJSScript(script);
}

SbBool
SoJavaScriptEngine::executeJSScript(JSScript * script) const
{
  jsval rval;
  JSBool ok = spidermonkey()->JS_ExecuteScript(this->context, this->global, script, &rval);
  if (ok) {
    if (SoJavaScriptEngine::debug()) {
      JSString * str = spidermonkey()->JS_ValueToString(this->context, rval);
      SoDebugError::postInfo("SoJavaScriptEngine::executeJSScript",
                             "script result: '%s'",
                             spidermonkey()->JS_GetStringBytes(str));
    }
    return TRUE;
  }
  else {
    // FIXME: improve on this. 20050526 mortene.
    SoDebugError::postWarning("SoJavaScriptEngine::executeJSScript",
                              "Script evaluation failed!");
  }
  return FALSE;
}

SbBool
SoJavaScriptEngine::executeFunction(const SbName & name,
                                  int argc, const SoField * argv, SoField * rval) const
{
  jsval * jsargv = new jsval[argc];

  for (int i=0; i<argc; ++i) {
    this->field2jsval(this->context, &argv[i], &jsargv[i]);
  }

  jsval rjsval;
  JSBool ok =
    spidermonkey()->JS_CallFunctionName(this->context, this->global,
                                        name.getString(), argc, jsargv, &rjsval);
  delete [] jsargv;

  if (ok) {
    if (SoJavaScriptEngine::debug()) {
      JSString * str = spidermonkey()->JS_ValueToString(this->context, rjsval);
      SoDebugError::postInfo("SoJavaScriptEngine::executeFunction",
                             "function: \"%s\" "
                             "result: '%s'",
                             name.getString(), spidermonkey()->JS_GetStringBytes(str));
    }

    SbBool ok2 = TRUE;
    if (rval != NULL) {
      ok2 = this->jsval2field(this->context, rjsval, rval);
    }
    return ok2;
  }
  else {
    SoDebugError::postWarning("SoJavaScriptEngine::executeFunctions",
                              "JS_CallFunctionName(..., \"%s\", ...) "
                              "failed!", name.getString());
    return FALSE;
  }
}

SbBool
SoJavaScriptEngine::field2jsval(const SoField * f, jsval * v)
{
  int n = this->handlerList.getLength();

  // go backwards. new handlers has precedence. 20050719 erikgors.
  while (n --> 0) {
    const SoJavaScriptEngine::JavascriptHandler & handler = this->handlerList[n];

    if (handler.field2jsval != NULL && f->isOfType(handler.type)) {
      handler.field2jsval(this->context, f, v);
      return TRUE;
    }
  }
  SoDebugError::postInfo("SoJavaScriptEngine::field2jsval", "no handler found for %s", f->getTypeId().getName().getString());
  *v = JSVAL_VOID;
  return FALSE;
}

SbBool
SoJavaScriptEngine::field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  SoJavaScriptEngine * engine = (SoJavaScriptEngine *)spidermonkey()->JS_GetContextPrivate(cx);
  assert(engine != NULL);
  return engine->field2jsval(f, v);
}

SbBool 
SoJavaScriptEngine::jsval2field(const jsval v, SoField * f)
{
  int n = this->handlerList.getLength();

  // go backwards. new handlers has precedence. 20050719 erikgors.
  while (n --> 0) {
    const SoJavaScriptEngine::JavascriptHandler & handler = this->handlerList[n];

    if (handler.jsval2field != NULL && f->isOfType(handler.type)) {
      if (handler.jsval2field(this->context, v, f)) {
        return TRUE;
      }
      else {
        JSString * jsstr = spidermonkey()->JS_ValueToString(this->context, v); 
        const char * str = spidermonkey()->JS_GetStringBytes(jsstr);
        SoDebugError::postWarning("SoJavaScriptEngine::jsval2field",
                                  "convertion of '%s' to SoField type '%s' failed",
                                  str, handler.type.getName().getString());
        return FALSE;
      }
    }
  }
  SoDebugError::postInfo("SoJavaScriptEngine::jsval2field", "no handler found for %s", f->getTypeId().getName().getString());
  return FALSE;
}

SbBool
SoJavaScriptEngine::jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  SoJavaScriptEngine * engine = (SoJavaScriptEngine *)spidermonkey()->JS_GetContextPrivate(cx);
  assert(engine != NULL);
  return engine->jsval2field(v, f);
}

void
SoJavaScriptEngine::addHandler(const SoType & type, CoinJSinit_t init, CoinJSfield2jsval_t field2jsval, CoinJSjsval2field_t jsval2field)
{
  SoJavaScriptEngine::JavascriptHandler handler;
  handler.type = type;
  handler.init = init;
  handler.field2jsval = field2jsval;
  handler.jsval2field = jsval2field;

  this->handlerList.append(handler);
}

void
SoJavaScriptEngine::initHandlers(void)
{ 
  int i;
  int num = this->handlerList.getLength();
  for (i=0; i<num; ++i) {
    const SoJavaScriptEngine::JavascriptHandler & handler = this->handlerList[i];
    if (handler.init != NULL) {
      handler.init(this->context, this->global);
    }
  }
}

SbBool
SoJavaScriptEngine::setScriptField(const SbName & name, const SoField * f) const
{
  jsval initval;
  SoJavaScriptEngine::field2jsval(this->context, f, &initval);
  const JSBool ok =
    spidermonkey()->JS_SetProperty(this->context,
                                   this->global,
                                   name.getString(), &initval);
  if (!ok) {
    SoDebugError::post("SoJavaScriptEngine::setScriptField",
                       "Could not set field '%s' for "
                       "Javascript engine.", name.getString());
  }
  return ok;
}

SbBool
SoJavaScriptEngine::unsetScriptField(const SbName & name) const
{
  const JSBool ok =
    spidermonkey()->JS_DeleteProperty(this->context,
                                      this->global,
                                      name.getString());
  if (!ok) {
    SoDebugError::post("SoJavaScriptEngine::unsetScriptField",
                       "Could not unset field '%s' for "
                       "Javascript engine.", name.getString());
  }
  return ok;
}
SbBool
SoJavaScriptEngine::getScriptField(const SbName & name, SoField * f) const
{
    jsval val;
    const JSBool ok = spidermonkey()->JS_GetProperty(this->context, this->global,
                                                     name.getString(), &val);
    if (!ok) {
      SoDebugError::post("SoJavaScriptEngine::getScriptField",
                         "Could not find field '%s' as SpiderMonkey "
                         "object property!", name.getString());
      return FALSE;
    }
    if (SoJavaScriptEngine::debug()) {
      JSString * str = spidermonkey()->JS_ValueToString(this->context, val);
      SoDebugError::postInfo("SoJavaScriptEngine::getScriptField", "trying to convert"
                             " \"%s\" to type %s", spidermonkey()->JS_GetStringBytes(str),
                             f->getTypeId().getName().getString());
    }


    return SoJavaScriptEngine::jsval2field(this->context, val, f);
}

SbBool
SoJavaScriptEngine::hasScriptField(const SbName & name) const
{
  jsval val;
  const JSBool ok = spidermonkey()->JS_GetProperty(this->context, this->global,
                                                   name.getString(), &val);
  assert(ok);


  // FIXME: this _should_ be good enough. 20050711 erikgors.
  JSType type = spidermonkey()->JS_TypeOfValue(this->context, val);
  return type != 0;
}
