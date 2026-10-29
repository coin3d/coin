#!/bin/sh
# Headless regression against the real gl_glx.cpp. See README.md.
set -eu
if [ "$#" -ne 1 ]; then
  echo "usage: $0 /absolute/path/to/build/lib" >&2
  exit 2
fi
LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
SOURCE=$(CDPATH= cd "$HERE/../../.." && pwd)
CXX=${CXX:-c++}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/coin-glx-fbccnt.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

# Pattern initialization makes removal of the initializer fail reliably in
# the real implementation. Keep its assertion enabled even for Release libs.
# CXXFLAGS/LDFLAGS allow sanitizer instrumentation of both implementation and
# test driver, which are compiled together here. Intentional flag splitting.
"$CXX" ${CXXFLAGS:-} -O1 -g -UNDEBUG -ftrivial-auto-var-init=pattern \
  -DCOIN_INTERNAL -DHAVE_CONFIG_H \
  -I"$SOURCE/src" -I"$SOURCE/include" \
  -I"$LIBDIR/../src" -I"$LIBDIR/../include" \
  "$HERE/repro.cpp" -o "$OUT/repro" -L"$LIBDIR" \
  ${LDFLAGS:-} -lCoin -lGL -lX11
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
"$OUT/repro"
