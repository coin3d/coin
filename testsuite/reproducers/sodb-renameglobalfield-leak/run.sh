#!/bin/sh
# Investigation repro for a suspected leak in
# SoDB::renameGlobalField(name, "") -- see repro.cpp for the full
# explanation. Run against an ASan build (LeakSanitizer enabled) to see
# whether the discarded global fields were actually freed:
#
#   testsuite/reproducers/sodb-renameglobalfield-leak/run.sh /path/to/asan-build/lib
#
# If leaked: LeakSanitizer reports "SUMMARY: ... byte(s) leaked" with a
# stack trace through SoDB::createGlobalField() / SoGlobalField::
# SoGlobalField() on exit, and this script exits non-zero.
# If not leaked: prints PASS and exits 0.

CDPATH= cd "$(dirname "$0")" || exit 2

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/asan-build/lib   (directory containing libCoin.so)" >&2
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
  echo "=== leak detected (or crash): repro exited with status $status ===" >&2
  exit "$status"
fi
echo "=== PASS: no leak detected ==="
