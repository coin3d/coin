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
#include <Inventor/C/base/string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <assert.h>
#include <string.h> /* strncasecmp() */
#include <stdio.h>
#include <ctype.h> /* tolower() */
#include <stdlib.h> /* atexit(), putenv() */
#ifdef HAVE_WINDOWS_H
#include <windows.h> /* GetEnvironmentVariable() */
#endif /* HAVE_WINDOWS_H */


/**************************************************************************/

/* FIXME: we should _really_ grab some BSD/PD-style licensed code to
   provide a replacement for the (v)snprintf functions. Not only would
   that provide a fail-safe option on obscure platforms, but as it is
   now, we also have problems with locale settings. Under the German
   locale, for instance, floating-point numbers comes out with ",",
   not ".", as the decimal-point separator. This is bad, as it causes
   us to write invalid .iv-files.

   Here is handegar's list of possible candidates (note that many of
   these can probably be ruled out due to the license restrictions --
   we need to be able to find code with a liberal enough license that
   we can re-license it under the Coin PEL and the LGPL, which
   probably effectively limits us to code in the PD, or possibly under
   BSD- or MIT-style licenses:

   1) The BSDs:

      - http://www.openbsd.org/cgi-bin/cvsweb/src/lib/libc/stdio/
      - http://www.freebsd.org/cgi/cvsweb.cgi/src/lib/libc/stdio/
      - http://cvsweb.netbsd.org/cgi-bin/cvsweb.cgi/basesrc/lib/libc/stdio/


   2) A portable snprintf under either GNU (L?)GPL or the "Frontier
      Artistic Lisence" (wahtever that is):

      - http://www.ijs.si/software/snprintf/


   3) TRIO (seems to be under an "MIT-like" license):

      - http://daniel.haxx.se/trio/


   4) Samba (under GNU GPL?):

      - http://samba.org/cgi-bin/cvsweb/samba/source/lib/snprintf.c


   5) Apache:

      - http://www.apache.org


   6) Caolán McNamara's (GNU GPL?):

      - http://www.csn.ul.ie/~caolan/publink/snprintf-1.1.tar.gz


   7) Castaglia (GNU GPL?):

      - http://www.castaglia.org/proftpd/doc/devel-guide/src/lib/vsnprintf.c.html


   8) Gnome (GNU (L?)GPL? -- looks very similar to the Samba
      implementation):

      - http://cvs.gnome.org/lxr/

   20021128 mortene.

   UPDATE 20021206 mortene: it looks like integrating any of these
   would be quite some effort, to get the build / configure stuff
   correct, to slim down the code of the chosen implementation, etc
   etc. I think our first step should perhaps be to let configure bail
   out if no "native" snprintf() is found on the system, with a good
   error message, a fallback, and a notice about mailing us info on
   the platform in question. Just so we avoid spending much time
   solving a problem which is really not there on any platform Coin is
   being used.
 */

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
  if ( ((unsigned int) len + 1) > n) return -1;
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

#ifdef HAVE_GETENVIRONMENTVARIABLE
struct envvar_data {
  char * name;
  char * val;
  struct envvar_data * next;
};

static void
envlist_cleanup(void)
{
  struct envvar_data * ptr = envlist_head;
  while ( ptr != NULL ) {
    struct envvar_data * tmp = ptr;
    free(ptr->name);
    free(ptr->val);
    ptr = ptr->next;
    free(tmp);
  }
}

#else
struct envvar_data {
  char * string;
  struct envvar_data * next;
};

static void
envlist_cleanup(void)
{
  struct envvar_data * ptr = envlist_head;
  while ( ptr != NULL ) {
    struct envvar_data * tmp = ptr;
    char * strptr = strchr(ptr->string, '=');
    if ( strptr ) *strptr = '\0';
    /* else huh? */
    putenv(ptr->string); /* remove string from environment */
    free(ptr->string);
    ptr = ptr->next;
    free(tmp);
  }
}

#endif /* HAVE_GETENVIRONMENTVARIABLE */

static void
envlist_append(struct envvar_data * item)
{
  item->next = NULL;
  if (envlist_head == NULL) {
    envlist_head = item;
    evnlist_tail = item;
    (void)atexit(envlist_cleanup);
  }
  else {
    envlist_tail->next = item;
    envlist_tail = item;
  }
}

/**************************************************************************/

