#!/bin/sh
set -eu

if [ "$#" -ne 2 ]; then
  echo "usage: $0 /path/to/build/lib valid-sentinel|valid-replacement|empty-noop" >&2
  exit 2
fi
if [ ! -d /dev/shm ] || [ ! -w /dev/shm ]; then
  echo "/dev/shm is unavailable or not writable" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd)
CASE=$2
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "/dev/shm/coin-sotemppath-replace.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
"$OUT/repro" "$CASE"
