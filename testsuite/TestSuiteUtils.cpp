/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoMemoryError.h>
#include <Inventor/errors/SoReadError.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>

#include <TestSuiteUtils.h>

using namespace SIM::Coin3D::Coin;

namespace {

int debuginfocount = 0;
int debugwarningcount = 0;
int debugerrorcount = 0;
int readerrorcount = 0;
int memoryerrorcount = 0;

struct filterset {
  char ** filters;
  filterset * next;
} * messagefilters = NULL;

int
should_filter(const SbString & msg)
{
  filterset * filters = messagefilters;
  while (filters != NULL) {
    const char * pattern = NULL;
    for (int i = 0; filters->filters[i] != NULL; ++i) {
      if (msg.find(filters->filters[i])) {
        return 1;
      }
    }
    filters = filters->next;
  }
  return 0;
}

void
debugerrormsg_handler(const SoError * error, void * data)
{
  assert(error);
  const SoDebugError * dbgerror = static_cast<const SoDebugError *>(error);
  switch (dbgerror->getSeverity()) {
  case SoDebugError::INFO:
    ++debuginfocount;
    break;
  case SoDebugError::WARNING:
    ++debugwarningcount;
    break;
  case SoDebugError::ERROR:
    ++debugerrorcount;
    break;
  default:
    assert(0 && "schizofrene SoDebugError error");
  }
  const SbString & msg = error->getDebugString();
  if (!should_filter(msg)) fprintf(stderr, "%s\n", msg.getString());
}

void
readerrormsg_handler(const SoError * error, void * data)
{
  ++readerrorcount;
  const SbString & msg = error->getDebugString();
  if (!should_filter(msg)) fprintf(stderr, "%s\n", msg.getString());
}

void
memoryerrormsg_handler(const SoError * error, void * data)
{
  ++memoryerrorcount;
  const SbString & msg = error->getDebugString();
  if (!should_filter(msg)) fprintf(stderr, "%s\n", msg.getString());
}

} // anonymous namespace

void
TestSuite::Init(void)
{
  SoDebugError::setHandlerCallback(debugerrormsg_handler, NULL);
  SoReadError::setHandlerCallback(readerrormsg_handler, NULL);
  SoMemoryError::setHandlerCallback(memoryerrormsg_handler, NULL);
}

void
TestSuite::PushMessageSuppressFilters(const char * patterns[])
{
  filterset * filters = new filterset;
  filters->next = messagefilters;
  messagefilters = filters;

  int count = 0;
  for (; patterns[count] != NULL; ++count) { }
  filters->filters = new char * [ count + 1 ];
  for (int i = 0; i < count; ++i ) {
    filters->filters[i] = new char [ strlen(patterns[i]) + 1 ];
    strcpy(filters->filters[i], patterns[i]);
  }
  filters->filters[count] = NULL;
}

void
TestSuite::PopMessageSuppressFilters(void)
{
  assert(messagefilters);
  filterset * filters = messagefilters;
  messagefilters = filters->next;

  for (int i = 0; filters->filters[i] != NULL; ++i) {
    delete [] filters->filters[i];
  }
  delete [] filters->filters;
  delete filters;
}

void
TestSuite::ResetDebugInfoCount(int count)
{
  debuginfocount = count;
}

int
TestSuite::GetDebugInfoCount(void)
{
  return debuginfocount;
}

void
TestSuite::ResetDebugWarningCount(int count)
{
  debugwarningcount = count;
}

int
TestSuite::GetDebugWarningCount(void)
{
  return debugwarningcount;
}

void
TestSuite::ResetDebugErrorCount(int count)
{
  debugerrorcount = count;
}

int
TestSuite::GetDebugErrorCount(void)
{
  return debugerrorcount;
}

void
TestSuite::ResetReadErrorCount(int count)
{
  readerrorcount = count;
}

int
TestSuite::GetReadErrorCount(void)
{
  return readerrorcount;
}

void
TestSuite::ResetMemoryErrorCount(int count)
{
  memoryerrorcount = count;
}

int
TestSuite::GetMemoryErrorCount(void)
{
  return memoryerrorcount;
}

SoNode *
TestSuite::ReadInventorFile(const char * filename)
{
  assert(filename);
  SoNode * root = NULL;
  {
    SoInput in;
    if (!in.openFile(filename)) {
      return NULL;
    }
    root = SoDB::readAll(&in);
  }
  return root;
}

int
TestSuite::WriteInventorFile(const char * filename, SoNode * root)
{
  assert(filename);
  assert(root);
  SoOutput out;
  if (!out.openFile(filename)) {
    return FALSE;
  }
  SoWriteAction wa(&out);
  root->ref();
  wa.apply(root);
  root->unrefNoDelete();
  return TRUE;
}
