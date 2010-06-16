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

#include <SoDebug.h>

#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SbName.h>

#include "tidbitsp.h"

inline unsigned int SbHashFunc(const void * key);
#include "misc/SbHash.h"
inline unsigned int SbHashFunc(const void * key)
{
  return SbHashFunc(reinterpret_cast<size_t>(key));
}

// *************************************************************************

/*!
  \class SoDebug SoDebug.h SoDebug.h
  The SoDebug class is a small collection of debugging-related functions.
*/

// *************************************************************************

namespace {

struct SoDebug_internal {
  static SbHash<void *, char *> * namedict;
  static void delete_namedict(void);
};

SbHash<void *, char *> * SoDebug_internal::namedict = NULL;

} // anonymous namespace

/*!
  This is a portable getenv-wrapper.

  \sa coin_getenv()
*/

const char *
SoDebug::GetEnv(const char * var)
{
  return coin_getenv(var);
}

/*!
  Real-time printf designed for use when use of standard printf() would
  cause timing problems.

  NOT IMPLEMENTED.  Currently it just forwards to printf().
*/

void
SoDebug::RTPrintf(const char * formatstr, ...)
{
  // FIXME: should print to string buffer, dump complete buffer now
  // and then instead
  va_list args;
  va_start(args, formatstr);
  vprintf(formatstr, args);
  va_end(args);
}

/*!
  Associate a name with an arbitrary pointer.  You can fetch the name of
  the pointer later with PtrName().

  \sa PtrName()
*/

void
SoDebug::NamePtr(const char * name, void * ptr)
{
  if ( SoDebug_internal::namedict == NULL ) {
    SoDebug_internal::namedict = new SbHash<void *, char *>;
    coin_atexit(SoDebug_internal::delete_namedict, CC_ATEXIT_NORMAL);
  }
  char * data = NULL;
  if ( SoDebug_internal::namedict->get(ptr, data) ) {
    free(data);
    SoDebug_internal::namedict->erase(ptr);
  }
  data = strdup(name);
  SoDebug_internal::namedict->put(ptr, data);
}

/*!
  Returns the name set on a pointer with NamePtr().  If no name has been
  set, "<unnamed>" is returned.

  \sa NamePtr()
*/


const char *
SoDebug::PtrName(void * ptr)
{
  static const char fallback[] = "<noName>";
  if ( SoDebug_internal::namedict == NULL ) return fallback;
  char * data = NULL;
  if ( SoDebug_internal::namedict->get(ptr, data) ) {
    if ( data ) {
      return data;
    }
  }
  return fallback;
}

/*!
  Writes the node to stdout.
*/

void
SoDebug::write(SoNode * node)
{
  node->ref();
  SoOutput out;
  if (node->getNodeType() == SoNode::VRML2) {
    out.setHeaderString("#VRML V2.0 utf8");
  }
  SoWriteAction wa(&out);
  wa.apply(node);
  node->unrefNoDelete();
}

/*!
  Writes the node to the given filename, or /tmp/debug.iv if filename is NULL.
*/

void
SoDebug::writeToFile(SoNode * node, const char * filename)
{
  node->ref();
  const char * fname = filename ? filename : "/tmp/debug.iv";
  SoOutput out;
  out.openFile(fname);
  if (node->getNodeType() == SoNode::VRML2) {
    out.setHeaderString("#VRML V2.0 utf8");
  }
  SoWriteAction wa(&out);
  wa.apply(node);
  out.closeFile();
  node->unrefNoDelete();
}

/*!
  Not implemented.
*/

void
SoDebug::writeField(SoField * field)
{
  SoFieldContainer * container = field->getContainer();
  if (!container) return;
  SbName name;
  container->getFieldName(field, name);

  SoOutput out;
  field->write(&out, name);
}

/*!
  Not implemented.
*/

void
SoDebug::printName(SoBase * base)
{
  SbName name = base->getName();
  if (name != SbName::empty()) {
    puts(name.getString());
  } else {
    puts(" not named ");
  }
}

// *************************************************************************

void
SoDebug_internal::delete_namedict(void)
{
  for(
     SbHash<void *, char *>::const_iterator iter =
       namedict->const_begin();
      iter!=namedict->const_end();
      ++iter
      ) {
    if ( iter->obj ) free((void *) iter->obj);
  }

  delete namedict;
  namedict = NULL;
}

// *************************************************************************
