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
    fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
  } while (0)

#define SO_VRMLNODE_ADD_EVENT_OUT(_field_) \
  do { \
    this->_field_.setFieldType(SoField::EVENTOUT_FIELD); \
    this->_field_.setContainer(this); \
    fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
  } while (0)

#define SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(_field_) \
  do { \
    this->_field_.setFieldType(SoField::EXPOSED_FIELD); \
    this->_field_.setContainer(this); \
    fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
  } while (0)

#define SO_VRMLNODE_ADD_EMPTY_MFIELD(_field_) \
  do { \
    this->_field_.setContainer(this); \
    fieldData->addField(this, SO__QUOTE(_field_), &this->_field_);\
  } while (0)


#define SO_VRMLNODE_ADD_FIELD(_field_, _defaultval_) \
  SO_NODE_ADD_FIELD(_field_, _defaultval_)

#define SO_VRMLNODE_ADD_EXPOSED_FIELD(_field_, _defaultval_) \
  this->_field_.setFieldType(SoField::EXPOSED_FIELD); \
  SO_NODE_ADD_FIELD(_field_, _defaultval_)

#define SO_VRML97_NODE_TYPE (SoNode::VRML2|SoNode::COIN_2_0)


#endif // COIN_SOVRMLMACROS_H
