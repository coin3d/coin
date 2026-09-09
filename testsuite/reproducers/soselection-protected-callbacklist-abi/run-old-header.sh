#!/bin/sh
# Compile all public headers from a fixed pre-PR revision, then use new Coin.
set -eu
if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
  echo "usage: $0 /path/to/build/lib [historical-ref]" >&2
  exit 2
fi
LIBDIR=$(CDPATH= cd "$1" && pwd)
HERE=$(CDPATH= cd "$(dirname "$0")" && pwd)
ROOT=$(CDPATH= cd "$HERE/../../.." && pwd)
BASE=${2:-c27cf9a72d302c8c1ee3367a8865a603ea26622a}
CXX=${CXX:-c++}
TEMP=$(mktemp -d "${TMPDIR:-/tmp}/coin-old-header.XXXXXX")
trap 'rm -rf "$TEMP"' EXIT HUP INT TERM
# Avoid silently mixing old SoSelection with new SoCallbackList/SbPList.
git -C "$ROOT" archive --output="$TEMP/headers.tar" "$BASE" include/Inventor
tar -xf "$TEMP/headers.tar" -C "$TEMP"
# Generated configuration headers must still come from the selected build.
"$CXX" ${CXXFLAGS:-} -O1 -g "$HERE/old-header-client.cpp" \
  -I"$TEMP/include" -I"$LIBDIR/../include" \
  -L"$LIBDIR" ${LDFLAGS:-} -lCoin -o "$TEMP/client"
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
"$TEMP/client"
