/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SbString SbString.h Inventor/SbString.h
  \brief The SbString class is a string class with convenience functions for string operations.
  \ingroup base

  This is the class used for storing and working with character
  strings. It automatically takes care of supporting all the
  "bookkeeping" tasks usually associated with working with character
  strings, like memory allocation and deallocation etc.

  This class should also be well suited for use by the application
  programmer throughout the application using the Coin library.

  \sa SbName
*/

#include <Inventor/SbString.h>

// FIXME: translate the find() and findAll() methods to C (will first
// need a growable array in C to handle the second argument to
// findAll()). 20020522 mortene.

// Helper function for find() and findAll().
static void
compute_prefix_function(SbIntList & pi, const SbString & str)
{
  int len = str.getLength();
  pi.append(0);
  int k = 0;

  for (int q = 1; q < len; q++){
    while(k > 0 && (str.operator[](k) != str.operator[](q)))
      k = pi.get(k);
    if (str.operator[](k) == str.operator[](q))
      k++;
    pi.append(k);
  }
}

/*!
  If \a s is found, the method returns the first index where \a s
  starts. Otherwise it returns -1.

  Note: SbString::find() is a Coin specific extension to the original
  Open Inventor API.

  \sa SbString::findAll()
  \since 2002-02-19
*/
int
SbString::find(const SbString & str) const
{
  int lenthis = this->getLength();
  int lenstr = str.getLength();

  if (!lenthis) return -1;
  if (lenstr > lenthis) return -1;

  SbIntList pi;
  compute_prefix_function(pi, str);
  int q = 0;

  for (int i = 0; i < lenthis; i ++){
    while (q > 0 && (str.operator[](q) != this->operator[](i)))
      q = pi.operator[](q - 1);
    if (str.operator[](q) == this->operator[](i))
      q++;
    if (q == lenstr){
      return (i - (lenstr - 1));
    }
  }
  return -1;
}

/*!
  All occurences of \a str is represented in \a found as indices to
  the characters where \a str starts. If 1 or more is found, \c TRUE
  is returned, else \c FALSE is returned.

  Note: SbString::findAll() is an extension to the original Open
  Inventor API.

  \sa SbString::find()
  \since 2002-02-19
*/

SbBool
SbString::findAll(const SbString & str, SbIntList & found) const
{
  // The KMP string matching algorithm is used for this method
  int lenthis = this->getLength();
  int lenstr = str.getLength();
  found.truncate(0);

  if (!lenthis) return FALSE;
  if (lenstr > lenthis) return FALSE;

  SbIntList pi;
  compute_prefix_function(pi, str);
  int q = 0;

  for (int i = 0; i < lenthis; i ++){
    while (q > 0 && (str.operator[](q) != this->operator[](i)))
      q = pi.operator[](q - 1);
    if (str.operator[](q) == this->operator[](i))
      q++;
    if (q == lenstr){
      found.append(i - (lenstr - 1));
      q = pi.operator[](q - 1);
    }
  }
  if (!found.getLength()) return FALSE;
  return TRUE;
}



/*!
  \fn SbString::SbString(void)
  This is the default constructor.  It initializes the string to be empty.
*/

/*!
  \fn SbString::SbString(const SbString & str)
  This is the copy constructor.
*/

/*!
  \fn SbString::SbString(const char * str)
  This constructor assigns from the given string.
*/

/*!
  \fn SbString::SbString(const char * str, int start, int end)

  This constructor constructs a string from the given substring from
  \a start to \a end indices inclusive. If \a end is -1, the substring
  from \a start until the end of the string is used.
*/


/*!
  \fn SbString::SbString(const int digits)

  Construct an SbString instance containing the \a digits of the
  integer argument.
 */

/*!
  \fn void SbString::addIntString(const int value)

  Constructs a string from the given integer (e.g. intToString(42)
  creates the string "42"), and adds this to the contents of the
  string.
*/

