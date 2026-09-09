#!/bin/sh
# Standalone sibling-path check; CoinTests also covers missing and shared paths.
set -eu
if [ "$#" -ne 1 ]; then
  echo "usage: $0 /path/to/build/lib" >&2
  exit 2
fi
LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-profiling-lookup.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

# Intentional splitting of compiler/linker flags for sanitizer builds.
"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" ${LDFLAGS:-} -lCoin
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
"$OUT/repro"
