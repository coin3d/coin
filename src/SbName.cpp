/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SbName SbName.h Inventor/SbName.h
  \brief The SbName class stores strings by reference in a hash table.

  The class is used by inventor for keywords and other unique names that are
  used all over the place and would waste tons of memory if they were
  duplicated for each reference.  Because strings are uniquely identified
  by their reference, string comparisons for SbName objects are very
  efficient.
*/

/*!
  \class SbNameEntry SbName.h Inventor/SbName.h
  \brief The SbNameEntry class is an internal class used by SbName.

  FIXME: write doc.
*/

#include <Inventor/SbName.h>

#include <Inventor/SbString.h>

#include <stdio.h>
#include <ctype.h>

/*!
  This is the default constructor.
*/

SbName::SbName(void)
{
  entry = SbNameEntry::insert("");
}

/*!
  Constructor. Adds the \a nameString string to the name table.
*/

SbName::SbName(const char *nameString)
{ 
  this->entry = SbNameEntry::insert( nameString ); 
};

/*!
  Constructor. Adds \a str to the name table.
*/

SbName::SbName(const SbString & str)
{
  this->entry = SbNameEntry::insert( str.getString() );
}

/*!
  Copy constructor.
*/

SbName::SbName( const SbName & name )
: entry( name.entry ) 
{ 
}

/*!
  The destructor.
*/

SbName::~SbName( void )
{
/*¡
  no unref?  investigate if SbName can be used for user-settable names.
  (in which case running servers sould get swamped without ref counts)
  990611 larsa
*/
}

/*!
  This method returns pointer to character array for the name.
*/

const char * 
SbName::getString( void ) const
{ 
  return entry->str; 
}

/*!
  This method returns the number of characters in the name.
*/

int 
SbName::getLength(void) const
{ 
  return strlen( entry->str ); 
}

/*!
  This method checks if the \a c character is a valid identifier start
  character for a name.

  \sa SbBool SbName::isIdentChar( const char c )

*/

SbBool
SbName::isIdentStartChar( const char c )
{
    if ( isdigit( c ) ) return FALSE;
    return isIdentChar( c );
}

/*!
  This method checks if the \a c character is a valid character for a
  name.

  \sa SbBool SbName::isIdentStartChar( const char c )
*/

SbBool
SbName::isIdentChar( const char c )
{
    if ( isalnum( c ) || c == '_' ) return TRUE;
    return FALSE;
}

/*!
  Returns \a TRUE if the given character is valid for use as the first
  character of a name for an object derived from a class inheriting
  SoBase.

  SoBase-derived objects needs to be named in a manner which will not
  clash with the special characters reserved as special tokens in the
  syntax rules of Open Inventor and VRML files.

  This method is not part of the Open Inventor API.

  \sa SbBool SbName::isBaseNameChar(const char c)
*/

SbBool
SbName::isBaseNameStartChar( const char c )
{
  if (c == '_' || (isascii(c) && isalpha(c))) return TRUE;
  return FALSE;
}

/*!
  Returns \a TRUE if the given character is valid for use in naming
  object instances of classes derived from SoBase.

  SoBase derived objects needs to be named in a manner which will not
  clash with the special characters reserved as special tokens in the
  syntax rules of Open Inventor and VRML files.

  This method is not part of the Open Inventor API.

  \sa SbBool SbName::isBaseNameStartChar(const char c)
*/

SbBool
SbName::isBaseNameChar( const char c )
{
  const char invalid[] = "\"\'+.\\{}";
  if (c <= 0x20 || c >= 0x7f || strchr(invalid, c)) return FALSE;
  return TRUE;
}

/*!
  This unary operator results in FALSE if the SbName object is non-empty and
  TRUE if the SbName object is empty.  An empty name contains the "" string.
*/

