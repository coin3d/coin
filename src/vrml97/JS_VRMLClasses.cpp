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

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>

#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>


#include <Inventor/SoDB.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>

#define JSVAL_IS_SFVEC2F(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &JS_SFVec2fClass, NULL))
#define JSVAL_IS_SFVEC3F(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &JS_SFVec3fClass, NULL))
#define JSVAL_IS_SFCOLOR(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &JS_SFColorClass, NULL))
#define JSVAL_IS_SFROTATION(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &JS_SFRotationClass, NULL))

// Factory methods for converting to javascript objects
static JSObject * JS_SFColorFactory(JSContext * cx, const SbColor & self);
static JSObject * JS_SFNodeFactory(JSContext * cx, SoNode * container);
static JSObject * JS_SFVec2fFactory(JSContext * cx, const SbVec2f & self);
static JSObject * JS_SFVec3fFactory(JSContext * cx, const SbVec3f & self);
static JSObject * JS_SFRotationFactory(JSContext * cx, const SbRotation & self);


// struct used when we need to connect private data to javascript objects
struct JS_SFPrivate {
  SoType type;
  void * obj;
};

// easy to use getIndex for SFVec2f, SFVec3f, SFRotation and SFColor
static int getIndex(jsval id, SoType type) {
  if (JSVAL_IS_INT(id)) {
    return JSVAL_TO_INT(id);
  }
  else if (JSVAL_IS_STRING(id)) {
    const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(id));
    if (SbName("x") == str && (type == SoSFVec2f::getClassTypeId() ||
                               type == SoSFVec3f::getClassTypeId() ||
                               type == SoSFRotation::getClassTypeId())) {
      return 0;
    }
    else if (SbName("y") == str && (type == SoSFVec3f::getClassTypeId() ||
                                    type == SoSFVec3f::getClassTypeId() ||
                                    type == SoSFRotation::getClassTypeId())) {
      return 1;
    }
    else if (SbName("z") == str && (type == SoSFVec3f::getClassTypeId() ||
                                    type == SoSFRotation::getClassTypeId())) {
      return 2;
    }
    else if (SbName("r") == str && type == SoSFColor::getClassTypeId()) {
      return 0;
    }
    else if (SbName("g") == str && type == SoSFColor::getClassTypeId()) {
      return 1;
    }
    else if (SbName("b") == str && type == SoSFColor::getClassTypeId()) {
      return 2;
    }
    else if (SbName("angle") == str && type == SoSFRotation::getClassTypeId()) {
      return 3;
    }

  }
  return -1;
}

static JSBool JS_SFGet(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  if (data == NULL) {
    // this will only happen when JS_NewObject calls "constructor" 
    assert(JSVAL_IS_STRING(id));
    const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(id));
    assert(SbName("constructor") == str);
    return JSVAL_FALSE;
  }

  int index = getIndex(id, data->type);

  if (index != -1) {
    float newVal;

    if (data->type == SoSFVec2f::getClassTypeId()) {
      if (index > 1) {
        return JSVAL_FALSE;
      }
      SbVec2f & vec = *(SbVec2f *)data->obj;
      newVal = vec[index];
    }
    else if (data->type == SoSFVec3f::getClassTypeId()) {
      if (index > 2) {
        return JSVAL_FALSE;
      }
      SbVec3f & vec = *(SbVec3f *)data->obj;
      newVal = vec[index];
    }
    else if (data->type == SoSFColor::getClassTypeId()) {
      if (index > 2) {
        return JSVAL_FALSE;
      }
      SbColor & col = *(SbColor *)data->obj;
      newVal = col[index];
    }
    else if (data->type == SoSFRotation::getClassTypeId()) {
      if (index > 3) {
        return JSVAL_FALSE;
      }
      SbVec4f & rot = *(SbVec4f *)data->obj;
      newVal = rot[index];
    }
    else {
      SoDebugError::postWarning("JS_SFGet", "wtf: %s", data->type.getName().getString());
      assert(0);
    }
    JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, newVal, rval);
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool JS_SFSet(JSContext * cx, JSObject * obj, jsval id, jsval * val)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  int index = getIndex(id, data->type);

  if (index != -1) {
    if (JSVAL_IS_NUMBER(*val)) {
      double number;
      spidermonkey()->JS_ValueToNumber(cx, *val, &number);

      if (data->type == SoSFVec2f::getClassTypeId()) {
        if (index > 1) {
          return JSVAL_FALSE;
        }
        SbVec2f & vec = *(SbVec2f *)data->obj;
        vec[index] = (float)number;
      }
      else if (data->type == SoSFVec3f::getClassTypeId()) {
        if (index > 2) {
          return JSVAL_FALSE;
        }
        SbVec3f & vec = *(SbVec3f *)data->obj;
        vec[index] = (float)number;
      }
      else if (data->type == SoSFColor::getClassTypeId()) {
        if (index > 2) {
          return JSVAL_FALSE;
        }
        SbColor & col = *(SbColor *)data->obj;
        col[index] = (float)number;
      }
      else if (data->type == SoSFRotation::getClassTypeId()) {
        if (index > 3) {
          return JSVAL_FALSE;
        }
        SbVec4f & rot = *(SbVec4f *)data->obj;
        rot[index] = (float)number;
      }
      else {
        assert(0);
      }
    }
    else {
      SoDebugError::postWarning("JS_SFSet", "val is not a number");
    }
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static void convertDoubles(JSContext * cx, uint32_t n, float * out, uint32_t argc, jsval * argv)
{
  uint32_t i;
  for (i=0; i<n && i<argc; ++i) {
    if (JSVAL_IS_NUMBER(argv[i])) {
      double val;
      spidermonkey()->JS_ValueToNumber(cx, argv[i], &val);
      out[i] = (float)val;
    }
    else {
      SoDebugError::postWarning("convertDoubles", "arg %d is not a number", i);
    }
  }
}

static void JS_SFDestructor(JSContext * cx, JSObject * obj)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (data->type == SoSFVec2f::getClassTypeId()) {
    SbVec2f * vec = (SbVec2f *)data->obj;
    delete vec;
  }
  else if (data->type == SoSFVec3f::getClassTypeId()) {
    SbVec3f * vec = (SbVec3f *)data->obj;
    delete vec;
  }
  else if (data->type == SoSFColor::getClassTypeId()) {
    SbColor * vec = (SbColor *)data->obj;
    delete vec;
  }
  else if (data->type == SoSFRotation::getClassTypeId()) {
    SbVec4f * vec = (SbVec4f *)data->obj;
    delete vec;
  }
  else {
    assert(0);
  }
  delete data;
}

