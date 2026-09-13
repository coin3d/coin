#!/bin/sh
# Reproducer for a NULL-pointer dereference in
# SoIndexedTriangleStripSet::generatePrimitives()
# (src/shapenodes/SoIndexedTriangleStripSet.cpp) when materialBinding is
# PER_PART_INDEXED (or PER_FACE_INDEXED) but materialIndex is left empty
# on the shape. See repro.cpp for the full explanation.
#
#   testsuite/reproducers/indexedtrianglestripset-perstrip-indexed-null/run.sh /path/to/build/lib
#
# Before the fix: SIGSEGV inside SoIndexedTriangleStripSet::generatePrimitives().
# After the fix: prints PASS and exits 0.

CDPATH= cd "$(dirname "$0")" || exit 2

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}

# Source tree's own public headers *must* come before the build dir's
# generated-headers-only include dir and any system-installed Coin
# (e.g. libcoin-dev) -- otherwise -I"$LIBDIR/../include" alone silently
# falls through to /usr/include/Inventor, compiling this reproducer
# against a different (and possibly ABI-incompatible) Coin version than
# the one $LIBDIR/libCoin.so was actually built from.
# The current directory is already the reproducer directory.
SRCINCLUDE="$(CDPATH= cd ../../.. && pwd)/include" || exit 2

"$CXX" -O1 -g repro.cpp -o repro -I"$SRCINCLUDE" -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
