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

/*
  This file containes various miniscule code fragments that don't really
  belong anywhere in Coin, but which is included to make it easier to keep
  Coin portable.
*/

#include <Inventor/C/tidbits.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <assert.h>
#include <string.h> /* strncasecmp() */
#include <stdio.h>
#include <ctype.h> /* tolower() */
#include <stdlib.h> /* atexit() */
#ifdef HAVE_WINDOWS_H
#include <windows.h> /* GetEnvironmentVariable() */
#endif /* HAVE_WINDOWS_H */


/**************************************************************************/
/*
  coin_vsnprintf() wrapper. Returns -1 if resultant string will be
  longer than n.
*/

#ifdef HAVE_VSNPRINTF

int
coin_vsnprintf(char * dst, unsigned int n, const char * fmtstr, va_list args)
{
  int length = vsnprintf(dst, (size_t)n, fmtstr, args);

  /* Not all vsnprintf() implementations returns -1 upon failure (this
     is what vsnprintf() from GNU libc is documented to do).

     At least with GNU libc 2.1.1, vsnprintf() does _not_ return -1
     (as documented in the snprintf(3) man-page) when we can't fit the
     constructed string within the given buffer, but rather the number
     of characters needed.

     IRIX 6.5 vsnprintf() just returns the number of characters until
     clipped.
  */
  if (length >= (((int)n) - 1)) { length = -1; }

  return length;
}

#elif defined HAVE__VSNPRINTF

int
coin_vsnprintf(char * dst, unsigned int n, const char * fmtstr, va_list args)
{
  /* This is how it is defined in MSVC++ 5.0. */
  return _vsnprintf(dst, (size_t)n, fmtstr, args);
}

#else /* neither vsnprintf() nor _vsnprintf() available, roll our own */

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
coin_vsnprintf(char * dst, unsigned int n, const char * fmtstr, va_list args)
{
  int len = vfprintf(nullfileptr(), fmtstr, args);
  if ((len + 1) > n) return -1;
  (void)vsprintf(dst, fmtstr, args);
  return len;
}
#endif /* coin_vsnprintf() */


/**************************************************************************/
/*
  coin_snprintf() wrapper. Returns -1 if resultant string will be
  longer than n.
*/

int
coin_snprintf(char * dst, unsigned int n, const char * fmtstr, ...)
{
  int len;
  va_list argptr;
  va_start(argptr, fmtstr);
  len = coin_vsnprintf(dst, n, fmtstr, argptr);
  va_end(argptr);
  return len;
}


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
coin_host_get_endianness(void)
{
  union temptype {
    uint32_t value;
    uint8_t  bytes[4];
  } temp;
  temp.bytes[0] = 0x00;
  temp.bytes[1] = 0x01;
  temp.bytes[2] = 0x02;
  temp.bytes[3] = 0x03;
  switch ( temp.value ) {
  case 0x03020100: return COIN_HOST_IS_LITTLEENDIAN;
  case 0x00010203: return COIN_HOST_IS_BIGENDIAN;
  /* might be more variations here for some obscure CPU architectures */
  }
  assert(0 && "system has unknown endianness");
  return COIN_HOST_IS_UNKNOWNENDIAN; /* maybe just as well exit()? */
}

static int coin_endianness = COIN_HOST_IS_UNKNOWNENDIAN;

static void
coin_swap_16bit_word(uint8_t * block)
{
  uint8_t tmp;

  tmp = block[1];
  block[1] = block[0];
  block[0] = tmp;
}

uint16_t
coin_hton_uint16(uint16_t value)
{
  if ( coin_endianness == COIN_HOST_IS_UNKNOWNENDIAN )
    coin_endianness = coin_host_get_endianness();
  switch ( coin_endianness ) {
  case COIN_HOST_IS_BIGENDIAN:
    /* value = value */
    break;
  case COIN_HOST_IS_LITTLEENDIAN:
    coin_swap_16bit_word((uint8_t *)&value);
    break;
  default:
    assert(0 && "system has unknown endianness");
  }
  return value;
}

uint16_t
coin_ntoh_uint16(uint16_t value)
{
  return coin_ntoh_uint16(value);
}

static void
coin_swap_32bit_word(uint8_t * block)
{
  uint8_t tmp;

  tmp = block[3];
  block[3] = block[0];
  block[0] = tmp;

  tmp = block[2];
  block[2] = block[1];
  block[1] = tmp;
}

uint32_t
coin_hton_uint32(uint32_t value)
{
  if ( coin_endianness == COIN_HOST_IS_UNKNOWNENDIAN )
    coin_endianness = coin_host_get_endianness();
  switch ( coin_endianness ) {
  case COIN_HOST_IS_BIGENDIAN:
    /* big-endian is the same order as network order */
    break;
  case COIN_HOST_IS_LITTLEENDIAN:
    coin_swap_32bit_word((uint8_t *)&value);
    break;
  default:
    assert(0 && "system has unknown endianness");
  }
  return value;
}

uint32_t
coin_ntoh_uint32(uint32_t value)
{
  return coin_hton_uint32(value);
}

float
coin_hton_float(float value)
{
  if ( coin_endianness == COIN_HOST_IS_UNKNOWNENDIAN )
    coin_endianness = coin_host_get_endianness();
  switch ( coin_endianness ) {
  case COIN_HOST_IS_BIGENDIAN:
    /* big-endian is the same order as network order */
    break;
  case COIN_HOST_IS_LITTLEENDIAN:
    coin_swap_32bit_word((uint8_t *)&value);
    break;
  default:
    assert(0 && "system has unknown endianness");
  }
  return value;
}

