#!/bin/sh
set -eu
if [ "$#" -ne 1 ]; then
  echo "usage: $0 /path/to/build/lib" >&2
  exit 2
fi
LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-callback-add-oracle.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM
"$CXX" ${CXXFLAGS:-} -std=c++11 -O1 -g -rdynamic "$HERE/repro.cpp" \
  -o "$OUT/repro" -I"$SOURCE/include" -I"$LIBDIR/../include" \
  -L"$LIBDIR" ${LDFLAGS:-} -lCoin
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

run_status()
{
  if "$OUT/repro" "$@"; then STATUS=0; else STATUS=$?; fi
}

run_status probe
if [ "$STATUS" -eq 77 ]; then
  echo "UNKNOWN S-A: replacement allocator does not interpose across the Coin DSO" >&2
  exit 77
fi
[ "$STATUS" -eq 0 ] || exit "$STATUS"

for control in raw-empty-control positive-ops; do
  run_status "$control"
  [ "$STATUS" -eq 0 ] || { echo "VIOLATED S-A control $control" >&2; exit 1; }
done

discover()
{
  case_name=$1
  k=0
  saw_throw=0
  while [ "$k" -le 64 ]; do
    run_status "$case_name" "$k"
    if [ "$STATUS" -eq 0 ]; then
      saw_throw=1
    elif [ "$STATUS" -eq 10 ]; then
      [ "$saw_throw" -eq 1 ] || { echo "VIOLATED S-A $case_name: no reachable bad_alloc" >&2; exit 1; }
      DISCOVERED=$k
      return
    elif [ "$STATUS" -eq 77 ]; then
      echo "UNKNOWN S-A $case_name" >&2
      exit 77
    else
      echo "VIOLATED S-A $case_name fail_after=$k" >&2
      exit 1
    fi
    k=$((k + 1))
  done
  echo "VIOLATED S-A $case_name: injection sweep exceeded 64" >&2
  exit 1
}

replay_forward()
{
  case_name=$1 max=$2 k=0
  while [ "$k" -le "$max" ]; do
    run_status "$case_name" "$k"
    expected=0; [ "$k" -eq "$max" ] && expected=10
    [ "$STATUS" -eq "$expected" ] || { echo "VIOLATED S-A nondeterminism $case_name fail_after=$k" >&2; exit 1; }
    k=$((k + 1))
  done
}

replay_reverse()
{
  case_name=$1 k=$2
  while [ "$k" -ge 0 ]; do
    run_status "$case_name" "$k"
    expected=0; [ "$k" -eq "$2" ] && expected=10
    [ "$STATUS" -eq "$expected" ] || { echo "VIOLATED S-A reverse nondeterminism $case_name fail_after=$k" >&2; exit 1; }
    k=$((k - 1))
  done
}

for case_name in raw-growth-raw raw-growth-typed typed-empty typed-mixed; do
  discover "$case_name"
  max=$DISCOVERED
  replay_forward "$case_name" "$max"
  replay_reverse "$case_name" "$max"
  replay_reverse "$case_name" "$max"
done

echo "PRESERVED S-A: deterministic fresh-process add oracle passed"
