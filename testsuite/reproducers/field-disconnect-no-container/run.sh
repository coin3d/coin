#!/bin/sh
# Reproducer for a NULL-pointer dereference in
# SoField::disconnect(SoEngineOutput*)'s COIN_DEBUG_EXTRA diagnostic, which
# accessed a field's container without the usual NULL-safety check. Only
# reachable in a Debug (COIN_DEBUG) build with COIN_DEBUG_EXTRA=1 and
# COIN_WARNING_LEVEL=3 set. Run manually against a Debug build tree's
# shared library:
#
#   COIN_DEBUG_EXTRA=1 COIN_WARNING_LEVEL=3 \
#     testsuite/reproducers/field-disconnect-no-container/run.sh /path/to/debug-build/lib
#
# Before the fix: crashes (SIGSEGV). After the fix: prints PASS and exits 0.

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/debug-build/lib   (directory containing a Debug libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}

"$CXX" -O0 -g repro.cpp -o repro -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export COIN_DEBUG_EXTRA=${COIN_DEBUG_EXTRA:-1}
export COIN_WARNING_LEVEL=${COIN_WARNING_LEVEL:-3}
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
