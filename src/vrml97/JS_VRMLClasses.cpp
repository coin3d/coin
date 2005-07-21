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

#include <Inventor/misc/SoJavaScriptEngine.h>
#include <Inventor/C/glue/spidermonkey.h>

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

// Macros for instance checking
#define JSVAL_IS_SFVEC2F(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &SFVec2fClass, NULL))
#define JSVAL_IS_SFVEC3F(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &SFVec3fClass, NULL))
#define JSVAL_IS_SFCOLOR(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &SFColorClass, NULL))
#define JSVAL_IS_SFROTATION(cx, jsval) (JSVAL_IS_OBJECT(jsval) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(jsval), &SFRotationClass, NULL))

// Factory methods for converting to javascript objects
static JSObject * SFColorFactory(JSContext * cx, const SbColor & self);
static JSObject * SFNodeFactory(JSContext * cx, SoNode * container);
static JSObject * SFRotationFactory(JSContext * cx, const SbRotation & self);
static JSObject * SFVec2fFactory(JSContext * cx, const SbVec2f & self);
static JSObject * SFVec3fFactory(JSContext * cx, const SbVec3f & self);

static JSBool getIndex(JSContext * cx, jsval id, const char * indexes[], int max)
{
  int index;

  if (JSVAL_IS_INT(id)) {
    index = JSVAL_TO_INT(id);
    if (index < 0 || index >= max) {
      spidermonkey()->JS_ReportError(cx, "index must be between 0 and %d", max);
      return -1;
    }
    return index;
  }
  else {
    JSString * jsstr = spidermonkey()->JS_ValueToString(cx, id);
    const char * str = spidermonkey()->JS_GetStringBytes(jsstr);

    for (index=0; index<3; ++index) {
      if (strcmp(str, indexes[index]) == 0) {
        return index;
      }
    }
    return -1;
  }
}

