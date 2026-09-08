#!/bin/sh
# Reproducer for Coin issue #174. Two complementary checks:
#
#  - repro.cpp: direct, minimal check that SoTransformerDragger::drag()
#    and ::dragFinish() no longer abort via assert() when whatkind is
#    WHATKIND_NONE (the state dragStart() leaves behind when it can't
#    determine which part was picked).
#
#  - repro_surrogate.cpp: full end-to-end check driving the real
#    SoDragger::handleEvent() pick machinery (mouse-down, -move, -up)
#    against a picked path that satisfies none of dragStart()'s 20
#    recognized part names via the surrogate-part mechanism -- this
#    exercises dragStart()'s own early-return live, not just its
#    downstream effect on drag()/dragFinish().
#
#   testsuite/reproducers/transformerdragger-notfound-nocrash/run.sh /path/to/build/lib
#
# Prints PASS and exits 0 if both checks hold.

CDPATH= cd "$(dirname "$0")" || exit 2

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}
SRCINCLUDE="$(CDPATH= cd ../../.. && pwd)/include" || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

"$CXX" -O1 -g repro.cpp -o repro -I"$SRCINCLUDE" -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi

"$CXX" -O1 -g repro_surrogate.cpp -o repro_surrogate -I"$SRCINCLUDE" -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2
./repro_surrogate
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro_surrogate exited with status $status ===" >&2
  exit "$status"
fi
