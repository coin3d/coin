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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOSUBNODE_H__
#define __SOSUBNODE_H__

#include <Inventor/misc/SoBasic.h> // for SO__QUOTE() definition

#define SO_NODE_HEADER(_nodeclass_) \
private: \
  static SoType classTypeId; \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const; \
  static void * createInstance(void);


#define SO_NODE_SOURCE(_nodeclass_) \
SoType _nodeclass_::classTypeId = SoType::badType(); \
void * _nodeclass_::createInstance(void) { return new _nodeclass_; } \
SoType _nodeclass_::getClassTypeId(void) { return _nodeclass_::classTypeId; } \
SoType _nodeclass_::getTypeId(void) const { return _nodeclass_::classTypeId; }


#define SO_NODE_CONSTRUCTOR(_nodeclass_) \
assert(_nodeclass_::classTypeId != SoType::badType());


#define SO_NODE_INIT_CLASS(_nodeclass_, _parentclass_, _parentname_) \
assert(_nodeclass_::classTypeId == SoType::badType()); \
assert(_parentclass_::getClassTypeId() != SoType::badType()); \
_nodeclass_::classTypeId = \
  SoType::createType(_parentclass_::getClassTypeId(), SO__QUOTE(_nodeclass_), \
                     &_nodeclass_::createInstance, \
                     _parentclass_::nextActionMethodIndex++);



#define SO_NODE_ADD_FIELD(_fieldname_, _defaultval_) \
do { \
  this->fieldData.addField(this, SO__QUOTE(_fieldname_), &this->_fieldname_);\
  this->_fieldname_.setValue _defaultval_;\
  this->_fieldname_.setContainer(this); \
  this->_fieldname_.setDefault(TRUE); \
} while (0)

#define SO_NODE_DEFINE_ENUM_VALUE(_enumname_, _enumval_) \
  this->fieldData.addEnumValue(SO__QUOTE(_enumname_), SO__QUOTE(_enumval_), \
                               _enumval_);

#endif // !__SOSUBNODE_H__
