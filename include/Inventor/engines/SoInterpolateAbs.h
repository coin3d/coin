/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOINTERPOLATEABS_H
#define COIN_SOINTERPOLATEABS_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFFloat.h>

class SoInterpolate : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_ABSTRACT_HEADER(SoInterpolate);

public:
  SoSFFloat alpha;
  SoEngineOutput output; // SoMFFloat

protected:
  SoInterpolate();
  virtual ~SoInterpolate();

public:
  static void initClass(void);
  static void initClasses(void);
};

#define SO_INTERPOLATE_HEADER(_class_) \
  SO_ENGINE_HEADER(_class_); \
  public: \
    _class_(); \
    static void initClass(); \
  protected: \
    virtual ~_class_(); \
  private: \
    virtual void evaluate()


//
// Could this macro _be_ any uglier ((c) Chandler, Friends)
//
// Considering the number of lines of code needed to implement
// the evaluate() method in each class, I'm amazed it is defined in
// a macro and not simply implemented for each class. But, I guess
// we'll have to supply this macro to keep the OIV compatibility,
// so here it is. Check the interpolator classes for examples on
// how to use it.
//                                               pederb, 20000309
//
#define SO_INTERPOLATE_SOURCE(_class_, _type_, _valtype_, _default0_, _default1_, _interpexp_) \
 \
SO_ENGINE_SOURCE(_class_); \
 \
_class_::_class_(void) \
{ \
  SO_ENGINE_CONSTRUCTOR(_class_); \
  SO_ENGINE_ADD_INPUT(alpha, (0.0f)); \
  SO_ENGINE_ADD_INPUT(input0, _default0_); \
  SO_ENGINE_ADD_INPUT(input1, _default1_); \
  SO_ENGINE_ADD_OUTPUT(output, _type_); \
  this->isBuiltIn = TRUE; \
} \
 \
_class_::~_class_() \
{ \
} \
 \
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

#define SO_INTERPOLATE_INITCLASS(_class_, _classname_) \
 \
void \
_class_::initClass(void) \
{ \
  SO_ENGINE_INIT_CLASS(_class_, SoInterpolate, "SoInterpolate"); \
}

#if defined(COIN_INTERNAL)
#define SO_INTERPOLATE_INTERNAL_INIT_CLASS(_class_) \
 \
void \
_class_::initClass(void) \
{ \
  SO_ENGINE_INTERNAL_INIT_CLASS(_class_); \
}
#endif // COIN_INTERNAL

#endif // !COIN_SOINTERPOLATEABS_H