JSClass JS_SFColorClass = {
  "SFColor", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_SFGet,
  JS_SFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_SFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};

JSClass JS_SFRotationClass = {
  "SFRotation", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_SFGet,
  JS_SFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_SFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};

JSClass JS_SFVec2fClass = {
  "SFVec2f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_SFGet,
  JS_SFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_SFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};

JSClass JS_SFVec3fClass = {
  "SFVec3f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_SFGet,
  JS_SFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_SFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};


static JSBool JS_SFVec2fConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  float vals[] = {0.0, 0.0};

  convertDoubles(cx, 2, vals, argc, argv);
  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFVec2f::getClassTypeId();
  priv->obj = new SbVec2f(vals);
  spidermonkey()->JS_SetPrivate(cx, obj, priv);
  *rval = OBJECT_TO_JSVAL(obj);

  return JSVAL_TRUE;
} 

static JSBool JS_SFVec3fConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  float vals[] = {0.0, 0.0, 0.0};

  convertDoubles(cx, 3, vals, argc, argv);
  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFVec3f::getClassTypeId();
  priv->obj = new SbVec3f(vals);
  spidermonkey()->JS_SetPrivate(cx, obj, priv);
  *rval = OBJECT_TO_JSVAL(obj);

  return JSVAL_TRUE;
} 

static JSBool JS_SFColorConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  float vals[] = {0.0, 0.0, 0.0};

  convertDoubles(cx, 3, vals, argc, argv);
  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFColor::getClassTypeId();
  priv->obj = new SbColor(vals);
  spidermonkey()->JS_SetPrivate(cx, obj, priv);
  *rval = OBJECT_TO_JSVAL(obj);

  return JSVAL_TRUE;
} 


static JSBool JS_SFRotationConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFRotation::getClassTypeId();
  spidermonkey()->JS_SetPrivate(cx, obj, priv);
  *rval = OBJECT_TO_JSVAL(obj);


  if (argc == 2) {
    if (JSVAL_IS_SFVEC3F(cx, argv[0])) {
      JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
      SbVec3f &vec = *(SbVec3f *)data->obj;

      if (JSVAL_IS_SFVEC3F(cx, argv[1])) { // new SFRotation(SFVec3f fromVector, SFVec3f toVector)
        JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[1]));
        SbVec3f &vec2 = *(SbVec3f *)data->obj;

        SbRotation rot(vec, vec2);
        SbVec3f axis;
        float rad;
        rot.getValue(axis, rad);

        priv->obj = new SbVec4f(axis[0], axis[1], axis[2], rad);

        return JSVAL_TRUE;
      }
      else if (JSVAL_IS_NUMBER(argv[1])) { // new SFRotation(SFVec3f axis, numeric angle)
        double number;
        spidermonkey()->JS_ValueToNumber(cx, argv[1], &number);

        priv->obj = new SbVec4f(vec[0], vec[1], vec[2], (float)number);

        return JSVAL_TRUE;
      }
    }
  }
  // new SFRotation(numeric x, numeric y, numeric z, numeric angle)
  // Missing values default to 0.0, except y, which defaults to 1.0.
  //
  // SbRotation will default to 0.0, 0.0, 1.0, when angle is 0.0
  // So we use SbVec4f to hold values for SFRotation, since we need to support
  // patterns like this:
  // var colors = new MFColor();
  // colors.length = 10;
  // colors[0].x = 1
  // colors[0].y = 0
  // colors[0].z = 0
  // colors[0].angle = 1.8
  // 
  // This will not work when SbRotation holds the values. 20050714 erikgors.

  float vals[] = {0.0, 1.0, 0.0, 0.0};

  convertDoubles(cx, 4, vals, argc, argv);
  priv->obj = new SbVec4f(vals[0], vals[1], vals[2], vals[3]);

  return JSVAL_TRUE;
} 


static JSBool JS_SFVec2f_add(JSContext * cx, JSObject * obj, uintN argc, 
                           jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC2F(cx, argv[0])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec2f & vec1 = *(SbVec2f *)data->obj;
    SbVec2f & vec2 = *(SbVec2f *)data2->obj;

    *rval = OBJECT_TO_JSVAL(JS_SFVec2fFactory(cx, vec1 + vec2));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool JS_SFVec3f_add(JSContext * cx, JSObject * obj, uintN argc, 
                           jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec3f & vec1 = *(SbVec3f *)data->obj;
    SbVec3f & vec2 = *(SbVec3f *)data2->obj;

    *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, vec1 + vec2));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool JS_SFVec2f_divide(JSContext * cx, JSObject * obj, uintN argc, 
                                jsval * argv, jsval * rval)
{
  SbVec2f & vec = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec2f newVec = vec / (float)number;
    *rval = OBJECT_TO_JSVAL(JS_SFVec2fFactory(cx, newVec));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool JS_SFVec3f_divide(JSContext * cx, JSObject * obj, uintN argc, 
                                jsval * argv, jsval * rval)
{
  SbVec3f & vec = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec3f newVec = vec / (float)number;
    *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, newVec));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool JS_SFVec2f_dot(JSContext *cx, JSObject *obj, uintN argc, 
                                  jsval *argv, jsval *rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec2f & vec = *(SbVec2f *)data->obj;

  if (argc >= 1 && JSVAL_IS_SFVEC2F(cx, argv[0])) {
    SbVec2f & vec2 = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    float dot = vec.dot(vec2);
    JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, dot, rval);
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool JS_SFVec3f_dot(JSContext *cx, JSObject *obj, uintN argc, 
                                  jsval *argv, jsval *rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec3f & vec = *(SbVec3f *)data->obj;

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {

    SbVec3f & vec2 = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    float dot = vec.dot(vec2);
    JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, dot, rval);
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool JS_SFVec2_length(JSContext * cx, JSObject * obj, uintN argc, 
                                  jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec2f * vec = (SbVec2f *)data->obj;
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, vec->length(), rval);
  return JSVAL_TRUE;
}

static JSBool JS_SFVec3_length(JSContext * cx, JSObject * obj, uintN argc, 
                                  jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec3f * vec = (SbVec3f *)data->obj;
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, vec->length(), rval);
  return JSVAL_TRUE;
}

