#!/bin/sh
# Build and run the GLX regression against an out-of-tree Coin build.

set -eu

if [ "$#" -ne 1 ]; then
  echo "usage: $0 /absolute/path/to/build/lib" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-bumprender-context.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

"$CXX" ${CXXFLAGS:-} -std=c++11 -O1 -g \
  "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$LIBDIR/../include" -I"$SOURCE/include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin -lGL -lX11

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

set +e
"$OUT/repro"
status=$?
set -e

case "$status" in
  0)
    echo "=== PASS: bump programs are context-correct and lifecycle-safe ==="
    ;;
  2)
    echo "=== INCONCLUSIVE: no suitable GLX compatibility context/ARB path ===" >&2
    ;;
  *)
    echo "=== FAIL: reproducer exited with status $status ===" >&2
    ;;
esac
exit "$status"
