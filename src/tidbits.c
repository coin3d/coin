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

/*
  This file containes various miniscule code fragments that don't really
  belong anywhere in Coin, but which is included to make it easier to keep
  Coin portable.
*/

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/system/inttypes.h>

#include <assert.h>
#include <string.h> /* strncasecmp() */
#include <stdio.h>
#include <ctype.h> /* tolower() */
#include <stdlib.h>
#ifdef HAVE_WINDOWS_H
#include <windows.h> /* GetEnvironmentVariable() */
#endif /* HAVE_WINDOWS_H */

#include <tidbits.h>

#ifndef HAVE_VSNPRINTF

#ifdef HAVE__VSNPRINTF

/* This is how it is defined in MSVC++ 5.0. */
#define vsnprintf _vsnprintf

#else /* !HAVE__VSNPRINTF */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if HAVE_IO_H
#include <io.h> /* defines open() on MSVC++ 5.0 */
#endif /* HAVE_IO_H */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

/*
  This is a butt-ugly hack to emulate the vsnprintf() extension
  available on some systems, without having to reimplement all the
  code needed to parse printf-format strings.

  vsnprintf() is necessary to use for copying a formatstring with a
  variable number of arguments into a char buffer without the
  possibility of overflows.

  Note that there's one exception to the similarity in functionality
  with "native" vsnprintf() implementations: the target buffer might
  not contain _any_ characters from the formatstr upon a -1 return
  value.

  All external functions and macros in this code should be conforming
  to ANSI C3.159-1989 / ISO 9899 or POSIX (or both), as far as I can
  see.

  Compile with something like this (example is under GCC and Linux),
  to generate a stand-alone program testing the function:
    gcc -Wall -DENABLE_TESTCODE=1 -DHAVE_UNISTD_H=1 -DHAVE_VSNPRINTF=1 \
        -o snprintf snprintf.c

  19991221 mortene. Thanks to larsa for the idea.

 */

/*
  FIXME: could perhaps grab a decent snprintf() implementation (with a
  liberal enough license for our use) from one of the BSD-Unixen?
  Investigate.  20010821 mortene.
 */

/* First a helper function to find a valid file pointer which is
   pointing at nothing (for a number of different definitions of
   "nothing"..). */
static FILE *
nullfileptr(void)
{
  static FILE * nullfp = NULL;

  if (!nullfp) {
    struct stat sbuf;
    const char unixdevnull[] = "/dev/null";
    int nullfd = -1;

    if ((stat(unixdevnull, &sbuf) == 0) && (sbuf.st_mode & S_IFCHR))
      nullfd = open(unixdevnull, O_WRONLY);

    if (nullfd == -1) {
      const char * tmpname = tmpnam(NULL);

      if (tmpname) {
        nullfd = open(tmpname, O_CREAT|O_WRONLY);
        (void)unlink(tmpname);
      }
    }

    if (nullfd != -1) nullfp = fdopen(nullfd, "w");
    if (!nullfp) nullfp = stderr; /* :^} (as a last resort.) 19991221 mortene. */
  }

  return nullfp;
}

int
vsnprintf(char * target, size_t n, const char * formatstr, va_list args)
{
  int len = vfprintf(nullfileptr(), formatstr, args);
  if (((size_t)(len+1)) > n) return -1;
  (void)vsprintf(target, formatstr, args);
  return len;
}

#endif /* !HAVE__VSNPRINTF */

#endif /* !HAVE_VSNPRINTF */


/**************************************************************************/

#ifndef HAVE_SNPRINTF

#ifdef HAVE__SNPRINTF

/* This is how it is defined in MSVC++ 5.0. */
#define snprintf _snprintf

#else /* !HAVE__SNPRINTF */

#include <stdarg.h>

int
snprintf(char * target, size_t n, const char * formatstr, ...)
{
  int len;
  va_list argptr;
  va_start(argptr, formatstr);
  len = vsnprintf(target, n, formatstr, argptr);
  va_end(argptr);
  return len;
}

#endif /* !HAVE__SNPRINTF */

#endif /* !HAVE_SNPRINTF */

/**************************************************************************/

/*** Singlelinked list for the environment variables. *********************/

/*
  FIXME: should implement a generic (macro-based) singlelinked list
  abstraction in C (the following code could be a good starting
  point). Then axe this code. Then move the various listhandling stuff
  in the Coin library from the SbList<> template to the C-based one to
  aid any future transition to pure C. 20010821 mortene.

  UPDATE: SbList isn't really a linked list, but in fact a growable
  array -- so the code below can not be used as a replacement. Still,
  we should have a generic linked list class. 20011220 mortene.
*/

static struct envvar_data * envlist_head = NULL;
static struct envvar_data * envlist_tail = NULL;

struct envvar_data {
  char * name;
  char * val;
  struct envvar_data * next;
};

static void
envlist_cleanup(void)
{
  struct envvar_data * ptr = envlist_head;
  while (ptr != NULL) {
    struct envvar_data * tmp = ptr;
    free(ptr->name);
    free(ptr->val);
    ptr = ptr->next;
    free(tmp);
  }
}

static void
envlist_append(struct envvar_data * item)
{
  item->next = NULL;
  if (envlist_head == NULL) {
    envlist_head = item;
    envlist_tail = item;
    (void)atexit(envlist_cleanup);
  }
  else {
    envlist_tail->next = item;
    envlist_tail = item;
  }
}

/**************************************************************************/

