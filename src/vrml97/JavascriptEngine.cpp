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
  
#include <Inventor/C/glue/spidermonkey.h>

typedef void (* JSfield2jsval_t)(JSContext *, const SoField *, jsval *);
typedef SbBool (* JSjsval2field_t)(JSContext *, const jsval, SoField *);
typedef JSObject * (* JSinit_t)(JSContext *, JSObject * obj);

class JavascriptEngine
{
public:
  JavascriptEngine();
  ~JavascriptEngine();

  static void init(void);
  static void shutdown(void);

  static SbBool debug(void);

  void executeScript(const char * script) const;
  void executeFunction(const SbName &name, int argc, const SoField * argv, SoField * rval=NULL) const;

  void setScriptField(const SbName & name, const SoField * f) const;
  SbBool getScriptField(const SbName & name, SoField * f) const;
  SbBool hasScriptField(const SbName & name) const;

  struct JavascriptHandler {
    SoType type;
    JSinit_t init;
    JSfield2jsval_t field2jsval;
    JSjsval2field_t jsval2field;

    JavascriptHandler()
    {
    }

    JavascriptHandler(const SoType &type, JSinit_t init,
                      JSfield2jsval_t field2jsval, JSjsval2field_t jsval2field)
      : type(type), init(init), field2jsval(field2jsval), jsval2field(jsval2field)
    {
    }
  };

  static void field2jsval(JSContext * cx, const SoField * f, jsval * v);
  static SbBool jsval2field(JSContext * cx, const jsval v, SoField * f);
  void initHandlers(void);


  SbList<JavascriptHandler::JavascriptHandler> handlerList;
  static JSRuntime * runtime;
  JSContext * context;
  JSObject * global;
};

JSRuntime * JavascriptEngine::runtime = NULL;

