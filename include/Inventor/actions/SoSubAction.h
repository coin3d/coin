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



#define SO_ACTION_ADD_METHOD(nodeclass, method) \
  do { \
    addMethod(nodeclass::getClassTypeId(), (SoActionMethod)method); \
  } while (0)


#define SO_ACTION_CONSTRUCTOR(classname) \
  do { \
    classname::traversalMethods = this->methods; \
  } while (0)


#define SO_ACTION_HEADER(classname) \
public: \
  virtual SoType getTypeId(void) const; \
  static SoType getClassTypeId(void); \
  static void addMethod(const SoType type, SoActionMethod method); \
  static void enableElement(const SoType type, const int stackindex); \
 \
protected: \
  virtual const SoEnabledElementsList & getEnabledElements(void) const; \
  static SoEnabledElementsList * enabledElements; \
  static SoActionMethodList * methods; \
 \
private: \
  static SoType classTypeId


#define SO_ACTION_SOURCE(classname) \
SoEnabledElementsList * classname::enabledElements = NULL; \
SoActionMethodList * classname::methods = NULL; \
SoType classname::classTypeId = SoType::badType(); \
SoType classname::getClassTypeId(void) { return classname::classTypeId; } \
SoType classname::getTypeId(void) const { return classname::classTypeId; } \
const SoEnabledElementsList & classname::getEnabledElements(void) const \
{ \
  assert(classname::enabledElements); \
  return *classname::enabledElements; \
} \
void \
classname::addMethod(const SoType type, SoActionMethod method) \
{ \
  assert(classname::methods); \
  classname::methods->addMethod(type, method); \
} \
void \
classname::enableElement(const SoType type, const int stackindex) \
{ \
  assert(classname::enabledElements); \
  classname::enabledElements->enable(type, stackindex); \
}


#define SO_ACTION_INIT_CLASS(classname, parentclassname) \
  do { \
    assert(classname::getClassTypeId() == SoType::badType()); \
    assert(parentclassname::getClassTypeId() != SoType::badType()); \
    classname::classTypeId = SoType::createType(parentclassname::getClassTypeId(), SO__QUOTE(classname)); \
    classname::enabledElements = new SoEnabledElementsList(parentclassname::enabledElements); \
    classname::methods = new SoActionMethodList(parentclassname::methods); \
  } while (0)



#endif // !COIN_SOSUBACTION_H
