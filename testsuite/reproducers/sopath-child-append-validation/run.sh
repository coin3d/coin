#!/bin/sh
# Compile and run each SoPath append contract case in its own process.
set -eu

if [ "$#" -ne 1 ]; then
  echo "usage: $0 /path/to/build/lib" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-sopath-append.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

# Intentional splitting of compiler/linker flags supports sanitizer builds.
"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/repro.cpp" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" ${LDFLAGS:-} -lCoin
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

failures=0
for case_name in $("$OUT/repro" --list); do
  echo "=== $case_name ==="
  if "$OUT/repro" "$case_name"; then
    :
  else
    status=$?
    echo "[$case_name] process failed with status $status" >&2
    failures=$((failures + 1))
  fi
done

if [ "$failures" -ne 0 ]; then
  echo "FAIL: $failures case process(es) failed" >&2
  exit 1
fi
echo "PASS: all SoPath append contract cases passed"
