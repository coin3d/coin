/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*
  This file containes various miniscule code fragments that don't really
  belong anywhere in Coin, but which is included to make it easier to keep
  Coin portable.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <assert.h>
#include <errno.h>
#include <math.h> /* isinf(), isnan(), finite() */
#include <float.h> /* _fpclass(), _isnan(), _finite() */
#include <locale.h>
#include <string.h> /* strncasecmp() */
#include <stdio.h>
#include <ctype.h> /* tolower() */
#include <stdlib.h> /* atexit(), putenv(), qsort(), atof() */
#ifdef HAVE_WINDOWS_H
#include <windows.h> /* GetEnvironmentVariable() */
#endif /* HAVE_WINDOWS_H */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_IO_H
#include <io.h> /* defines open() on MSVC++ 5.0 */
#endif /* HAVE_IO_H */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif /* HAVE_IEEEFP_H */

#ifdef HAVE_DIRECT_H
#include <direct.h> /* _getcwd() */
#endif /* HAVE_DIRECT_H */

#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/base/list.h>
#include <Inventor/C/errors/debugerror.h>

/**************************************************************************/

/* FIXME: we should grab some BSD/PD-style licensed code to provide a
   replacement for the (v)snprintf functions. This would provide a
   fail-safe option on obscure platforms.

   Here is handegar's list of possible candidates (note that many of
   these can probably be ruled out due to the license restrictions --
   we need to be able to find code with a liberal enough license that
   we can re-license it under the Coin PEL and the GPL, which
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

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */

#ifdef COIN_THREADSAFE
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>
static cc_mutex * atexit_list_monitor = NULL;
#endif /* COIN_THREADSAFE */

/* ********************************************************************** */

/* Called from SoDB::init() exactly once, a call which is guaranteed
   free from race conditions. */
void
coin_init_tidbits(void)
{
#ifdef COIN_THREADSAFE
  atexit_list_monitor = cc_mutex_construct();
#endif /* COIN_THREADSAFE */
}

/* ********************************************************************** */

/*
  coin_vsnprintf() wrapper. Returns -1 if resultant string will be
  longer than n.
*/

typedef int func_vsnprintf(char *, size_t, const char *, va_list);

static int
coin_common_vsnprintf(func_vsnprintf * func,
                      char * dst, size_t n,
                      const char * fmtstr, va_list args)
{
  int length;
  static int debug = -1;

  if (debug == -1) {
    const char * env = coin_getenv("COIN_DEBUG_NPRINTF");
    debug = (env && (atoi(env) > 0)) ? 1 : 0;
  }

  /* Note: subtract one from n as the buffer size, because libDCE on
     HP-UX has a vsnprintf() implementation which will otherwise
     overwrite the array with one byte (it doesn't count the trailing
     \0, as e.g. GNU libc does, and like a C99-conforming
     implementation would). */
  assert(n > 1);
  n -= 1;

  /* Can not use cc_debugerror_* interface(), as that could cause an
     infinite recursion. */
  if (debug) { printf("dst==%p, n==%u, fmtstr=='%s'\n", dst, n, fmtstr); }

#ifdef HAVE_VA_COPY_MACRO
  /* The C99 va_copy() is available, so use that to help us "rewind"
     the va_list between invocations. */
  {
    va_list argscopy;
    va_copy(argscopy, args);
    length = (*func)(dst, n, fmtstr, argscopy);
    va_end(argscopy);
  }
#else /* !HAVE_VA_COPY_MACRO */
  /* No va_copy() available, so we just assume the system's
     vsnprintf() to "rewind" the va_list after use. This assumption is
     "sanity checked" from within SoDB::init(). */
  length = (*func)(dst, n, fmtstr, args);
#endif /* !HAVE_VA_COPY_MACRO */

  if (debug) { printf("==> length==%d\n", length); }

  /* Not all vsnprintf() implementations returns -1 upon failure (this
     is what vsnprintf() from GNU libc is documented to do).

     At least with GNU libc 2.1.1, vsnprintf() does _not_ return -1
     (as documented in the snprintf(3) man-page) when we can't fit the
     constructed string within the given buffer, but rather the number
     of characters needed, excluding the terminating \0. The latter is
     also the behavior specified by C99.

     IRIX 6.5 vsnprintf() just returns the number of characters until
     clipped, i.e. input argument n minus one.

     For the C run-times that comes with MSVC++ 5.0 and MSVC++ 6.0,
     _vsnprintf() is specified to return -1 on overruns. (To match
     C99, one would expect Microsoft to change this, though. Perhaps
     done for MSVC++ v7?)


     For reference, here's a simple test example which checks the
     behavior of snprintf() / vsnprintf(), when trying to fit 26
     characters plus a terminating \0 into a 20-character buffer:

     -----8<----------- [snip] ---------------8<----------- [snip] ---------
     #include <stdio.h>

     int
     main(void)
     {
       char buffer[20] = "";
       const int ret =
         snprintf(buffer, sizeof(buffer), "%s",
                  "abcdefghijklmnopqrstuvwxyz");
       (void)printf("snprintf()==%d\n", ret);
       (void)printf("buffer=\"%s\"\n", buffer);
       return 0;
     }
     -----8<----------- [snip] ---------------8<----------- [snip] ---------
  */

  if (length >= (((int)n) - 1)) { length = -1; }

  return length;
}

