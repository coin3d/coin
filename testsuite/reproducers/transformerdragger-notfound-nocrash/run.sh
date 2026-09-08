#!/bin/sh
# Reproducer for Coin issue #174 -- see repro.cpp for the full
# explanation. Confirms that SoTransformerDragger::drag() and
# ::dragFinish() no longer abort via assert() when whatkind is
# WHATKIND_NONE (the state dragStart() leaves behind when it can't
# determine which part was picked).
#
#   testsuite/reproducers/transformerdragger-notfound-nocrash/run.sh /path/to/build/lib
#
# Prints PASS and exits 0 if the check holds.

CDPATH= cd "$(dirname "$0")" || exit 2

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}
SRCINCLUDE="$(CDPATH= cd ../../.. && pwd)/include" || exit 2

"$CXX" -O1 -g repro.cpp -o repro -I"$SRCINCLUDE" -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
