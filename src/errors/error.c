#include <Inventor/C/errors/error.h>
#include <stdio.h>
#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_UNISTD_H
#include <unistd.h> // STDERR_FILENO
#endif // HAVE_UNISTD_H

#ifndef STDERR_FILENO
// stderr should always be on file descriptor 2, according to POSIX.
#define STDERR_FILENO 2
#endif // STDERR_FILENO


/* FIXME: should be hidden from public API, and only visible to
   subclasses. 20020526 mortene. */
void
cc_default_handler_cb(const cc_error * err, void * data)
{
  // It is not possible to "pass" C library data from the application
  // to a MSWin .DLL, so this is necessary to get hold of the stderr
  // FILE*. Just using fprintf(stderr, ...) or fprintf(stdout, ...)
  // directly will result in a crash when Coin has been compiled as a
  // .DLL.
  static FILE * coin_stderr = NULL;
  if (!coin_stderr) { coin_stderr = fdopen(STDERR_FILENO, "w"); }

  if (coin_stderr) {
    const cc_string * str = cc_error_get_debug_string(err);
    (void)fprintf(coin_stderr, "%s\n", cc_string_get_text(str));
    (void)fflush(coin_stderr);
  }
}

static cc_error_cb * callback = cc_default_handler_cb;
static void * callback_data = NULL;


void
cc_error_construct(cc_error * me)
{
  cc_string_construct(&(me->debugstring));
}

void
cc_error_clean(cc_error * me)
{
  cc_string_clean(&(me->debugstring));
}

void
cc_error_set_debug_string(cc_error * me, const char * str)
{
  cc_string_set_text(&(me->debugstring), str);
}

void
cc_error_append_to_debug_string(cc_error * me, const char * str)
{
  cc_string_append_text(&(me->debugstring), str);
}

void
cc_error_handle(cc_error * me)
{
  void * arg = NULL;
  cc_error_cb * function = cc_error_get_handler(&arg);
  assert(function != NULL);
  (*function)(me, arg);
}

cc_error_cb *
cc_error_get_handler(void ** data)
{
  *data = callback_data;
  return callback;
}

const cc_string *
cc_error_get_debug_string(const cc_error * me)
{
  return &(me->debugstring);
}
