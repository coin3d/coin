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

#ifndef COIN_SOVRMLSUBINTERPOLATOR_H
#define COIN_SOVRMLSUBINTERPOLATOR_H

#include <Inventor/nodes/SoSubNode.h>

#define SO_INTERPOLATOR_HEADER(classname) \
  SO_NODE_HEADER(classname)
#define SO_INTERPOLATOR_ABSTRACT_HEADER(classname) \
  SO_NODE_ABSTRACT_HEADER(classname)

#define SO_INTERPOLATOR_SOURCE(classname) \
  SO_NODE_SOURCE(classname)

#define SO_INTERPOLATOR_ABSTRACT_SOURCE(classname) \
  SO_NODE_ABSTRACT_SOURCE(classname)

#define SO_INTERPOLATOR_INIT_CLASS(classname, printname, parentclass) \
  SO_NODE_INIT_CLASS(classname, printname, parentclass)

#define SO_INTERPOLATOR_INIT_ABSTRACT_CLASS(classname,printname,parent) \
  SO_NODE_INIT_ABSTRACT_CLASS(classname, printname, parent)

#define SO_INTERPOLATOR_CONSTRUCTOR(classname) \
  SO_NODE_CONSTRUCTOR(classname)

#define SO_INTERPOLATOR_ADD_INPUT(inputName, defaultValue) \
  SO_NODE_ADD_FIELD(inputName, defaultValue)

#define SO_INTERPOLATOR_ADD_OUTPUT(outputName, outputType)
#define SO_INTERPOLATOR_DEFINE_ENUM_VALUE(enumType, enumValue) \
  SO_NODE_DEFINE_ENUM_VALUE(enumType, enumValue)
#define SO_INTERPOLATOR_IS_FIRST_INSTANCE() \
  SO_NODE_IS_FIRST_INSTANCE()

#define SO_INTERPOLATOR_SET_SF_ENUM_TYPE(fieldName, enumType) \
  SO_NODE_SET_SF_ENUM_TYPE(fieldName, enumType)

#define SO_INTERPOLATOR_SET_MF_ENUM_TYPE(fieldName, enumType) \
  SO_NODE_SET_MF_ENUM_TYPE(fieldName, enumType)

#define SO_INTERPOLATOR_OUTPUT(outputName, outputType, method)

#endif // ! COIN_SOVRMLSUBINTERPOLATOR_H
