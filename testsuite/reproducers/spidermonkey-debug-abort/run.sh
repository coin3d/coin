#!/bin/sh
# Reproducer for src/glue/spidermonkey.cpp aborting (Debug builds) or
# silently leaving spidermonkey()->available TRUE over NULL function
# pointers (Release builds) when a SpiderMonkey library is found but
# doesn't export the ~2007-era flat C API this glue expects -- true of
# any currently-maintained SpiderMonkey release. Run manually against a
# build tree's shared library, with COIN_SPIDERMONKEY_LIBNAME pointing at
# an installed SpiderMonkey library that does NOT export the old API
# (e.g. Debian/Ubuntu's libmozjs-115):
#
#   COIN_SPIDERMONKEY_LIBNAME=libmozjs-115.so.0 \
#     testsuite/reproducers/spidermonkey-debug-abort/run.sh /path/to/build/lib
#
# Before the fix, in a Debug build: aborts (SIGABRT) inside spidermonkey()
# on the first missing symbol. Before the fix, in a Release build: no
# abort, but spidermonkey()->available incorrectly reads TRUE. After the
# fix: neither build type aborts, and available correctly reads FALSE.
#
# Without COIN_SPIDERMONKEY_LIBNAME set to a library that's actually
# installed and doesn't match this glue's API, this reproducer is
# inconclusive (exits 2) rather than a pass or a fail.

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}

"$CXX" -O0 -g repro.cpp -o repro -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export COIN_DEBUG_EXTRA=${COIN_DEBUG_EXTRA:-1}
./repro
status=$?
if [ "$status" -eq 2 ]; then
  echo "=== INCONCLUSIVE: see repro's own message above ===" >&2
  exit 2
elif [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
