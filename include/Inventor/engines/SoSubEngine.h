/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOSUBENGINE_H
#define COIN_SOSUBENGINE_H

#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/fields/SoFieldData.h>
#include <assert.h>

//
// FIXME: document macros. pederb, 20000309
//

#define PRIVATE_ENGINE_TYPESYSTEM_HEADER( ) \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const; \
private: \
  static SoType classTypeId

#define SO_ENGINE_ABSTRACT_HEADER(_classname_) \
  PRIVATE_ENGINE_TYPESYSTEM_HEADER(); \
protected: \
  static const SoFieldData ** getInputDataPtr(void); \
  static const SoEngineOutputData ** getOutputDataPtr(void); \
public: \
  virtual const SoFieldData * getFieldData(void) const; \
  virtual const SoEngineOutputData * getOutputData(void) const; \
private: \
  static unsigned int classinstances; \
  static SoFieldData * inputdata; \
  static const SoFieldData ** parentinputdata; \
  static SoEngineOutputData * outputdata; \
  static const SoEngineOutputData ** parentoutputdata

#define SO_ENGINE_HEADER(_classname_) \
    SO_ENGINE_ABSTRACT_HEADER(_classname_); \
  public: \
    static void * createInstance(void)


#define PRIVATE_ENGINE_TYPESYSTEM_SOURCE(_class_) \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
/* Don't set value explicitly to SoType::badType(), to avoid a bug in */ \
/* Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()). */ \
SoType _class_::classTypeId

#define SO_ENGINE_ABSTRACT_SOURCE(_class_) \
PRIVATE_ENGINE_TYPESYSTEM_SOURCE(_class_); \
 \
unsigned int _class_::classinstances = 0; \
SoFieldData * _class_::inputdata = NULL; \
const SoFieldData ** _class_::parentinputdata = NULL; \
SoEngineOutputData * _class_::outputdata = NULL; \
const SoEngineOutputData ** _class_::parentoutputdata = NULL; \
 \
const SoFieldData ** \
_class_::getInputDataPtr(void) \
{ \
  return (const SoFieldData **)&_class_::inputdata; \
} \
 \
const SoFieldData * \
_class_::getFieldData(void) const \
{ \
  return _class_::inputdata; \
} \
 \
const SoEngineOutputData ** \
_class_::getOutputDataPtr(void) \
{ \
  return (const SoEngineOutputData**)&_class_::outputdata; \
} \
 \
const SoEngineOutputData * \
_class_::getOutputData(void) const \
{ \
  return _class_::outputdata; \
}

#define SO_ENGINE_SOURCE(_class_) \
SO_ENGINE_ABSTRACT_SOURCE(_class_); \
 \
void * \
_class_::createInstance(void) \
{ \
  return new _class_; \
}

#define SO_ENGINE_IS_FIRST_INSTANCE() \
  (classinstances == 1)

#define SO_ENGINE_CONSTRUCTOR(_class_) \
  do { \
    _class_::classinstances++; \
    /* Catch attempts to use an engine class which has not been initialized. */ \
    assert(_class_::classTypeId != SoType::badType()); \
    /* Initialize a inputdata container for the class only once. */ \
    if (!_class_::inputdata) { \
      _class_::inputdata = \
        new SoFieldData(_class_::parentinputdata ? \
                        *_class_::parentinputdata : NULL); \
      _class_::outputdata = \
        new SoEngineOutputData(_class_::parentoutputdata ? \
                               *_class_::parentoutputdata : NULL); \
    } \
    /* Extension classes from the application programmers should not be */ \
    /* considered native. This is important to get the export code to do */ \
    /* the Right Thing. */ \
    this->isBuiltIn = FALSE; \
  } while (0)


#define PRIVATE_COMMON_ENGINE_INIT_CODE(_class_, _classname_, _createfunc_, _parentclass_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(_parentclass_::getClassTypeId() != SoType::badType()); \
 \
    /* Set up entry in the type system. */ \
    _class_::classTypeId = \
      SoType::createType(_parentclass_::getClassTypeId(), \
                         _classname_, \
                         _createfunc_); \
 \
    /* Store parent's data pointers for later use in the constructor. */ \
    _class_::parentinputdata = _parentclass_::getInputDataPtr(); \
    _class_::parentoutputdata = _parentclass_::getOutputDataPtr(); \
  } while (0)


#define SO_ENGINE_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_ENGINE_INIT_CODE(_class_, classname, &_class_::createInstance, _parentclass_); \
  } while (0)


#define SO_ENGINE_INIT_ABSTRACT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_COMMON_ENGINE_INIT_CODE(_class_, classname, NULL, _parentclass_); \
  } while (0)


#define SO_ENGINE_ADD_INPUT(_input_, _defaultval_) \
  do { \
    this->_input_.setValue _defaultval_;\
    this->_input_.setContainer(this); \
    if (SO_ENGINE_IS_FIRST_INSTANCE()) { \
      inputdata->addField(this, SO__QUOTE(_input_), &this->_input_);\
    } \
  } while (0)

#define SO_ENGINE_ADD_OUTPUT(_output_, _type_) \
  do { \
    if (SO_ENGINE_IS_FIRST_INSTANCE()) { \
      outputdata->addOutput(this, SO__QUOTE(_output_), \
                            &this->_output_, \
                            _type_::getClassTypeId()); \
    } \
    this->_output_.setContainer(this); \
  } while(0)


#define SO_ENGINE_DEFINE_ENUM_VALUE(_enumname_, _enumval_) \
  do { \
    if (SO_ENGINE_IS_FIRST_INSTANCE()) \
      inputdata->addEnumValue(SO__QUOTE(_enumname_), \
                              SO__QUOTE(_enumval_), _enumval_); \
  } while (0)

#define SO_ENGINE_OUTPUT(_engineout_, _fieldtype_, _writeop_) \
  do { \
    if (_engineout_.isEnabled()) { \
      /* No fields can be added or removed during this loop, as it */ \
      /* is a "closed" operation. (The fields are disabled for */ \
      /* notification while the loop runs). */ \
      int SO_ENGINE_OUTPUT_numconnections = _engineout_.getNumConnections(); \
      /* The reason we use the perverted variable names is to */ \
      /* avoid the possibility of getting _extremely_ hard */ \
      /* to find bugs when _writeop_ contains the same variable */ \
      /* names we are using internally in the macro. */ \
      for (int SO_ENGINE_OUTPUT_i = 0; SO_ENGINE_OUTPUT_i < SO_ENGINE_OUTPUT_numconnections; SO_ENGINE_OUTPUT_i++) { \
        _fieldtype_ * SO_ENGINE_OUTPUT_field = (_fieldtype_*) _engineout_[SO_ENGINE_OUTPUT_i]; \
        if (!SO_ENGINE_OUTPUT_field->isReadOnly()) { SO_ENGINE_OUTPUT_field->_writeop_; } \
      } \
      /* paranoid assertion */ \
      assert(_engineout_.getNumConnections() == SO_ENGINE_OUTPUT_numconnections); \
    } \
  } while (0)

#define SO_COMPOSE__HEADER(_name_) \
  SO_ENGINE_HEADER(_name_); \
  private: \
    virtual void evaluate(); \
  protected: \
    ~_name_();\
  public: \
   _name_(); \
    static void initClass()

#endif // !COIN_SOSUBENGINE_H
