#!/bin/sh
# Reproducer for the discarded I/O-failure status in stl_writer_destroy()
# (src/foreignfiles/steel.l) when writing a binary STL to a device that
# rejects every write (/dev/full). Requires a Debug (non-NDEBUG) build
# of libCoin, since the fix's writeok check compiles to nothing under
# NDEBUG (Release/RelWithDebInfo) -- see repro.cpp for the full
# explanation. Linux/POSIX-only and not part of the default `ctest`
# suite for that reason. Run manually against a Debug build tree's
# shared library:
#
#   testsuite/reproducers/stl-writer-devfull/run.sh /path/to/debug-build/lib
#
# Before the fix, or against a non-Debug build: exits 1, reporting that
# the write to /dev/full was reported as successful instead of
# aborting. After the fix, against a Debug build: exits 0, reporting
# that patching the binary STL header aborted via assert() as expected.

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/debug-build/lib   (directory containing a Debug libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}
INCDIR="$(cd "$LIBDIR/../include" && pwd)" || exit 2
SRCINCDIR="$(cd "$(pwd)/../../../include" && pwd)" || exit 2

"$CXX" -O0 -g -I"$SRCINCDIR" -I"$INCDIR" repro.cpp -o repro -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
echo "=== PASS ==="
