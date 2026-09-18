#!/bin/sh
set -u

if [ "$#" -lt 1 ] || [ "$#" -gt 3 ]; then
  echo "usage: $0 /path/to/build/lib [forward|reverse] [baseline-ref]" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd) || exit 2
ORDER=${2:-forward}
BASE=${3:-2adeaa8a039067a76d098e8e2be52b431f97d18e}
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
ROOT=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-sopath-copy-old.XXXXXX") || exit 2
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

if ! command -v timeout >/dev/null 2>&1; then
  echo "[old-header-suite] UNKNOWN: timeout command unavailable" >&2
  exit 2
fi

if ! git -C "$ROOT" archive --output="$OUT/headers.tar" "$BASE" include/Inventor ||
   ! tar -xf "$OUT/headers.tar" -C "$OUT"; then
  echo "[old-header-suite] UNKNOWN: could not materialize baseline headers" >&2
  exit 2
fi

if ! "$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/old-header-client.cpp" \
  -I"$OUT/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin -o "$OUT/client"; then
  echo "[old-header-suite] UNKNOWN: compile/link failed" >&2
  exit 2
fi

case "$ORDER" in
  forward) cases='empty_default partial_new_head int_max_count temppath_result_audits' ;;
  reverse) cases='temppath_result_audits int_max_count partial_new_head empty_default' ;;
  *) echo "order must be forward or reverse" >&2; exit 2 ;;
esac

if ! ulimit -v 524288 2>/dev/null; then
  echo "[old-header-suite] UNKNOWN: could not apply address-space limit" >&2
  exit 2
fi

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
preserved=0
violated=0
unknown=0
for name in $cases; do
  timeout 10s "$OUT/client" "$name"
  status=$?
  if [ "$status" -eq 2 ]; then
    echo "RESULT old_header_$name Unknown"
    unknown=$((unknown + 1))
  elif [ "$status" -eq 0 ]; then
    echo "RESULT old_header_$name Preserved"
    preserved=$((preserved + 1))
  else
    echo "RESULT old_header_$name Violated"
    violated=$((violated + 1))
  fi
done

echo "SUMMARY order=$ORDER preserved=$preserved violated=$violated unknown=$unknown"
test "$unknown" -eq 0 && test "$violated" -eq 0
