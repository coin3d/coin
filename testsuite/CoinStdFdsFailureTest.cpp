#include "tidbitsp.h"

#include <cstdio>
#include <fcntl.h>
#include <sys/resource.h>

extern "C" void free_std_fds(void);

int
main(void)
{
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) return 77;
  }

  struct rlimit original;
  if (getrlimit(RLIMIT_NOFILE, &original) != 0 || original.rlim_max < 3) {
    return 77;
  }

  struct rlimit exhausted = original;
  exhausted.rlim_cur = 3;
  if (setrlimit(RLIMIT_NOFILE, &exhausted) != 0) return 77;

  if (coin_get_stdin() != NULL || coin_get_stdout() != NULL ||
      coin_get_stderr() != NULL) {
    std::fprintf(stderr, "standard stream opened without a saved descriptor\n");
    return 1;
  }
  free_std_fds();
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) {
      std::fprintf(stderr, "standard descriptor %d was closed\n", fd);
      return 1;
    }
  }

  if (setrlimit(RLIMIT_NOFILE, &original) != 0 ||
      coin_get_stdin() == NULL || coin_get_stdout() == NULL ||
      coin_get_stderr() == NULL) {
    std::fprintf(stderr, "standard streams could not be reopened\n");
    return 1;
  }
  free_std_fds();
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) return 1;
  }
  return 0;
}
