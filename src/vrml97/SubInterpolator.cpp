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

#ifndef COIN_SOVRMLSUBINTERPOLATOR_H
#define COIN_SOVRMLSUBINTERPOLATOR_H

#define SO_INTERPOLATOR_HEADER(classname)
#define SO_INTERPOLATOR_ABSTRACT_HEADER(classname)

#define SO_INTERPOLATOR_SOURCE(classname)
#define SO_INTERPOLATOR_ABSTRACT_SOURCE(classname)

#define SO_INTERPOLATOR_INIT_CLASS
#define SO_INTERPOLATOR_INIT_ABSTRACT_CLASS

#define SO_INTERPOLATOR_CONSTRUCTOR(classname)
#define SO_INTERPOLATOR_ADD_INPUT(inputName, defaultValue)
#define SO_INTERPOLATOR_ADD_OUTPUT(outputName, outputType)
#define SO_INTERPOLATOR_DEFINE_ENUM_VALUE(enumType, enumValue)
#define SO_INTERPOLATOR_IS_FIRST_INSTANCE()

#define SO_INTERPOLATOR_SET_SF_ENUM_TYPE(fieldName, enumType)
#define SO_INTERPOLATOR_SET_MF_ENUM_TYPE(fieldName, enumType)

#define SO_INTERPOLATOR_OUTPUT(outputName, outputType, method)

#endif // ! COIN_SOVRMLSUBINTERPOLATOR_H
