#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if HAVE_IO_H
#include <io.h> /* defines open() on MSVC++ 5.0 */
#endif /* HAVE_IO_H */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <stdio.h>
#include <stdarg.h>


#ifndef HAVE_VSNPRINTF

#if HAVE__VSNPRINTF

/* This is how it is defined in MSVC++ 5.0. */
#define vsnprintf _vsnprintf

#else /* !HAVE__VSNPRINTF */

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

  All external functions and macros in this function should be
  conforming to ANSI C3.159-1989 / ISO 9899 or POSIX (or both), as far
  as I can see.

  Compile with something like this (example is under GCC and Linux),
  to generate a stand-alone program testing the function:
    gcc -Wall -DENABLE_TESTCODE=1 -DHAVE_UNISTD_H=1 -DHAVE_VSNPRINTF=1 \
        -o vsnprintf vsnprintf.c

  19991221 mortene. Thanks to larsa for the idea.

 */
int
vsnprintf(char * target, size_t n, const char * formatstr, ...)
{
  int len;
  va_list argptr;
  static FILE * nullfp = NULL;

  if (!nullfp) {
    const char unixdevnull[] = "/dev/null";
    struct stat sbuf;
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

  va_start(argptr, formatstr);
  len = vfprintf(nullfp, formatstr, argptr);
  if ((unsigned int)len+1 > n) return -1;

  (void)vsprintf(target, formatstr, argptr);
  va_end(argptr);

  return len;
}

#endif /* !HAVE__VSNPRINTF */

#endif /* !HAVE_VSNPRINTF */


#if ENABLE_TESTCODE

#define BUFSIZE 256

int
vsnprintf_wrapper(char * target, size_t n, const char * formatstr, ...)
{
  int len;
  va_list argptr;
  va_start(argptr, formatstr);
  len = vsnprintf(target, n, formatstr, argptr);
  va_end(argptr);
  return len;
}

int
main(void)
{
  char strbuff[BUFSIZE];
  int nr = vsnprintf_wrapper(strbuff, BUFSIZE,
                             "This code written by %s %s, %04d-%02d-%02d",
                             "Morten", "Eriksen", 1999, 12, 21);

  if (nr != -1)
    (void)fprintf(stdout, "string: ``%s''\nstringlength: %d\n", strbuff, nr);
  else
    (void)fprintf(stderr, "buffer too small\n");

  return 0;
}
#endif /* ENABLE_TESTCODE */