/*
  When working with MSWindows applications using Coin as a DLL,
  setenv() / getenv() will not work as expected, as the application
  and the DLL has different instances of the C library with different
  datastructures on different heaps. That's why we need this
  abstraction around the C-libs getenv() vs the Win32 API
  GetEnvironmentVariable() function.

  Note: do not deallocate the returned string -- they are supposed to
  be permanently available to all callers. Deallocating the resources
  is the responsibility of the application exit cleanup code.
*/
const char *
coin_getenv(const char * envname)
{
  /* Important note: this code is identical to the getenv() code in
     So@Gui@/.../common/SoAny.cpp.in. If you do bugfixes or whatever,
     keep them in sync! */

#ifdef HAVE_GETENVIRONMENTVARIABLE
  int neededsize;

  /* Try to find envvar among those requested earlier on. */
  struct envvar_data * ptr = envlist_head;
  while (ptr != NULL) {
    if (strcmp(ptr->name, envname) == 0) { return ptr->val; }
    ptr = ptr->next;
  }

  neededsize = GetEnvironmentVariable(envname, NULL, 0);
  if (neededsize > 1) {
    int resultsize;
    struct envvar_data * item;
    char * tmpbuf = (char *)malloc(neededsize + 1);
    /* Augh. Could we handle this any better? */
    if (tmpbuf == NULL) { return NULL; }
    resultsize = GetEnvironmentVariable(envname, tmpbuf, neededsize);
    /* Augh. Could we handle this any better? */
    if (resultsize != neededsize-1) {
      free(tmpbuf);
      return NULL;
    }

    item = (struct envvar_data *)malloc(sizeof(struct envvar_data));
    /* Augh. Could we handle this any better? */
    if (item == NULL) {
      free(tmpbuf);
      return NULL;
    }
    item->name = strdup(envname);
    /* FIXME: won't work with environment variables that are changed
       "mid-execution". Rather nasty flaw if this is to be considered
       a general replacement for getenv(). 20010821 mortene.*/
    item->val = strdup(tmpbuf);
    /* Augh. Could we handle this any better? */
    if ((item->name == NULL) || (item->val == NULL)) {
      free(tmpbuf);
      if (item->name != NULL) { free(item->name); }
      if (item->val != NULL) { free(item->val); }
      free(item);
      return NULL;
    }
    envlist_append(item);
    return item->val;
  }
  /* FIXME: fall through to the standard C-lib's getenv() if
     GetEnvironmentVariable() fails? If this can cause a crash, it's
     obviously a bad idea, but if not -- it should be
     done. Investigate. 20010821 mortene. */
  return NULL;
#else /* !HAVE_GETENVIRONMENTVARIABLE */
  return getenv(envname);
#endif /* !HAVE_GETENVIRONMENTVARIABLE */
}

/**************************************************************************/

/*
  strncasecmp() is not available on all platforms (it's neither ISO C
  nor POSIX). At least MSVC doesn't have it.
 */
int
coin_strncasecmp(const char * s1, const char * s2, int len)
{
#ifdef HAVE_STRNCASECMP

  return strncasecmp(s1, s2, len);

#else /* !HAVE_STRNCASECMP */

  assert(s1 && s2);
  while (len > 0) {
    int c1 = tolower(*s1);
    int c2 = tolower(*s2);
    if (c1 < c2) { return -1; }
    if (c1 > c2) { return +1; }
    if (c1=='\0' && c2=='\0') { return 0; } /* in case len is too large */
    len--; s1++; s2++;
  }
  return 0;

#endif /* !HAVE_STRNCASECMP */
}

/**************************************************************************/

int
coin_host_is_bigendian(void)
{
  union temptype {
    uint32_t value;
    char bytes[4];
  } temp;
  temp.bytes[0] = 0x00;
  temp.bytes[1] = 0x01;
  temp.bytes[2] = 0x02;
  temp.bytes[3] = 0x03;
  switch ( temp.value ) {
  case 0x00010203:
    return 1;
  case 0x03020100:
    return 0;
  default:
    assert(0 && "system has unknown endianness");
    exit(1);
  }
  return 0;
}

static int coin_bigendian = -1;

uint16_t
coin_hton_uint16(uint16_t value)
{
  if ( coin_bigendian == -1 ) coin_bigendian = coin_host_is_bigendian();
  if ( coin_bigendian ) return value;
  return ((value << 8) & 0xff00) | ((value >> 8) & 0x00ff);
}

uint16_t
coin_ntoh_uint16(uint16_t value)
{
  if ( coin_bigendian == -1 ) coin_bigendian = coin_host_is_bigendian();
  if ( coin_bigendian ) return value;
  return ((value << 8) & 0xff00) | ((value >> 8) & 0x00ff);
}

uint32_t
coin_hton_uint32(uint32_t value)
{
  if ( coin_bigendian == -1 ) coin_bigendian = coin_host_is_bigendian();
  if ( coin_bigendian ) return value;
  value = ((value >> 16) & 0x0000ffff) | ((value << 16) & 0xffff0000);
  return  ((value >>  8) & 0x00ff00ff) | ((value <<  8) & 0xff00ff00);
}

uint32_t
coin_ntoh_uint32(uint32_t value)
{
  if ( coin_bigendian == -1 ) coin_bigendian = coin_host_is_bigendian();
  if ( coin_bigendian ) return value;
  value = ((value >> 16) & 0x0000ffff) | ((value << 16) & 0xffff0000);
  return  ((value >>  8) & 0x00ff00ff) | ((value <<  8) & 0xff00ff00);
}

/**************************************************************************/

/*
  isascii() is neither ANSI C nor POSIX, but a BSD extension and SVID
  extension.
 */
int
coin_isascii(int c)
{
  return (c >= 0x00) && (c < 0x80);
}

/**************************************************************************/
