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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SbString SbString.h Inventor/SbString.h
  \brief The SbString class is a string class with convenience functions for string operations.

  \ingroup base

  FIXME: write doc.

  \sa SbName
*/

#include <Inventor/SbString.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>
#include <string.h>

/*!
  This is the default constructor.  It initializes the string to be empty.
*/

SbString::SbString(void)
{
  this->sstring = this->staticStorage; 
  this->sstring[0] = '\0';
} // SbString()

/*!
  This is the copy constructor.
*/

SbString::SbString(const SbString & str)
{ 
  this->sstring = this->staticStorage; 
  *this = str.sstring; 
}

/*!
  This constructor assigns from the given string.
*/

SbString::SbString(const char * str)
{
  this->sstring = this->staticStorage; 
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
			      "start index (%d) is out of bounds [0,%d>. "
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
			      "end index (%d) is out of bounds [0,%d>. "
			      "Clamped to %d.",end,len,len-1);
    end=len;
  }
  if (start>end+1) {
    SoDebugError::postWarning("SbString::SbString",
			      "start index (%d) is greater than end index "
			      "(%d). Empty string created.",start,end);
    start=0;
    end=-1;
  }
#endif // COIN_DEBUG

  int size=end-start+1;
  
  if (size<SB_STRING_STATIC_STORAGE_SIZE)
    this->sstring=this->staticStorage;
  else {
    this->sstring=new char[size+1];
    this->storageSize=size+1;
  }
  
  strncpy(this->sstring,str+start,size);
  this->sstring[size]='\0';
}

/*!
  Constructs a string from the given integer (e.g. SbString(42) creates
  the string "42"), and replaces the contents of the string with the ASCII
  representation of the number.
*/
void
SbString::intToString(const int digitString)
{
  // Find storage size we need for the supplied integer value.
  int w = 1;
  if (digitString < 0) w++;
  int tmp = digitString;
  while (tmp >= 10) {
    tmp /= 10;
    w++;
  }

  this->expand(w);

  tmp = digitString;
  if (tmp < 0) {
    tmp = -tmp;
    *this += '-';
    w--;
  }

  int f = 1;
  for (int i=0; i < w-1; i++) f *= 10;
  while (w) {
    int s = tmp / f;
    tmp = tmp % f;
    f /= 10;
    w--;

    *this += (char)s + 0x30;
  }
}

/*!
  This method returns a new string which contains a substring defined by the
  given indices \a startChar and \a endChar (inclusive).
  If \a endChar is -1, the substring from \a startChar to the end of the
  string is used.

  This will return a string which is (\a endChar - \a startChar + 1) characters
  long, i.e. if this string is "foo/bar" and we call by
  SbString::getSubString(0, 3), the returned string will be "foo/".
*/

SbString
SbString::getSubString(int startChar, int endChar) const
{
  if (endChar == -1) endChar = this->getLength()-1;

#if COIN_DEBUG
  if (endChar < 0 || endChar >= this->getLength()) {
    SoDebugError::postWarning("SbString::getSubString",
			      "endChar index %d out of bounds.",
			      endChar);
    return SbString("");
  }
  if (startChar < 0 || startChar >= this->getLength()) {
    SoDebugError::postWarning("SbString::getSubString",
			      "startChar index %d out of bounds.",
			      startChar);
    return SbString("");
  }
  if (startChar > endChar) {
    SoDebugError::postWarning("SbString::getSubString",
			      "startChar idx %d larger than endChar idx %d.",
			      startChar, endChar);
    return SbString("");
  }
#endif // COIN_DEBUG
  return SbString(this->sstring, startChar, endChar);
}

/*!
  This method deletes the substring defined by \a startChar and \a endChar
  (inclusive).  If \a endChar is -1, the substring from \a startChar to the
  end of the string is deleted.
*/

