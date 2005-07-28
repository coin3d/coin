#ifndef COIN_SOJAVASCRIPTENGINE_H
#define COIN_SOJAVASCRIPTENGINE_H

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
  
#include <Inventor/misc/SoScriptEngine.h>
#include <Inventor/C/glue/spidermonkey.h>
#include <Inventor/lists/SbList.h>

#define RUNTIME_MAXBYTES 4194304

// FIXME: change this to use the Cheshire Cat/Bridge Pattern.
// 20050721 erikgors.

typedef void (* CoinJSfield2jsval_t)(JSContext *, const SoField *, jsval *);
typedef SbBool (* CoinJSjsval2field_t)(JSContext *, const jsval, SoField *);
typedef JSObject * (* CoinJSinit_t)(JSContext *, JSObject * obj);

class COIN_DLL_API SoJavaScriptEngine : public SoScriptEngine {

public:
  SoJavaScriptEngine();
  virtual ~SoJavaScriptEngine();

  virtual SbBool executeScript(const SbName & name, const SbString & script) const;
  virtual SbBool executeFile(const SbName & filename) const;

  virtual SbBool executeFunction(const SbName &name, int argc, const SoField * argv, SoField * rval=NULL) const;

  virtual SbBool setScriptField(const SbName & name, const SoField * f) const;
  virtual SbBool unsetScriptField(const SbName & name) const;
  virtual SbBool getScriptField(const SbName & name, SoField * f) const;
  virtual SbBool hasScriptField(const SbName & name) const;

  // Everthing under here is javascript specific
  static void init(uint32_t maxbytes=RUNTIME_MAXBYTES);
  static void shutdown(void);
  static SbBool debug(void);

  static SbBool field2jsval(JSContext * cx, const SoField * f, jsval * v);
  static SbBool jsval2field(JSContext * cx, const jsval v, SoField * f);

  SbBool field2jsval(const SoField * f, jsval * v);
  SbBool jsval2field(const jsval v, SoField * f);

  static JSRuntime * getRuntime(void);
  JSContext * getContext(void);
  JSObject * getGlobal(void);

  // new handlers will get precedence over old handlers
  void addHandler(const SoType & type, CoinJSinit_t init, CoinJSfield2jsval_t field2jsval, CoinJSjsval2field_t jsval2field);

  SbBool executeJSScript(JSScript * script) const;

protected:
  static void setRuntime(JSRuntime * runtime);
  void setContext(JSContext * context);
  void setGlobal(JSObject * global);

private:
  // Constants
  static size_t CONTEXT_STACK_CHUNK_SIZE; /* stack chunk size */

  static JSRuntime * runtime;
  JSContext * context;
  JSObject * global;

  struct JavascriptHandler {
    SoType type;
    CoinJSinit_t init;
    CoinJSfield2jsval_t field2jsval;
    CoinJSjsval2field_t jsval2field;
  };

  SbList<JavascriptHandler> handlerList;
};

#endif // !COIN_SOJAVASCRIPTENGINE_H
