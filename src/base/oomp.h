#ifndef COIN_BASE_OOMP_H
#define COIN_BASE_OOMP_H

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#include <cstdio>
#include <cstdlib>

/* For legacy entry points whose callers require a valid result and have no
   error path. Keep the diagnostic independent of Coin allocators. */
static inline void
coin_oom_abort(const char * operation)
{
  std::fputs("Coin: out of memory in ", stderr);
  std::fputs(operation, stderr);
  std::fputc('\n', stderr);
  std::abort();
}

#endif
