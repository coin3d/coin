#!/bin/sh
set -eu

if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
  echo "usage: $0 /path/to/build/lib {contract|factory|frompath} [case]" >&2
  exit 2
fi

LIBDIR=$(CDPATH= cd "$1" && pwd)
MODE=$2
CASE=${3:-all}
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-nodekitpath-contract.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

SANITIZERS=${SANITIZERS:--fsanitize=undefined,vptr -fno-sanitize-recover=undefined}
COMMON_FLAGS="-std=c++11 -O1 -g -fno-omit-frame-pointer"

case "$MODE" in
  contract|factory)
    INPUT="$HERE/repro.cpp"
    ;;
  frompath)
    INPUT="$HERE/frompath.cpp"
    ;;
  *)
    echo "unknown mode: $MODE" >&2
    exit 2
    ;;
esac

# Intentional flag splitting permits callers to add compiler and sanitizer
# flags.  Compilation failure is a contract violation for a required API; it
# is never converted into a passing test.
"$CXX" ${CXXFLAGS:-} $COMMON_FLAGS $SANITIZERS "$INPUT" -o "$OUT/repro" \
  -I"$SOURCE/include" -I"$LIBDIR/../include" -L"$LIBDIR" \
  ${LDFLAGS:-} $SANITIZERS -lCoin

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
if [ "$MODE" = factory ]; then
  "$OUT/repro" "$CASE"
elif [ "$MODE" = frompath ]; then
  "$OUT/repro"
else
  "$OUT/repro" "$CASE"
fi
