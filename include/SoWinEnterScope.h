/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOWINENTERSCOPE_H
#define COIN_SOWINENTERSCOPE_H

// This is just a dummy file to be sourcecode compatible with TGS'
// Inventor on MSWindows platforms. They use this header file to set
// up the __declspec(dllimport) define for application programs to
// properly import the DLL's API.
//
// This is not such a good idea, as __declspec(dllimport) should only
// conditionally be defined when the application programmer is using
// the Coin library as a DLL, and not when using it as a static
// library.
//
// When compiling application code against Coin and linking with a
// Coin dynamic library (.dll), you need to add the define "COIN_DLL"
// to the preprocessor / compiler.
//
//                                                  20001220 mortene.

#ifdef COIN_INTERNAL
#error "BUILD ERROR: do not include this file when compiling the library."
#endif /* COIN_INTERNAL */


#endif /* !COIN_SOWINENTERSCOPE_H */
