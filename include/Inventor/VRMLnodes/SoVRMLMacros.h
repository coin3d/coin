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

#ifndef COIN_SOVRMLMACROS_H
#define COIN_SOVRMLMACROS_H

#include <Inventor/nodes/SoSubNode.h>

#define SO_VRMLNODE_INTERNAL_CONSTRUCTOR(_class_) \
  SO_NODE_INTERNAL_CONSTRUCTOR(_class_); \
  this->setNodeType(SoNode::VRML2);

#define SO_VRMLNODE_ADD_EVENT_IN(_field_) \
  do { \
    this->_field_.setFieldType(SoField::EVENTIN_FIELD); \
    this->_field_.setContainer(this); \
    if (SO_NODE_IS_FIRST_INSTANCE()) { \
      fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
    } \
  } while (0)

#define SO_VRMLNODE_ADD_EVENT_OUT(_field_) \
  do { \
    this->_field_.setFieldType(SoField::EVENTOUT_FIELD); \
    this->_field_.setContainer(this); \
    if (SO_NODE_IS_FIRST_INSTANCE()) { \
      fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
    } \
  } while (0)

#define SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(_field_) \
  do { \
    this->_field_.setFieldType(SoField::EXPOSED_FIELD); \
    this->_field_.setContainer(this); \
    if (SO_NODE_IS_FIRST_INSTANCE()) { \
      fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
    } \
  } while (0)

#define SO_VRMLNODE_ADD_EMPTY_MFIELD(_field_) \
  do { \
    this->_field_.setContainer(this); \
    if (SO_NODE_IS_FIRST_INSTANCE()) { \
      fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
    } \
  } while (0)


#define SO_VRMLNODE_ADD_FIELD(_field_, _defaultval_) \
  SO_NODE_ADD_FIELD(_field_, _defaultval_)

#define SO_VRMLNODE_ADD_EXPOSED_FIELD(_field_, _defaultval_) \
  this->_field_.setFieldType(SoField::EXPOSED_FIELD); \
  SO_NODE_ADD_FIELD(_field_, _defaultval_)

#define SO_VRML97_NODE_TYPE (SoNode::VRML2|SoNode::COIN_2_0)


#endif // COIN_SOVRMLMACROS_H
