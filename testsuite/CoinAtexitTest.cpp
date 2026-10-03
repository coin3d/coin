#include <Inventor/SoDB.h>
#include <Inventor/C/tidbits.h>

#include "tidbitsp.h"

#include <cstdio>
#if !defined(_WIN32)
#include <csignal>
#include <sys/wait.h>
#include <unistd.h>
#endif

static int calls[4];
static int count = 0;
static bool exiting_in_callback = false;

static void
first_callback(void)
{
  calls[count++] = 1;
}

static void
recursive_callback(void)
{
  calls[count++] = 2;
  exiting_in_callback = coin_is_exiting() != FALSE;
  coin_atexit_cleanup();
}

static void
last_callback(void)
{
  calls[count++] = 3;
}

static void
second_round_callback(void)
{
  calls[count++] = 4;
}

static void
register_during_cleanup(void)
{
  cc_coin_atexit(first_callback);
}

int
main(void)
{
  SoDB::init();
  cc_coin_atexit(first_callback);
  cc_coin_atexit(recursive_callback);
  cc_coin_atexit(last_callback);
  SoDB::finish();

  if (count != 3 || calls[0] != 3 || calls[1] != 2 || calls[2] != 1 ||
      !exiting_in_callback || coin_is_exiting()) {
    std::fprintf(stderr, "coin_atexit order or recursive cleanup failed\n");
    return 1;
  }

  SoDB::init();
  cc_coin_atexit(second_round_callback);
  SoDB::finish();
  if (count != 4 || calls[3] != 4 || coin_is_exiting()) {
    std::fprintf(stderr, "coin_atexit could not restart after cleanup\n");
    return 1;
  }

#if !defined(_WIN32)
  const pid_t child = fork();
  if (child < 0) {
    std::perror("fork");
    return 1;
  }
  if (child == 0) {
    SoDB::init();
    cc_coin_atexit(register_during_cleanup);
    SoDB::finish();
    _exit(0);
  }
  int status = 0;
  if (waitpid(child, &status, 0) != child ||
      !WIFSIGNALED(status) || WTERMSIG(status) != SIGABRT) {
    std::fprintf(stderr, "registration during cleanup did not abort\n");
    return 1;
  }
#endif
  return 0;
}