/*!
  \fn SbString SbString::getSubString(int startidx, int endidx) const

  This method returns a new string which contains a substring defined by the
  given indices \a startidx and \a endChar (inclusive).
  If \a endChar is -1, the substring from \a startidx to the end of the
  string is used.

  This will return a string which is (\a endChar - \a startidx + 1) characters
  long, i.e. if this string is "foo/bar" and we call by
  SbString::getSubString(0, 3), the returned string will be "foo/".
*/

/*!
  \fn void SbString::deleteSubString(int startidx, int endidx)

  This method deletes the substring defined by \a startidx and \a endidx
  (inclusive).  If \a endidx is -1, the substring from \a startidx to the
  end of the string is deleted.
*/

/*!
  \fn SbString::~SbString()

  The destructor. Deallocates any internal resources used during the
  lifetime of the SbString instance.
*/

/*!
  \fn uint32_t SbString::hash(void)

  This method returns a reasonable hash value for the current string.

  \sa uint32_t SbString::hash(const char * s)
*/

/*!
  \fn uint32_t SbString::hash(const char * s)

  This static method returns a hash value for the given string.
*/

/*!
  \fn int SbString::getLength(void) const

  This method returns the length of the string.
*/

/*!
  \fn const char * SbString::getString(void) const

  This method returns the pointer to the string (character array).
*/

/*!
  \fn void SbString::makeEmpty(SbBool freeold)

  This method clears the string, making it an empty string ("").
  If \a freeold is \c TRUE (which is the default), the memory used by
  the old string is freed.  Otherwise, memory will be kept and reused
  when the string is manipulated later.
*/

/*!
  \fn char SbString::operator[](int index) const

  Returns character at position \a index in the string.

  \sa getSubString()
*/

/*!
  \fn SbString & SbString::operator=(const char * str)

  This is the assignment operator.
*/

/*!
  \fn SbString & SbString::operator=(const SbString & str)

  Assign from the given string.
*/

/*!
  \fn SbString & SbString::operator+=(const char * str)

  Concatenate the given string to the end of the current one.
*/

/*!
  \fn SbString & SbString::operator+=(const SbString & str)

  Concatenate the given string to the end of the current one.
*/

/*!
  \fn SbString & SbString::operator+=(const char c)

  Concatenate the given character to the end of the current string.

  \note This member function is not compatible with OpenInventor.
*/

/*!
  \fn int SbString::operator!(void) const

  This unary operator results in \c TRUE if the current string is empty ("")
  or \c FALSE otherwise.
*/

/*!
  \fn int operator==(const SbString & str, const char * s)

  Equality operator. Check if the strings have the same contents.
*/

/*!
  \fn int operator==(const char * s, const SbString & str)

  Equality operator. Check if the strings have the same contents.
*/

/*!
  \fn int operator==(const SbString & str1, const SbString & str2)

  Equality operator. Check if the strings have the same contents.
*/

/*!
  \fn int operator!=(const SbString & str, const char * s)

  Inequality operator.
*/

/*!
  \fn int operator!=(const char * s, const SbString & str)

  Inequality operator.
*/

/*!
  \fn int operator!=(const SbString & str1, const SbString & str2)

  Inequality operator.
*/

/*!
  \fn SbString & SbString::sprintf(const char * formatstr, ...)

  Set SbString instance to the formatted string \a formatstr,
  replacing the current contents.  The control characters within \a
  formatstr and the remaining arguments should follow the conventions
  of the printf() call.

  Note that this function is not part of the original Open Inventor
  API.
*/

/*!
  \fn SbString & SbString::vsprintf(const char * formatstr, va_list args)

  Set SbString instance to the formatted string \a formatstr,
  replacing the current contents.  The control characters within \a
  formatstr and the arguments of the \a args argument list should
  follow the conventions of the printf() call.

  Note that this function is not part of the original Open Inventor
  API.
*/