// FIXME: number of aliases must not be lower than max. This may lead to
// unsafe programming. 20050721 erikgors.
template <class Base, int max, const char * aliases[]>
static JSBool SFGet(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{
  int index = getIndex(cx, id, aliases, max);
  if (index == -1) {
    return JSVAL_FALSE;
  }

  Base * data = (Base *)spidermonkey()->JS_GetPrivate(cx, obj);
  assert(data != NULL);
  assert(spidermonkey()->JS_NewDoubleValue(cx, (double)(*data)[index], rval));
  return JSVAL_TRUE;
}

template <class Base, int max, const char * aliases[]>
static JSBool SFSet(JSContext * cx, JSObject * obj, jsval id, jsval * val)
{
  int index = getIndex(cx, id, aliases, max);
  if (index == -1) {
    return JSVAL_FALSE;
  }

  SbColor * data = (SbColor *)spidermonkey()->JS_GetPrivate(cx, obj);
  assert(data != NULL);

  // FIXME: number may be NaN, PositiveInfinity and NegativeInfinity.
  // Should be checked for every time we run JS_ValueToNumber.
  // ie: "asdf" will become NaN. 20050720 erikgors.
  double number;
  spidermonkey()->JS_ValueToNumber(cx, *val, &number);
  (*data)[index] = (float)number;
  return JSVAL_TRUE;
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

template <class Type>
static void SFDestructor(JSContext * cx, JSObject * obj)
{
  Type * data = (Type *)spidermonkey()->JS_GetPrivate(cx, obj);
  assert(data != NULL);
  delete data;
}

struct SFClass {
  JSClass * cls;
  JSFunctionSpec * functions;
};

const char * SFColorAliases[] = {"r", "g", "b"};

JSClass SFColorClass = {
  "SFColor", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  SFGet<SbColor, 3, SFColorAliases>,
  SFSet<SbColor, 3, SFColorAliases>,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  SFDestructor<SbColor>,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};

const char * SFRotationAliases[] = {"x", "y", "z", "angle"};

JSClass SFRotationClass = {
  "SFRotation", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  SFGet<SbVec4f, 4, SFRotationAliases>,
  SFSet<SbVec4f, 4, SFRotationAliases>,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  SFDestructor<SbVec4f>,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};

JSClass SFVec2fClass = {
  "SFVec2f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  SFGet<SbVec4f, 2, SFRotationAliases>,
  SFSet<SbVec4f, 2, SFRotationAliases>,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  SFDestructor<SbVec2f>,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};

JSClass SFVec3fClass = {
  "SFVec3f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  SFGet<SbVec4f, 3, SFRotationAliases>,
  SFSet<SbVec4f, 3, SFRotationAliases>,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  SFDestructor<SbVec3f>,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0 
};

// *************************************************************************
// constructors

template <class Base, int max, float defaultValues[]>
static JSBool SFConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  float vals[max];

  uint32_t i;
  for (i=0; i<max; ++i) {
    vals[i] = defaultValues[i];
    if (i<argc) {
      double val;
      if (spidermonkey()->JS_ValueToNumber(cx, argv[i], &val)) {
        vals[i] = (float)val;
      }
      else {
        spidermonkey()->JS_ReportError(cx, "WARNING: failed converting argument %d "
                                           "to a double", i + 1);
      }
    }
  }

  Base * data = new Base(vals);
  spidermonkey()->JS_SetPrivate(cx, obj, data);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

// FIXME: Will this pollute namespace? 20050721 erikgors.
float defaultValues[] = {0.0, 0.0, 0.0, 0.0};
float defaultRotationValues[] = {0.0, 1.0, 0.0, 0.0};

#define SFColorConstructor SFConstructor<SbColor, 3, defaultValues>
#define SFVec2fConstructor SFConstructor<SbVec2f, 2, defaultValues>
#define SFVec3fConstructor SFConstructor<SbVec3f, 3, defaultValues>

static JSBool SFRotationConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  if (argc == 2) {
    if (JSVAL_IS_SFVEC3F(cx, argv[0])) {
      SbVec3f & vec =
        *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

      SbVec4f * data = new SbVec4f();
      spidermonkey()->JS_SetPrivate(cx, obj, data);
      *rval = OBJECT_TO_JSVAL(obj);
      // new SFRotation(SFVec3f fromVector, SFVec3f toVector)
      if (JSVAL_IS_SFVEC3F(cx, argv[1])) {
        SbVec3f & vec2 = 
          *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[1]));

        SbRotation rot(vec, vec2);
        SbVec3f axis;
        float rad;
        rot.getValue(axis, rad);

        data->setValue(axis[0], axis[1], axis[2], rad);
        return JSVAL_TRUE;
      }
      // new SFRotation(SFVec3f axis, numeric angle)
      else {
        SbVec4f * data = new SbVec4f();
        spidermonkey()->JS_SetPrivate(cx, obj, data);
        *rval = OBJECT_TO_JSVAL(obj);

        double number = 0.0;
        spidermonkey()->JS_ValueToNumber(cx, argv[1], &number);

        data->setValue(vec[0], vec[1], vec[2], (float)number);
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

  return SFConstructor<SbVec4f, 4, defaultRotationValues>(cx, obj, argc, argv, rval);
} 

// *************************************************************************
// functions

static JSBool SFVec2f_add(JSContext * cx, JSObject * obj, uintN argc, 
                           jsval * argv, jsval * rval)
{
  SbVec2f & vec1 = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC2F(cx, argv[0])) {
    SbVec2f & vec2 = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    SbVec2f result = vec1 + vec2;
    *rval = OBJECT_TO_JSVAL(SFVec2fFactory(cx, result));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool SFVec3f_add(JSContext * cx, JSObject * obj, uintN argc, 
                           jsval * argv, jsval * rval)
{
  SbVec3f & vec1 = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    SbVec3f & vec2 = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    SbVec3f result = vec1 + vec2;
    *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, result));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool SFVec2f_divide(JSContext * cx, JSObject * obj, uintN argc, 
                                jsval * argv, jsval * rval)
{
  SbVec2f & vec = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec2f newVec = vec / (float)number;
    *rval = OBJECT_TO_JSVAL(SFVec2fFactory(cx, newVec));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFVec3f_divide(JSContext * cx, JSObject * obj, uintN argc, 
                                jsval * argv, jsval * rval)
{
  SbVec3f & vec = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec3f newVec = vec / (float)number;
    *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, newVec));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFVec2f_dot(JSContext *cx, JSObject *obj, uintN argc, 
                                  jsval *argv, jsval *rval)
{
  SbVec2f & vec1 = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC2F(cx, argv[0])) {
    SbVec2f & vec2 = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    float dot = vec1.dot(vec2);
    JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, dot, rval);
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFVec3f_dot(JSContext *cx, JSObject *obj, uintN argc, 
                                  jsval *argv, jsval *rval)
{
  SbVec3f & vec = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {

    SbVec3f & vec2 = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));

    float dot = vec.dot(vec2);
    JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, dot, rval);
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFVec2_length(JSContext * cx, JSObject * obj, uintN argc, 
                                  jsval * argv, jsval * rval)
{
  SbVec2f * vec = (SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, vec->length(), rval);
  return JSVAL_TRUE;
}

static JSBool SFVec3_length(JSContext * cx, JSObject * obj, uintN argc, 
                                  jsval * argv, jsval * rval)
{
  SbVec3f * vec = (SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, vec->length(), rval);
  return JSVAL_TRUE;
}

static JSBool SFVec2f_multiply(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  
  SbVec2f & vec = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec2f newVec = vec * (float)number;

    *rval = OBJECT_TO_JSVAL(SFVec2fFactory(cx, newVec));

    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFVec3f_multiply(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  SbVec3f & vec = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_NUMBER(argv[0])) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[0], &number);

    SbVec3f newVec = vec * (float)number;
    *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, newVec));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFVec2f_normalize(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  SbVec2f vec = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);
  vec.normalize();
  *rval = OBJECT_TO_JSVAL(SFVec2fFactory(cx, vec));
  return JSVAL_TRUE;
}

static JSBool SFVec3f_normalize(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  SbVec3f vec = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);
  vec.normalize();
  *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, vec));
  return JSVAL_TRUE;
}

static JSBool SFVec3f_negate(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  SbVec3f vec = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);
  vec.negate();
  *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, vec));
  return JSVAL_TRUE;
}

