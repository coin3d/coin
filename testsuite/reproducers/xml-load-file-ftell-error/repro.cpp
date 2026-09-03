// Reproducer for a -Wsign-compare bug in cc_xml_load_file()
// (src/xml/utils.cpp): ftell()'s return value was never checked for
// -1 (error), which happens whenever the underlying stream doesn't
// support seeking -- e.g. a FIFO/pipe. On error, bufsize became -1,
// `new char[bufsize + 1]` allocated a ZERO-byte buffer, and the
// resulting unsigned/signed mismatch in `pos != bufsize` /
// `bufsize - pos` asked fread() to read up to SIZE_MAX bytes into
// that 0-byte buffer.
//
// This uses a POSIX named pipe (mkfifo) -- a completely ordinary way
// for ftell() to legitimately fail on Linux/macOS -- to reproduce it
// against the real, exported cc_xml_load_file() symbol. Not portable
// to Windows (no fork()/mkfifo()), which is why this lives here as a
// standalone reproducer instead of in the cross-platform CoinTests
// suite; see load_file_roundtrip in src/xml/utils.cpp for the
// portable regression test covering the (fixed) happy path.
//
// See run.sh in this directory for how to build and run this against
// a given libCoin build.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern "C" char * cc_xml_load_file(const char * path);

int main()
{
  const char * fifopath = "./xml_load_file_repro.fifo";
  unlink(fifopath);
  if (mkfifo(fifopath, 0600) != 0) {
    perror("mkfifo");
    return 2;
  }

  pid_t pid = fork();
  if (pid == 0) {
    // child: writer -- feed some bytes into the pipe
    FILE * wf = fopen(fifopath, "wb");
    if (wf) {
      const char msg[] = "hello from the pipe, this is more than zero bytes of data";
      fwrite(msg, 1, sizeof(msg), wf);
      fclose(wf);
    }
    _exit(0);
  }

  std::fprintf(stderr, "[repro] calling cc_xml_load_file() on a non-seekable FIFO\n");
  std::fflush(stderr);

  char * result = cc_xml_load_file(fifopath);

  std::fprintf(stderr, "[repro] returned %s (did not crash) -- PASS\n",
               result ? "non-NULL" : "NULL");

  int status = 0;
  waitpid(pid, &status, 0);
  unlink(fifopath);

  delete[] result;
  return 0;
}