/* FIXME: should getenv/setenv/unsetenv be made mt-safe by locking access
 * to the envlist linked list under Win32?  20030205 larsa */

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
  neededsize = GetEnvironmentVariable(envname, NULL, 0);
  /* neededsize includes the \0-terminating character */
  if ( neededsize >= 1 ) {
    int resultsize;
    struct envvar_data * envptr;
    char * valbuf = (char *) malloc(neededsize);
    if ( valbuf == NULL ) {
      /* Augh. Could we handle this any better? */
      /* If we already bookkeep a buffer for this variable, we /could/ try
         to reuse it (much work for a non-100% solution).  20030205 larsa */
      return NULL;
    }
    resultsize = GetEnvironmentVariable(envname, valbuf, neededsize);
    if ( resultsize != (neededsize - 1) ) {
      /* Augh. Could we handle this any better? */
      /* How about looping to top and trying again (in case the reason is mt
         and envval being changed in the background, or maybe just asserting?
         20030205 larsa */
      free(valbuf);
      return NULL;
    }

    /*
      The GetEnvironmentVariable() signature forces us to allocate the space
      for the value string on the outside of the call, as opposed to the UNIX
      getenv() function.  We therefore have to do bookkeeping and maintain
      this linked list of allocated buffers that should be cleaned up on exit
      (atexit()).  We don't keep it for lookup of values - we actually can't
      use the valus as value caches in case they have been changed from other
      parts of the application.  We only keep them so we can free them later.
    */

    /* Try to find bookkeeped envvar buffer among those registered earlier. */
    envptr = envlist_head;
    while ( (envptr != NULL) && (strcmp(envptr->name, envname) != 0) )
      envptr = envptr->next;

    /* We can avoid this if-else by always freeing the envvar_data for the
       variable upfront, but it's a tad less efficient. */
    if ( envptr != NULL ) {
      /* We are already bookkeeping a buffer for this variable.
       * => free previous value buffer and bookkeep the new one instead */
      free(envptr->val);
      envptr->val = valbuf;
    } else {
      /* We aren't bookkeeping a buffer for this one yet. */
      envptr = (struct envvar_data *) malloc(sizeof(struct envvar_data));
      if ( envptr == NULL ) {
        /* Augh. Could we handle this any better? */
	/* We can alternatively ignore the bookkeeping and leak the buffer
           - 20030205 larsa */
        free(valbuf);
        return NULL;
      }
      envptr->name = strdup(envname);
      if ( envptr->name == NULL ) {
        /* Augh. Could we handle this any better? */
	/* We can alternatively ignore the bookkeeping and leak the buffer
           - 20030205 larsa */
	free(envptr);
	free(valbuf);
	return NULL;
      }
      envptr->val = valbuf;
      envlist_append(envptr);
    }
    return envptr->val;
  }
  return NULL;
#else /* !HAVE_GETENVIRONMENTVARIABLE */
  return getenv(envname);
#endif /* !HAVE_GETENVIRONMENTVARIABLE */
}

SbBool
coin_setenv(const char * name, const char * value, int overwrite)
{
#ifdef HAVE_GETENVIRONMENTVARIABLE
/*
  The value is changed by the application, so we no longer need to
  guarantee old buffers' existence to the outside world.  We therefore
  free buffers we are bookkeeping here.  This code can be disabled
  without any consequence though.
*/
  struct envvar_data * envptr, * prevptr;
  envptr = envlist_head;
  prevptr = NULL;
  while ( (envptr != NULL) && (strcmp(envptr->name, name) != 0) ) {
    prevptr = envptr;
    envptr = envptr->next;
  }
  if ( envptr ) {
    /* unlink node */
    if ( prevptr ) prevptr->next = envptr->next;
    else envlist_head = envptr->next;
    if ( envlist_tail == envptr ) envlist_tail = prevptr;
    /* free node */
    free(envptr->name);
    free(envptr->val);
    free(envptr);
  }

  /* FIXME: This is from Win32s 1.3 Bug List - how should we handle it?
  and what's with the typo in the function name?  20030205 larsa
  ====================================================================
  SetEnvironmentVariables() does not handle an empty string, an equal
  sign (=), or foreign lowercase characters in the variable name.
  */

  if ( overwrite || (GetEnvironmentVariable(name, NULL, 0) == 0) )
    return SetEnvironmentVariable(name, value) ? TRUE : FALSE;
  else
    return TRUE;
#else /* !HAVE_GETENVIRONMENTVARIABLE */
  if ( !getenv(name) || overwrite ) {
    /* ugh - this looks like a mess, but things must be ordered very strictly */

    struct envvar_data * envptr, * prevptr;
    cc_string str;
    char * strbuf, * oldbuf;
    int len;
    cc_string_construct(&str);
    cc_string_sprintf(&str, "%s=%s", name, value);
    strbuf = (char *) malloc(cc_string_length(&str)+1);
    if ( strbuf ) strcpy(strbuf, cc_string_get_text(&str));
    cc_string_clean(&str);
    if ( !strbuf ) {
      /* handle this better? */
      return FALSE;
    }

    envptr = envlist_head;
    len = strlen(name) + 1;
    while ( envptr && strncmp(strbuf, envptr->string, len) != 0 )
      envptr = envptr->next;

    if ( envptr ) {
      oldbuf = envptr->string;
    } else {
      oldbuf = NULL;
      envptr = (struct envvar_data *) malloc(sizeof(struct envvar_data));
      if ( !envptr ) {
        /* handle this better? */
        free(strbuf);
        return FALSE;
      }
    }
    envptr->string = strbuf;

    if ( putenv(envptr->string) == -1 ) { /* denied! */
      if ( oldbuf ) {
	/* we had old value - setting new didn't work, so we do a rollback and assume we
	   still need to do bookkeeping of the old value */
	free(envptr->string);
	envptr->string = oldbuf;
	return FALSE;
      } else {
	free(envptr->string);
	free(envptr);
      }
      return FALSE;
    } else {
      if ( oldbuf ) free(oldbuf);
      else envlist_append(envptr);
    }
  }
  return TRUE;
#endif /* !HAVE_GETENVIRONMENTVARIABLE */
}