#ifdef HAVE_VSNPRINTF

int
coin_vsnprintf(char * dst, unsigned int n, const char * fmtstr, va_list args)
{
  return coin_common_vsnprintf(vsnprintf, dst, (size_t)n, fmtstr, args);
}

#elif defined HAVE__VSNPRINTF

int
coin_vsnprintf(char * dst, unsigned int n, const char * fmtstr, va_list args)
{
  return coin_common_vsnprintf(_vsnprintf, dst, (size_t)n, fmtstr, args);
}

#else /* neither vsnprintf() nor _vsnprintf() available, roll our own */

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

  20040203 larsa: Finally solved a strange problem of having files
  popping up in the root directory (on windows computers, and on the
  current drive) with names like "s3n4" and "s25o" etc. with just a
  few characters of text in them.  It turned out to be this fallback
  that created them, and I had forgotten to use the SIM_AC_CHECK_NPRINTF
  macro in the configure script.  The fallback is in other words not
  entirely "safe", since it pollutes the filesystem on windows computers.
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
        (void)remove(tmpname);
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
  int len;

#ifdef HAVE_VA_COPY_MACRO

  /* The C99 va_copy() is available, so use that to help us "rewind"
     the va_list between invocations. */
  {
    va_list argscopy;

    va_copy(argscopy, args);
    len = vfprintf(nullfileptr(), fmtstr, argscopy);
    va_end(argscopy);

    if (((unsigned int) len + 1) > n) return -1;

    va_copy(argscopy, args);
    (void)vsprintf(dst, fmtstr, argscopy);
    va_end(argscopy);
  }

#else /* !HAVE_VA_COPY_MACRO */

  /* No va_copy() available, so we just assume the system's vfprintf()
     to "rewind" the va_list after use. This assumption is "sanity
     checked" from within SoDB::init(). */
  len = vfprintf(nullfileptr(), fmtstr, args);
  if (((unsigned int) len + 1) > n) return -1;
  (void)vsprintf(dst, fmtstr, args);