int 
SbName::operator ! (void) const
{ 
  return entry->isEmpty(); 
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int 
operator == ( const SbName & lhs, const char *rhs )
{ 
  return lhs.entry->isEqual( rhs ); 
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int 
operator == ( const char *lhs, const SbName & rhs )
{ 
  return rhs.entry->isEqual( lhs ); 
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int 
operator == ( const SbName & lhs, const SbName & rhs )
{ 
  return (lhs.entry == rhs.entry); 
}
  
/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int
operator != ( const SbName & lhs, const char *rhs )
{ 
  return ! lhs.entry->isEqual( rhs ); 
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int 
operator != ( const char *lhs, const SbName & rhs )
{ 
  return ! rhs.entry->isEqual( lhs ); 
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int 
operator != ( const SbName & lhs, const SbName & rhs )
{ 
  return lhs.entry != rhs.entry; 
}

/*!
  This operator returns a pointer to the character array for the name string.
  It is intended for implicit use.  Use SbName::getString() explicitly instead
  of this operator - it might be removed later.

  \sa const char * SbName::getString(void)
*/

SbName::operator const char * ( void ) const
{ 
  return entry->str; 
}

const int CHUNK_SIZE = 65536-32; // leave some room for other data
const int NAME_TABLE_SIZE = 1999;

struct SbNameChunk {
  // FIXME: CHUNK_SIZE places a stupid limitation on something which
  // should be allowed to grow dynamically unbounded.
  // 19990608 mortene.
  // DONTFIXME: Won't new chunks be created in a dynamic unbounded way?
    char mem[ CHUNK_SIZE ];
    char * curByte;
    int bytesLeft;
    SbNameChunk * next;
}; // struct SbNameChunk

int SbNameEntry::nameTableSize;
SbNameEntry * * SbNameEntry::nameTable;
SbNameChunk * SbNameEntry::chunk; 

// *************************************************************************

/*!
  This static method initializes static data for the SbNameEntry class.
*/

void
SbNameEntry::initClass( void )
{
    nameTableSize = NAME_TABLE_SIZE;
    nameTable = new SbNameEntry * [ nameTableSize ];
    for ( int i = 0; i < nameTableSize; i++ )
        nameTable[i] = NULL;
    chunk = NULL;
}

/*!
  FIXME: write doc
*/

void
SbNameEntry::print_info(
    void )
{
    for ( int i = 0; i < nameTableSize; i++) {
        SbNameEntry * entry = nameTable[ i ];
        int cnt = 0;
        while ( entry != NULL ) {
            entry = entry->next;
            cnt++;
        }
        printf("name entry: %d, cnt: %d\n", i, cnt);
    }
}

/*!
  FIXME: write doc.
*/

const char *
SbNameEntry::findStringAddress(
    const char * s )
{
    int len = strlen( s ) + 1;

    // names > CHUNK_SIZE characters are truncated.
    if ( len >= CHUNK_SIZE )
      len=CHUNK_SIZE;

    if ( chunk == NULL || chunk->bytesLeft < len ) {
      SbNameChunk * newChunk = new SbNameChunk;
      
      newChunk->curByte = newChunk->mem;
      newChunk->bytesLeft = CHUNK_SIZE;
      newChunk->next = chunk;
      
      chunk = newChunk;
    }
    
    strncpy(chunk->curByte, s, len);
    s = chunk->curByte;
    
    chunk->curByte += len;
    chunk->bytesLeft -= len;

    return s;
}

/*!
  FIXME: write doc.
*/

const SbNameEntry *
SbNameEntry::insert(
    const char * const str )
{
    if ( nameTableSize == 0 )
        initClass();
  
    unsigned long h = SbString::hash( str );
    unsigned long i = h % nameTableSize;
    SbNameEntry * entry = nameTable[i];
    SbNameEntry * head = entry;

    while ( entry != NULL ) {
        if ( entry->hashValue == h && entry->isEqual( str ) )
            break;
        entry = entry->next;
    }
  
    if ( entry == NULL ) {
        entry = new SbNameEntry( findStringAddress( str ), h, head );
        nameTable[ i ] = entry;
    }

    return entry;
}

/*!
  \fn SbBool SbNameEntry::isEmpty(void) const

  FIXME: write doc.
*/

/*!
  \fn SbBool SbNameEntry::isEqual( const char * const string ) const

  FIXME: write doc.
*/