void
coin_unsetenv(const char * name)
{
#ifdef HAVE_GETENVIRONMENTVARIABLE
/*
  The value is removed by the application, so we no longer need to
  guarantee old buffers' existence to the outside world.  We therefore
  free buffers we are bookkeeping here.  This code can be disabled
  without any consequence though.
*/
  struct envvar_data * envptr, * prevptr;
  envptr = envlist_head;
  prevptr = NULL;
  while ( (envptr != NULL) && (strcmp(envptr->name, name) != 0) ) {
    prevptr = envptr;
    envptr = envptr->next;
  }
  if ( envptr ) {
    /* unlink node */
    if ( prevptr ) prevptr->next = envptr->next;
    else envlist_head = envptr->next;
    if ( envlist_tail == envptr ) envlist_tail = prevptr;
    /* free node */
    free(envptr->name);
    free(envptr->val);
    free(envptr);
  }
  SetEnvironmentVariable(name, NULL);
#else /* !HAVE_GETENVIRONMENTVARIABLE */
  int len;
  struct envvar_data * envptr, * prevptr;
  /* we assume we don't need to bookkeep this string */
  /* if name contains '=' (abuse of function), then we're in deep trouble
     when/if name is freed - it's not our responsibility to be /that/
     paranoid though */
  putenv((char *) name);
  /* remove bookkeeping for environment string */
  prevptr = NULL;
  envptr = envlist_head;
  len = strlen(name);
  while ( envptr && !((strncmp(name, envptr->string, len) == 0) && (envptr->string[len] == '=')) ) {
    prevptr = envptr;
    envptr = envptr->next;
  }
  if ( envptr ) {
    if ( prevptr ) prevptr->next = envptr->next;
    else envlist_head = envptr->next;
    if ( envptr == envlist_tail ) envlist_tail = prevptr;
    free(envptr->string);
    free(envptr);
  }
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

static int coin_endianness = COIN_HOST_IS_UNKNOWNENDIAN;

int
coin_host_get_endianness(void)
{
  union temptype {
    uint32_t value;
    uint8_t  bytes[4];
  } temp;

  if (coin_endianness != COIN_HOST_IS_UNKNOWNENDIAN)
    return coin_endianness;

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
coin_is_power_of_two(uint32_t x)
{
  return (x != 0) && ((x & (x - 1)) == 0);
}

/*
  Returns nearest upward number for x that is a power of two.

  Note that if "x" is already a power of two, we will still return the
  *next* number that is a power of two, and not x itself.
 */
uint32_t
coin_next_power_of_two(uint32_t x)
{
  assert((x < (uint32_t)(1 << 31)) && "overflow");

  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  /* This will make it work with 64-bit numbers: */
  /* x |= (x >> 32); */

  return x + 1;
}

/*
  Returns nearest upward number for x that is a power of two, or x
  itself if it is already a power of two.

  ("geq" is short for "Greater or EQual".)
 */
uint32_t
coin_geq_power_of_two(uint32_t x)
{
  if (coin_is_power_of_two(x)) return x;
  return coin_next_power_of_two(x);
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