void
SbString::deleteSubString(int startChar, int endChar)
{
#if COIN_DEBUG
  int len=this->getLength();
  if (startChar<0) {
    SoDebugError::postWarning("SbString::deleteSubString",
			      "startChar index (%d) should be >= 0. "
			      "Clamped to 0.",startChar);
    startChar=0;
  }
  else if (startChar>=len) {
    SoDebugError::postWarning("SbString::deleteSubString",
			      "startChar index (%d) is out of bounds [0,%d>. "
			      "Clamped to %d.",startChar,len,len-1);
    startChar=len-1;
  }
  if (endChar<-1) {
    SoDebugError::postWarning("SbString::deleteSubString",
			      "endChar index should be >=-1. Clamping to -1.");
    endChar=-1;
  }
  else if (endChar>=len) {
    SoDebugError::postWarning("SbString::deleteSubString",
			      "endChar index (%d) is out of bounds [0,%d>. "
			      "Clamped to %d.",endChar,len,len-1);
    endChar=len-1;
  }
  if (startChar>endChar) {
    SoDebugError::postWarning("SbString::deleteSubString",
			      "start index (%d) is greater than end index "
			      "(%d). Nothing deleted.",startChar,endChar);
    return;
  }
#endif // COIN_DEBUG

  if (endChar==-1) endChar=this->getLength();

  memmove(this->sstring+startChar,
	  this->sstring+endChar+1,
	  strlen(this->sstring)-endChar);
}

/*!
  The destructor.
*/

SbString::~SbString()
{
  if (this->sstring != this->staticStorage)
    delete[] this->sstring;
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
SbString::getLength() const
{ 
  return strlen(this->sstring); 
}

/*!
  This method returns the pointer to the string (character array).
*/

const char * 
SbString::getString() const
{ 
  return this->sstring; 
}

/*!
  This method clears the string, making it an empty string ("").
  If \a freeOld is \e TRUE (which is the default), the memory used by the
  old string is freed.  Otherwise, memory will be kept and reused when the
  string is manipulated later.
*/

void
SbString::makeEmpty(SbBool freeOld)
{
  if (this->sstring != this->staticStorage) {
    if (freeOld) delete[] this->sstring;
    this->sstring = this->staticStorage;
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
  static char dummystring[1] = {0};
  if (str == NULL) str = dummystring; // handle NULL pointers

  int size = strlen(str) + 1;
  if (str >= this->sstring && str < this->sstring + 
      (this->sstring != this->staticStorage ? 
       this->storageSize : SB_STRING_STATIC_STORAGE_SIZE)) {
    
    this->deleteSubString(0,str-this->sstring);
    return *this;
  }
  
  if (size < SB_STRING_STATIC_STORAGE_SIZE) {
    if (this->sstring != this->staticStorage)
      this->makeEmpty();
  }
  
  else if (this->sstring == this->staticStorage)
    this->sstring = new char[size];
  
  else if (size > this->storageSize) {
    delete[] this->sstring;
    this->sstring = new char[size];
  }
  
  strcpy(this->sstring, str);
  this->storageSize = size;
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
    strcat(this->sstring, str);
  }
  return *this;
}

/*!
  Concatenate the given string to the end of the current one.
*/

SbString &
SbString::operator +=(const SbString & str)
{
  (*this)+=str.getString();
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
  This unary operator results in \e TRUE if the current string is empty ("")
  or \e FALSE otherwise.
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
  return s && str.sstring[0] == s[0] && ! strcmp(str.sstring, s);
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
  if (!s) return TRUE;
  return (str.sstring[0] != s[0] || strcmp(str.sstring, s));
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

/*!
  FIXME: write doc.
*/

void
SbString::expand(int bySize)
{
  int newSize = strlen(this->sstring) + bySize + 1;
  
  if (newSize >= SB_STRING_STATIC_STORAGE_SIZE &&
      (this->sstring == this->staticStorage || newSize > this->storageSize)) {
    
    char * newString = new char[newSize];
    
    strcpy(newString, this->sstring);
    
    if (this->sstring != this->staticStorage)
      delete[] this->sstring;
    
    this->sstring = newString;
    this->storageSize = newSize;
  }
}

/*!
  Dump the state of this object to the \a file stream.  Only works in
  debug version of library, method does nothing in an optimized compile.
*/

void
SbString::print(FILE * file) const
{
#if COIN_DEBUG
  fprintf(file, "'%s'\n", this->getString());
#endif // COIN_DEBUG
} // print()

