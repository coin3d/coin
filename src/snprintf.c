/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

/*
  This file containes replacement code for snprintf() and vsnprintf()
  on systems where these extension functions are not available.
*/


#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>


#if !HAVE_VSNPRINTF

#if HAVE__VSNPRINTF

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
  if ((unsigned int)len+1 > n) return -1;
  (void)vsprintf(target, formatstr, args);
  return len;
}

#endif /* !HAVE__VSNPRINTF */

#endif /* !HAVE_VSNPRINTF */


/**************************************************************************/

#if !HAVE_SNPRINTF

#if HAVE__SNPRINTF

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
