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
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-callback-copy-oracle.XXXXXX")
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
  echo "UNKNOWN S-B: replacement allocator does not interpose across the Coin DSO" >&2
  exit 77
fi
[ "$STATUS" -eq 0 ] || exit "$STATUS"

for control in positive-chain self self; do
  run_status "$control"
  [ "$STATUS" -eq 0 ] || { echo "VIOLATED S-B control $control" >&2; exit 1; }
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
      [ "$saw_throw" -eq 1 ] || { echo "VIOLATED S-B $case_name: no reachable bad_alloc" >&2; exit 1; }
      DISCOVERED=$k
      return
    elif [ "$STATUS" -eq 77 ]; then
      echo "UNKNOWN S-B $case_name" >&2
      exit 77
    else
      echo "VIOLATED S-B $case_name fail_after=$k" >&2
      exit 1
    fi
    k=$((k + 1))
  done
  echo "VIOLATED S-B $case_name: injection sweep exceeded 64" >&2
  exit 1
}

replay_forward()
{
  case_name=$1 max=$2 k=0
  while [ "$k" -le "$max" ]; do
    run_status "$case_name" "$k"
    expected=0; [ "$k" -eq "$max" ] && expected=10
    [ "$STATUS" -eq "$expected" ] || { echo "VIOLATED S-B nondeterminism $case_name fail_after=$k" >&2; exit 1; }
    k=$((k + 1))
  done
}

replay_reverse()
{
  case_name=$1 k=$2
  while [ "$k" -ge 0 ]; do
    run_status "$case_name" "$k"
    expected=0; [ "$k" -eq "$2" ] && expected=10
    [ "$STATUS" -eq "$expected" ] || { echo "VIOLATED S-B reverse nondeterminism $case_name fail_after=$k" >&2; exit 1; }
    k=$((k - 1))
  done
}

for case_name in copy-raw copy-typed copy-mixed assign-raw-mixed \
                 assign-typed-raw assign-mixed-typed assign-mixed-mixed; do
  discover "$case_name"
  max=$DISCOVERED
  replay_forward "$case_name" "$max"
  replay_reverse "$case_name" "$max"
  replay_reverse "$case_name" "$max"
done

echo "PRESERVED S-B: deterministic fresh-process copy/assignment oracle passed"
