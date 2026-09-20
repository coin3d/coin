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
if ! command -v timeout >/dev/null 2>&1; then
  echo "timeout command is unavailable" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "/dev/shm/coin-sopath-copy.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin
if ! ulimit -v 524288 2>/dev/null; then
  echo "could not apply the address-space limit" >&2
  exit 2
fi

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
cases='empty_default start_negative start_equal_full_length start_above_full_length start_int_max count_negative count_int_min full_default full_explicit partial_from_head partial_new_head_one partial_new_head_exact zero_to_end_new_head excessive_from_head excessive_new_head int_max_count hidden_full_default hidden_start_after_visible temppath_result_audits'
for case_name in $cases; do
  if ! timeout 10s "$OUT/repro" "$case_name"; then
    echo "FAIL: $case_name" >&2
    exit 1
  fi
done