static JSBool SFVec2f_substract(JSContext * cx, JSObject * obj, uintN argc, 
                                 jsval * argv, jsval * rval)
{
  SbVec2f & vec1 = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC2F(cx, argv[0])) {
    SbVec2f & vec2 = *(SbVec2f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    SbVec2f result = vec1 - vec2;
    *rval = OBJECT_TO_JSVAL(SFVec2fFactory(cx, result));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFVec3f_substract(JSContext * cx, JSObject * obj, uintN argc, 
                                   jsval * argv, jsval * rval)
{
  SbVec3f & vec1 = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    SbVec3f & vec2 = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    SbVec3f result = vec1 - vec2;
    *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, result));
    return JSVAL_TRUE;
  }

  return JSVAL_FALSE;
}

static JSBool SFColor_setHSV(JSContext * cx, JSObject * obj, uintN argc, 
                                   jsval * argv, jsval * rval)
{
  if (argc != 3) {
    return JSVAL_FALSE;
  }
  SbColor & color = *(SbColor *)spidermonkey()->JS_GetPrivate(cx, obj);

  float vals[3];
  convertDoubles(cx, 3, vals, argc, argv);

  color.setHSVValue(vals);
  
  *rval = JSVAL_VOID;
  return JSVAL_TRUE;
}

static JSBool SFColor_getHSV(JSContext * cx, JSObject * obj, uintN argc, 
                                jsval * argv, jsval * rval)
{
  SbColor & color = *(SbColor *)spidermonkey()->JS_GetPrivate(cx, obj);

  float vals[3];
  color.getHSVValue(vals);

  jsval vector[3];
  for (int i=0; i<3; ++i) {
    spidermonkey()->JS_NewDoubleValue(cx, vals[i], &vector[i]);
  }

  *rval = OBJECT_TO_JSVAL(spidermonkey()->JS_NewArrayObject(cx, 3, vector));

  return JSVAL_TRUE;
}

static JSBool SFRotation_getAxis(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  SbVec4f & rot = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec3f axis(rot[0], rot[1], rot[2]);
  *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, axis));
  return JSVAL_TRUE;
}

static JSBool SFRotation_inverse(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  SbVec4f & rot = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, obj);

  SbVec3f axis(rot[0], rot[1], rot[2]);
  SbRotation newRot(axis, rot[3]);
  newRot.invert();

  *rval = OBJECT_TO_JSVAL(SFRotationFactory(cx, newRot));
  return JSVAL_TRUE;
}

static JSBool SFRotation_multiply(JSContext * cx, JSObject * obj, uintN argc, 
                                     jsval * argv, jsval * rval)
{
  SbVec4f & vec1 = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFROTATION(cx, argv[0])) {
    SbVec4f & vec2 = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    SbVec3f axis1(vec1[0], vec1[1], vec1[2]);
    SbVec3f axis2(vec2[0], vec2[1], vec2[2]);
    SbRotation result = SbRotation(axis1, vec1[3]) * SbRotation(axis2, vec2[3]);

    *rval = OBJECT_TO_JSVAL(SFRotationFactory(cx, result));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFRotation_multVec(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  SbVec4f & vec = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec3f axis(vec[0], vec[1], vec[2]);
  SbRotation rot(axis, vec[3]);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    SbVec3f & src = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    SbVec3f dst;
    rot.multVec(src, dst);

    *rval = OBJECT_TO_JSVAL(SFVec3fFactory(cx, dst));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFRotation_setAxis(JSContext * cx, JSObject * obj, uintN argc, 
                                    jsval * argv, jsval * rval)
{
  SbVec4f & rot = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (argc >= 1 && JSVAL_IS_SFVEC3F(cx, argv[0])) {
    SbVec3f & axis = *(SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    rot[0] = axis[0];
    rot[1] = axis[1];
    rot[2] = axis[2];

    *rval = JSVAL_VOID;
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSBool SFRotation_slerp(JSContext * cx, JSObject * obj, uintN argc, 
                                  jsval * argv, jsval * rval)
{
  SbVec4f & vec = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, obj);
  SbVec3f axis(vec[0], vec[1], vec[2]);
  SbRotation rot(axis, vec[3]);

  if (argc >= 2 && JSVAL_IS_SFROTATION(cx, argv[0]) && JSVAL_IS_NUMBER(argv[1])) {
    SbVec4f & vec2 = *(SbVec4f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0]));
    SbVec3f axis2(vec2[0], vec2[1], vec2[2]);
    SbRotation dest(axis2, vec2[3]);

    double number;
    spidermonkey()->JS_ValueToNumber(cx, argv[1], &number);

    SbRotation result = SbRotation::slerp(rot, dest, (float)number);

    *rval = OBJECT_TO_JSVAL(SFRotationFactory(cx, result));
    return JSVAL_TRUE;
  }
  return JSVAL_FALSE;
}

static JSFunctionSpec SFVec2fFunctions[] = {
  {"add", SFVec2f_add, 1, 0, 0},
  {"divide", SFVec2f_divide, 1, 0, 0},
  {"dot", SFVec2f_dot, 1, 0, 0},
  {"length", SFVec2_length, 0, 0, 0},
  {"multiply", SFVec2f_multiply, 1, 0, 0},
  {"normalize", SFVec2f_normalize, 0, 0, 0},
  {"substract", SFVec2f_substract, 1, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSFunctionSpec SFVec3fFunctions[] = {
  {"add", SFVec3f_add, 1, 0, 0},
  {"divide", SFVec3f_divide, 1, 0, 0},
  {"dot", SFVec3f_dot, 1, 0, 0},
  {"length", SFVec3_length, 0, 0, 0},
  {"multiply", SFVec3f_multiply, 1, 0, 0},
  {"normalize", SFVec3f_normalize, 0, 0, 0},
  {"negate", SFVec3f_negate, 0, 0, 0},
  {"substract", SFVec3f_substract, 1, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSFunctionSpec SFColorFunctions[] = {
  {"setHSV", SFColor_setHSV, 3, 0, 0},
  {"getHSV", SFColor_getHSV, 0, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSFunctionSpec SFRotationFunctions[] = {
  {"getAxis", SFRotation_getAxis, 0, 0, 0},
  {"inverse", SFRotation_inverse, 0, 0, 0},
  {"multiply", SFRotation_multiply, 1, 0, 0},
  {"multVec", SFRotation_multVec, 1, 0, 0},
  {"setAxis", SFRotation_setAxis, 1, 0, 0},
  {"slerp", SFRotation_slerp, 2, 0, 0},
  {NULL, NULL, 0, 0, 0}
};

static JSObject * SFColorFactory(JSContext * cx, const SbColor & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &SFColorClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, SFColorFunctions);

  SbColor * color = new SbColor(self);
  spidermonkey()->JS_SetPrivate(cx, obj, color);

  return obj;
}

static JSObject * SFRotationFactory(JSContext * cx, const SbRotation & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &SFRotationClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, SFRotationFunctions);

  SbVec3f axis;
  float angle;
  self.getValue(axis, angle);
  SbVec4f * data = new SbVec4f(axis[0], axis[1], axis[2], angle);
  spidermonkey()->JS_SetPrivate(cx, obj, data);

  return obj;
}

static JSObject * SFVec2fFactory(JSContext * cx, const SbVec2f & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &SFVec2fClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, SFVec2fFunctions);

  SbVec2f * data = new SbVec2f(self);
  spidermonkey()->JS_SetPrivate(cx, obj, data);
  return obj;
}

static JSObject * SFVec3fFactory(JSContext * cx, const SbVec3f & self)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &SFVec3fClass, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, SFVec3fFunctions);

  SbVec3f * data = new SbVec3f(self);
  spidermonkey()->JS_SetPrivate(cx, obj, data);

  return obj;
}

static JSObject * SFVec2f_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &SFVec2fClass, 
                               SFVec2fConstructor, 0,
                               NULL, SFVec2fFunctions, NULL, NULL);
}

static JSObject * SFVec3f_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &SFVec3fClass, 
                               SFVec3fConstructor, 0,
                               NULL, SFVec3fFunctions, NULL, NULL);
}

