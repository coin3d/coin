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

#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <SoDebug.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/misc/SbHash.h>

// *************************************************************************

/*!
  \class SoDebug SoDebug.h SoDebug.h
  The SoDebug class is a small collection of debugging-related functions.
*/

// *************************************************************************

struct SoDebug_internal {
  static SbHash<char *, void *> * namedict;
  static void delete_namedict(void);
  static void delete_namedict_entry(void * & key, char * & obj, void * closure);
};

SbHash<char *, void *> * SoDebug_internal::namedict = NULL;

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
    SoDebug_internal::namedict = new SbHash<char *, void *>;
    coin_atexit(SoDebug_internal::delete_namedict, CC_ATEXIT_NORMAL);
  }
  char * data = NULL;
  if ( SoDebug_internal::namedict->get(ptr, data) ) {
    free(data);
    SoDebug_internal::namedict->remove(ptr);
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
  static const char fallback[] = "<unnamed>";
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
  Not implemented.
*/

void
SoDebug::write(SoNode * node)
{
  // FIXME: stub
}

/*!
  Not implemented.
*/

void
SoDebug::writeToFile(SoNode * node, const char * filename)
{
  // FIXME: stub
}

/*!
  Not implemented.
*/

void
SoDebug::writeField(SoField * field)
{
  // FIXME: stub
}

/*!
  Not implemented.
*/

void
SoDebug::printName(SoBase * base)
{
  // FIXME: stub
}

// *************************************************************************

void
SoDebug_internal::delete_namedict(void)
{
  namedict->apply((SbHash<char *, void *>::SbHashApplyFunc *) SoDebug_internal::delete_namedict_entry, NULL);
  delete namedict;
  namedict = NULL;
}

void
SoDebug_internal::delete_namedict_entry(void * & key, char * & obj, void * closure)
{
  if ( obj ) free((void *) obj);
}

// *************************************************************************
