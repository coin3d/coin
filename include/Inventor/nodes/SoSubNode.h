/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOSUBNODE_H__
#define __SOSUBNODE_H__

#include <Inventor/misc/SoBasic.h> // for SO__QUOTE() definition
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>


#define SO_NODE_ABSTRACT_HEADER(_class_) \
private: \
  static unsigned int classinstances; \
  static SoType classTypeId; \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const


#define SO_NODE_HEADER(_class_) \
  SO_NODE_ABSTRACT_HEADER(_class_); \
  static void * createInstance(void)


#define SO_NODE_ABSTRACT_SOURCE(_class_) \
unsigned int _class_::classinstances = 0; \
SoType _class_::classTypeId = SoType::badType(); \
 \
SoType \
_class_::getClassTypeId(void) \
{ \
  return _class_::classTypeId; \
} \
 \
SoType \
_class_::getTypeId(void) const \
{ \
  return _class_::classTypeId; \
} \



#define SO_NODE_SOURCE(_class_) \
SO_NODE_ABSTRACT_SOURCE(_class_) \
 \
void * \
_class_::createInstance(void) \
{ \
  return new _class_; \
}


#define SO_NODE_IS_FIRST_INSTANCE() \
  (classinstances == 1)


#define SO_NODE_CONSTRUCTOR(_class_) \
  do { \
    _class_::classinstances++; \
    /* Catch attempts to use a node class which has not been initialized. */ \
    assert(_class_::classTypeId != SoType::badType()); \
    /* Extensions classes from the application programmers should not be \
       considered native. This is important to get the export code to do \
       the Right Thing. */ \
    this->isBuiltIn = FALSE; \
  } while (0)


#if defined(__SOLIB_INTERNAL__)
#define SO_NODE_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    _class_::classinstances++; \
    /* Catch attempts to use a node class which has not been initialized. */ \
    assert(_class_::classTypeId != SoType::badType()); \
  } while (0)
#endif // INTERNAL macro definition


#define SO_NODE_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* It's safe to use a temp variable here without fear of namespace */ \
    /* collisions, due to the fact that we're in the do-while local scope. */ \
    SoType parenttype = SoType::fromName(_parentname_); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(parenttype != SoType::badType()); \
 \
    _class_::classTypeId = \
      SoType::createType(parenttype, SO__QUOTE(_class_), \
                         &_class_::createInstance, \
                         SoNode::nextActionMethodIndex++); \
  } while (0)


#if defined(__SOLIB_INTERNAL__)
#define SO_NODE_INTERNAL_INIT_CLASS(_class_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(inherited::getClassTypeId() != SoType::badType()); \
 \
    const char * classname = SO__QUOTE(_class_); \
    _class_::classTypeId = \
      SoType::createType(inherited::getClassTypeId(), \
                         &classname[2], \
                         &_class_::createInstance, \
                         SoNode::nextActionMethodIndex++); \
  } while (0)
#endif // INTERNAL macro definition



#define SO_NODE_INIT_ABSTRACT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* It's safe to use a temp variable here without fear of namespace */ \
    /* collisions, due to the fact that we're in the do-while local scope. */ \
    SoType parenttype = SoType::fromName(_parentname_); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(parenttype != SoType::badType()); \
 \
    _class_::classTypeId = \
      SoType::createType(parenttype, SO__QUOTE(_class_), \
                         NULL, \
                         SoNode::nextActionMethodIndex++); \
  } while (0)


#if defined(__SOLIB_INTERNAL__)
#define SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(_class_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(inherited::getClassTypeId() != SoType::badType()); \
 \
    const char * classname = SO__QUOTE(_class_); \
    _class_::classTypeId = \
      SoType::createType(inherited::getClassTypeId(), \
                         &classname[2], \
                         NULL, \
                         SoNode::nextActionMethodIndex++); \
  } while (0)
#endif // INTERNAL macro definition


#define SO_NODE_ADD_FIELD(_fieldname_, _defaultval_) \
do { \
  this->fieldData.addField(this, SO__QUOTE(_fieldname_), &this->_fieldname_);\
  this->_fieldname_.setValue _defaultval_;\
  this->_fieldname_.setContainer(this); \
  this->_fieldname_.setDefault(TRUE); \
} while (0)


#define SO_NODE_DEFINE_ENUM_VALUE(_enumname_, _enumval_) \
  this->fieldData.addEnumValue(SO__QUOTE(_enumname_), SO__QUOTE(_enumval_), \
                               _enumval_)

#endif // !__SOSUBNODE_H__