static JSBool JS_SFVec2f_multiply(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec2f & vec = *(SbVec2f *)data->obj;

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec2f newVec = vec * (float)number;

    *rval = OBJECT_TO_JSVAL(JS_SFVec2fFactory(cx, newVec));

    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool JS_SFVec3f_multiply(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec3f & vec = *(SbVec3f *)data->obj;

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec3f newVec = vec * (float)number;
    *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, newVec));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool JS_SFVec2f_normalize(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  SbVec2f vec = *(SbVec2f *)data->obj;
  vec.normalize();
  *rval = OBJECT_TO_JSVAL(JS_SFVec2fFactory(cx, vec));

  return JSVAL_TRUE;
}

static JSBool JS_SFVec3f_normalize(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  SbVec3f vec = *(SbVec3f *)data->obj;
  vec.normalize();
  *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, vec));

  return JSVAL_TRUE;
}

static JSBool JS_SFVec3f_negate(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  SbVec3f vec = *(SbVec3f *)data->obj;
  vec.negate();
  *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, vec));

  return JSVAL_TRUE;
}

static JSBool JS_SFVec2f_substract(JSContext * cx, JSObject * obj, uintN argc, 
                                 jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC2F(cx, argv[0])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec2f & vec1 = *(SbVec2f *)data->obj;
    SbVec2f & vec2 = *(SbVec2f *)data2->obj;

    *rval = OBJECT_TO_JSVAL(JS_SFVec2fFactory(cx, vec1 - vec2));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool JS_SFVec3f_substract(JSContext * cx, JSObject * obj, uintN argc, 
                                   jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec3f & vec1 = *(SbVec3f *)data->obj;
    SbVec3f & vec2 = *(SbVec3f *)data2->obj;

    *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, vec1 - vec2));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool JS_SFColor_setHSV(JSContext * cx, JSObject * obj, uintN argc, 
                                   jsval * argv, jsval * rval)
{
  if (argc != 3) {
    return JSVAL_FALSE;
  }
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbColor * color = (SbColor *)data->obj;

  float vals[3];
  convertDoubles(cx, 3, vals, argc, argv);

  color->setHSVValue(vals);
  
  *rval = JSVAL_FALSE;
  return JSVAL_TRUE;
}

static JSBool JS_SFColor_getHSV(JSContext * cx, JSObject * obj, uintN argc, 
                                jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbColor * color = (SbColor *)data->obj;

  float vals[3];
  color->getHSVValue(vals);

  jsval vector[3];
  for (int i=0; i<3; ++i) {
    spidermonkey()->JS_NewDoubleValue(cx, vals[i], &vector[i]);
  }

  *rval = OBJECT_TO_JSVAL(spidermonkey()->JS_NewArrayObject(cx, 3, vector));

  return JSVAL_TRUE;
}

static JSBool JS_SFRotation_getAxis(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  const SbVec4f & rot = *(SbVec4f *)data->obj;

  SbVec3f axis(rot[0], rot[1], rot[2]);

  *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, axis));
  return JSVAL_TRUE;
}

static JSBool JS_SFRotation_inverse(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  const SbVec4f & rot = *(SbVec4f *)data->obj;

  SbVec3f axis(rot[0], rot[1], rot[2]);
  SbRotation newRot(axis, rot[3]);
  newRot.invert();

  *rval = OBJECT_TO_JSVAL(JS_SFRotationFactory(cx, newRot));
  return JSVAL_TRUE;
}

static JSBool JS_SFRotation_multiply(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFROTATION(cx, argv[0])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec4f & vec1 = *(SbVec4f *)data->obj;
    SbVec4f & vec2 = *(SbVec4f *)data2->obj;
    SbVec3f axis1(vec1[0], vec1[1], vec1[2]);
    SbVec3f axis2(vec2[0], vec2[1], vec2[2]);

    SbRotation result = SbRotation(axis1, vec1[3]) * SbRotation(axis2, vec2[3]);

    *rval = OBJECT_TO_JSVAL(JS_SFRotationFactory(cx, result));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool JS_SFRotation_multVec(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec4f & vec = *(SbVec4f *)data->obj;
  
  SbVec3f axis(vec[0], vec[1], vec[2]);
  SbRotation rot(axis, vec[3]);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec3f & src = *(SbVec3f *)data2->obj;
    SbVec3f dst;
    rot.multVec(src, dst);

    *rval = OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, dst));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool JS_SFRotation_setAxis(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec4f & rot = *(SbVec4f *)data->obj;

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec3f & axis = *(SbVec3f *)data2->obj;
    rot[0] = axis[0];
    rot[1] = axis[1];
    rot[2] = axis[2];

    *rval = JSVAL_VOID;
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool JS_SFRotation_slerp(JSContext * cx, JSObject * obj, uintN argc, 
                                  jsval * argv, jsval * rval)
{
  JS_SFPrivate * data = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec4f & vec = *(SbVec4f *)data->obj;
  SbVec3f axis(vec[0], vec[1], vec[2]);
  SbRotation rot(axis, vec[3]);

  if (argc >= 2 && JSVAL_IS_SFROTATION(cx, argv[0]) && JSVAL_IS_NUMBER(argv[1])) {
    JS_SFPrivate * data2 = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    SbVec4f & vec2 = *(SbVec4f *)data->obj;
    SbVec3f axis2(vec2[0], vec2[1], vec2[2]);
    SbRotation dest(axis2, vec2[3]);

    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[1], &number);

    *rval = OBJECT_TO_JSVAL(JS_SFRotationFactory(cx, SbRotation::slerp(rot, dest, (float)number)));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSFunctionSpec JS_SFVec2fFunctions[] = {
  {"add", JS_SFVec2f_add, 1, 0, 0},
  {"divide", JS_SFVec2f_divide, 1, 0, 0},
  {"dot", JS_SFVec2f_dot, 1, 0, 0},
  {"length", JS_SFVec2_length, 0, 0, 0},
  {"multiply", JS_SFVec2f_multiply, 1, 0, 0},
  {"normalize", JS_SFVec2f_normalize, 0, 0, 0},
  {"substract", JS_SFVec2f_substract, 1, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSFunctionSpec JS_SFVec3fFunctions[] = {
  {"add", JS_SFVec3f_add, 1, 0, 0},
  {"divide", JS_SFVec3f_divide, 1, 0, 0},
  {"dot", JS_SFVec3f_dot, 1, 0, 0},
  {"length", JS_SFVec3_length, 0, 0, 0},
  {"multiply", JS_SFVec3f_multiply, 1, 0, 0},
  {"normalize", JS_SFVec3f_normalize, 0, 0, 0},
  {"negate", JS_SFVec3f_negate, 0, 0, 0},
  {"substract", JS_SFVec3f_substract, 1, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSFunctionSpec JS_SFColorFunctions[] = {
  {"setHSV", JS_SFColor_setHSV, 3, 0, 0},
  {"getHSV", JS_SFColor_getHSV, 0, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSFunctionSpec JS_SFRotationFunctions[] = {
  {"getAxis", JS_SFRotation_getAxis, 0, 0, 0},
  {"inverse", JS_SFRotation_inverse, 0, 0, 0},
  {"multiply", JS_SFRotation_multiply, 1, 0, 0},
  {"multVec", JS_SFRotation_multVec, 1, 0, 0},
  {"setAxis", JS_SFRotation_setAxis, 1, 0, 0},
  {"slerp", JS_SFRotation_slerp, 2, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSObject * JS_SFVec2fFactory(JSContext * cx, const SbVec2f & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &JS_SFVec2fClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, JS_SFVec2fFunctions);

  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFVec2f::getClassTypeId();
  priv->obj = new SbVec2f(self);
  spidermonkey()->JS_SetPrivate(cx, obj, priv);


  return obj;
}

static JSObject * JS_SFVec3fFactory(JSContext * cx, const SbVec3f & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &JS_SFVec3fClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, JS_SFVec3fFunctions);

  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFVec3f::getClassTypeId();
  priv->obj = new SbVec3f(self);
  spidermonkey()->JS_SetPrivate(cx, obj, priv);

  return obj;
}

static JSObject * JS_SFColorFactory(JSContext * cx, const SbColor & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &JS_SFColorClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, JS_SFColorFunctions);

  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFColor::getClassTypeId();
  priv->obj = new SbColor(self);
  spidermonkey()->JS_SetPrivate(cx, obj, priv);

  return obj;
}

static JSObject * JS_SFRotationFactory(JSContext * cx, const SbRotation & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &JS_SFRotationClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, JS_SFRotationFunctions);

  JS_SFPrivate * priv = new JS_SFPrivate;
  priv->type = SoSFRotation::getClassTypeId();
  SbVec3f axis;
  float angle;
  priv->obj = new SbVec4f(axis[0], axis[1], axis[2], axis[3]);
  spidermonkey()->JS_SetPrivate(cx, obj, priv);

  return obj;
}

static JSObject * JS_SFVec2f_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_SFVec2fClass, 
                               JS_SFVec2fConstructor, 0,
                               NULL, JS_SFVec2fFunctions, NULL, NULL);
}

static JSObject * JS_SFVec3f_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_SFVec3fClass, 
                               JS_SFVec3fConstructor, 0,
                               NULL, JS_SFVec3fFunctions, NULL, NULL);
}

static JSObject * JS_SFColor_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_SFColorClass, 
                               JS_SFColorConstructor, 0,
                               NULL, JS_SFColorFunctions, NULL, NULL);
}

