/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSUBNODEENGINE_H
#define COIN_SOSUBNODEENGINE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/engines/SoSubEngine.h>

#define SO_NODEENGINE_ABSTRACT_HEADER(_class_) \
  SO_NODE_ABSTRACT_HEADER(_class_); \
protected: \
  static const SoEngineOutputData ** getOutputDataPtr(void); \
public: \
  virtual const SoEngineOutputData * getOutputData(void) const; \
private: \
  static SoEngineOutputData * outputdata; \
  static const SoEngineOutputData ** parentoutputdata

#define SO_NODEENGINE_HEADER(_class_) \
  SO_NODEENGINE_ABSTRACT_HEADER(_class_); \
  public: \
    static void * createInstance(void)

#define SO_NODEENGINE_ABSTRACT_SOURCE(_class_) \
SO_NODE_ABSTRACT_SOURCE(_class_); \
SoEngineOutputData * _class_::outputdata = NULL; \
const SoEngineOutputData ** _class_::parentoutputdata = NULL; \
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

#define SO_NODEENGINE_SOURCE(_class_) \
SO_NODEENGINE_ABSTRACT_SOURCE(_class_); \
 \
void * \
_class_::createInstance(void) \
{ \
  return new _class_; \
}

#define SO_NODEENGINE_CONSTRUCTOR(_class_) \
  do { \
    _class_::classinstances++; \
    /* Catch attempts to use an engine class which has not been initialized. */ \
    assert(_class_::classTypeId != SoType::badType()); \
    /* Initialize a inputdata container for the class only once. */ \
    if (!_class_::fieldData) { \
      _class_::fieldData = \
        new SoFieldData(_class_::parentFieldData ? \
                        *_class_::parentFieldData : NULL); \
      _class_::outputdata = \
        new SoEngineOutputData(_class_::parentoutputdata ? \
                               *_class_::parentoutputdata : NULL); \
    } \
    /* Extension classes from the application programmers should not be */ \
    /* considered native. This is important to get the export code to do */ \
    /* the Right Thing. */ \
    this->isBuiltIn = FALSE; \
  } while (0)


#define SO_NODEENGINE_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  SO_NODE_INIT_CLASS(_class_, _parentclass_, _parentname_); \
  do { \
    _class_::parentoutputdata = _parentclass_::getOutputDataPtr(); \
  } while (0)

#define SO_NODEENGINE_INIT_ABSTRACT_CLASS(_class_, _parentclass_, _parentname_) \
  SO_NODE_INIT_ABSTRACT_CLASS(_class_, _parentclass_, _parentname_); \
  do { \
    _class_::parentoutputdata = _parentclass_::getOutputDataPtr(); \
  } while (0)

#endif // COIN_SOSUBNODEENGINE_H
