/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLMACROS_H
#define COIN_SOVRMLMACROS_H

#include <Inventor/nodes/SoSubNode.h>

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