void
JavascriptEngine::init(void)
{
  JSRuntime * rt = JavascriptEngine::runtime =
    spidermonkey()->JS_NewRuntime(/* maxbytes allocated before GC runs: */ 4 * 1024 * 1024);

  if (!rt) {
    SoDebugError::postWarning("JavascriptEngine::initJS",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to instantiate a JSRuntime!");
    return;
  }
}

void
JavascriptEngine::shutdown(void)
{
  if (JavascriptEngine::runtime) {
    spidermonkey()->JS_DestroyRuntime(JavascriptEngine::runtime);
  }

  spidermonkey()->JS_ShutDown();
}

SbBool
JavascriptEngine::debug(void)
{
  static int d = -1;
  if (d == -1) {
    const char * env = coin_getenv("COIN_DEBUG_VRMLSCRIPT");
    d = (env && (atoi(env) > 0)) ? 1 : 0;

  }
  return d ? TRUE : FALSE;
}

static void
SpiderMonkey_ErrorHandler(JSContext * cx, const char * message, JSErrorReport * report)
{                               
  SoDebugError::postWarning("SpiderMonkey_ErrorHandler",
                            "lineno: %d; %s code: (%s) filename: (%s)", report->lineno, message,
                            report->linebuf, report->filename);
}   

JavascriptEngine::JavascriptEngine()
{
  JSContext * cx = this->context =
    spidermonkey()->JS_NewContext(JavascriptEngine::runtime, /* stack chunk size: */ 8192);
  if (!cx) {
    SoDebugError::postWarning("JavascriptEngine::JavascriptEngine",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to set up a JSContext!");
    JavascriptEngine::shutdown();
    return;
  }

  (void)spidermonkey()->JS_SetErrorReporter(cx, SpiderMonkey_ErrorHandler);

  static JSClass jclass = {
    "Coin SoVRMLScript global object class", 0,
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

  JSObject * global = this->global =
    spidermonkey()->JS_NewObject(cx, &jclass, NULL, NULL);
  if (!global) {
    SoDebugError::postWarning("JavascriptEngine::JavascriptEngine",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to set up a global JSObject!");
    JavascriptEngine::shutdown();
    return;
  }

  JSBool ok = spidermonkey()->JS_InitStandardClasses(cx, global);
  if (!ok) {
    SoDebugError::postWarning("JavascriptEngine::JavascriptEngine",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to init standard classes for "
                              "global JSObject!");
    JavascriptEngine::shutdown();
    return;
  }

  spidermonkey()->JS_SetContextPrivate(cx, this);
}

JavascriptEngine::~JavascriptEngine()
{
  spidermonkey()->JS_DestroyContext(this->context);
}

void
JavascriptEngine::executeScript(const char * script) const
{
  if (JavascriptEngine::debug()) {
    SoDebugError::postInfo("SoVRMLScript::executeScript", "script=='%s'", script);
  }

  jsval rval;
  JSBool ok =
    spidermonkey()->JS_EvaluateScript(this->context,
                                      this->global,
                                      script, strlen(script),
                                      "inline-script", 1, &rval);
  if (JavascriptEngine::debug()) {
    SoDebugError::postInfo("SoVRMLScript::executeScript", "JS_EvaluateScript() => %d", ok);
  }

  if (ok) {
    JSString * str =
      spidermonkey()->JS_ValueToString(this->context, rval);

    if (JavascriptEngine::debug()) {
      SoDebugError::postInfo("SoVRMLScript::executeScript",
                             "script result: '%s'",
                             spidermonkey()->JS_GetStringBytes(str));
    }

  }
  else {
    // FIXME: improve on this. 20050526 mortene.
    SoDebugError::postWarning("SoVRMLScript::executeScript",
                              "Script evaluation failed!");
  }
}

void
JavascriptEngine::executeFunction(const SbName &name,
                                  int argc, const SoField * argv, SoField *rval) const
{
//  assert(name == "Posicion");
  jsval jsargv[argc];

  for (int i=0; i<argc; ++i) {
    this->field2jsval(this->context, &argv[i], &jsargv[i]);
  }

  jsval rjsval;
  JSBool ok =
    spidermonkey()->JS_CallFunctionName(this->context, this->global,
                                        name.getString(), argc, jsargv, &rjsval);
  if (ok) {
    JSString * str =
      spidermonkey()->JS_ValueToString(this->context, rjsval);

    if (JavascriptEngine::debug()) {
      SoDebugError::postInfo("JavascriptEngine::executeFunction",
                             "function: \"%s\" "
                             "result: '%s'",
                             name.getString(), spidermonkey()->JS_GetStringBytes(str));
    }

  }
  else {
    SoDebugError::postWarning("JavascriptEngine::executeFunctions",
                              "JS_CallFunctionName(..., \"%s\", ...) "
                              "failed!", name.getString());
  }

  if (rval != NULL) {
    this->jsval2field(this->context, rjsval, rval);
  }
}

void
JavascriptEngine::field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  int i;
  void * data = spidermonkey()->JS_GetContextPrivate(cx);
  JavascriptEngine * engine = (JavascriptEngine *)data;

  int num = engine->handlerList.getLength();
  for (i=0; i<num; ++i) {
    const JavascriptEngine::JavascriptHandler & convert = engine->handlerList[i];

    if (f->isOfType(convert.type)) {
      convert.field2jsval(cx, f, v);
      return;
    }
  }
  SoDebugError::postInfo("JavascriptEngine::field2jsval", "couldnt convert %s", f->getTypeId().getName().getString());
  *v = JSVAL_FALSE;
  assert(0);
}

SbBool 
JavascriptEngine::jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  int i;
  void * data = spidermonkey()->JS_GetContextPrivate(cx);
  JavascriptEngine * engine = (JavascriptEngine *)data;


  int num = engine->handlerList.getLength();
  for (i=0; i<num; ++i) {
    const JavascriptEngine::JavascriptHandler & convert = engine->handlerList[i];

    if (f->isOfType(convert.type)) {
      return convert.jsval2field(cx, v, f);
    }
  }
  SoDebugError::postInfo("JavascriptEngine::jsval2field", "couldnt convert %s", f->getTypeId().getName().getString());
  assert(0);
}

void
JavascriptEngine::initHandlers(void)
{ 
  int i;
  int num = this->handlerList.getLength();
  for (i=0; i<num; ++i) {
    const JavascriptEngine::JavascriptHandler & handler = this->handlerList[i];
    if (handler.init != NULL) {
      handler.init(this->context, this->global);
    }
  }
}

void
JavascriptEngine::setScriptField(const SbName & name, const SoField * f) const
{
  jsval initval;
  JavascriptEngine::field2jsval(this->context, f, &initval);
  const JSBool ok =
    spidermonkey()->JS_SetProperty(this->context,
                                   this->global,
                                   name.getString(), &initval);
  if (!ok) {
    SoDebugError::post("JavascriptEngine::setScriptField",
                       "Could not set field '%s' for "
                       "Javascript engine.", name.getString());
  }
}

SbBool
JavascriptEngine::getScriptField(const SbName & name, SoField * f) const
{
    jsval val;
    const JSBool ok = spidermonkey()->JS_GetProperty(this->context, this->global,
                                                     name.getString(), &val);
    if (!ok) {
      SoDebugError::post("JavascriptEngine::getScriptField",
                         "Could not find field '%s' as SpiderMonkey "
                         "object property!", name.getString());
      return FALSE;
    }
    if (JavascriptEngine::debug()) {
      JSString * str = spidermonkey()->JS_ValueToString(this->context, val);
      SoDebugError::postInfo("JavascriptEngine::getScriptField", "trying to convert"
                             " \"%s\" to type %s", spidermonkey()->JS_GetStringBytes(str),
                             f->getTypeId().getName().getString());
    }


    return JavascriptEngine::jsval2field(this->context, val, f);
}

SbBool
JavascriptEngine::hasScriptField(const SbName & name) const
{
  jsval val;
  const JSBool ok = spidermonkey()->JS_GetProperty(this->context, this->global,
                                                   name.getString(), &val);
  assert(ok);


  // FIXME: this _should_ be good enough. 20050711 erikgors.
  JSType type = spidermonkey()->JS_TypeOfValue(this->context, val);
  return type != 0;
}