float fisk[] = {0.0, 0.0, 0.0};
static JSObject * SFColor_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &SFColorClass, 
                               SFColorConstructor, 0,
                               NULL, SFColorFunctions, NULL, NULL);
}

static JSObject * SFRotation_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &SFRotationClass, 
                               SFRotationConstructor, 0,
                               NULL, SFRotationFunctions, NULL, NULL);
}

// *************************************************************************
// SFNode

static JSBool SFNode_get(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
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
    SoDebugError::postWarning("SFNode_get", "node is 'undefined'");
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
      SoJavaScriptEngine::field2jsval(cx, out, rval);
      return JSVAL_TRUE;
    }
    else {
      SoDebugError::postWarning("SFNode_get", "field %s does not exists", str.getString());
    }
  }
  
  return JSVAL_FALSE;
}

static JSBool SFNode_set(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{ 
  SoNode * container = (SoNode *)spidermonkey()->JS_GetPrivate(cx, obj);

  if (container == NULL) {
    SoDebugError::postWarning("SFNode_set", "node is 'undefined'");
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
      SoJavaScriptEngine::jsval2field(cx, *rval, in);
      if (SoJavaScriptEngine::debug()) {
        SoDebugError::postInfo("SFNode_set", "setting field %s", str.getString());
      }
      return JSVAL_TRUE;
    }
    else {
      SoDebugError::postWarning("SFNode_set", "field %s does not exists", str.getString());
      const SoFieldData * data = container->getFieldData();
      int numFields = data->getNumFields();
      for (int i=0; i<numFields; ++i) {
        const SbName & name = data->getFieldName(i);
        const SoField * f = data->getField(container, i);
        SoDebugError::postInfo("SFNode_set",
                               "possible field: %s", name.getString());
      }

    }
  }
  
  return JSVAL_FALSE;
}

static void SFNodeDestructor(JSContext * cx, JSObject * obj)
{
  SoNode * container = (SoNode *)spidermonkey()->JS_GetPrivate(cx, obj);
  assert(container != NULL);
  container->unref();
}

JSClass SFNodeClass = {
  "SFNode", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  SFNode_get,
  SFNode_set,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  SFNodeDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

static JSObject * SFNodeFactory(JSContext * cx, SoNode * container)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, &SFNodeClass, NULL, NULL);
  spidermonkey()->JS_SetPrivate(cx, obj, container);
  container->ref();
  return obj;
}

