// Reproducer for two related bugs in src/glue/spidermonkey.cpp's
// spidermonkey():
//
// 1. REGISTER_FUNC() only assert()s a missing symbol, a no-op in Release
//    builds -- so sm->available stayed optimistically TRUE with NULL
//    function pointers underneath whenever a library opened successfully
//    but didn't export this glue's expected ~2007-era flat C API (true of
//    any currently-maintained SpiderMonkey, which exports a name-mangled
//    C++ API instead). Callers that correctly check
//    spidermonkey()->available before using the API would be misled into
//    calling through NULL function pointers.
//
// 2. In a Debug build, that same assert() does NOT no-op -- it fires and
//    aborts the whole process on the very first missing symbol. A
//    "library found, but it's a version this glue can't drive" is an
//    ordinary, expected runtime condition, not a programmer-error
//    invariant violation, so it should be handled identically in Debug
//    and Release (gracefully mark unavailable), not crash only in Debug.
//
// A later fix attempt addressed (1) by re-deriving availability from a
// hand-picked subset of 5 "important" functions after all REGISTER_FUNC
// calls -- but that subset didn't match what SoJavaScriptEngine.cpp and
// JS_VRMLClasses.cpp actually call (over 90 distinct functions between
// them; the check didn't even include an actually-used one,
// JS_ExecuteScript, while including JS_EvaluateScript, which neither
// caller calls at all), so partial-API-match libraries could still slip
// through with available=1. It also did nothing about (2): the
// process-killing assert() still runs, and still aborts before that
// later check is ever reached.
//
// This reproducer needs a real SpiderMonkey library installed that does
// NOT export this glue's expected old C API -- true of essentially any
// current distro package (e.g. Debian/Ubuntu's libmozjs-115). Point
// COIN_SPIDERMONKEY_LIBNAME at it. See run.sh in this directory.

#include <cstdio>
#include <cstdlib>

extern "C" {
#include <Inventor/C/glue/spidermonkey.h>
}

int main()
{
  const char * libname = getenv("COIN_SPIDERMONKEY_LIBNAME");
  if (!libname || !libname[0]) {
    fprintf(stderr, "[repro] COIN_SPIDERMONKEY_LIBNAME not set -- inconclusive, "
                    "point it at an installed SpiderMonkey library (e.g. "
                    "libmozjs-115.so.0) to run this\n");
    return 2;
  }

  // Debug builds: if the assert() bug is present, this call never
  // returns -- the process aborts (SIGABRT) inside spidermonkey() itself,
  // on the first symbol from this glue's expected API that the installed
  // library doesn't export.
  const SpiderMonkey_t * sm = spidermonkey();

  fprintf(stderr, "[repro] spidermonkey()->available = %d\n", sm->available);

  // Regardless of build type: a library that doesn't export this glue's
  // full expected API must leave available FALSE, not TRUE with some
  // functions actually still NULL underneath.
  if (sm->available) {
    fprintf(stderr, "[repro] FAIL: available=1, but this reproducer assumes "
                    "the configured library does not export this glue's "
                    "old API (if it does, this check is inconclusive for "
                    "this library -- try a newer SpiderMonkey package)\n");
    return 1;
  }

  fprintf(stderr, "[repro] PASS: survived without aborting, and available "
                  "correctly reports FALSE\n");
  return 0;
}