static JSObject * JS_SFRotation_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_SFRotationClass, 
                               JS_SFRotationConstructor, 0,
                               NULL, JS_SFRotationFunctions, NULL, NULL);
}

// *************************************************************************
// SFNode

static JSBool JS_SFNode_get(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{ 
  SoNode * container = (SoNode *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (container == NULL) {
    // this will only happen when JS_NewObject calls "constructor" 
    // or the node is "undefined"
    
    if (JSVAL_IS_STRING(id)) {
      const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(id));
      if (SbName("constructor") == str) {
        return JSVAL_FALSE;
      }
    }
    SoDebugError::postWarning("JS_SFNode_get", "node is 'undefined'");
    return JSVAL_FALSE;
  }

  if (JSVAL_IS_STRING(id)) {
    SbString str(spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(id)));

    SoField * out = container->getField(SbName(str));

    int len = str.getLength();
    static const char post[] = "_changed";
    static const size_t postLen = sizeof(post) - 1;

    if (out == NULL && len > (int)postLen &&
        str.getSubString(len - postLen) == post) {
      out = container->getField(SbName(str.getSubString(0, len - postLen - 1)));
    }

    if (out != NULL) {
      JavascriptEngine::field2jsval(cx, out, rval);
      return JSVAL_TRUE;
    }
    else {
      SoDebugError::postWarning("JS_SFNode_get", "field %s does not exists", str.getString());
    }
  }
  
  return JSVAL_FALSE;
}

static JSBool JS_SFNode_set(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{ 
  SoNode * container = (SoNode *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (container == NULL) {
    SoDebugError::postWarning("JS_SFNode_set", "node is 'undefined'");
    return JSVAL_FALSE;
  }

  if (JSVAL_IS_STRING(id)) {
    SbString str(spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(id)));

    SoField * in = container->getField(SbName(str));

    int len = str.getLength();
    static const char pre[] = "set_";
    static const size_t preLen = sizeof(pre) - 1;

    if (in == NULL && len > (int)preLen &&
        str.getSubString(0, preLen - 1) == pre) {
      in = container->getField(SbName(str.getSubString(preLen)));
    }

    if (in != NULL) {
      JavascriptEngine::jsval2field(cx, *rval, in);
      if (JavascriptEngine::debug()) {
        SoDebugError::postInfo("JS_SFNode_set", "setting field %s", str.getString());
      }
      return JSVAL_TRUE;
    }
    else {
      SoDebugError::postWarning("JS_SFNode_set", "field %s does not exists", str.getString());
      const SoFieldData * data = container->getFieldData();
      int numFields = data->getNumFields();
      for (int i=0; i<numFields; ++i) {
        const SbName & name = data->getFieldName(i);
        const SoField * f = data->getField(container, i);
        SoDebugError::postInfo("JS_SFNode_set",
                               "possible field: %s", name.getString());
      }

    }
  }
  
  return JSVAL_FALSE;
}

static void JS_SFNodeDestructor(JSContext * cx, JSObject * obj)
{
  SoNode * container = (SoNode *)spidermonkey()->JS_GetPrivate(cx, obj);
  assert(container != NULL);
  container->unref();
}

JSClass JS_SFNodeClass = {
  "SFNode", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_SFNode_get,
  JS_SFNode_set,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_SFNodeDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

static JSObject * JS_SFNodeFactory(JSContext * cx, SoNode * container)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &JS_SFNodeClass, NULL, NULL);
  spidermonkey()->JS_SetPrivate(cx, obj, container);
  container->ref();
  return obj;
}

