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

#ifndef COIN_SOSUBENGINEP_H
#define COIN_SOSUBENGINEP_H

// The macro definitions in this file is used internally by Coin
// classes, and mirrors some of the public macros available in
// SoSubEngine.h with a few modifications so they are suited for the
// builtin classes.
//
// The macros in this file are not made visible for use by the
// application programmer.

#ifndef COIN_INTERNAL
#error this is a private header file
#endif // !COIN_INTERNAL

#include <Inventor/engines/SoSubEngine.h>
#include "tidbitsp.h"

// Be aware that any changes to the SO_ENGINE_INTERNAL_CONSTRUCTOR
// macro should be matched by similar changes to the constructor in
// the SO_INTERPOLATE_SOURCE macro (which have to use
// SO_ENGINE_CONSTRUCTOR because it is "public").

#define SO_ENGINE_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_ENGINE_CONSTRUCTOR(_class_); \
    /* Restore value of isBuiltIn flag (which is set to FALSE */ \
    /* in the SO_ENGINE_CONSTRUCTOR() macro). */ \
    this->isBuiltIn = TRUE; \
  } while (0)


#define SO_ENGINE_INTERNAL_INIT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_ENGINE_INIT_CODE(_class_, &classname[2], &_class_::createInstance, inherited); \
  } while (0)


#define SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_ENGINE_INIT_CODE(_class_, &classname[2], NULL, inherited); \
  } while (0)


#define SO_INTERPOLATE_INTERNAL_INIT_CLASS(_class_) \
 \
void \
_class_::initClass(void) \
{ \
  SO_ENGINE_INTERNAL_INIT_CLASS(_class_); \
}


#define SO_INTERPOLATE_INTERNAL_SOURCE(_class_, _type_, _valtype_, _default0_, _default1_, _interpexp_) \
 \
SO_ENGINE_SOURCE(_class_); \
 \
_class_::_class_(void) \
{ \
  PRIVATE_SO_INTERPOLATE_CONSTRUCTOR(_class_, _type_, _valtype_, _default0_, _default1_); \
  this->isBuiltIn = TRUE; \
} \
 \
PRIVATE_SO_INTERPOLATE_DESTRUCTOR(_class_) \
PRIVATE_SO_INTERPOLATE_EVALUATE(_class_, _type_, _valtype_, _interpexp_)


#define SO_INTERNAL_ENGINE_SOURCE_DYNAMIC_IO(_class_) \
PRIVATE_ENGINE_TYPESYSTEM_SOURCE(_class_); \
unsigned int _class_::classinstances = 0; \
SoFieldData * _class_::inputdata = NULL; \
const SoFieldData ** _class_::parentinputdata = NULL; \
SoEngineOutputData * _class_::outputdata = NULL; \
const SoEngineOutputData ** _class_::parentoutputdata = NULL; \
 \
const SoFieldData ** _class_::getInputDataPtr(void) \
{ \
  assert(0 && "function not in use for _class_"); \
  return NULL; \
} \
 \
const SoFieldData * _class_::getFieldData(void) const \
{ \
  return this->dynamicinput; \
} \
 \
const SoEngineOutputData ** _class_::getOutputDataPtr(void) \
{ \
  assert(0 && "function not in use for _class_"); \
  return NULL; \
} \
 \
const SoEngineOutputData * _class_::getOutputData(void) const \
{ \
  return this->dynamicoutput; \
} \
 \
void * _class_::createInstance(void) \
{ \
  return new _class_; \
} \
 \
void \
_class_::atexit_cleanup(void) \
{ \
  delete _class_::inputdata; \
  delete _class_::outputdata; \
  _class_::inputdata = NULL; \
  _class_::outputdata = NULL; \
  _class_::parentinputdata = NULL; \
  _class_::parentoutputdata = NULL; \
  _class_::classTypeId STATIC_SOTYPE_INIT; \
  _class_::classinstances = 0; \
}

#endif // !COIN_SOSUBENGINEP_H
