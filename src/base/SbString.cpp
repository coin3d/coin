/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SbString SbString.h Inventor/SbString.h
  \brief The SbString class is a string class with convenience functions for string operations.
  \ingroup base

  Internally for the Coin library, this is the class used for storing
  and working with character strings. It automatically takes care of
  supporting all the "bookkeeping" tasks usually associated with
  working with character strings, like memory allocation and
  deallocation etc.

  This class should also be well suited for use by the application
  programmer throughout the application using the Coin library.

  \sa SbName
*/

#include <Inventor/SbString.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "../tidbits.h" // coin_vsnprintf()
#include <assert.h>
#include <string.h>


/*!
  This is the default constructor.  It initializes the string to be empty.
*/

SbString::SbString(void)
{
  this->sstring = this->staticstorage;
  this->storagesize = SB_STRING_STATIC_STORAGE_SIZE;
  this->sstring[0] = '\0';
}

/*!
  This is the copy constructor.
*/

SbString::SbString(const SbString & str)
{
  this->sstring = this->staticstorage;
  this->storagesize = SB_STRING_STATIC_STORAGE_SIZE;
  *this = str.sstring;
}

/*!
  This constructor assigns from the given string.
*/

SbString::SbString(const char * str)
{
  this->sstring = this->staticstorage;
  this->storagesize = SB_STRING_STATIC_STORAGE_SIZE;
  *this = str;
}

/*!
  This constructor constructs a string from the given substring from
  \a start to \a end indices inclusive. If \a end is -1, the substring
  from \a start until the end of the string is used.
*/

SbString::SbString(const char * str, int start, int end)
{
#if COIN_DEBUG
  int len=strlen(str);
  if (start<0) {
    SoDebugError::postWarning("SbString::SbString",
                              "start index (%d) should be >= 0. Clamped to 0.",
                              start);
    start=0;
  }
  else if (start>len) {
    SoDebugError::postWarning("SbString::SbString",
                              "start index (%d) is out of bounds [0, %d>. "
                              "Clamped to %d.", start, len, len-1);
    start=len;
  }
  if (end<0) {
    SoDebugError::postWarning("SbString::SbString",
                              "end index (%d) should be >= 0. Clamped to 0.",
                              end);
    end=0;
  }
  else if (end>len) {
    SoDebugError::postWarning("SbString::SbString",
                              "end index (%d) is out of bounds [0, %d>. "
                              "Clamped to %d.", end, len, len-1);
    end=len;
  }
  if (start>end+1) {
    SoDebugError::postWarning("SbString::SbString",
                              "start index (%d) is greater than end index "
                              "(%d). Empty string created.", start, end);
    start=0;
    end=-1;
  }
#endif // COIN_DEBUG

  int size = end - start + 1;

  if (size < SB_STRING_STATIC_STORAGE_SIZE) {
    this->sstring = this->staticstorage;
    this->storagesize = SB_STRING_STATIC_STORAGE_SIZE;
  }
  else {
    this->sstring = new char[size+1];
    this->storagesize = size+1;
  }

  (void)strncpy(this->sstring, str + start, size);
  this->sstring[size]='\0';
}

/*!
  Construct an SbString instance containing the \a digits of the
  integer argument.
 */
SbString::SbString(const int digits)
{
  this->sstring = this->staticstorage;
  this->storagesize = SB_STRING_STATIC_STORAGE_SIZE;
  this->sstring[0] = '\0';

  this->addIntString(digits);
}

/*!
  Constructs a string from the given integer (e.g. intToString(42)
  creates the string "42"), and adds this to the contents of the
  string.
*/
void
SbString::addIntString(const int value)
{
  SbString s;
  (void)s.sprintf("%d", value);
  *this += s;
}

/*!
  This method returns a new string which contains a substring defined by the
  given indices \a startidx and \a endChar (inclusive).
  If \a endChar is -1, the substring from \a startidx to the end of the
  string is used.

  This will return a string which is (\a endChar - \a startidx + 1) characters
  long, i.e. if this string is "foo/bar" and we call by
  SbString::getSubString(0, 3), the returned string will be "foo/".
*/