static JSBool JS_SFNodeConstructor(JSContext * cx, JSObject * obj, 
                                    uintN argc, jsval * argv, jsval *rval)
{
  // spidermonkey ignores the return value

  if (argc >= 1 && JSVAL_IS_STRING(argv[0])) {
    JSString * js = JSVAL_TO_STRING(argv[0]);
    char * str = spidermonkey()->JS_GetStringBytes(js);
    size_t len = spidermonkey()->JS_GetStringLength(js);
    
    // FIXME: what about UTF8? 20050701 erikgors.

    if (JavascriptEngine::debug()) {
      SoDebugError::postInfo("JS_SFNodeConstructor",
                             "creating new node with str = '%s'", str);
    }
                             

    SoInput input = SoInput();
    const char * array[2];
    array[0] = str;
    array[1] = NULL;
    input.setStringArray(array);

    SoVRMLGroup * group = SoDB::readAllVRML(&input);

    if (group == NULL) {
      SoDebugError::postWarning("JS_SFNodeConstructor", "input is not legal VRML string");
      return JSVAL_FALSE;
    }
    if (group->getNumChildren() > 1) {
      SoDebugError::postWarning("JS_SFNodeConstructor", "more than one top-level node, "
                                                         "result is undefined");
      return JSVAL_FALSE;
    }
    if (group->getNumChildren() == 0) {
      SoDebugError::postWarning("JS_SFNodeConstructor", "no top-level node, "
                                                         "result is undefined");
      *rval = JSVAL_VOID;
      return JSVAL_FALSE;
    }

    SoNode * node = group->getChild(0);

    node->ref();
    spidermonkey()->JS_SetPrivate(cx, obj, node);

    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSObject * JS_SFNode_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_SFNodeClass,
                                      JS_SFNodeConstructor, 0,
                                      NULL, NULL, NULL, NULL);
}

// *************************************************************************
// VrmlMatrix

/*
static JSBool JS_VrmlMatrix_get(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{
  if (JSVAL_IS_INT(id)) {
    int index = JSVAL_TO_INT(id);
    if (index > 4 || index < 0) {
      return JSVAL_FALSE;
    }
  }
}
*/

// *************************************************************************
// jsval2field

static SbBool JS_SF_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_SFVEC2F(cx, v) || JSVAL_IS_SFVEC3F(cx, v) ||
      JSVAL_IS_SFCOLOR(cx, v) || JSVAL_IS_SFROTATION(cx, v)) {

    JS_SFPrivate * priv = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(v));

    if (f->isOfType(SoSFColor::getClassTypeId())) {
      ((SoSFColor *)f)->setValue(*(SbColor *)priv->obj);
    }
    else if (f->isOfType(SoSFImage::getClassTypeId())) {
      // FIXME: implement this. 20050701 erikgors.
      assert(0);
    }
    else if (f->isOfType(SoSFRotation::getClassTypeId())) {
      const SbVec4f & vec = *(SbVec4f *)priv->obj;
      SbVec3f axis(vec[0], vec[1], vec[2]);
      ((SoSFRotation *)f)->setValue(SbRotation(axis, vec[3]));
    }
    else if (f->isOfType(SoSFVec2f::getClassTypeId())) {
      ((SoSFVec2f *)f)->setValue(*(SbVec2f *)priv->obj);
    }
    else if (f->isOfType(SoSFVec3f::getClassTypeId())) {
      ((SoSFVec3f *)f)->setValue(*(SbVec3f *)priv->obj);
    }
    else {
      assert(0 && "should not happen");
    }
    return TRUE;
  }
  else {
    SoDebugError::postWarning("JS_SF_jsval2field",
                              "coulnt not convert jsval");
    return FALSE;
  }
}


static SbBool JS_SFBool_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_BOOLEAN(v)) {
    const SbBool b = JSVAL_TO_BOOLEAN(v);
    ((SoSFBool *)f)->setValue(b);
  }
  else {
    JSBool b;
    if (spidermonkey()->JS_ValueToBoolean(cx, v, &b)) {
      ((SoSFBool *)f)->setValue(b);
    }
    else {
      SoDebugError::postWarning("JS_SFBool_jsval2field",
                                "bool convertion of jsval failed");
      return FALSE;
    }
  }
  return TRUE;
}

static SbBool JS_SFNode_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_OBJECT(v) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(v), &JS_SFNodeClass, NULL)) {
    SoNode * node = (SoNode *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(v));
    ((SoSFNode *)f)->setValue(node);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("JS_SFNode::jsval2field",
                              "coulnt not convert jsval to SoNode");
    return FALSE;
  }
}

// basic types
//
static SbBool JS_SFInt32_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_INT(v)) {
    const int32_t val = JSVAL_TO_INT(v);
    ((SoSFInt32 *)f)->setValue(val);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("JS_SFInt32_jsval2field",
                              "int convertion of jsval failed");
    return FALSE;
  }
}

static SbBool JS_SFFloat_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_NUMBER(v)) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, v, &number);
    ((SoSFFloat *)f)->setValue((float)number);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("JS_SFFloat_jsval2field",
                              "float convertion of jsval failed");
    return FALSE;
  }
}

static SbBool JS_SFTime_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_NUMBER(v)) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, v, &number);
    ((SoSFTime*)f)->setValue(SbTime(number));
    return TRUE;
  }
  else {
    SoDebugError::postWarning("JS_SFFloat_jsval2field",
                              "float convertion of jsval failed");
    return FALSE;
  }
}

static SbBool JS_SFString_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_STRING(v)) {
    const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(v));
    ((SoSFString *)f)->setValue(str);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("JS_SFString_jsval2field",
                              "str convertion of jsval failed");
    return FALSE;
  }
}

// *************************************************************************
// field2jsval

static void JS_SFColor_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbColor & val = ((SoSFColor *)f)->getValue();
  JSObject * obj = JS_SFColorFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

// FIXME: SFImage missing

static void JS_SFNode_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  SoNode * node = ((SoSFNode *)f)->getValue();
  JSObject * obj = JS_SFNodeFactory(cx, node);
  *v = OBJECT_TO_JSVAL(obj);
}

static void JS_SFRotation_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbRotation & val = ((SoSFRotation *)f)->getValue();
  JSObject * obj = JS_SFRotationFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

static void JS_SFVec2f_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbVec2f & val = ((SoSFVec2f *)f)->getValue();
  JSObject * obj = JS_SFVec2fFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

static void JS_SFVec3f_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbVec3f & val = ((SoSFVec3f *)f)->getValue();
  JSObject * obj = JS_SFVec3fFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

// base types

static void JS_SFBool_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const SbBool val = ((SoSFBool *)f)->getValue();
  *v = BOOLEAN_TO_JSVAL(val);
}

