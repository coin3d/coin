#!/bin/sh
# Verification repro for the SoScale/SoRotation/SoRotationXYZ identity
# optimization -- see repro.cpp for the full explanation. Checks
# accumulated matrix and bounding box across identity -> non-identity ->
# identity transitions, with caching on throughout.
#
#   testsuite/reproducers/identity-transform-node-cache-check/run.sh /path/to/build/lib
#
# Prints PASS and exits 0 if every check passes.

CDPATH= cd "$(dirname "$0")" || exit 2

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}
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
