#!/bin/sh
set -u

if [ "$#" -lt 1 ] || [ "$#" -gt 3 ]; then
  echo "usage: $0 /path/to/build/lib [forward|reverse] [baseline-ref]" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd) || exit 2
ORDER=${2:-forward}
BASE=${3:-40c6372d2984a94c6ac17f059caee7b8d6374654}
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
ROOT=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-sopath-range-old.XXXXXX") || exit 2
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

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
  forward) cases='sopath_pop_empty sofullpath_pop_empty' ;;
  reverse) cases='sofullpath_pop_empty sopath_pop_empty' ;;
  *) echo "order must be forward or reverse" >&2; exit 2 ;;
esac

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
violated=0
for name in $cases; do
  "$OUT/client" "$name"
  status=$?
  if [ "$status" -eq 2 ]; then
    echo "RESULT old_header_$name Unknown"
    exit 2
  elif [ "$status" -eq 0 ]; then
    echo "RESULT old_header_$name Preserved"
  else
    echo "RESULT old_header_$name Violated"
    violated=$((violated + 1))
  fi
done

echo "SUMMARY order=$ORDER preserved=$((2 - violated)) violated=$violated unknown=0"
test "$violated" -eq 0
