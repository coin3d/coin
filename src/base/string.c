/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/C/base/string.h>
#include "../tidbits.h" // snprintf() and vsnprintf() definitions.

#include <assert.h>
#include <string.h>
#include <stdlib.h>

/* ********************************************************************** */

void
cc_string_struct_init(cc_string * string_struct)
{
  string_struct->pointer = string_struct->buffer;
  string_struct->bufsize = CC_STRING_MIN_SIZE;
  string_struct->buffer[0] = '\0';
} /* cc_string_struct_init() */

void
cc_string_struct_clean(cc_string * string_struct)
{
  if ( string_struct->pointer != string_struct->buffer )
    free(string_struct->pointer);
} /* cc_string_struct_clean() */

/* ********************************************************************** */

void
cc_string_remove_substring(cc_string * me, int start, int end)
{
  int len;
  len = strlen(me->pointer);
  if ( end == -1 ) end = len - 1;

#if COIN_DEBUG
  if ( start < 0 || start >= len || end < 0 || end >= len || start > end ) {
    /*
    SoDebugError::postWarning("cc_string_remove_substring",
                              "invalid arguments [%d, %d] for string ``%s''",
                              startidx, endidx, this->sstring);
    */
    return;
  }
#endif // COIN_DEBUG
  (void) memmove(me->pointer + start, me->pointer + end + 1, len - end);
} /* cc_string_remove_substring() */

static
void
cc_string_expand_buffer(cc_string * me, int additional)
{
  int newsize;
  char * newbuf;
  newsize = strlen(me->pointer) + additional + 1;
  if ( newsize > me->bufsize ) {
    newbuf = (char *) malloc(newsize);
    assert(newbuf != NULL);
    (void) strcpy(newbuf, me->pointer);
    if ( me->pointer != me->buffer )
      free(me->pointer);
    me->pointer = newbuf;
    me->bufsize = newsize;
  }
} /* cc_string_expand_buffer() */

/* ********************************************************************** */

cc_string *
cc_string_construct(const char * text)
{
  cc_string * me;
  me = (cc_string *) malloc(sizeof(cc_string));
  assert(me != NULL);
  cc_string_struct_init(me);
  cc_string_set_text(me, text);
  return me;
} /* cc_string_construct() */

cc_string *
cc_string_construct_new(void)
{
  cc_string * me;
  me = (cc_string *) malloc(sizeof(cc_string));
  assert(me != NULL);
  cc_string_struct_init(me);
  return me;
} /* cc_string_construct_new() */

cc_string *
cc_string_construct_subtext(const char * text, int start, int end)
{
  cc_string * me;
  me = (cc_string *) malloc(sizeof(cc_string));
  assert(me != NULL);
  cc_string_struct_init(me);
  cc_string_set_subtext(me, text, start, end);
  return me;
} /* cc_string_construct_subtext() */

cc_string *
cc_string_clone(cc_string * string)
{
  cc_string * me;
  me = (cc_string *) malloc(sizeof(cc_string));
  assert(me != NULL);
  cc_string_struct_init(me);
  cc_string_set_text(me, string->pointer);
  return me;
} /* cc_string_clone() */

void
cc_string_destruct(cc_string * me)
{
  cc_string_struct_clean(me);
  free(me);
} /* cc_string_destruct() */

/* ********************************************************************** */

void
cc_string_set_text(cc_string * me, const char * text)
{
  static char emptystring[] = "";
  int size;
  if ( text == NULL ) text = emptystring;

  if ( text >= me->pointer && text < (me->pointer + me->bufsize) ) {
    /* text is within own buffer */
    cc_string_remove_substring(me, 0, text - me->pointer);
    return;
  }
  size = strlen(text) + 1;
  if ( size > me->bufsize )
    cc_string_expand_buffer(me, size - strlen(me->pointer) - 1);
  (void) strcpy(me->pointer, text);
} /* cc_string_set_text() */

void
cc_string_set_subtext(cc_string * me, const char * text, int start, int end)
{
  static char * emptystring = "";
  int len, size;
  if ( text == NULL ) text = emptystring;
  len = strlen(text);

#if COIN_DEBUG && 0 /* FIXME */
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

  /* if new substring fits in internal buffer, freeing allocated
     buffer will not happen - should this be changed? */
  size = end - start + 1;
  if ( size >= me->bufsize ) {
    if ( me->pointer != me->buffer )
      free(me->pointer);
    me->pointer = (char *) malloc(size + 1);
    me->bufsize = size + 1;
  }
  (void) strncpy(me->pointer, text + start, size);
  me->pointer[size] = '\0';
} /* cc_string_set_subtext() */

void
cc_string_set_integer(cc_string * me, int integer)
{
  me->pointer[0] = '\0';
  cc_string_append_integer(me, integer);
} /* cc_string_set_integer() */

