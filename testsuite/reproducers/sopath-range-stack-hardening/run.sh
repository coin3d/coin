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
OUT=$(mktemp -d "/dev/shm/coin-sopath-range.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
cases='truncate_negative_empty truncate_negative_nonempty truncate_above_empty truncate_above_nonempty truncate_zero truncate_current truncate_intermediate sopath_pop_empty sofullpath_pop_empty sopath_pop_nonempty sofullpath_pop_nonempty'
for case_name in $cases; do
  if ! "$OUT/repro" "$case_name"; then
    echo "FAIL: $case_name" >&2
    exit 1
  fi
done
