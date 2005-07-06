
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <SoDebug.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/misc/SbHash.h>

// *************************************************************************

struct SoDebug_internal {
  static SbHash<char *, void *> * namedict;
  static void delete_namedict(void);
  static void delete_namedict_entry(void * & key, char * & obj, void * closure);
};

SbHash<char *, void *> * SoDebug_internal::namedict = NULL;

const char *
SoDebug::GetEnv(const char * var)
{
  return coin_getenv(var);
}

void
SoDebug::RTPrintf(const char * formatstr, ...)
{
  // FIXME: should print to string buffer, dump complete buffer now
  // and then instead
  va_list args;
  va_start(formatstr, args);
  vprintf(formatstr, args);
  va_end(args);
}

void
SoDebug::NamePtr(const char * name, void * ptr)
{
  if ( SoDebug_internal::namedict == NULL ) {
    SoDebug_internal::namedict = new SbHash<char *, void *>;
    cc_coin_atexit(SoDebug_internal::delete_namedict);
  }
  char * data = NULL;
  if ( SoDebug_internal::namedict->get(ptr, data) ) {
    free(data);
    SoDebug_internal::namedict->remove(ptr);
  }
  data = strdup(name);
  SoDebug_internal::namedict->put(ptr, data);
}

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

void
SoDebug::write(SoNode * node)
{
  // FIXME: stub
}

void
SoDebug::writeToFile(SoNode * node, const char * filename)
{
  // FIXME: stub
}

void
SoDebug::writeField(SoField * field)
{
  // FIXME: stub
}

void
SoDebug::printName(SoBase * base)
{
  // FIXME: stub
}

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
