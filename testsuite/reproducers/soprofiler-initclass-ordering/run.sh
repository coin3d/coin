#!/bin/sh
# Run all startup modes in fresh processes. See README.md.
set -eu
if [ "$#" -ne 1 ]; then
  echo "usage: $0 /path/to/build/lib" >&2
  exit 2
fi
LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
CMAKE=${CMAKE:-cmake}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-profiler-init.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM
# Intentional flag splitting supports instrumented library/test builds.
"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" ${LDFLAGS:-} -lCoin
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
for mode in unset off 0 on 1 syncgl invalid; do
  "$CMAKE" -DTEST_EXECUTABLE="$OUT/repro" -DPROFILE_MODE="$mode" -P "$HERE/verify.cmake"
done
