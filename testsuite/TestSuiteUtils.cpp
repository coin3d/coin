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

#include <TestSuiteUtils.h>

using namespace SIM::Coin3D::Coin;

namespace {
int debugwarningcount = 0;
int debugerrorcount = 0;
int readwarningcount = 0;
int readerrorcount = 0;
} // anonymous

// TODO: add system for push/pop-filtering specific warnings away from the
// testsuite run output.

void
TestSuite::Init(void)
{
  // FIXME: add error handlers that increment counters
}

void
TestSuite::ResetDebugWarningCount(void)
{
  debugwarningcount = 0;
}

int
TestSuite::GetDebugWarningCount(void)
{
  return debugwarningcount;
}

void
TestSuite::ResetDebugErrorCount(void)
{
  debugerrorcount = 0;
}

int
TestSuite::GetDebugErrorCount(void)
{
  return debugerrorcount;
}

void
TestSuite::ResetReadWarningCount(void)
{
  readwarningcount = 0;
}

int
TestSuite::GetReadWarningCount(void)
{
  return readwarningcount;
}

void
TestSuite::ResetReadErrorCount(void)
{
  readerrorcount = 0;
}

int
TestSuite::GetReadErrorCount(void)
{
  return readerrorcount;
}

