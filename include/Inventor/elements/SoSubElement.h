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

#ifndef COIN_SOSUBELEMENT_H
#define COIN_SOSUBELEMENT_H

#include <Inventor/SbBasic.h> // for SO__QUOTE() definition
#include <Inventor/SbName.h>
#include <assert.h>
// Include SoElement.h to be Open Inventor compatible at compile-time.
#include <Inventor/elements/SoElement.h>

/*
 * This file contains macro definitions with common declarations and
 * definitions used in Coin elements.
 *
 * FIXME: document macros and how they are used to set up a new
 * user-extension skeleton element class (or just point to the example
 * code in examples/advanced/elements/). 19991209 mortene.
 *
 */

#define SO_ELEMENT_ABSTRACT_HEADER(_class_) \
private: \
  static SoType classTypeId; \
protected: \
  _class_(void); \
  static int classStackIndex; \
public: \
  static SoType getClassTypeId(void); \
  static int getClassStackIndex(void)

#define SO_ELEMENT_HEADER(_class_) \
  SO_ELEMENT_ABSTRACT_HEADER(_class_); \
public: \
  static void * createInstance(void)


#define PRIVATE_SOELEMENT_VARIABLES(_class_) \
int _class_::classStackIndex; \
SoType _class_::classTypeId = SoType::badType(); \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
int _class_::getClassStackIndex(void) { return _class_::classStackIndex; }


#define SO_ELEMENT_ABSTRACT_SOURCE(_class_) \
PRIVATE_SOELEMENT_VARIABLES(_class_) \
_class_::_class_(void) { }

#define SO_ELEMENT_SOURCE(_class_) \
PRIVATE_SOELEMENT_VARIABLES(_class_) \
_class_::_class_(void) { this->setTypeId(_class_::classTypeId); \
                         this->setStackIndex(_class_::classStackIndex); } \
void * _class_::createInstance(void) { return (void *) new _class_; }


#define PRIVATE_SOELEMENT_INIT(_class_, _parent_, _instantiate_) \
  do { \
    assert(_class_::classTypeId == SoType::badType()); \
    assert(_parent_::getClassTypeId() != SoType::badType()); \
    _class_::classTypeId = SoType::createType(_parent_::getClassTypeId(), \
                                              SO__QUOTE(_class_), \
                                              _instantiate_); \
    if (_parent_::classStackIndex < 0) _class_::classStackIndex = _class_::createStackIndex(_class_::classTypeId); \
    else _class_::classStackIndex = _parent_::classStackIndex; \
  } while (0)


#define SO_ELEMENT_INIT_ABSTRACT_CLASS(_class_, _parent_) \
  PRIVATE_SOELEMENT_INIT(_class_, _parent_, NULL)

#define SO_ELEMENT_INIT_CLASS(_class_, _parent_) \
  PRIVATE_SOELEMENT_INIT(_class_, _parent_, &_class_::createInstance)


#endif // !COIN_SOSUBELEMENT_H
