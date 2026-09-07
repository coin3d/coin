#!/bin/sh
# Reproducer for issue #402: SoGLLazyElement::beginCaching()/endCaching()
# were not reentrant, so a nested caching scope (a SoShape's
# SoPrimitiveVertexCache validating while an ancestor SoSeparator's
# SoGLCacheList recording is already open) clobbered the enclosing scope's
# bookkeeping, causing the enclosing endCaching() to dereference a NULL
# pointer. Run manually against a build tree's shared library:
#
#   testsuite/reproducers/lazyelement-nested-caching-crash/run.sh /path/to/build/lib
#
# Before the fix: crashes (SIGSEGV) inside the second (outer) endCaching()
# call. After the fix: prints PASS and exits 0.
#
# Needs a working GLX context. If offscreen rendering fails to get direct
# rendering (common in headless/sandboxed environments), try:
#   COIN_GLX_PIXMAP_DIRECT_RENDERING=1 testsuite/reproducers/lazyelement-nested-caching-crash/run.sh ...

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}

"$CXX" -O1 -g repro.cpp -o repro -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -eq 2 ]; then
  echo "=== INCONCLUSIVE: repro couldn't get a GL context in this environment ===" >&2
  exit 2
elif [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
