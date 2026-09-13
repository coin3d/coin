// Reproducer for the discarded I/O-failure status in stl_writer_destroy()
// (src/foreignfiles/steel.l), part of the binary STL writer: after all
// facets have been written, this function seeks back to the file's
// facet-count field and patches it in with the real count, tracking the
// fflush()/fseek()/fwrite() results in a `writeok` accumulator that was
// set but never checked.
//
// The fix (branch fix/unused-but-set-variable-clang, which this branch
// is stacked on) adds `if (!writeok) { assert(!"failed to write binary
// STL facet count to header"); }` right after that sequence. assert()
// compiles to nothing under NDEBUG (Release/RelWithDebInfo), matching
// this project's established behavior-preserving fix for every other
// -Wunused-but-set-variable site in that branch -- so this reproducer
// is only observable against a Debug (non-NDEBUG) build of libCoin.
//
// /dev/full is used to force a real, deterministic write failure: it
// accepts writes into the C library's stdio buffer without complaint,
// but every actual write() syscall against it fails with ENOSPC once
// the buffer is flushed -- which is exactly what
// SoSTLFileKit::writeFile()'s binary path does once per facet (small,
// buffered fwrite() calls) and then, in stl_writer_destroy(), an
// explicit fflush() to push all of that through before seeking back to
// patch the header. Linux/POSIX-only, which is why this lives here as
// a standalone reproducer instead of in the cross-platform CoinTests
// suite.
//
// The failing write happens deep inside library code invoked from
// SoSTLFileKit::writeFile(), so this runs it in a forked child process
// and checks whether that child was killed by SIGABRT, rather than
// letting an abort take down this reproducer itself.
//
// Before the fix, or in a Release/RelWithDebInfo build: writeFile()
// returns TRUE regardless (both before and after the fix -- every
// caller up the chain already discards stl_writer_put_binary_facet()'s
// and stl_writer_destroy()'s return values, so this specific fix has
// no effect on writeFile()'s own return value; see the commit message
// for fix/unused-but-set-variable-clang for the full picture) and the
// child exits normally despite every byte having failed to write.
// After the fix, in a Debug build: the header-patching step aborts via
// assert() instead of silently reporting the (bogus) success it always
// has.
//
// See run.sh in this directory for how to build and run this against a
// given Debug libCoin build.

#include "../CoinCleanup.h"
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/annex/ForeignFiles/SoSTLFileKit.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>

int
main()
{
  struct stat st;
  if (stat("/dev/full", &st) != 0) {
    fprintf(stderr, "[repro] SKIP: /dev/full is not available on this system "
            "(this reproducer is Linux/POSIX-only)\n");
    return 0;
  }

  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoInteraction::init();

  SoSeparator * scene = new SoSeparator;
  scene->ref();
  scene->addChild(new SoCube);

  pid_t pid = fork();
  if (pid == 0) {
    SoSTLFileKit * kit = new SoSTLFileKit;
    kit->ref();
    kit->binary = TRUE;
    SbBool readok = kit->readScene(scene);
    if (!readok) {
      fprintf(stderr, "[repro-child] readScene() unexpectedly failed\n");
      kit->unref();
      _exit(2);
    }
    SbBool writeok = kit->writeFile("/dev/full");
    fprintf(stderr, "[repro-child] writeFile(\"/dev/full\") returned %s "
            "without aborting, despite every byte having failed to write "
            "-- writeFile()'s own return value is unaffected by this fix "
            "(see repro.cpp), this is expected in a Release/RelWithDebInfo "
            "build\n", writeok ? "TRUE" : "FALSE");
    kit->unref();
    _exit(0);
  }

  int status = 0;
  waitpid(pid, &status, 0);
  scene->unref();

  if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
    fprintf(stderr, "[repro] PASS: child aborted via assert() while "
            "patching the binary STL header after every write to "
            "/dev/full failed, as expected from the fix in a Debug "
            "build\n");
    return 0;
  }

  fprintf(stderr, "[repro] FAIL: child did not abort (wait status 0x%x). "
          "This reproducer requires a Debug (non-NDEBUG) build of "
          "libCoin, since the fix's writeok check compiles to nothing "
          "under NDEBUG -- rebuild with -DCMAKE_BUILD_TYPE=Debug and "
          "point run.sh at that build's lib directory.\n", status);
  return 1;
}
