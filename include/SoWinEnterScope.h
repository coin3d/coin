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
