/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOINTERPOLATE_H
#define COIN_SOINTERPOLATE_H


#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFFloat.h>

class COIN_DLL_API SoInterpolate : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_ABSTRACT_HEADER(SoInterpolate);

public:
  static void initClass(void);
  static void initClasses(void);

  SoSFFloat alpha;
  SoEngineOutput output; // type varies for subclasses

protected:
  SoInterpolate();
  virtual ~SoInterpolate();
};



//// Start macros ////////////////////////////////////////////////////////


#define SO_INTERPOLATE_HEADER(_class_) \
  SO_ENGINE_HEADER(_class_); \
  public: \
    _class_(); \
    static void initClass(); \
  protected: \
    virtual ~_class_(); \
  private: \
    virtual void evaluate()


#define PRIVATE_SO_INTERPOLATE_CONSTRUCTOR(_class_, _type_, _valtype_, _default0_, _default1_) \
  SO_ENGINE_CONSTRUCTOR(_class_); \
  SO_ENGINE_ADD_INPUT(alpha, (0.0f)); \
  SO_ENGINE_ADD_INPUT(input0, _default0_); \
  SO_ENGINE_ADD_INPUT(input1, _default1_); \
  SO_ENGINE_ADD_OUTPUT(output, _type_)

#define PRIVATE_SO_INTERPOLATE_DESTRUCTOR(_class_) \
_class_::~_class_() \
{ \
}

#define PRIVATE_SO_INTERPOLATE_EVALUATE(_class_, _type_, _valtype_, _interpexp_) \
void \
_class_::evaluate(void) \
{ \
  int n0 = this->input0.getNum(); \
  int n1 = this->input1.getNum(); \
  float a = this->alpha.getValue(); \
  for (int i = SbMax(n0, n1) - 1; i >= 0; i--) { \
    _valtype_ v0 = this->input0[SbMin(i, n0-1)]; \
    _valtype_ v1 = this->input1[SbMin(i, n1-1)]; \
    SO_ENGINE_OUTPUT(output, _type_, set1Value(i, _interpexp_)); \
  } \
}


// Considering the number of lines of code needed to implement
// the evaluate() method in each class, I'm amazed it is defined in
// a macro and not simply implemented for each class. But, I guess
// we'll have to supply this macro to keep the OIV compatibility,
// so here it is. Check the interpolator classes for examples on
// how to use it.
//                                               pederb, 20000309
//
// There's another version of this macro for internal use in the
// SoSubEngineP.h file, so match any changes you do here with that
// macro also -- if applicable.
//
//                                               mortene, 20000505

#define SO_INTERPOLATE_SOURCE(_class_, _type_, _valtype_, _default0_, _default1_, _interpexp_) \
 \
SO_ENGINE_SOURCE(_class_); \
 \
_class_::_class_(void) \
{ \
  PRIVATE_SO_INTERPOLATE_CONSTRUCTOR(_class_, _type_, _valtype_, _default0_, _default1_); \
  this->isBuiltIn = FALSE; \
} \
 \
PRIVATE_SO_INTERPOLATE_DESTRUCTOR(_class_) \
PRIVATE_SO_INTERPOLATE_EVALUATE(_class_, _type_, _valtype_, _interpexp_)


#define SO_INTERPOLATE_INITCLASS(_class_, _classname_) \
 \
void \
_class_::initClass(void) \
{ \
  SO_ENGINE_INIT_CLASS(_class_, SoInterpolate, "SoInterpolate"); \
}


//// End macros //////////////////////////////////////////////////////////


#ifndef COIN_INTERNAL
// Include these header files for better Open Inventor compatibility.
#include <Inventor/engines/SoInterpolateFloat.h>
#include <Inventor/engines/SoInterpolateVec2f.h>
#include <Inventor/engines/SoInterpolateVec3f.h>
#include <Inventor/engines/SoInterpolateVec4f.h>
#include <Inventor/engines/SoInterpolateRotation.h>
#endif // !COIN_INTERNAL


#endif // !COIN_SOINTERPOLATE_H
