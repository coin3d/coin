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

/*
  This file containes replacement code for snprintf() and vsnprintf()
  on systems where these extension functions are not available.
*/

/* FIXME: now also with portable abstraction coin_getenv() for reading
   system environment variables. Should therefore rename this
   file accordingly. 20010821 mortene. */

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_WINDOWS_H
#include <windows.h> /* GetEnvironmentVariable() */
#endif /* HAVE_WINDOWS_H */

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
#include <stdarg.h>

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

#ifdef _WIN32 /* FIXME: use configure check to test for GetEnvironmentVariable(). 20010821 mortene. */
#define HAVE_GETENVIRONMENTVARIABLE 1
#endif /* _WIN32 */

/**************************************************************************/

/*** Singlelinked list for the environment variables. *********************/

/* FIXME: should implement a generic (macro-based) singlelinked list
   abstraction in C (the following code could be a good starting
   point). Then axe this code. Then move the various listhandling
   stuff in the Coin library from the SbList<> template to the C-based
   one to aid any future transition to pure C. 20010821 mortene. */

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
*/
const char *
coin_getenv(const char * envname)
{
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

#if ENABLE_TESTCODE

#define BUFSIZE 256

int
main(void)
{
  char strbuff[BUFSIZE];
  int nr = snprintf(strbuff, BUFSIZE,
                    "This code written by %s %s, %04d-%02d-%02d",
                    "Morten", "Eriksen", 1999, 12, 21);

  if (nr != -1)
    (void)fprintf(stdout, "string: ``%s''\nstringlength: %d\n", strbuff, nr);
  else
    (void)fprintf(stderr, "buffer too small\n");

  return 0;
}
#endif /* ENABLE_TESTCODE */
