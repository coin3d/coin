#ifndef COIN_TESTSUITEUTILS_H
#define COIN_TESTSUITEUTILS_H

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

#include <string>
#include <vector>

class SoNode;

namespace SIM { namespace Coin3D { namespace Coin { namespace TestSuite {

typedef bool test_files_CB(SoNode * root, std::string & filename);

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

SoNode * ReadInventorFile(const char * filename);
int WriteInventorFile(const char * filename, SoNode * root);

void test_all_files(const std::string & search_directory,
                    std::vector<std::string> & suffixes,
                    test_files_CB * testFunction);

} } } } // namespace

#endif // !COIN_TESTSUITEUTILS_H