static JSBool SFNodeConstructor(JSContext * cx, JSObject * obj, 
                                    uintN argc, jsval * argv, jsval *rval)
{
  // spidermonkey ignores the return value

  if (argc >= 1 && JSVAL_IS_STRING(argv[0])) {
    JSString * js = JSVAL_TO_STRING(argv[0]);
    char * str = spidermonkey()->JS_GetStringBytes(js);
    size_t len = spidermonkey()->JS_GetStringLength(js);
    
    // FIXME: what about UTF8? 20050701 erikgors.

    if (SoJavaScriptEngine::debug()) {
      SoDebugError::postInfo("SFNodeConstructor",
                             "creating new node with str = '%s'", str);
    }
                             

    SoInput input = SoInput();
    const char * array[2];
    array[0] = str;
    array[1] = NULL;
    input.setStringArray(array);

    SoVRMLGroup * group = SoDB::readAllVRML(&input);

    if (group == NULL) {
      SoDebugError::postWarning("SFNodeConstructor", "input is not legal VRML string");
      return JSVAL_FALSE;
    }
    if (group->getNumChildren() > 1) {
      SoDebugError::postWarning("SFNodeConstructor", "more than one top-level node, "
                                                         "result is undefined");
      return JSVAL_FALSE;
    }
    if (group->getNumChildren() == 0) {
      SoDebugError::postWarning("SFNodeConstructor", "no top-level node, "
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

static JSObject * SFNode_init(JSContext * cx, JSObject * obj)
{
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &SFNodeClass,
                                      SFNodeConstructor, 0,
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

static SbBool SFColor_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_SFCOLOR(cx, v)) {
    SbColor * color = (SbColor *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(v));
    assert(color != NULL);
    ((SoSFColor *)f)->setValue(*color);
    return TRUE;
  }
  return FALSE;
}

static SbBool SFRotation_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_SFROTATION(cx, v)) {
    SbVec4f * rot = (SbVec4f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(v));
    assert(rot != NULL);
    SbVec3f axis((*rot)[0], (*rot)[1], (*rot)[2]);
    ((SoSFRotation *)f)->setValue(SbRotation(axis, (*rot)[3]));
    return TRUE;
  }
  return FALSE;
}

static SbBool SFVec2f_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_SFVEC2F(cx, v)) {
    SbVec2f * vec = (SbVec2f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(v));
    assert(vec != NULL);
    ((SoSFVec2f *)f)->setValue(*vec);
    return TRUE;
  }
  return FALSE;
}

static SbBool SFVec3f_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_SFVEC3F(cx, v)) {
    SbVec3f * vec = (SbVec3f *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(v));
    assert(vec != NULL);
    ((SoSFVec3f *)f)->setValue(*vec);
    return TRUE;
  }
  return FALSE;
}

static SbBool SFBool_jsval2field(JSContext * cx, const jsval v, SoField * f)
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
      SoDebugError::postWarning("SFBool_jsval2field",
                                "bool convertion of jsval failed");
      return FALSE;
    }
  }
  return TRUE;
}

static SbBool SFNode_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_OBJECT(v) && spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(v), &SFNodeClass, NULL)) {
    SoNode * node = (SoNode *)spidermonkey()->JS_GetPrivate(cx, JSVAL_TO_OBJECT(v));
    ((SoSFNode *)f)->setValue(node);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("SFNode::jsval2field",
                              "coulnt not convert jsval to SoNode");
    return FALSE;
  }
}

// basic types
//
static SbBool SFInt32_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_INT(v)) {
    const int32_t val = JSVAL_TO_INT(v);
    ((SoSFInt32 *)f)->setValue(val);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("SFInt32_jsval2field",
                              "int convertion of jsval failed");
    return FALSE;
  }
}

static SbBool SFFloat_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_NUMBER(v)) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, v, &number);
    ((SoSFFloat *)f)->setValue((float)number);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("SFFloat_jsval2field",
                              "float convertion of jsval failed");
    return FALSE;
  }
}

static SbBool SFTime_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_NUMBER(v)) {
    double number;
    spidermonkey()->JS_ValueToNumber(cx, v, &number);
    ((SoSFTime*)f)->setValue(SbTime(number));
    return TRUE;
  }
  else {
    SoDebugError::postWarning("SFFloat_jsval2field",
                              "float convertion of jsval failed");
    return FALSE;
  }
}

static SbBool SFString_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_STRING(v)) {
    const char * str = spidermonkey()->JS_GetStringBytes(JSVAL_TO_STRING(v));
    ((SoSFString *)f)->setValue(str);
    return TRUE;
  }
  else {
    SoDebugError::postWarning("SFString_jsval2field",
                              "str convertion of jsval failed");
    return FALSE;
  }
}

// *************************************************************************
// field2jsval

static void SFColor_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbColor & val = ((SoSFColor *)f)->getValue();
  JSObject * obj = SFColorFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

// FIXME: SFImage missing

static void SFNode_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  SoNode * node = ((SoSFNode *)f)->getValue();
  JSObject * obj = SFNodeFactory(cx, node);
  *v = OBJECT_TO_JSVAL(obj);
}

static void SFRotation_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbRotation & val = ((SoSFRotation *)f)->getValue();
  JSObject * obj = SFRotationFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