SbString
SbString::getSubString(int startidx, int endidx) const
{
  if (endidx == -1) endidx = this->getLength()-1;

#if COIN_DEBUG
  if (endidx < 0 || endidx >= this->getLength()) {
    SoDebugError::postWarning("SbString::getSubString",
                              "endidx index %d out of bounds.",
                              endidx);
    return SbString("");
  }
  if (startidx < 0 || startidx >= this->getLength()) {
    SoDebugError::postWarning("SbString::getSubString",
                              "startidx index %d out of bounds.",
                              startidx);
    return SbString("");
  }
  if (startidx > endidx) {
    SoDebugError::postWarning("SbString::getSubString",
                              "startidx idx %d larger than endidx idx %d.",
                              startidx, endidx);
    return SbString("");
  }
#endif // COIN_DEBUG
  return SbString(this->sstring, startidx, endidx);
}

/*!
  This method deletes the substring defined by \a startidx and \a endidx
  (inclusive).  If \a endidx is -1, the substring from \a startidx to the
  end of the string is deleted.
*/

void
SbString::deleteSubString(int startidx, int endidx)
{
  int len = this->getLength();
  if (endidx == -1) endidx = len - 1;

#if COIN_DEBUG
  if (startidx < 0 || startidx >= len || endidx < 0 || endidx >= len ||
      startidx > endidx) {
    SoDebugError::postWarning("SbString::deleteSubString",
                              "invalid arguments [%d, %d] for string ``%s''",
                              startidx, endidx, this->sstring);
    return;
  }
#endif // COIN_DEBUG

  (void)memmove(this->sstring + startidx, this->sstring + endidx + 1,
                strlen(this->sstring) - endidx);
}

/*!
  The destructor.
*/

SbString::~SbString()
{
  if (this->sstring != this->staticstorage) delete[] this->sstring;
}

/*!
  This method returns a reasonable hash value for the current string.

  \sa uint32_t SbString::hash(const char * s)
*/

uint32_t
SbString::hash(void)
{
  return SbString::hash(this->sstring);
}

/*!
  This static method returns a hash value for the given string.
*/

uint32_t
SbString::hash(const char * s)
{
  uint32_t total, shift;

  total = shift = 0;
  while (*s) {
    total = total ^ ((*s) << shift);
    shift+=5;
    if (shift>24) shift -= 24;
    s++;
  }
  return total;
}

/*!
  This method returns the length of the string.
*/
int
SbString::getLength(void) const
{
  // FIXME: jeez.. shouldn't this be cached? 20011123 mortene.
  return strlen(this->sstring);
}

/*!
  This method returns the pointer to the string (character array).
*/

const char *
SbString::getString(void) const
{
  return this->sstring;
}

/*!
  This method clears the string, making it an empty string ("").
  If \a freeold is \c TRUE (which is the default), the memory used by the
  old string is freed.  Otherwise, memory will be kept and reused when the
  string is manipulated later.
*/

void
SbString::makeEmpty(SbBool freeold)
{
  if (this->sstring != this->staticstorage) {
    if (freeold) delete[] this->sstring;
    this->sstring = this->staticstorage;
    this->storagesize = SB_STRING_STATIC_STORAGE_SIZE;
  }
  this->sstring[0] = '\0';
}

/*!
  Returns character at position \a index in the string.

  \sa getSubString()
*/
char
SbString::operator [](int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->getLength()) {
    SoDebugError::postWarning("SbString::operator[]",
                              "index %d out of bounds.", index);
    return 0;
  }
#endif // COIN_DEBUG
  return this->sstring[index];
}

/*!
  This is the assignment operator.
*/

SbString &
SbString::operator =(const char * str)
{
  static char dummystring[] = "";
  if (str == NULL) str = dummystring; // handle NULL pointers

  int size = strlen(str) + 1;

  // Detect if the str pointer is within our own string buffer.
  if (str >= this->sstring && str < (this->sstring + this->storagesize)) {
    this->deleteSubString(0, str - this->sstring);
    return *this;
  }

  if (size > this->storagesize) this->expand(size - strlen(this->sstring) - 1);
  (void)strcpy(this->sstring, str);
  return *this;
}

