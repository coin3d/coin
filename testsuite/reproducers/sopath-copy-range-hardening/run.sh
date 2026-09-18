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
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-sopath-copy.XXXXXX") || exit 2
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

if ! command -v timeout >/dev/null 2>&1; then
  echo "[suite] UNKNOWN: timeout command unavailable" >&2
  exit 2
fi

if ! "$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin; then
  echo "[suite] UNKNOWN: compile/link failed" >&2
  exit 2
fi

forward_cases='empty_default start_negative start_equal_full_length start_above_full_length start_int_max count_negative count_int_min full_default full_explicit partial_from_head partial_new_head_one partial_new_head_exact zero_to_end_new_head excessive_from_head excessive_new_head int_max_count hidden_full_default hidden_start_after_visible temppath_result_audits'
reverse_cases='temppath_result_audits hidden_start_after_visible hidden_full_default int_max_count excessive_new_head excessive_from_head zero_to_end_new_head partial_new_head_exact partial_new_head_one partial_from_head full_explicit full_default count_int_min count_negative start_int_max start_above_full_length start_equal_full_length start_negative empty_default'
case "$ORDER" in
  forward) cases=$forward_cases ;;
  reverse) cases=$reverse_cases ;;
  *) echo "order must be forward or reverse" >&2; exit 2 ;;
esac

if ! ulimit -v 524288 2>/dev/null; then
  echo "[suite] UNKNOWN: could not apply address-space limit" >&2
  exit 2
fi

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
preserved=0
violated=0
unknown=0
for name in $cases; do
  timeout 10s "$OUT/repro" "$name"
  status=$?
  if [ "$status" -eq 2 ]; then
    echo "RESULT $name Unknown"
    unknown=$((unknown + 1))
  elif [ "$status" -eq 0 ]; then
    echo "RESULT $name Preserved"
    preserved=$((preserved + 1))
  else
    echo "RESULT $name Violated"
    violated=$((violated + 1))
  fi
done

echo "SUMMARY order=$ORDER preserved=$preserved violated=$violated unknown=$unknown"
test "$unknown" -eq 0 && test "$violated" -eq 0