static void SFVec2f_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbVec2f & val = ((SoSFVec2f *)f)->getValue();
  JSObject * obj = SFVec2fFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

static void SFVec3f_field2jsval(JSContext * cx, const SoField * f, jsval *v)
{
  const SbVec3f & val = ((SoSFVec3f *)f)->getValue();
  JSObject * obj = SFVec3fFactory(cx, val); 
  *v = OBJECT_TO_JSVAL(obj);
}

// base types

static void SFBool_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const SbBool val = ((SoSFBool *)f)->getValue();
  *v = BOOLEAN_TO_JSVAL(val);
}

static void SFInt32_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const int32_t val = ((SoSFInt32 *)f)->getValue();
  *v = INT_TO_JSVAL(val);
}

static void SFFloat_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const float val = ((SoSFFloat *)f)->getValue();
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, val, v);
}

static void SFTime_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const SbTime & time = ((SoSFTime *)f)->getValue();
  JSBool ok = spidermonkey()->JS_NewDoubleValue(cx, time.getValue(), v);
}

static void SFString_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  const SbString & val = ((SoSFString *)f)->getValue();
  JSString * str = spidermonkey()->JS_NewStringCopyZ(cx, val.getString());
  *v = STRING_TO_JSVAL(str);
}


// *************************************************************************
// MF classes

typedef SbBool (* JSCheckType_t)(JSContext *cx, JSObject * obj, jsval val);

struct MFPrivate {
  SoType type;
  JSObject * array;
  JSClass * cls;
  JSNative constructor;
  JSCheckType_t checkType;
};

static JSBool MFGet(JSContext * cx, JSObject * obj, jsval id, jsval * rval)
{
  MFPrivate * data = (MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

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

static void MF_resize(JSContext * cx, MFPrivate * data, int newLength) {
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
        assert(newObj != NULL);
        ok = data->constructor(cx, newObj, 0, NULL, &val);
        assert(ok);
        assert(newObj == JSVAL_TO_OBJECT(val));
      }

      ok = spidermonkey()->JS_SetElement(cx, data->array, index, &val);
      assert(ok);
    }
  }
}

static JSBool MFSet(JSContext * cx, JSObject * obj, jsval id, jsval * val)
{
  MFPrivate * data = (MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

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
      MF_resize(cx, data, index + 1);
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
      MF_resize(cx, data, JSVAL_TO_INT(*val));
      return JSVAL_TRUE;
    }
  }

  return JSVAL_FALSE;
}

static SbBool MFCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  if (JSVAL_IS_OBJECT(val)) {
    MFPrivate * data = (MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);
    return spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(val), data->cls, NULL);
  }
  return FALSE;
}

static void MFConstructor(JSContext * cx, JSObject * obj,
                              const SoType & type, JSClass * cls,
                              JSNative constructor, JSCheckType_t checkType,
                              uintN argc, jsval * argv)
{
  MFPrivate * data = new MFPrivate;
  data->type = type;
  data->cls = cls;
  data->array = spidermonkey()->JS_NewArrayObject(cx, 0, NULL);
  data->constructor = constructor;
  data->checkType = checkType;

  // add gc protection
  JSBool ok = spidermonkey()->JS_AddRoot(cx, &data->array);
  assert(ok);

  spidermonkey()->JS_SetPrivate(cx, obj, data);

  MF_resize(cx, data, argc);
  for (int i=0; i<(int)argc; ++i) {
    if (data->checkType(cx, obj, argv[i])) {
      ok = spidermonkey()->JS_SetElement(cx, data->array, i, &argv[i]);
      assert(ok);
    }
  }
}

