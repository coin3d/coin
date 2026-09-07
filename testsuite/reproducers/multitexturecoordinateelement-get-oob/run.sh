#!/bin/sh
# Reproducer for an out-of-bounds array read in
# SoMultiTextureCoordinateElement::get2()/get3()/get4()
# (src/elements/SoMultiTextureCoordinateElement.cpp) when a shape's
# textureCoordIndex references an index beyond the number of texture
# coordinates actually supplied. See repro.cpp for the full explanation.
#
#   testsuite/reproducers/multitexturecoordinateelement-get-oob/run.sh /path/to/build/lib
#
# Before the fix: assert() abort (Debug) or a silent out-of-bounds heap
# read (Release) inside SoMultiTextureCoordinateElement::get4().
# After the fix: prints PASS and exits 0.

cd "$(dirname "$0")"

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
SRCINCLUDE="$(cd "$(dirname "$0")/../../.." && pwd)/include"

"$CXX" -O1 -g repro.cpp -o repro -I"$SRCINCLUDE" -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
