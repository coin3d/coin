#ifndef COIN_SOSUBNODEP_H
#define COIN_SOSUBNODEP_H

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

// The macro definitions in this file is used internally by Coin
// classes, and mirrors some of the public macros available in
// SoSubNode.h with a few modifications so they are suited for the
// builtin classes.
//
// The macros in this file are not made visible for use by the
// application programmer.

#ifndef COIN_INTERNAL
#error this is a private header file
#endif // !COIN_INTERNAL

// only internal nodes can use this macro and pass "inherited" as arg #4
#define PRIVATE_INTERNAL_COMMON_INIT_CODE(_class_, _classname_, _createfunc_, _parentclass_) \
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
    cc_coin_atexit_static_internal(reinterpret_cast<coin_atexit_f *>(_class_::atexit_cleanup)); \
  } while (0)

#define SO_NODE_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SoBase::staticDataLock(); \
    SO_NODE_CONSTRUCTOR_NOLOCK(_class_); \
    /* Restore value of isBuiltIn flag (which is set to FALSE */ \
    /* in the SO_NODE_CONSTRUCTOR() macro. */ \
    this->isBuiltIn = TRUE; \
    SoBase::staticDataUnlock(); \
  } while (0)


#define SO_NODE_INTERNAL_INIT_CLASS(_class_, _fileformats_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_INTERNAL_COMMON_INIT_CODE(_class_, &classname[2], &_class_::createInstance, inherited); \
    SoNode::setCompatibilityTypes(_class_::getClassTypeId(), _fileformats_); \
  } while (0)


#define SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(_class_, _fileformats_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_INTERNAL_COMMON_INIT_CODE(_class_, &classname[2], NULL, inherited); \
    SoNode::setCompatibilityTypes(_class_::getClassTypeId(), _fileformats_); \
  } while (0)


// *************************************************************************

// Convenience defines to use for the second parameter of the
// SO_NODE_INTERNAL_INIT_CLASS() macro (for internal node classes).
//
// It's is handy to use these defines instead of the enum flags
// directly, as these can be updated in this one central location,
// instead of having to update all node source files on each new Coin
// major release.

#define SO_FROM_COIN_4_0 \
  (SoNode::COIN_4_0)

#define SO_FROM_COIN_3_0 \
  (SoNode::COIN_3_0|SO_FROM_COIN_4_0)

#define SO_FROM_COIN_2_5 \
  (SoNode::COIN_2_5|SO_FROM_COIN_3_0)

#define SO_FROM_COIN_2_4 \
  (SoNode::COIN_2_4|SO_FROM_COIN_2_5)

#define SO_FROM_COIN_2_3 \
  (SoNode::COIN_2_3|SO_FROM_COIN_2_4)

#define SO_FROM_COIN_2_2 \
  (SoNode::COIN_2_2|SO_FROM_COIN_2_3)

#define SO_FROM_COIN_2_0 \
  (SoNode::COIN_2_0|SO_FROM_COIN_2_2)

#define SO_FROM_COIN_1_0 \
  (SoNode::COIN_1_0|SO_FROM_COIN_2_0)

// *************************************************************************

#define SO_FROM_INVENTOR_6_0 \
  (SoNode::INVENTOR_6_0)

#define SO_FROM_INVENTOR_5_0 \
  (SoNode::INVENTOR_5_0)

// Several releases missing in-between here, but we don't seem to have
// any nodes from those yet...  -mortene.

#define SO_FROM_INVENTOR_2_6 \
  (SoNode::INVENTOR_2_6|SO_FROM_INVENTOR_5_0)

#define SO_FROM_INVENTOR_2_5 \
  (SoNode::INVENTOR_2_5|SO_FROM_INVENTOR_2_6)

#define SO_FROM_INVENTOR_2_1 \
  (SoNode::INVENTOR_2_1|SO_FROM_INVENTOR_2_5|SO_FROM_COIN_1_0)

#define SO_FROM_INVENTOR_2_0 \
  (SoNode::INVENTOR_2_0|SO_FROM_INVENTOR_2_1)

#define SO_FROM_INVENTOR_1 \
  (SoNode::INVENTOR_1|SO_FROM_INVENTOR_2_0)

// *************************************************************************

#endif // !COIN_SOSUBNODEP_H