static void MFDestructor(JSContext * cx, JSObject * obj)
{
  MFPrivate * data = (MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

  // remove gc protection
  spidermonkey()->JS_RemoveRoot(cx, &data->array);
  delete data;
}

static JSFunctionSpec MFFunctions[] = {
//  {"toString", MF_toString, 0, 0, 0},
  {NULL, NULL, 0, 0, 0}
};    

JSClass MFColorClass = {
  "MFColor", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass MFRotationClass = {
  "MFRotation", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass MFVec2fClass = {
  "MFVec2f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass MFVec3fClass = {
  "MFVec3f", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass MFFloatClass = {
  "MFFloat", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass MFInt32Class = {
  "MFInt32", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass MFStringClass = {
  "MFString", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

JSClass MFTimeClass = {
  "MFTime", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};


JSClass MFNodeClass = {
  "MFNode", JSCLASS_HAS_PRIVATE,
  spidermonkey()->JS_PropertyStub,
  spidermonkey()->JS_PropertyStub,
  MFGet,
  MFSet,
  spidermonkey()->JS_EnumerateStub,
  spidermonkey()->JS_ResolveStub,
  spidermonkey()->JS_ConvertStub,
  MFDestructor,
  NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, 0
};

static JSBool MFColorConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFBool::getClassTypeId(),
                    &SFColorClass, SFConstructor<SbColor, 3, fisk>,
                    MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
} 

static JSObject * MFColor_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFColorClass,
                                      MFColorConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static JSBool MFRotationConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFRotation::getClassTypeId(),
                    &SFRotationClass, SFRotationConstructor,
                    MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFRotation_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFRotationClass,
                                      MFRotationConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static JSBool MFVec2fConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFVec2f::getClassTypeId(),
                    &SFVec2fClass, SFVec2fConstructor,
                    MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFVec2f_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFVec2fClass,
                                      MFVec2fConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static JSBool MFVec3fConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFVec3f::getClassTypeId(),
                    &SFVec3fClass, SFVec3fConstructor,
                    MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFVec3f_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFVec3fClass,
                                      MFVec3fConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static SbBool JS_numberCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  return JSVAL_IS_NUMBER(val);
}

static JSBool MFFloatConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFFloat::getClassTypeId(),
                    NULL, NULL,
                    JS_numberCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFFloat_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFFloatClass,
                                      MFFloatConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static SbBool JS_intCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  return JSVAL_IS_INT(val);
}

static JSBool MFInt32Constructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFInt32::getClassTypeId(),
                    NULL, NULL,
                    JS_intCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFInt32_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFInt32Class,
                                      MFInt32Constructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static SbBool JS_stringCheckType(JSContext * cx, JSObject * obj, jsval val)
{
  return JSVAL_IS_STRING(val);
}

static JSBool MFStringConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFString::getClassTypeId(),
                    NULL, NULL,
                    JS_stringCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFString_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFStringClass,
                                      MFStringConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static JSBool MFTimeConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFTime::getClassTypeId(),
                    NULL, NULL,
                    JS_numberCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFTime_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFTimeClass,
                                      MFTimeConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

static JSBool MFNodeConstructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
  MFConstructor(cx, obj, SoSFNode::getClassTypeId(),
                    &SFNodeClass, NULL,
                    MFCheckType, argc, argv);
  *rval = OBJECT_TO_JSVAL(obj);
  return JSVAL_TRUE;
}

static JSObject * MFNode_init(JSContext * cx, JSObject * obj)
{   
  return spidermonkey()->JS_InitClass(cx, obj, NULL, &MFNodeClass,
                                      MFNodeConstructor, 0,
                                      NULL, MFFunctions, NULL, NULL);
}

// *************************************************************************
// jsval2field

template <class MFFieldClass, JSClass * cls, class SFFieldClass>
static SbBool MF_jsval2field(JSContext * cx, const jsval v, SoField * f)
{
  if (JSVAL_IS_OBJECT(v) && 
      spidermonkey()->JS_InstanceOf(cx, JSVAL_TO_OBJECT(v), cls, NULL)) {
    JSObject * obj = JSVAL_TO_OBJECT(v);
    MFPrivate * priv = (MFPrivate *)spidermonkey()->JS_GetPrivate(cx, obj);

    assert(priv != NULL);

    jsval element;
    uint32_t i;
    uint32_t num;
    JSBool ok = spidermonkey()->JS_GetArrayLength(cx, priv->array, &num);

    SFFieldClass * data = (SFFieldClass *)SFFieldClass::createInstance();
    
    for (i=0; i<num; ++i) {
      ok = spidermonkey()->JS_GetElement(cx, obj, i, &element);
      assert(ok);

      assert(SoJavaScriptEngine::jsval2field(cx, element, data));
      ((MFFieldClass *)f)->set1Value(i, data->getValue());
    }
    delete data;
    return TRUE;
  }
  return FALSE;
}

// *************************************************************************
// field2jsval

struct MFNamespace {
  struct ClassDescriptor {
    JSClass * cls;
    JSFunctionSpec * functions;
    JSNative constructor;
  };

  static ClassDescriptor Color;
  static ClassDescriptor Float;
  static ClassDescriptor Int32;
  static ClassDescriptor Node;
  static ClassDescriptor Rotation;
  static ClassDescriptor String;
  static ClassDescriptor Time;
  static ClassDescriptor Vec2f;
  static ClassDescriptor Vec3f;
};

MFNamespace::ClassDescriptor MFNamespace::Color = {
  &MFColorClass,
  MFFunctions,
  MFColorConstructor
};

MFNamespace::ClassDescriptor MFNamespace::Float= {
  &MFFloatClass,
  MFFunctions,
  MFFloatConstructor
};

MFNamespace::ClassDescriptor MFNamespace::Int32 = {
  &MFInt32Class,
  MFFunctions,
  MFInt32Constructor
};

MFNamespace::ClassDescriptor MFNamespace::Node = {
  &MFNodeClass,
  MFFunctions,
  MFNodeConstructor
};

MFNamespace::ClassDescriptor MFNamespace::Rotation = {
  &MFRotationClass,
  MFFunctions,
  MFRotationConstructor
};

MFNamespace::ClassDescriptor MFNamespace::String = {
  &MFStringClass,
  MFFunctions,
  MFStringConstructor
};

MFNamespace::ClassDescriptor MFNamespace::Time = {
  &MFTimeClass,
  MFFunctions,
  MFTimeConstructor
};

MFNamespace::ClassDescriptor MFNamespace::Vec2f = {
  &MFVec2fClass,
  MFFunctions,
  MFVec2fConstructor
};

MFNamespace::ClassDescriptor MFNamespace::Vec3f = {
  &MFVec3fClass,
  MFFunctions,
  MFVec3fConstructor
};

template <class MFFieldClass, class SFFieldClass, MFNamespace::ClassDescriptor * desc>
static void MF_field2jsval(JSContext * cx, const SoField * f, jsval * v)
{
  JSObject * obj = spidermonkey()->JS_NewObject(cx, desc->cls, NULL, NULL);
  spidermonkey()->JS_DefineFunctions(cx, obj, desc->functions);

  int num = ((SoMField *)f)->getNum();
  jsval * vals = new jsval[num];

  MFFieldClass & mf = *(MFFieldClass *)f;

  SFFieldClass * data = (SFFieldClass *)SFFieldClass::createInstance();
  for (int i=0; i<num; ++i) {
    data->setValue(mf[i]);
    assert(SoJavaScriptEngine::field2jsval(cx, data, &vals[i]));
  }

  jsval rval;
  desc->constructor(cx, obj, num, vals, &rval);
  *v = OBJECT_TO_JSVAL(obj);
  delete data;
  delete vals;
}

// *************************************************************************
// helper function to add all classes to engine

static void JS_addVRMLclasses(SoJavaScriptEngine * engine)
{
  // Bool
  engine->addHandler(
    SoSFBool::getClassTypeId(), NULL,
    SFBool_field2jsval, SFBool_jsval2field);

  // Color
  engine->addHandler(
    SoSFColor::getClassTypeId(), SFColor_init,
    SFColor_field2jsval, SFColor_jsval2field);
  engine->addHandler(
    SoMFColor::getClassTypeId(), MFColor_init,
    MF_field2jsval<SoMFColor, SoSFColor, &MFNamespace::Color>,
    MF_jsval2field<SoMFColor, &MFColorClass, SoSFColor>);

  // Float
  engine->addHandler(
    SoSFFloat::getClassTypeId(), NULL,
    SFFloat_field2jsval, SFFloat_jsval2field);
  engine->addHandler(
    SoMFFloat::getClassTypeId(), MFFloat_init,
    MF_field2jsval<SoMFFloat, SoSFFloat, &MFNamespace::Float>,
    MF_jsval2field<SoMFFloat, &MFFloatClass, SoSFFloat>);

  // Int32
  engine->addHandler(
    SoSFInt32::getClassTypeId(), NULL,
    SFInt32_field2jsval, SFInt32_jsval2field);
  engine->addHandler(
    SoMFInt32::getClassTypeId(), MFInt32_init,
    MF_field2jsval<SoMFInt32, SoSFInt32, &MFNamespace::Int32>,
    MF_jsval2field<SoMFInt32, &MFInt32Class, SoSFInt32>);

  // Node
  engine->addHandler(
    SoSFNode::getClassTypeId(), SFNode_init,
    SFNode_field2jsval, SFNode_jsval2field);
  engine->addHandler(
    SoMFNode::getClassTypeId(), MFNode_init,
    MF_field2jsval<SoMFNode, SoSFNode, &MFNamespace::Node>,
    MF_jsval2field<SoMFNode, &MFNodeClass, SoSFNode>);

  // Rotation
  engine->addHandler(
    SoSFRotation::getClassTypeId(), SFRotation_init,
    SFRotation_field2jsval, SFRotation_jsval2field);
  engine->addHandler(
    SoMFRotation::getClassTypeId(), MFRotation_init,
    MF_field2jsval<SoMFRotation, SoSFRotation, &MFNamespace::Rotation>,
    MF_jsval2field<SoMFRotation, &MFRotationClass, SoSFRotation>);

  // String
  engine->addHandler(
    SoSFString::getClassTypeId(), NULL,
    SFString_field2jsval, SFString_jsval2field);
  engine->addHandler(
    SoMFString::getClassTypeId(), MFString_init,
    MF_field2jsval<SoMFString, SoSFString, &MFNamespace::String>,
    MF_jsval2field<SoMFString, &MFStringClass, SoSFString>);

  // Time
  engine->addHandler(
    SoSFTime::getClassTypeId(), NULL,
    SFTime_field2jsval, SFTime_jsval2field);
  engine->addHandler(
    SoMFTime::getClassTypeId(), MFTime_init,
    MF_field2jsval<SoMFTime, SoSFTime, &MFNamespace::Time>,
    MF_jsval2field<SoMFTime, &MFTimeClass, SoSFTime>);

  // Vec2f
  engine->addHandler(
    SoSFVec2f::getClassTypeId(), SFVec2f_init,
    SFVec2f_field2jsval, SFVec2f_jsval2field);
  engine->addHandler(
    SoMFVec2f::getClassTypeId(), MFVec2f_init,
    MF_field2jsval<SoMFVec2f, SoSFVec2f, &MFNamespace::Vec2f>,
    MF_jsval2field<SoMFVec2f, &MFVec2fClass, SoSFVec2f>);

  // Vec3f
  engine->addHandler(
    SoSFVec3f::getClassTypeId(), SFVec3f_init,
    SFVec3f_field2jsval, SFVec3f_jsval2field);
  engine->addHandler(
    SoMFVec3f::getClassTypeId(), MFVec3f_init,
    MF_field2jsval<SoMFVec3f, SoSFVec3f, &MFNamespace::Vec3f>,
    MF_jsval2field<SoMFVec3f, &MFVec3fClass, SoSFVec3f>);
}