/*!
  Assign from the given string.
*/

SbString &
SbString::operator = (const SbString & str)
{
  return (*this = str.sstring);
}

/*!
  Concatenate the given string to the end of the current one.
*/

SbString &
SbString::operator +=(const char * str)
{
  if (str) {
    this->expand(strlen(str));
    (void)strcat(this->sstring, str);
  }
  return *this;
}

/*!
  Concatenate the given string to the end of the current one.
*/

SbString &
SbString::operator +=(const SbString & str)
{
  (*this) += str.getString();
  return *this;
}

/*!
  Concatenate the given character to the end of the current string.

  \note This member function is not compatible with OpenInventor.
*/

SbString &
SbString::operator +=(const char c)
{
  this->expand(1);
  int pos = strlen(this->sstring);
  this->sstring[pos] = c;
  this->sstring[pos+1] = '\0';
  return *this;
}

/*!
  This unary operator results in \c TRUE if the current string is empty ("")
  or \c FALSE otherwise.
*/

int
SbString::operator ! (void) const
{
  return (this->sstring[0] == '\0');
}

/*!
  Equality operator. Check if the strings have the same contents.
*/

int
operator ==(const SbString & str, const char * s)
{
  return s && (str.sstring[0] == s[0]) && (strcmp(str.sstring, s) == 0);
}

/*!
  Equality operator. Check if the strings have the same contents.
*/

int
operator == (const char * s, const SbString & str)
{
  return (str == s);
}

/*!
  Equality operator. Check if the strings have the same contents.
*/

int
operator == (const SbString & str1, const SbString & str2)
{
  return (str1 == str2.sstring);
}

/*!
  Inequality operator.
*/

int
operator !=(const SbString & str, const char * s)
{
  return ((str == s) ? FALSE : TRUE);
}

/*!
  Inequality operator.
*/

int
operator != (const char * s, const SbString & str)
{
  return (str != s);
}

/*!
  Inequality operator.
*/

int
operator != (const SbString & str1, const SbString & str2)
{
  return (str1 != str2.sstring);
}

// Set the internal buffer size to the length of the current string
// plus additional plus 1.
void
SbString::expand(int additional)
{
  int newsize = strlen(this->sstring) + additional + 1;

  if (newsize > this->storagesize) {
    char * newstring = new char[newsize];
    (void)strcpy(newstring, this->sstring);

    if (this->sstring != this->staticstorage) delete[] this->sstring;

    this->sstring = newstring;
    this->storagesize = newsize;
  }
}

/*!
  Set SbString instance to the formatted string \a formatstr,
  replacing the current contents.  The control characters within \a
  formatstr and the remaining arguments should follow the conventions
  of the printf() call.

  Note that this function is not part of the original Open Inventor
  API.
*/
SbString &
SbString::sprintf(const char * formatstr, ...)
{
  va_list args;
  va_start(args, formatstr);

  SbBool expand;
  do {
    int length = coin_vsnprintf(this->sstring, this->storagesize, formatstr, args);
    expand = (length == -1);
    if (expand) this->expand(1024); // increase linearly in 1Kb intervals
  } while (expand);

  va_end(args);
  return *this;
}

/*!
  Set SbString instance to the formatted string \a formatstr,
  replacing the current contents.  The control characters within \a
  formatstr and the arguments of the \a args argument list should
  follow the conventions of the printf() call.

  Note that this function is not part of the original Open Inventor
  API.
*/
SbString &
SbString::vsprintf(const char * formatstr, va_list args)
{
  SbBool expand;
  do {
    int length = coin_vsnprintf(this->sstring, this->storagesize, formatstr, args);
    expand = (length == -1);
    if (expand) this->expand(1024); // increase linearly in 1Kb intervals
  } while (expand);

  return *this;
}


/*!
  Dump the state of this object to the \a file stream.  Only works in
  debug version of library, method does nothing in an optimized compile.
*/

void
SbString::print(FILE * file) const
{
#if COIN_DEBUG
  (void)fprintf(file, "'%s'\n", this->getString());
#endif // COIN_DEBUG
}