static void JS_SFInt32_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const int32_t val = ((SoSFInt32 *)f)->getValue();
  *v = INT_TO_JSVAL(val);
}

static void JS_SFFloat_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const float val = ((SoSFFloat *)f)->getValue();
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, val, v);
}

static void JS_SFTime_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const SbTime & time = ((SoSFTime *)f)->getValue();
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, time.getValue(), v);
}

static void JS_SFString_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const SbString & val = ((SoSFString *)f)->getValue();
  JSString * str = spidermonkey()->JS_NewStringCopyZ(cx, val.getString());
  *v = STRING_TO_JSVAL(str);
}


// *************************************************************************
// MF classes

typedef SbBool (* JSCheckType_t)(JSContext *cx, JSObject * obj, jsval val);

struct JS_MFPrivate {
  SoType type;
  JSObject * array;
  JSClass * cls;
  JSNative constructor;
  JSCheckType_t checkType;
};

static JSBool JS_MFGet(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{
  JS_MFPrivate * data = (JS_MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (JSVAL_IS_INT(id)) {
    int index = JSVAL_TO_INT(id);
    return spidermonkey()->JS_GetElement(cx, data->array, index, rval);
  }
  else if (JSVAL_IS_STRING(id)) {
    const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(id));
    if (SbName("length") == str) {
      uint32_t length;
      JSBool ok = spidermonkey()->JS_GetArrayLength(cx, data->array, &length);
      assert(ok);
      *rval = INT_TO_JSVAL(length);
      return JSVAL_TRUE;
    }
  }

  return JSVAL_FALSE;
}

static void JS_MF_resize(JSContext * cx, JS_MFPrivate * data, int newLength) {
  uint32_t length;
  JSBool ok = spidermonkey()->JS_GetArrayLength(cx, data->array, &length);
  assert(ok);

  if ((int)length > newLength) {
    spidermonkey()->JS_SetArrayLength(cx, data->array, newLength);
  }
  else {
    // expand and fill with new objects
    for (int index=length; index<newLength; ++index) {
      jsval val;

      if (data->type == SoSFInt32::getClassTypeId() ||
          data->type == SoSFFloat::getClassTypeId()) {
        val = INT_TO_JSVAL(0);
      }
      else if (data->type == SoSFString::getClassTypeId()) {
        JSString * str = spidermonkey()->JS_NewStringCopyZ(cx, "");
        val = STRING_TO_JSVAL(str);
      }
      else if (data->type == SoSFNode::getClassTypeId()) {
        // All elements not explicitly initialized are set to NULL
      }
      else {
        JSObject * newObj = spidermonkey()->JS_NewObject(cx, data->cls, NULL, NULL);
        ok = data->constructor(cx, newObj, 0, NULL, &val);
        assert(ok);
        assert(newObj == JSVAL_TO_OBJECT(val));
      }

      ok = spidermonkey()->JS_SetElement(cx, data->array, index, &val);
      assert(ok);
    }
  }
}

static JSBool JS_MFSet(JSContext * cx, JSObject * obj, jsval id, jsval * val)
{
  JS_MFPrivate * data = (JS_MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (JSVAL_IS_INT(id)) {
    // Check if val is of wrong type
    if (!data->checkType(cx, obj, *val)) {
      return JSVAL_FALSE;
    }

    int index = JSVAL_TO_INT(id);

    // check for bounds
    if (index < 0) {
      return JSVAL_FALSE;
    }

    // resize if necessary
    uint32_t length;
    JSBool ok = spidermonkey()->JS_GetArrayLength(cx, data->array, &length);
    assert(ok);
    if (index >= (int)length) {
      JS_MF_resize(cx, data, index + 1);
    }

    // assign it
    ok = spidermonkey()->JS_SetElement(cx, data->array, index, val);
    assert(ok);

    return JSVAL_TRUE;
  }
  else if (JSVAL_IS_STRING(id)) {
    const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(id));
    if (SbName("length") == str && JSVAL_IS_INT(*val)) {
      // resize to new length
      JS_MF_resize(cx, data, JSVAL_TO_INT(*val));
      return JSVAL_TRUE;
    }
  }

  return JSVAL_FALSE;
}

static SbBool JS_MFCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  if (JSVAL_IS_OBJECT(val)) {
    JS_MFPrivate * data = (JS_MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
    return spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(val), data->cls, NULL);
  }
  return FALSE;
}

static void JS_MFConstructor(JSContext * cx, JSObject * obj,
                              const SoType & type, JSClass * cls,
                              JSNative constructor, JSCheckType_t checkType,
                              uintN argc, jsval * argv)
{
  JS_MFPrivate * data = new JS_MFPrivate;
  data->type = type;
  data->cls = cls;
  data->array = spidermonkey()->JS_NewArrayObject(cx, 0, NULL);
  data->constructor = constructor;
  data->checkType = checkType;

  // add gc protection
  JSBool ok = spidermonkey()->JS_AddRoot(cx, &data->array);
  assert(ok);

  spidermonkey()->JS_SetPrivate(cx, obj, data);

  JS_MF_resize(cx, data, argc);
  for (int i=0; i<(int)argc; ++i) {
    if (data->checkType(cx, obj, argv[i])) {
      ok = spidermonkey()->JS_SetElement(cx, data->array, i, &argv[i]);
      assert(ok);
    }
  }
}

