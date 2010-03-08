#ifndef COIN_SOSUBNODE_H
#define COIN_SOSUBNODE_H

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

// *************************************************************************

#include <string.h> /* strcmp used in assert() */
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/C/tidbits.h>

// *************************************************************************

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
  static void atexit_cleanup(void); \
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

// *************************************************************************

#define PRIVATE_NODE_TYPESYSTEM_SOURCE(_class_) \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
SoType _class_::classTypeId STATIC_SOTYPE_INIT


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
  return const_cast<const SoFieldData **>(&_class_::fieldData); \
} \
 \
const SoFieldData * \
_class_::getFieldData(void) const \
{ \
  return _class_::fieldData; \
} \
 \
void \
_class_::atexit_cleanup(void) \
{ \
  delete _class_::fieldData; \
  _class_::fieldData = NULL; \
  _class_::parentFieldData = NULL; \
  SoType::removeType(_class_::classTypeId.getName()); \
  _class_::classTypeId STATIC_SOTYPE_INIT; \
  _class_::classinstances = 0; \
}

// FIXME: document. 20000103 mortene.
#define SO_NODE_SOURCE(_class_) \
SO_NODE_ABSTRACT_SOURCE(_class_) \
 \
/*! \
  Creates a new instance of the class type corresponding to the SoType object. \
*/ \
void * \
_class_::createInstance(void) \
{ \
  return new _class_; \
}

// *************************************************************************

// FIXME: document. 20000103 mortene.
#define SO_NODE_IS_FIRST_INSTANCE() \
   (classinstances == 1)

// FIXME: document. 20000103 mortene.
#define SO_NODE_CONSTRUCTOR_NOLOCK(_class_) \
  do { \
    _class_::classinstances++; \
    /* Catch attempts to use a node class which has not been initialized. */ \
    assert(_class_::classTypeId != SoType::badType() && "you forgot init()!"); \
    /* Initialize a fielddata container for the class only once. */ \
    if (!_class_::fieldData) { \
      /* FIXME: this is a "static" memory leak. 20030131 mortene. */ \
      /* Internal Coin nodes are handled properly though. pederb, 20041122 */ \
      _class_::fieldData = \
        new SoFieldData(_class_::parentFieldData ? \
                        *_class_::parentFieldData : NULL); \
    } \
    /* Extension classes from the application programmers should not be \
       considered native. This is important to get the export code to do \
       the Right Thing. */ \
    this->isBuiltIn = FALSE; \
  } while (0)

// FIXME: document. 20000103 mortene.
#define SO_NODE_CONSTRUCTOR(_class_) \
  do { \
    SoBase::staticDataLock(); \
    SO_NODE_CONSTRUCTOR_NOLOCK(_class_); \
    SoBase::staticDataUnlock(); \
  } while (0)

// *************************************************************************

// FIXME: create-type with get-next and inc-next must be an atomic step in an MT
// environment  20020216 larsa
// FIXME: you can not use the _parentclass_ argument directly as a class specifier
// in initClass because such usage causes dynamic loading to fail when trying to
// invoke the initClass method in the case where _parentclass_ also has to be
// dynamically loaded.  that is the reason for this roundabout way of getting the
// parent class type.  20020216 larsa

#define PRIVATE_COMMON_INIT_CODE(_class_, _classname_, _createfunc_, _parentclass_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType() && "don't init() twice!"); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(strcmp(SO__QUOTE(_parentclass_), "inherited")); \
    SoType parentType(SoType::fromName(SO__QUOTE(_parentclass_))); \
    assert(parentType != SoType::badType() && "you forgot init() on parentclass!"); \
 \
    /* Set up entry in the type system. */ \
    _class_::classTypeId = \
      SoType::createType(parentType, \
                         _classname_, \
                         _createfunc_, \
                         SoNode::getNextActionMethodIndex()); \
    SoNode::incNextActionMethodIndex(); \
 \
    /* Store parent's fielddata pointer for later use in the constructor. */ \
    _class_::parentFieldData = _parentclass_::getFieldDataPtr(); \
\
    /* Make sure also external nodes are cleaned up */ \
    cc_coin_atexit_static_internal(_class_::atexit_cleanup); \
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


// *************************************************************************

// FIXME: document. 20000103 mortene.
#define SO_NODE_ADD_FIELD(_field_, _defaultval_) \
  do { \
    this->_field_.setValue _defaultval_;\
    this->_field_.setContainer(this); \
    fieldData->addField(this, SO__QUOTE(_field_), &this->_field_); \
  } while (0)

// New for Coin-3
#define SO_NODE_ADD_EMPTY_MFIELD(_field_) \
  do { \
    this->_field_.setContainer(this); \
    fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
  } while (0)


// FIXME: document. 20000103 mortene.
#define SO_NODE_DEFINE_ENUM_VALUE(_enumname_, _enumval_) \
  do { \
    fieldData->addEnumValue(SO__QUOTE(_enumname_), \
                            SO__QUOTE(_enumval_), _enumval_); \
  } while (0)

// *************************************************************************

#endif // !COIN_SOSUBNODE_H
