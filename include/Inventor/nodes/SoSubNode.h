#ifndef COIN_SOSUBNODE_H
#define COIN_SOSUBNODE_H

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

#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/nodes/SoNode.h>

/*
  The most used macros and their arguments:

  header files:
    SO_NODE_HEADER(classname)
    SO_NODE_ABSTRACT_HEADER(classname)

  source files:
    SO_NODE_SOURCE(classname)
    SO_NODE_ABSTRACT_SOURCE(classname)
    in constructor:
      SO_NODE_CONSTRUCTOR(classname)
      SO_NODE_DEFINE_ENUM_VALUE(enumname, enumvalue)
      SO_NODE_ADD_FIELD(fieldname, (defaultvalue))
    in initClass():
      SO_NODE_INIT_CLASS(classname, parentclassname, parentclassname)
      SO_NODE_INIT_ABSTRACT_CLASS(classname, parentclassname, parentclassname)
*/

#define PRIVATE_NODE_TYPESYSTEM_HEADER( ) \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const; \
private: \
  static SoType classTypeId


// FIXME: document. 20000103 mortene.
#define SO_NODE_ABSTRACT_HEADER(_class_) \
  PRIVATE_NODE_TYPESYSTEM_HEADER(); \
protected: \
  static const SoFieldData ** getFieldDataPtr(void); \
  virtual const SoFieldData * getFieldData(void) const; \
private: \
  static const SoFieldData ** parentFieldData; \
  static SoFieldData * fieldData; \
  /* Counts number of instances of subclasses aswell as "direct" */ \
  /* instances from non-abstract classes. */ \
  static unsigned int classinstances


// FIXME: document. 20000103 mortene.
#define SO_NODE_HEADER(_class_) \
  SO_NODE_ABSTRACT_HEADER(_class_); \
private: \
  static void * createInstance(void)


#define PRIVATE_NODE_TYPESYSTEM_SOURCE(_class_) \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
/* Don't set value explicitly to SoType::badType(), to avoid a bug in */ \
/* Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()). */ \
SoType _class_::classTypeId


// FIXME: document. 20000103 mortene.
#define SO_NODE_ABSTRACT_SOURCE(_class_) \
PRIVATE_NODE_TYPESYSTEM_SOURCE(_class_); \
 \
unsigned int _class_::classinstances = 0; \
const SoFieldData ** _class_::parentFieldData = NULL; \
SoFieldData * _class_::fieldData = NULL; \
 \
const SoFieldData ** \
_class_::getFieldDataPtr(void) \
{ \
  return (const SoFieldData **)(&_class_::fieldData); \
} \
 \
const SoFieldData * \
_class_::getFieldData(void) const \
{ \
  return _class_::fieldData; \
}


// FIXME: document. 20000103 mortene.
#define SO_NODE_SOURCE(_class_) \
SO_NODE_ABSTRACT_SOURCE(_class_); \
 \
void * \
_class_::createInstance(void) \
{ \
  return new _class_; \
}


// FIXME: document. 20000103 mortene.
#define SO_NODE_IS_FIRST_INSTANCE() \
  (classinstances == 1)


// FIXME: document. 20000103 mortene.
#define SO_NODE_CONSTRUCTOR(_class_) \
  do { \
    _class_::classinstances++; \
    /* Catch attempts to use a node class which has not been initialized. */ \
    assert(_class_::classTypeId != SoType::badType() && "you forgot init()!"); \
    /* Initialize a fielddata container for the class only once. */ \
    if (!_class_::fieldData) { \
      _class_::fieldData = \
        new SoFieldData(_class_::parentFieldData ? \
                        *_class_::parentFieldData : NULL); \
    } \
    /* Extension classes from the application programmers should not be \
       considered native. This is important to get the export code to do \
       the Right Thing. */ \
    this->isBuiltIn = FALSE; \
  } while (0)


#define PRIVATE_COMMON_INIT_CODE(_class_, _classname_, _createfunc_, _parentclass_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType() && "don't init() twice!"); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(_parentclass_::getClassTypeId() != SoType::badType() && "you forgot init() on parentclass!"); \
 \
    /* Set up entry in the type system. */ \
    _class_::classTypeId = \
      SoType::createType(_parentclass_::getClassTypeId(), \
                         _classname_, \
                         _createfunc_, \
                         SoNode::getNextActionMethodIndex()); \
    SoNode::incNextActionMethodIndex(); \
 \
    /* Store parent's fielddata pointer for later use in the constructor. */ \
    _class_::parentFieldData = _parentclass_::getFieldDataPtr(); \
  } while (0)


// FIXME: document. 20000103 mortene.
#define SO_NODE_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_INIT_CODE(_class_, classname, &_class_::createInstance, _parentclass_); \
  } while (0)


// FIXME: document. 20000103 mortene.
#define SO_NODE_INIT_ABSTRACT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_INIT_CODE(_class_, classname, NULL, _parentclass_); \
  } while (0)


// FIXME: document. 20000103 mortene.
#define SO_NODE_ADD_FIELD(_field_, _defaultval_) \
  do { \
    this->_field_.setValue _defaultval_;\
    this->_field_.setContainer(this); \
    if (SO_NODE_IS_FIRST_INSTANCE()) { \
      fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
    } \
  } while (0)


// FIXME: document. 20000103 mortene.
#define SO_NODE_DEFINE_ENUM_VALUE(_enumname_, _enumval_) \
  do { \
    if (SO_NODE_IS_FIRST_INSTANCE()) \
      fieldData->addEnumValue(SO__QUOTE(_enumname_), \
                              SO__QUOTE(_enumval_), _enumval_); \
  } while (0)

#endif // !COIN_SOSUBNODE_H