static void JS_MFDestructor(JSContext * cx, JSObject * obj)
{
  JS_MFPrivate * data = (JS_MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  // remove gc protection
  spidermonkey()->JS_RemoveRoot(cx, &data->array);
  delete data;
}

static JSFunctionSpec JS_MFFunctions[] = {
//  {"toString", JS_MF_toString, 0, 0, 0},
  {NULL, NULL, 0, 0, 0}
};    

JSClass JS_MFColorClass = {
  "MFColor", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass JS_MFRotationClass = {
  "MFRotation", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass JS_MFVec2fClass = {
  "MFVec2f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass JS_MFVec3fClass = {
  "MFVec3f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass JS_MFFloatClass = {
  "MFFloat", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass JS_MFInt32Class = {
  "MFInt32", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass JS_MFStringClass = {
  "MFString", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass JS_MFTimeClass = {
  "MFTime", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};


JSClass JS_MFNodeClass = {
  "MFNode", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  JS_MFGet,
  JS_MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  JS_MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

static JSBool JS_MFColorConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFBool::getClassTypeId(),
                    &JS_SFColorClass, JS_SFColorConstructor,
                    JS_MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
} 

static JSObject * JS_MFColor_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFColorClass,
                                      JS_MFColorConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static JSBool JS_MFRotationConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFRotation::getClassTypeId(),
                    &JS_SFRotationClass, JS_SFRotationConstructor,
                    JS_MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFRotation_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFRotationClass,
                                      JS_MFRotationConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static JSBool JS_MFVec2fConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFVec2f::getClassTypeId(),
                    &JS_SFVec2fClass, JS_SFVec2fConstructor,
                    JS_MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFVec2f_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFVec2fClass,
                                      JS_MFVec2fConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static JSBool JS_MFVec3fConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFVec3f::getClassTypeId(),
                    &JS_SFVec3fClass, JS_SFVec3fConstructor,
                    JS_MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFVec3f_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFVec3fClass,
                                      JS_MFVec3fConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static SbBool JS_numberCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  return JSVAL_IS_NUMBER(val);
}

static JSBool JS_MFFloatConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFFloat::getClassTypeId(),
                    NULL, NULL,
                    JS_numberCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFFloat_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFFloatClass,
                                      JS_MFFloatConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static SbBool JS_intCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  return JSVAL_IS_INT(val);
}

static JSBool JS_MFInt32Constructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFInt32::getClassTypeId(),
                    NULL, NULL,
                    JS_intCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFInt32_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFInt32Class,
                                      JS_MFInt32Constructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static SbBool JS_stringCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  return JSVAL_IS_STRING(val);
}

static JSBool JS_MFStringConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFString::getClassTypeId(),
                    NULL, NULL,
                    JS_stringCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFString_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFStringClass,
                                      JS_MFStringConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static JSBool JS_MFTimeConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFTime::getClassTypeId(),
                    NULL, NULL,
                    JS_numberCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFTime_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFTimeClass,
                                      JS_MFTimeConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

static JSBool JS_MFNodeConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  JS_MFConstructor(cx, obj, SoSFNode::getClassTypeId(),
                    &JS_SFNodeClass, NULL,
                    JS_MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * JS_MFNode_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &JS_MFNodeClass,
                                      JS_MFNodeConstructor, 0,
                                      NULL, JS_MFFunctions, NULL, NULL);
}

typedef void (* SetValue_t)(SoField *, int, JSContext *, jsval);
typedef jsval (* GetValue_t)(const SoField *, int, JSContext *);

// MFColor
static void JS_MFColorSetValue(SoField * f, int i, JSContext * cx, jsval element)
{
  JS_SFPrivate * priv = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(element));
  ((SoMFColor *)f)->set1Value(i, *(SbColor *)priv->obj);
}

static jsval JS_MFColorGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFColor & mf = *((SoMFColor *)f);
  return OBJECT_TO_JSVAL(JS_SFColorFactory(cx, mf[i]));
}

// MFFloat
static void JS_MFFloatSetValue(SoField * f, int i, JSContext * cx, jsval element)
{ 
  double number;
  spidermonkey()->JS_ValueToNumber(cx, element, &number);
  ((SoMFFloat *)f)->set1Value(i, (float)number);
}

static jsval JS_MFFloatGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFFloat & mf = *((SoMFFloat *)f);
  jsval val;
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, mf[i], &val);
  assert(ok);
  return val;
}

// MFInt32
static void JS_MFInt32SetValue(SoField * f, int i, JSContext * cx, jsval element)
{ 
  assert(JSVAL_IS_INT(element)); 
  ((SoMFInt32 *)f)->set1Value(i, JSVAL_TO_INT(element));
}

static jsval JS_MFInt32GetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFInt32 & mf = *((SoMFInt32 *)f);
  return INT_TO_JSVAL(mf[i]);
}

// MFNode
static void JS_MFNodeSetValue(SoField * f, int i, JSContext * cx, jsval element)
{                                                                                            JS_SFPrivate * priv = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(element));
  ((SoMFNode *)f)->set1Value(i, (SoNode *)priv->obj);
}

static jsval JS_MFNodeGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFNode & mf = *((SoMFNode *)f);
  return OBJECT_TO_JSVAL(JS_SFNodeFactory(cx, mf[i]));
}

// MFRotation
static void JS_MFRotationSetValue(SoField * f, int i, JSContext * cx, jsval element)
{
  JS_SFPrivate * priv = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(element));
  ((SoMFRotation *)f)->set1Value(i, *(SbRotation *)priv->obj);
}

static jsval JS_MFRotationGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFRotation & mf = *((SoMFRotation *)f);
  return OBJECT_TO_JSVAL(JS_SFRotationFactory(cx, mf[i]));
}

// MFString
static void JS_MFStringSetValue(SoField * f, int i, JSContext * cx, jsval element)
{
  const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(element));
  ((SoMFString *)f)->set1Value(i, SbString(str));
}

static jsval JS_MFStringGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFString & mf = *((SoMFString *)f);
  return STRING_TO_JSVAL(spidermonkey()->JS_NewStringCopyZ(cx, mf[i].getString()));
}

// MFTime
static void JS_MFTimeSetValue(SoField * f, int i, JSContext * cx, jsval element)
{
  double number;
  spidermonkey()->JS_ValueToNumber(cx, element, &number);
  ((SoMFTime *)f)->set1Value(i, (float)number);
}

static jsval JS_MFTimeGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFTime & mf = *((SoMFTime *)f);
  jsval val;
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, mf[i].getValue(), &val);
  assert(ok);
  return val;
}

// MFVec2f
static void JS_MFVec2fSetValue(SoField * f, int i, JSContext * cx, jsval element)
{
  JS_SFPrivate * priv = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(element));
  ((SoMFVec2f *)f)->set1Value(i, *(SbVec2f *)priv->obj);
}

static jsval JS_MFVec2fGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFVec2f & mf = *((SoMFVec2f *)f);
  return OBJECT_TO_JSVAL(JS_SFVec2fFactory(cx, mf[i]));
}


// MFVec3f
static void JS_MFVec3fSetValue(SoField * f, int i, JSContext * cx, jsval element)
{
  JS_SFPrivate * priv = (JS_SFPrivate *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(element));
  ((SoMFVec3f *)f)->set1Value(i, *(SbVec3f *)priv->obj);
}

static jsval JS_MFVec3fGetValue(const SoField * f, int i, JSContext * cx)
{
  const SoMFVec3f & mf = *((SoMFVec3f *)f);
  return OBJECT_TO_JSVAL(JS_SFVec3fFactory(cx, mf[i]));
}


// *************************************************************************
// jsval2field

