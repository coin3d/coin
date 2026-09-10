// Reproducer for the unchecked ftell() result in cc_xml_load_file().
// run.sh compiles the private implementation directly into this executable,
// so cc_xml_load_file() does not need to be exported from the Coin DLL.
#include "xml/utils.h"

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

int
main()
{
  const char * path = "./input.fifo";
  unlink(path);
  if (mkfifo(path, 0600) != 0) {
    perror("mkfifo");
    return 2;
  }
  const pid_t pid = fork();
  if (pid < 0) {
    unlink(path);
    return 2;
  }
  if (pid == 0) {
    // The fixed reader returns after ftell() fails and can close the FIFO
    // before this write completes. EPIPE is therefore an expected outcome.
    signal(SIGPIPE, SIG_IGN);
    FILE * writer = fopen(path, "wb");
    if (!writer) _exit(2);
    const char message[] = "more than zero bytes";
    const size_t count = fwrite(message, 1, sizeof(message), writer);
    const int flushed = fflush(writer);
    const bool ok = (count == sizeof(message) && flushed == 0) || errno == EPIPE;
    fclose(writer);
    _exit(ok ? 0 : 2);
  }

  char * result = cc_xml_load_file(path);
  const bool rejected = result == NULL;
  delete [] result;
  int status = 0;
  pid_t waited;
  do { waited = waitpid(pid, &status, 0); } while (waited < 0 && errno == EINTR);
  unlink(path);
  if (waited != pid || !WIFEXITED(status) || WEXITSTATUS(status) != 0) return 2;
  if (!rejected) {
    std::fprintf(stderr,
                 "FAIL: cc_xml_load_file() returned non-NULL after ftell() failed\n");
    return 1;
  }
  std::fprintf(stderr,
               "PASS: cc_xml_load_file() returned NULL after ftell() failed\n");
  return 0;
}