float
coin_ntoh_float(float value)
{
  return coin_hton_float(value);
}

/**************************************************************************/

/*
  isascii() is neither ANSI C nor POSIX, but a BSD extension and SVID
  extension.
 */
SbBool
coin_isascii(const int c)
{
  return (c >= 0x00) && (c < 0x80);
}

/* We provide our own version of the isspace() method, as we don't
   really want it to be locale-dependent (which is known to have
   caused trouble for us with some specific German characters under
   MSWindows, at least). */
SbBool
coin_isspace(const char c)
{
  /* This is what isspace() does under the POSIX and C locales,
     according to the GNU libc man page. */
  return (c==' ') || (c=='\n') || (c=='\r') ||
         (c=='\f') || (c=='\t') || (c=='\v');
}

/**************************************************************************/

/* Quick check to see if an integer value is equal to 2^n, for any
   n=[0, ...]. */
SbBool
coin_is_power_of_two(unsigned int x)
{
  return (x != 0) && ((x & (x - 1)) == 0);
}

/*
  Calculate the view volume jitter vector when doing multipass
  antialiasing rendering.
*/
void
coin_viewvolume_jitter(int numpasses, int curpass, const int * vpsize, float * jitter)
{
  // jitter values from OpenGL Programming Guide
  static float jitter2[] = {
    0.25f, 0.77f,
    0.75f, 0.25f
  };
  static float jitter3[] = {
    0.5033922635f, 0.8317967229f,
    0.7806016275f, 0.2504380877f,
    0.2261828938f, 0.4131553612f
  };
  static float jitter4[] = {
    0.375f, 0.25f,
    0.125f, 0.75f,
    0.875f, 0.25f,
    0.625f, 0.75f
  };
  static float jitter5[] = {
    0.5f, 0.5f,
    0.3f, 0.1f,
    0.7f, 0.9f,
    0.9f, 0.3f,
    0.1f, 0.7f
  };
  static float jitter6[] = {
    0.4646464646f, 0.4646464646f,
    0.1313131313f, 0.7979797979f,
    0.5353535353f, 0.8686868686f,
    0.8686868686f, 0.5353535353f,
    0.7979797979f, 0.1313131313f,
    0.2020202020f, 0.2020202020f
  };
  static float jitter8[] = {
    0.5625f, 0.4375f,
    0.0625f, 0.9375f,
    0.3125f, 0.6875f,
    0.6875f, 0.8125f,
    0.8125f, 0.1875f,
    0.9375f, 0.5625f,
    0.4375f, 0.0625f,
    0.1875f, 0.3125f
  };
  static float jitter9[] = {
    0.5f, 0.5f,
    0.1666666666f, 0.9444444444f,
    0.5f, 0.1666666666f,
    0.5f, 0.8333333333f,
    0.1666666666f, 0.2777777777f,
    0.8333333333f, 0.3888888888f,
    0.1666666666f, 0.6111111111f,
    0.8333333333f, 0.7222222222f,
    0.8333333333f, 0.0555555555f
  };
  static float jitter12[] = {
    0.4166666666f, 0.625f,
    0.9166666666f, 0.875f,
    0.25f, 0.375f,
    0.4166666666f, 0.125f,
    0.75f, 0.125f,
    0.0833333333f, 0.125f,
    0.75f, 0.625f,
    0.25f, 0.875f,
    0.5833333333f, 0.375f,
    0.9166666666f, 0.375f,
    0.0833333333f, 0.625f,
    0.583333333f, 0.875f
  };
  static float jitter16[] = {
    0.375f, 0.4375f,
    0.625f, 0.0625f,
    0.875f, 0.1875f,
    0.125f, 0.0625f,
    0.375f, 0.6875f,
    0.875f, 0.4375f,
    0.625f, 0.5625f,
    0.375f, 0.9375f,
    0.625f, 0.3125f,
    0.125f, 0.5625f,
    0.125f, 0.8125f,
    0.375f, 0.1875f,
    0.875f, 0.9375f,
    0.875f, 0.6875f,
    0.125f, 0.3125f,
    0.625f, 0.8125f
  };

  static float * jittertab[] = {
    jitter2,
    jitter3,
    jitter4,
    jitter5,
    jitter6,
    jitter8,
    jitter8,
    jitter9,
    jitter12,
    jitter12,
    jitter12,
    jitter16,
    jitter16,
    jitter16,
    jitter16
  };

  float * jittab = jittertab[numpasses-2];

  /* FIXME: support more rendering passes by generating jitter tables
   * using some clever algorithm. pederb, 2001-02-21 */
  if (numpasses > 16) numpasses = 16;
  if (curpass >= numpasses) curpass = numpasses - 1;

  if (numpasses < 2) {
    jitter[0] = 0.0f;
    jitter[1] = 0.0f;
    jitter[2] = 0.0f;
  }
  else {
    jitter[0] = jittab[curpass*2] * 2.0f / ((float)vpsize[0]);
    jitter[1] = jittab[curpass*2+1] * 2.0f / ((float)vpsize[1]);
    jitter[2] = 0.0f;
  }
}


/**************************************************************************/

/* This little atexit() wrapper is used throughout the sourcecode to
   make the OSF1/cxx compiler accept C++ functions as it's input
   argument. Problem reported by Guy Barrand. */

typedef void(*atexit_func_type)(void);

void
coin_atexit(coin_atexit_f * f)
{
  (void)atexit((atexit_func_type)f);
}

/**************************************************************************/
