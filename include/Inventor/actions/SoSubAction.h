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

#ifndef COIN_SOSUBACTION_H
#define COIN_SOSUBACTION_H

#ifndef COIN_INTERNAL
// Added to be Inventor compliant.
#include <Inventor/SbString.h>
#include <Inventor/actions/SoAction.h>
#endif // !COIN_INTERNAL


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
  static COIN_DLL_IMPORT SoEnabledElementsList * enabledElements; \
  static COIN_DLL_IMPORT SoActionMethodList * methods; \
 \
private: \
  static SoType classTypeId


#define SO_ACTION_SOURCE(_classname_) \
SoEnabledElementsList * _classname_::enabledElements = NULL; \
SoActionMethodList * _classname_::methods = NULL; \
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
    _classname_::enabledElements = new SoEnabledElementsList(_parentclassname_::enabledElements); \
    _classname_::methods = new SoActionMethodList(_parentclassname_::methods); \
  } while (0)



#endif // !COIN_SOSUBACTION_H
