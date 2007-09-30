#ifndef COIN_TESTSUITEUTILS_H
#define COIN_TESTSUITEUTILS_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

namespace SIM { namespace Coin3D { namespace Coin { namespace TestSuite {

void Init(void);

void PushMessageSuppressFilters(const char * patterns[]);
void PopMessageSuppressFilters(void);

void ResetDebugInfoCount(int count = 0);
int GetDebugInfoCount(void);

void ResetDebugWarningCount(int count = 0);
int GetDebugWarningCount(void);

void ResetDebugErrorCount(int count = 0);
int GetDebugErrorCount(void);

void ResetReadErrorCount(int count = 0);
int GetReadErrorCount(void);

void ResetMemoryErrorCount(int count = 0);
int GetMemoryErrorCount(void);

} } } } // namespace

#endif // !COIN_TESTSUITEUTILS_H