static SbBool JS_MF_jsval2field(JSContext * cx, const jsval v, SoField * f, JSClass * cls, SetValue_t setValue)
{
  if (JSVAL_IS_OBJECT(v) && 
      spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(v), cls, NULL)) {
    JSObject * obj = JSVAL_TO_OBJECT(v);
    JS_MFPrivate * data = (JS_MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

    assert(data != NULL);

    jsval element;
    uint32_t i;
    uint32_t num;
    JSBool ok = spidermonkey()->JS_GetArrayLength(cx, data->array, &num);

    
    for (i=0; i<num; ++i) {
      ok = spidermonkey()->JS_GetElement(cx, obj, i, &element);
      assert(ok);

      setValue(f, i, cx, element);
    }
    return TRUE;
  }
  return FALSE;
}

static SbBool JS_MFColor_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFColorClass, JS_MFColorSetValue);
}

static SbBool JS_MFFloat_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFFloatClass, JS_MFFloatSetValue);
}

static SbBool JS_MFInt32_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFInt32Class, JS_MFInt32SetValue);
}

static SbBool JS_MFNode_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFNodeClass, JS_MFNodeSetValue);
}

static SbBool JS_MFRotation_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFRotationClass, JS_MFRotationSetValue);
}

static SbBool JS_MFString_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFStringClass, JS_MFStringSetValue);
}

static SbBool JS_MFTime_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFTimeClass, JS_MFTimeSetValue);
}

static SbBool JS_MFVec2f_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFVec2fClass, JS_MFVec2fSetValue);
}

static SbBool JS_MFVec3f_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  return JS_MF_jsval2field(cx, v, f, &JS_MFVec3fClass, JS_MFVec3fSetValue);
}

// *************************************************************************
// field2jsval

static void JS_MF_field2jsval(JSContext * cx, const SoField * f, jsval * v,
                              JSClass * cls, JSFunctionSpec * functions,
                              JSNative constructor, GetValue_t getValue)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, cls, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, functions);

  int num = ((SoMField *)f)->getNum();
  jsval * vals = new jsval[num];

  for (int i=0; i<num; ++i) {
    vals[i] = getValue(f, i, cx);
  }

  jsval rval;
  constructor(cx, obj, num, vals, &rval);
  *v = OBJECT_TO_JSVAL(obj);
  delete vals;
}

static void JS_MFColor_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFColorClass, JS_MFFunctions, JS_MFColorConstructor, JS_MFColorGetValue);
}

static void JS_MFFloat_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFFloatClass, JS_MFFunctions, JS_MFFloatConstructor, JS_MFFloatGetValue);
}

static void JS_MFInt32_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFInt32Class, JS_MFFunctions, JS_MFInt32Constructor, JS_MFInt32GetValue);
}

static void JS_MFNode_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFNodeClass, JS_MFFunctions, JS_MFNodeConstructor, JS_MFNodeGetValue);
}

static void JS_MFRotation_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFRotationClass, JS_MFFunctions, JS_MFRotationConstructor, JS_MFRotationGetValue);
}

static void JS_MFString_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFStringClass, JS_MFFunctions, JS_MFStringConstructor, JS_MFStringGetValue);
}

static void JS_MFTime_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFTimeClass, JS_MFFunctions, JS_MFTimeConstructor, JS_MFTimeGetValue);
}

static void JS_MFVec2f_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFVec2fClass, JS_MFFunctions, JS_MFVec2fConstructor, JS_MFVec2fGetValue);
}

static void JS_MFVec3f_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JS_MF_field2jsval(cx, f, v, &JS_MFVec3fClass, JS_MFFunctions, JS_MFVec3fConstructor, JS_MFVec3fGetValue);
}

// *************************************************************************
// helper function to add all classes to engine

static void JS_addVRMLclasses(JavascriptEngine * engine)
{
  // Bool
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFBool::getClassTypeId(), NULL,
    JS_SFBool_field2jsval, JS_SFBool_jsval2field));

  // Color
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFColor::getClassTypeId(), JS_SFColor_init,
    JS_SFColor_field2jsval, JS_SF_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFColor::getClassTypeId(), JS_MFColor_init,
    JS_MFColor_field2jsval, JS_MFColor_jsval2field));

  // Float
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFFloat::getClassTypeId(), NULL,
    JS_SFFloat_field2jsval, JS_SFFloat_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFFloat::getClassTypeId(), JS_MFFloat_init,
    JS_MFFloat_field2jsval, JS_MFFloat_jsval2field));

  // Int32
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFInt32::getClassTypeId(), NULL,
    JS_SFInt32_field2jsval, JS_SFInt32_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFInt32::getClassTypeId(), JS_MFInt32_init,
    JS_MFInt32_field2jsval, JS_MFInt32_jsval2field));

  // Node
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFNode::getClassTypeId(), JS_SFNode_init,
    JS_SFNode_field2jsval, JS_SFNode_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFNode::getClassTypeId(), JS_MFNode_init,
    JS_MFNode_field2jsval, JS_MFNode_jsval2field));

  // Rotation
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFRotation::getClassTypeId(), JS_SFRotation_init,
    JS_SFRotation_field2jsval, JS_SF_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFRotation::getClassTypeId(), JS_MFRotation_init,
    JS_MFRotation_field2jsval, JS_MFRotation_jsval2field));

  // String
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFString::getClassTypeId(), NULL,
    JS_SFString_field2jsval, JS_SFString_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFString::getClassTypeId(), JS_MFString_init,
    JS_MFString_field2jsval, JS_MFString_jsval2field));

  // Time
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFTime::getClassTypeId(), NULL,
    JS_SFTime_field2jsval, JS_SFTime_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFTime::getClassTypeId(), JS_MFTime_init,
    JS_MFTime_field2jsval, JS_MFTime_jsval2field));

  // Vec2f
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFVec2f::getClassTypeId(), JS_SFVec2f_init,
    JS_SFVec2f_field2jsval, JS_SF_jsval2field));
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFVec2f::getClassTypeId(), JS_MFVec2f_init,
    JS_MFVec2f_field2jsval, JS_MFVec2f_jsval2field));

  // Vec3f
  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoSFVec3f::getClassTypeId(), JS_SFVec3f_init,
    JS_SFVec3f_field2jsval, JS_SF_jsval2field));

  engine->handlerList.append(JavascriptEngine::JavascriptHandler(
    SoMFVec3f::getClassTypeId(), JS_MFVec3f_init,
    JS_MFVec3f_field2jsval, JS_MFVec3f_jsval2field));
}
