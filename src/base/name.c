/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/C/base/name.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

/* ********************************************************************** */

cc_name *
cc_name_struct_malloc(void)
{
  cc_name * name_struct;
  name_struct = (cc_name *) malloc(sizeof(cc_name));
  assert(name_struct != NULL);
  return name_struct;  
} /* cc_name_struct_malloc() */

void
cc_name_struct_init(cc_name * name_struct)
{
  assert(name_struct != NULL);
} /* cc_name_struct_init() */

void
cc_name_struct_clean(cc_name * name_struct)
{
  assert(name_struct != NULL);
} /* cc_name_struct_clean() */

void
cc_name_struct_free(cc_name * name_struct)
{
  if ( name_struct ) free(name_struct);
} /* cc_name_struct_free() */

/* ********************************************************************** */

#if 0

class COIN_DLL_API SbName {
public:
  SbName(void);
  SbName(const char * nameString);
  SbName(const SbString & str);
  SbName(const SbName & name);
  ~SbName(void);

  const char * getString(void) const;
  int getLength(void) const;
  static SbBool isIdentStartChar(const char c);
  static SbBool isIdentChar(const char c);
  static SbBool isBaseNameStartChar(const char c);
  static SbBool isBaseNameChar(const char c);

  int operator ! (void) const;
  friend COIN_DLL_API int operator == (const SbName & lhs, const char * rhs);
  friend COIN_DLL_API int operator == (const char * lhs, const SbName & rhs);
  friend COIN_DLL_API int operator == (const SbName & lhs, const SbName & rhs);
  friend COIN_DLL_API int operator != (const SbName & lhs, const char * rhs);
  friend COIN_DLL_API int operator != (const char * lhs, const SbName & rhs);
  friend COIN_DLL_API int operator != (const SbName & lhs, const SbName & rhs);

  operator const char * (void) const;

private:
  const class SbNameEntry * entry;
};
COIN_DLL_API int operator == (const SbName & lhs, const char * rhs);
COIN_DLL_API int operator == (const char * lhs, const SbName & rhs);
COIN_DLL_API int operator == (const SbName & lhs, const SbName & rhs);
COIN_DLL_API int operator != (const SbName & lhs, const char * rhs);
COIN_DLL_API int operator != (const char * lhs, const SbName & rhs);
COIN_DLL_API int operator != (const SbName & lhs, const SbName & rhs);

static const int CHUNK_SIZE = 65536-32; // leave some room for other data
static const int NAME_TABLE_SIZE = 1999;

struct SbNameChunk {
  char mem[CHUNK_SIZE];
  char * curByte;
  int bytesLeft;
  SbNameChunk * next;
};

class SbNameEntry {
public:
  SbBool isEmpty(void) const
    { return (! *str); }
  SbBool isEqual(const char * const string) const
    { return (strcmp(str, string) == 0); }

  static void print_info();

  static void initClass(void);
  SbNameEntry(const char * const s, const unsigned long h,
               SbNameEntry * const n)
    { str = s; hashValue = h; next = n; };
  static const SbNameEntry * insert(const char * const s);

  static const char * findStringAddress(const char * s);

  const char * str;

private:
  static int nameTableSize;
  static SbNameEntry ** nameTable;
  static struct SbNameChunk * chunk;
  unsigned long hashValue;
  SbNameEntry * next;
};

int SbNameEntry::nameTableSize;
SbNameEntry * * SbNameEntry::nameTable;
SbNameChunk * SbNameEntry::chunk;

// This static method initializes static data for the SbNameEntry
// class.
void
SbNameEntry::initClass(void)
{
  SbNameEntry::nameTableSize = NAME_TABLE_SIZE;
  SbNameEntry::nameTable = new SbNameEntry * [ SbNameEntry::nameTableSize ];
  for (int i = 0; i < SbNameEntry::nameTableSize; i++) { SbNameEntry::nameTable[i] = NULL; }
  SbNameEntry::chunk = NULL;
}

void
SbNameEntry::print_info(void)
{
  for (int i = 0; i < SbNameEntry::nameTableSize; i++) {
    SbNameEntry * entry = SbNameEntry::nameTable[ i ];
    int cnt = 0;
    while (entry != NULL) {
      entry = entry->next;
      cnt++;
    }
    printf("name entry: %d, cnt: %d\n", i, cnt);
  }
}

const char *
SbNameEntry::findStringAddress(const char * s)
{
  int len = strlen(s) + 1;

    // names > CHUNK_SIZE characters are truncated.
  if (len >= CHUNK_SIZE) { len = CHUNK_SIZE; }

  if (chunk == NULL || chunk->bytesLeft < len) {
    SbNameChunk * newChunk = new SbNameChunk;

    newChunk->curByte = newChunk->mem;
    newChunk->bytesLeft = CHUNK_SIZE;
    newChunk->next = chunk;

    chunk = newChunk;
  }

  (void)strncpy(chunk->curByte, s, len);
  s = chunk->curByte;

  chunk->curByte += len;
  chunk->bytesLeft -= len;

  return s;
}

