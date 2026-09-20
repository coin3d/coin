#!/bin/sh
set -eu

if [ "$#" -ne 1 ]; then
  echo "usage: $0 /path/to/build/lib" >&2
  exit 2
fi
if [ ! -d /dev/shm ] || [ ! -w /dev/shm ]; then
  echo "/dev/shm is unavailable or not writable" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "/dev/shm/coin-sopath-append.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

failures=0
for case_name in $("$OUT/repro" --list); do
  if ! "$OUT/repro" "$case_name"; then
    echo "FAIL: $case_name" >&2
    failures=$((failures + 1))
  fi
done

test "$failures" -eq 0