#endif /* !HAVE_VA_COPY_MACRO */

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
  while (ptr != NULL) {
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
  while (ptr != NULL) {
    struct envvar_data * tmp = ptr;
    char * strptr = strchr(ptr->string, '=');
    if (strptr) *strptr = '\0';
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
    envlist_tail = item;
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
  is the responsibility of the application exit cleanup code (i.e. the
  internal library cleanup code -- application programmers won't need
  to care about it).
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
  if (neededsize >= 1) {
    int resultsize;
    struct envvar_data * envptr;
    char * valbuf = (char *) malloc(neededsize);
    if (valbuf == NULL) {
      /* Augh. Could we handle this any better? */
      /* If we already bookkeep a buffer for this variable, we /could/ try
         to reuse it (much work for a non-100% solution).  20030205 larsa */
      return NULL;
    }
    resultsize = GetEnvironmentVariable(envname, valbuf, neededsize);
    if (resultsize != (neededsize - 1)) {
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
    while ((envptr != NULL) && (strcmp(envptr->name, envname) != 0))
      envptr = envptr->next;

    /* We can avoid this if-else by always freeing the envvar_data for the
       variable upfront, but it's a tad less efficient. */
    if (envptr != NULL) {
      /* We are already bookkeeping a buffer for this variable.
       * => free previous value buffer and bookkeep the new one instead */
      free(envptr->val);
      envptr->val = valbuf;
    }
    else {
      /* We aren't bookkeeping a buffer for this one yet. */
      envptr = (struct envvar_data *) malloc(sizeof(struct envvar_data));
      if (envptr == NULL) {
        /* Augh. Could we handle this any better? */
	/* We can alternatively ignore the bookkeeping and leak the buffer
           - 20030205 larsa */
        free(valbuf);
        return NULL;
      }
      envptr->name = strdup(envname);
      if (envptr->name == NULL) {
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
  while ((envptr != NULL) && (strcmp(envptr->name, name) != 0)) {
    prevptr = envptr;
    envptr = envptr->next;
  }
  if (envptr) {
    /* unlink node */
    if (prevptr) prevptr->next = envptr->next;
    else envlist_head = envptr->next;
    if (envlist_tail == envptr) envlist_tail = prevptr;
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

  if (overwrite || (GetEnvironmentVariable(name, NULL, 0) == 0))
    return SetEnvironmentVariable(name, value) ? TRUE : FALSE;
  else
    return TRUE;
#else /* !HAVE_GETENVIRONMENTVARIABLE */
  if (!getenv(name) || overwrite) {
    /* ugh - this looks like a mess, but things must be ordered very strictly */

    struct envvar_data * envptr, * prevptr;
    cc_string str;
    char * strbuf, * oldbuf;
    int len;
    cc_string_construct(&str);
    cc_string_sprintf(&str, "%s=%s", name, value);
    strbuf = (char *) malloc(cc_string_length(&str)+1);
    if (strbuf) strcpy(strbuf, cc_string_get_text(&str));
    cc_string_clean(&str);
    if (!strbuf) {
      /* handle this better? */
      return FALSE;
    }

    envptr = envlist_head;
    len = strlen(name) + 1;
    while (envptr && strncmp(strbuf, envptr->string, len) != 0)
      envptr = envptr->next;

    if (envptr) {
      oldbuf = envptr->string;
    }
    else {
      oldbuf = NULL;
      envptr = (struct envvar_data *) malloc(sizeof(struct envvar_data));
      if (!envptr) {
        /* handle this better? */
        free(strbuf);
        return FALSE;
      }
    }
    envptr->string = strbuf;

    if (putenv(envptr->string) == -1) { /* denied! */
      if (oldbuf) {
	/* we had old value - setting new didn't work, so we do a rollback and assume we
	   still need to do bookkeeping of the old value */
	free(envptr->string);
	envptr->string = oldbuf;
	return FALSE;
      }
      else {
	free(envptr->string);
	free(envptr);
      }
      return FALSE;
    }
    else {
      if (oldbuf) free(oldbuf);
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
  while ((envptr != NULL) && (strcmp(envptr->name, name) != 0)) {
    prevptr = envptr;
    envptr = envptr->next;
  }
  if (envptr) {
    /* unlink node */
    if (prevptr) prevptr->next = envptr->next;
    else envlist_head = envptr->next;
    if (envlist_tail == envptr) envlist_tail = prevptr;
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
  while (envptr && !((strncmp(name, envptr->string, len) == 0) && (envptr->string[len] == '='))) {
    prevptr = envptr;
    envptr = envptr->next;
  }
  if (envptr) {
    if (prevptr) prevptr->next = envptr->next;
    else envlist_head = envptr->next;
    if (envptr == envlist_tail) envlist_tail = prevptr;
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
  switch (temp.value) {
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
  if (coin_endianness == COIN_HOST_IS_UNKNOWNENDIAN)
    coin_endianness = coin_host_get_endianness();
  switch (coin_endianness) {
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
  return coin_hton_uint16(value);
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
  if (coin_endianness == COIN_HOST_IS_UNKNOWNENDIAN)
    coin_endianness = coin_host_get_endianness();
  switch (coin_endianness) {
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
  if (coin_endianness == COIN_HOST_IS_UNKNOWNENDIAN)
    coin_endianness = coin_host_get_endianness();
  switch (coin_endianness) {
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
  /* jitter values from OpenGL Programming Guide */
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

void free_std_fds(void);

typedef void(*atexit_func_type)(void);

static cc_list * atexit_list = NULL;
static SbBool isexiting = FALSE;

typedef struct {
  char * name;
  coin_atexit_f * func;
  int32_t priority;
  uint32_t cnt;
} tb_atexit_data;

static int
atexit_qsort_cb(const void * q0, const void * q1)
{
  tb_atexit_data * p0, * p1;

  p0 = *((tb_atexit_data**) q0);
  p1 = *((tb_atexit_data**) q1);

  /* sort list on ascending priorities, so that high priority
     callbacks are called first  */
  if (p0->priority < p1->priority) return -1;
  if (p0->priority > p1->priority) return 1;

  /* when priority is equal, use LIFO */
  if (p0->cnt < p1->cnt) return -1;
  return 1;
}

/*
   Calls all atexit functions. Invoked from SoDB::cleanup().
*/
void
coin_atexit_cleanup(void)
{
  int i, n;
  tb_atexit_data * data;
  const char * debugstr;
  SbBool debug = FALSE;

  if (!atexit_list) return;

  isexiting = TRUE;

  debugstr = coin_getenv("COIN_DEBUG_CLEANUP");
  debug = debugstr && (atoi(debugstr) > 0);

  n = cc_list_get_length(atexit_list);
  qsort(cc_list_get_array(atexit_list), n, sizeof(void*), atexit_qsort_cb);

  for (i = n-1; i >= 0; i--) {
    data = (tb_atexit_data*) cc_list_get(atexit_list, i);
    if (debug) {
      cc_debugerror_postinfo("coin_atexit_cleanup", "invoking %s()", data->name);
    }
    data->func();
    free(data->name);
    free((void*)data);
  }

  // Close stdin/stdout/stderr if any of them have been opened
  free_std_fds();

  cc_list_destruct(atexit_list);
  atexit_list = NULL;
  isexiting = FALSE;

#ifdef COIN_THREADSAFE
  cc_mutex_destruct(atexit_list_monitor);
  atexit_list_monitor = NULL;
#endif /* COIN_THREADSAFE */

  if (debug) { cc_debugerror_postinfo("coin_atexit_cleanup", "fini"); }
}

/*
  This replacement for the C library's atexit() function is used for
  two reasons: first, we want to control the internal order of when
  clean-up callbacks are invoked (as can be done by setting the
  priority argument accordingly), second, we want to be able to do a
  controlled clean-up in advance of the C library's shutdown, to make
  sure that we get cleaned up before any DLLs we are using are thrown
  out of the process.

  Callbacks with higher priority will be called first. On equal
  priority callbacks will be made last-in-first-out ("LIFO").

  For this function a higher number is a higher priority.  An atexit
  function with priority 2 will trigger before an atexit function with
  priority 1.

  Functions passed to this method should be cast to the apropriate
  method signature (coin_atexit_f), so the OSF1/cxx compiler can
  accept a C++ function as it's input argument. (Problem reported by
  Guy Barrand.)
*/

void
coin_atexit_func(const char * name, coin_atexit_f * f, int32_t priority)
{
#ifdef COIN_THREADSAFE
  /* This function being not mt-safe seemed to be the only cause of
     problems when constructing SoNode-derived classes in parallel
     threads. So for that extra bit of undocumented, unofficial,
     under-the-table mt-safety, this should take care of it. */

  /*
    Need this test, since the thread system calls coin_atexit
    before tidbits is initialized.
  */
  if (atexit_list_monitor) {
    cc_mutex_lock(atexit_list_monitor);
  }
#endif /* COIN_THREADSAFE */

  assert(!isexiting && "tried to attach an atexit function while exiting");

  if (atexit_list == NULL) {
    atexit_list = cc_list_construct();
    /* The atexit() registration was disabled, since it has proved
       dangerous to let the C library trigger the callbacks.

       There is for instance the known problem with deallocating
       resources from a DLL we're using (like OpenAL), as the DLL
       could already have been "offloaded" or simply cleaned up /
       cleaned out when our callback triggers.

       We therefore now force cleanup at exit to be done explicitly
       from application code by invoking the SoDB::cleanup() method,
       which then invokes the coin_atexit_cleanup() method.

       mortene.
    */
    /* (void)atexit(coin_atexit_cleanup); */
  }

  {
    tb_atexit_data * data;

    data = (tb_atexit_data*) malloc(sizeof(tb_atexit_data));
    data->name = strdup(name);
    data->func = f;
    data->priority = priority;
    data->cnt = cc_list_get_length(atexit_list);

    cc_list_append(atexit_list, data);
  }

#ifdef COIN_THREADSAFE
  if (atexit_list_monitor) {
    cc_mutex_unlock(atexit_list_monitor);
  }
#endif /* COIN_THREADSAFE */
}

/*
 * Public version of the coin_atexit function which always sets the priority to -2.
 */
void
cc_coin_atexit(coin_atexit_f * f)
{
  coin_atexit(f, -2);
}


/*
  Returns \c TRUE if we are currently iterating over the functions
  registered with coin_atexit(), otherwise \c FALSE.
 */
SbBool
coin_is_exiting(void)
{
  return isexiting;
}

/**************************************************************************/

/*
  It is not possible to "pass" C library data from the application
  to a MSWin .DLL, so this is necessary to get hold of the stderr
  FILE*.  Just using fprintf(stderr, ...) or fprintf(stdout, ...)
  directly will result in a crash when Coin has been compiled as a
  .DLL.
*/

/* These constants are fixed according to POSIX */
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

/*
  FIXME: if Coin does an fclose() on one of these, what happens when the
  cached variable is used later on?  We should make sure Coin doesn't
  fclose() one of these...  Or we should open a new FILE pointer each time
  it is called?  Would it be safe to check if the FILE * is closed and
  then open a new one?  20030217 larsa
*/

static FILE * coin_stdin = NULL;
static FILE * coin_stdout = NULL;
static FILE * coin_stderr = NULL;

void
free_std_fds(void)
{
  // Close stdin/stdout/stderr
  if (coin_stdin) { fclose(coin_stdin); coin_stdin = NULL; }
  if (coin_stdout) { fclose(coin_stdout); coin_stdout = NULL; }
  if (coin_stderr) { fclose(coin_stderr); coin_stderr = NULL; }
}

FILE *
coin_get_stdin(void)
{
  if ( ! coin_stdin ) coin_stdin = fdopen(STDIN_FILENO, "r");
  return coin_stdin;
}

FILE *
coin_get_stdout(void)
{
  if ( ! coin_stdout ) coin_stdout = fdopen(STDOUT_FILENO, "w");
  return coin_stdout;
}

FILE *
coin_get_stderr(void)
{
  if ( ! coin_stderr ) coin_stderr = fdopen(STDERR_FILENO, "w");
  return coin_stderr;
}

/**************************************************************************/

SbBool
coin_locale_set_portable(cc_string * storeold)
{
  const char * loc;

  const char * deflocale = setlocale(LC_NUMERIC, NULL);
  if (strcmp(deflocale, "C") == 0) { return FALSE; }

  /* Must copy deflocale string, as it will be changed on the next
     invocation of setlocale(). */
  cc_string_construct(storeold);
  cc_string_set_text(storeold, deflocale);

  loc = setlocale(LC_NUMERIC, "C");
  assert(loc != NULL && "could not set locale to supposed portable C locale");
  return TRUE;
}

void
coin_locale_reset(cc_string * storedold)
{
  const char * l = setlocale(LC_NUMERIC, cc_string_get_text(storedold));
  assert(l != NULL && "could not reset locale");
  cc_string_clean(storedold);
}

double
coin_atof(const char * ptr)
{
  /* Avoid trying to read decimal point as ",", and reading with
     thousands separator, as defined for some locales, influencing the
     atof() call. */

  double v;
  cc_string storedlocale;
  SbBool changed = coin_locale_set_portable(&storedlocale);
  v = atof(ptr);
  if (changed) { coin_locale_reset(&storedlocale); }
  return v;
}

/**************************************************************************/

/* helper function for ascii85 handling */
static int
coin_encode_ascii85(const unsigned char * in, unsigned char * out)
{
  uint32_t data =
    ((uint32_t)(in[0])<<24) |
    ((uint32_t)(in[1])<<16) |
    ((uint32_t)(in[2])<< 8) |
    ((uint32_t)(in[3]));

  if (data == 0) {
    out[0] = 'z';
    return 1;
  }
  out[4] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[3] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[2] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[1] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[0] = (unsigned char) (data%85 + '!');
  return 5;
}

void
coin_output_ascii85(FILE * fp,
                    const unsigned char val,
                    unsigned char * tuple,
                    unsigned char * linebuf,
                    int * tuplecnt, int * linecnt,
                    const int rowlen,
                    const SbBool flush)
{
  int i;
  if (flush) {
    /* fill up tuple */
    for (i = *tuplecnt; i < 4; i++) tuple[i] = 0;
  }
  else {
    tuple[(*tuplecnt)++] = val;
  }
  if (flush || *tuplecnt == 4) {
    if (*tuplecnt) {
      int add = coin_encode_ascii85(tuple, linebuf + *linecnt);
      if (flush) {
        if (add == 1) {
          for (i = 0; i < 5; i++) linebuf[*linecnt + i] = '!';
        }
        *linecnt += *tuplecnt + 1;
      }
      else *linecnt += add;
      *tuplecnt = 0;
    }
    if (*linecnt >= rowlen) {
      unsigned char store = linebuf[rowlen];
      linebuf[rowlen] = 0;
      fprintf(fp, "%s\n", linebuf);
      linebuf[rowlen] = store;
      for (i = rowlen; i < *linecnt; i++) {
        linebuf[i-rowlen] = linebuf[i];
      }
      *linecnt -= rowlen;
    }
    if (flush && *linecnt) {
      linebuf[*linecnt] = 0;
      fprintf(fp, "%s\n", linebuf);
    }
  }
}

void
coin_flush_ascii85(FILE * fp,
                   unsigned char * tuple,
                   unsigned char * linebuf,
                   int * tuplecnt, int * linecnt,
                   const int rowlen)
{
  coin_output_ascii85(fp, 0, tuple, linebuf, tuplecnt, linecnt, rowlen, TRUE);
}

/**************************************************************************/

SbBool
coin_parse_versionstring(const char * versionstr,
                         int * major,
                         int * minor,
                         int * patch)
{
  char buffer[256];
  char * dotptr;

  *major = 0;
  if (minor) *minor = 0;
  if (patch) *patch = 0;
  if (versionstr == NULL) return FALSE;

  (void)strncpy(buffer, versionstr, 255);
  buffer[255] = '\0'; /* strncpy() will not null-terminate if strlen > 255 */
  dotptr = strchr(buffer, '.');
  if (dotptr) {
    char * spaceptr;
    char * start = buffer;
    *dotptr = '\0';
    *major = atoi(start);
    if (minor == NULL) return TRUE;
    start = ++dotptr;

    dotptr = strchr(start, '.');
    spaceptr = strchr(start, ' ');
    if (!dotptr && spaceptr) dotptr = spaceptr;
    if (dotptr && spaceptr && spaceptr < dotptr) dotptr = spaceptr;
    if (dotptr) {
      int terminate = *dotptr == ' ';
      *dotptr = '\0';
      *minor = atoi(start);
      if (patch == NULL) return TRUE;
      if (!terminate) {
        start = ++dotptr;
        dotptr = strchr(start, ' ');
        if (dotptr) *dotptr = '\0';
        *patch = atoi(start);
      }
    }
    else {
      *minor = atoi(start);
    }
  }
  else {
    cc_debugerror_post("coin_parse_versionstring",
                       "Invalid versionstring: \"%s\"\n", versionstr);
    return FALSE;
  }
  return TRUE;
}

/* Wrapper to use either _getcwd() (Win32) or getcwd() (POSIX.1) */
static char *
getcwd_wrapper(char * buf, size_t size)
{
#ifdef HAVE__GETCWD
  return _getcwd(buf, (int)size);
#elif defined(HAVE_GETCWD)
  return getcwd(buf, size);
#else /* HAVE_GETCWD */
  /* FIXME: abort compilation? pederb, 2003-08-18 */
  return NULL;
#endif /* ! HAVE_GETCWD */
}

/**************************************************************************/

/* Stores the name of the current working directory in the \a str
   argument.

   Returns TRUE if current working directory could be found, FALSE if
   not. If FALSE is returned, an error message will be stored in \a
   str.

   \a str must have been initialized before being passed to this
   function.

   The rationale behind this wrapper around the POSIX.1 getcwd()
   function is to abstract away the extra operations necessary to
   handle that the input buffer is guaranteed to be large enough.
*/
SbBool
coin_getcwd(cc_string * str)
{
  char buf[256], * dynbuf = NULL;
  size_t bufsize = sizeof(buf);
  char * cwd = getcwd_wrapper(buf, bufsize);

  while ((cwd == NULL) && (errno == ERANGE)) {
    bufsize *= 2;
    if (dynbuf != NULL) { free(dynbuf); }
    dynbuf = (char *)malloc(bufsize);
    cwd = getcwd_wrapper(dynbuf, bufsize);
  }
  if (cwd == NULL) { cc_string_set_text(str, strerror(errno)); }
  else { cc_string_set_text(str, cwd); }

  if (dynbuf != NULL) { free(dynbuf); }
  return cwd ? TRUE : FALSE;
}

/**************************************************************************/

/* Returns -1 if value equals negative infinity, +1 if it is equal to
   positive infinity, or 0 if the number is not infinite.

   Note that on some systems, this method will always return 0
   (i.e. false positives).
*/
int
coin_isinf(double value)
{
#ifdef HAVE_ISINF
  return isinf(value);
#elif defined(HAVE_FPCLASS)
  if (fpclass(value) == FP_NINF) { return -1; }
  if (fpclass(value) == FP_PINF) { return +1; }
  return 0;
#elif defined(HAVE__FPCLASS)
  if (_fpclass(value) == _FPCLASS_NINF) { return -1; }
  if (_fpclass(value) == _FPCLASS_PINF) { return +1; }
  return 0;
#else
  /* FIXME: it might be possible to investigate the fp bits and decide
     in a portable manner whether or not they represent an infinite. A
     groups.google.com search turned up inconclusive. 20030919
     mortene. */
  return 0;
#endif
}

/* Returns 0 if the bitpattern of the \a value argument is not a valid
   floating point number, otherwise returns non-zero.

   Note that on some systems, this method will always return 0
   (i.e. false positives).
*/
int
coin_isnan(double value)
{
#ifdef HAVE_ISNAN
  return isnan(value);
#elif defined(HAVE__ISNAN)
  return _isnan(value);
#elif defined(HAVE_FPCLASS)
  if (fpclass(value) == FP_SNAN) { return 1; }
  if (fpclass(value) == FP_QNAN) { return 1; }
  return 0;
#elif defined(HAVE__FPCLASS)
  if (_fpclass(value) == _FPCLASS_SNAN) { return 1; }
  if (_fpclass(value) == _FPCLASS_QNAN) { return 1; }
  return 0;
#else
  /* FIXME: it might be possible to investigate the fp bits and decide
     in a portable manner whether or not they represent a NaN. A
     groups.google.com search turned up inconclusive. 20030919
     mortene. */
  return 0;
#endif
}

/* Returns 0 if the bitpattern of the \a value argument is not a valid
   floating point number, or an infinite number, otherwise returns
   non-zero.

   Note that on some systems, this method will always return 1
   (i.e. false positives).
*/
int
coin_finite(double value)
{
#ifdef HAVE_FINITE
  return finite(value);
#elif defined(HAVE__FINITE)
  return _finite(value);
#else
  return !coin_isinf(value) && !coin_isnan(value);
#endif
}

/**************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
