#!/bin/sh
set -u

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
  echo "usage: $0 /path/to/build/lib [forward|reverse]" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd) || exit 2
ORDER=${2:-forward}
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-sopath-range.XXXXXX") || exit 2
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

if ! "$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin; then
  echo "[suite] UNKNOWN: compile/link failed" >&2
  exit 2
fi

forward_cases='truncate_negative_empty truncate_negative_nonempty truncate_above_empty truncate_above_nonempty truncate_zero truncate_current truncate_intermediate sopath_pop_empty sofullpath_pop_empty sopath_pop_nonempty sofullpath_pop_nonempty'
reverse_cases='sofullpath_pop_nonempty sopath_pop_nonempty sofullpath_pop_empty sopath_pop_empty truncate_intermediate truncate_current truncate_zero truncate_above_nonempty truncate_above_empty truncate_negative_nonempty truncate_negative_empty'
case "$ORDER" in
  forward) cases=$forward_cases ;;
  reverse) cases=$reverse_cases ;;
  *) echo "order must be forward or reverse" >&2; exit 2 ;;
esac

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
violated=0
for name in $cases; do
  "$OUT/repro" "$name"
  status=$?
  if [ "$status" -eq 2 ]; then
    echo "RESULT $name Unknown"
    exit 2
  elif [ "$status" -eq 0 ]; then
    echo "RESULT $name Preserved"
  else
    echo "RESULT $name Violated"
    violated=$((violated + 1))
  fi
done

echo "SUMMARY order=$ORDER preserved=$((11 - violated)) violated=$violated unknown=0"
test "$violated" -eq 0