void
cc_string_set_string(cc_string * me, const cc_string * string)
{
  cc_string_set_text(me, string->pointer);
} /* cc_string_set_string() */

/* ********************************************************************** */

void
cc_string_append_string(cc_string * me, const cc_string * string)
{
  cc_string_append_text(me, string->pointer);
} /* cc_string_append_string() */

void
cc_string_append_text(cc_string * me, const char * text)
{
  if ( text ) {
    cc_string_expand_buffer(me, strlen(text));
    (void) strcat(me->pointer, text);
  }
} /* cc_string_append_text() */

void
cc_string_append_integer(cc_string * me, const int digits)
{
  int i, f, s, w, len, tmp;

  /* FIXME: Cheesus, this is lame. Use log10 instead. 19991216 mortene. */
  w = 1;
  if ( digits < 0 ) w++; /* sign */
  tmp = digits;
  while ( tmp >= 10 ) {
    tmp /= 10;
    w++;
  }

  cc_string_expand_buffer(me, w);

  tmp = digits;
  len = strlen(me->pointer);
  if ( tmp < 0 ) {
    tmp = -tmp;
    me->pointer[len++] = '-';
    w--;
  }

  f = 1;
  for ( i = 0; i < w - 1; i++ ) f *= 10;
  while ( w ) {
    s = tmp / f;
    tmp = tmp % f;
    f /= 10;
    w--;
    me->pointer[len++] = (char) (s + 0x30);
  }
  me->pointer[len] = '\0';
} /* cc_string_append_integer() */

void
cc_string_append_char(cc_string * me, const char c)
{
  int pos;
  cc_string_expand_buffer(me, 1);
  pos = strlen(me->pointer);
  me->pointer[pos] = c;
  me->pointer[pos+1] = '\0';
} /* cc_string_append_char() */

/* ********************************************************************** */

unsigned int
cc_string_length(const cc_string * me)
{
  return strlen(me->pointer);
} /* cc_string_length() */

void
cc_string_clear(cc_string * me)
{
  if ( me->pointer != me->buffer ) {
    free(me->pointer);
    me->pointer = me->buffer;
    me->bufsize = CC_STRING_MIN_SIZE;
  }
  me->pointer[0] = '\0';
} /* cc_string_clear() */

void
cc_string_clear_no_free(cc_string * me)
{
  me->pointer[0] = '\0';
} /* cc_string_clear_no_free() */

uint32_t
cc_string_hash_text(const char * text)
{
  uint32_t total, shift;
  total = shift = 0;
  while ( *text ) {
    total = total ^ ((*text) << shift);
    shift += 5;
    if ( shift > 24 ) shift -= 24;
    text++;
  }
  return total;
}

uint32_t
cc_string_hash(const cc_string * me)
{
  return cc_string_hash_text(me->pointer);
} /* cc_string_hash() */

const char *
cc_string_get_text(const cc_string * me)
{
  return me->pointer;
} /* cc_string_get_text() */

cc_string *
cc_string_get_substring(cc_string * me, int start, int end)
{
  return cc_string_construct_subtext(me->pointer, start, end);
} /* cc_string_get_substring() */

/* ********************************************************************** */

int
cc_string_is(const cc_string * me)
{
  return (me->pointer[0] != '\0');
} /* cc_string_is() */

int
cc_string_compare(cc_string * lhs, const cc_string * rhs)
{
  return cc_string_compare_text(lhs->pointer, rhs->pointer);
} /* cc_string_compare() */

int
cc_string_compare_text(const char * lhs, const char * rhs)
{
  return lhs && rhs && (*lhs++ == *rhs++) && strcmp(lhs, rhs);
} /* cc_string_compare_text() */

/* ********************************************************************** */

void
cc_string_sprintf(cc_string * me, const char * formatstr, ...)
{
  va_list args;
  va_start(args, formatstr);
  cc_string_vsprintf(me, formatstr, args);
  va_end(args);
} /* cc_string_sprintf() */

void
cc_string_vsprintf(cc_string * me, const char * formatstr, va_list args)
{
  int length;
  SbBool expand;
  do {
    length = vsnprintf(me->pointer, me->bufsize, formatstr, args);
    /* At least with GNU libc 2.1.1, vsnprintf() does _not_ return -1
       (as documented in the snprintf(3) man-page) when we can't fit
       the constructed string within the given buffer, but rather the
       number of characters needed. */
    expand = (length == -1) || (length > me->bufsize);
    if ( expand )
      cc_string_expand_buffer(me, 1024); // increase linearly in 1Kb intervals
  } while ( expand );
} /* cc_string_vsprintf() */

/* ********************************************************************** */