const SbNameEntry *
SbNameEntry::insert(const char * const str)
{
  if (nameTableSize == 0) { initClass(); }

  unsigned long h = SbString::hash(str);
  unsigned long i = h % nameTableSize;
  SbNameEntry * entry = nameTable[i];
  SbNameEntry * head = entry;

  while (entry != NULL) {
    if (entry->hashValue == h && entry->isEqual( str) )
      break;
    entry = entry->next;
  }

  if (entry == NULL) {
    entry = new SbNameEntry(findStringAddress( str), h, head );
    nameTable[ i ] = entry;
  }

  return entry;
}


// *************************************************************************

/*!
  This is the default constructor.
*/

SbName::SbName(void)
{
  this->entry = SbNameEntry::insert("");
}

/*!
  Constructor. Adds the \a nameString string to the name table.
*/

SbName::SbName(const char * nameString)
{
  this->entry = SbNameEntry::insert(nameString);
}

/*!
  Constructor. Adds \a str to the name table.
*/

SbName::SbName(const SbString & str)
{
  this->entry = SbNameEntry::insert(str.getString());
}

/*!
  Copy constructor.
*/

SbName::SbName(const SbName & name)
  : entry(name.entry)
{
}

/*!
  The destructor.
*/

SbName::~SbName()
{
  // FIXME: no unref?  investigate if SbName can be used for
  // user-settable names.  (in which case running servers should get
  // swamped without ref counts) 19990611 larsa
}

/*!
  This method returns pointer to character array for the name.
*/

const char *
SbName::getString(void) const
{
  return this->entry->str;
}

/*!
  This method returns the number of characters in the name.
*/

int
SbName::getLength(void) const
{
  // FIXME: shouldn't we cache this value for subsequent faster
  // execution? 20010909 mortene.
  return strlen(this->entry->str);
}

/*!
  This method checks if the \a c character is a valid identifier start
  character for a name.

  \sa SbBool SbName::isIdentChar(const char c)

*/
SbBool
SbName::isIdentStartChar(const char c)
{
  if (isdigit(c) ) return FALSE;
  return SbName::isIdentChar(c);
}

/*!
  This method checks if the \a c character is a valid character for a
  name.

  \sa SbBool SbName::isIdentStartChar(const char c)
*/
SbBool
SbName::isIdentChar(const char c)
{
  return (isalnum(c) || c == '_');
}

/*!
  Returns \c TRUE if the given character is valid for use as the first
  character of a name for an object derived from a class inheriting
  SoBase.

  SoBase derived objects needs to be named in a manner which will not
  clash with the special characters reserved as tokens in the syntax
  rules of Open Inventor and VRML files.

  This method is not part of the original Open Inventor API.

  \sa isBaseNameChar()
*/
SbBool
SbName::isBaseNameStartChar(const char c)
{
  if (c == '_' || (isascii(c) && isalpha(c))) return TRUE;
  return FALSE;
}

/*!
  Returns \c TRUE if the given character is valid for use in naming
  object instances of classes derived from SoBase.

  SoBase derived objects needs to be named in a manner which will not
  clash with the special characters reserved as tokens in the syntax
  rules of Open Inventor and VRML files.

  This method is not part of the original Open Inventor API.

  \sa isBaseNameStartChar()
*/
SbBool
SbName::isBaseNameChar(const char c)
{
  static const char invalid[] = "\"\'+.\\{}";
  if (c <= 0x20 || c >= 0x7f || strchr(invalid, c)) return FALSE;
  return TRUE;
}

/*!
  This unary operator results in \c FALSE if the SbName object is
  non-empty and \c TRUE if the SbName object is empty.  An empty name
  contains a null-length string.
*/

int
SbName::operator ! (void) const
{
  return this->entry->isEmpty();
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int
operator == (const SbName & lhs, const char *rhs)
{
  return lhs.entry->isEqual(rhs);
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int
operator == (const char *lhs, const SbName & rhs)
{
  return rhs.entry->isEqual(lhs);
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int
operator == (const SbName & lhs, const SbName & rhs)
{
  return (lhs.entry == rhs.entry);
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int
operator != (const SbName & lhs, const char *rhs)
{
  return ! lhs.entry->isEqual(rhs);
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int
operator != (const char *lhs, const SbName & rhs)
{
  return ! rhs.entry->isEqual(lhs);
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int
operator != (const SbName & lhs, const SbName & rhs)
{
  return lhs.entry != rhs.entry;
}

/*!
  This operator returns a pointer to the character array for the name string.
  It is intended for implicit use.  Use SbName::getString() explicitly instead
  of this operator - it might be removed later.

  \sa const char * SbName::getString(void)
*/

SbName::operator const char * (void) const
{
  return this->entry->str;
}

#endif
