#ifndef COIN_SOSUBACTION_H
#define COIN_SOSUBACTION_H

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

#ifndef COIN_INTERNAL
// Added to be Inventor compliant.
#include <Inventor/SbString.h>
#include <Inventor/actions/SoAction.h>
#else // !COIN_INTERNAL
#include <Inventor/SbName.h>
#endif // COIN_INTERNAL


#define SO_ACTION_ADD_METHOD(_nodeclass_, _method_) \
  do { \
    addMethod(_nodeclass_::getClassTypeId(), (SoActionMethod)_method_); \
  } while (0)


#define SO_ACTION_CONSTRUCTOR(_classname_) \
  do { \
    _classname_::traversalMethods = this->methods; \
  } while (0)


#define SO_ACTION_HEADER(_classname_) \
public: \
  virtual SoType getTypeId(void) const; \
  static SoType getClassTypeId(void); \
  static void addMethod(const SoType type, SoActionMethod method); \
  static void enableElement(const SoType type, const int stackindex); \
 \
protected: \
  virtual const SoEnabledElementsList & getEnabledElements(void) const; \
  /* These two methods are not available in the original OIV API. */ \
  /* They have been added as a work-around for Win32 DLL headaches. */ \
  /* See further explanation below. 20000808 mortene. */ \
  static SoEnabledElementsList * getClassEnabledElements(void); \
  static SoActionMethodList * getClassActionMethods(void); \
 \
private: \
  /* The enabledElements and methods variables are protected in */ \
  /* the original OIV API. This is not such a good idea, since */ \
  /* exposed static class member variables is a major grievance */ \
  /* with regard to Win32 DLLs. */ \
  static SoEnabledElementsList * enabledElements; \
  static SoActionMethodList * methods; \
  static SoType classTypeId


#define SO_ACTION_SOURCE(_classname_) \
SoEnabledElementsList * _classname_::enabledElements = NULL; \
SoActionMethodList * _classname_::methods = NULL; \
SoEnabledElementsList * _classname_::getClassEnabledElements(void) { return _classname_::enabledElements; } \
SoActionMethodList * _classname_::getClassActionMethods(void) { return _classname_::methods; } \
/* Don't set value explicitly to SoType::badType(), to avoid a bug in */ \
/* Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()). */ \
SoType _classname_::classTypeId; \
SoType _classname_::getClassTypeId(void) { return _classname_::classTypeId; } \
SoType _classname_::getTypeId(void) const { return _classname_::classTypeId; } \
const SoEnabledElementsList & _classname_::getEnabledElements(void) const \
{ \
  assert(_classname_::enabledElements); \
  return *_classname_::enabledElements; \
} \
void \
_classname_::addMethod(const SoType type, SoActionMethod method) \
{ \
  assert(_classname_::methods); \
  _classname_::methods->addMethod(type, method); \
} \
void \
_classname_::enableElement(const SoType type, const int stackindex) \
{ \
  assert(_classname_::enabledElements); \
  _classname_::enabledElements->enable(type, stackindex); \
}


#define SO_ACTION_INIT_CLASS(_classname_, _parentclassname_) \
  do { \
    assert(_classname_::getClassTypeId() == SoType::badType()); \
    assert(_parentclassname_::getClassTypeId() != SoType::badType()); \
    _classname_::classTypeId = SoType::createType(_parentclassname_::getClassTypeId(), SO__QUOTE(_classname_)); \
    _classname_::enabledElements = new SoEnabledElementsList(_parentclassname_::getClassEnabledElements()); \
    _classname_::methods = new SoActionMethodList(_parentclassname_::getClassActionMethods()); \
  } while (0)

#endif // !COIN_SOSUBACTION_H
