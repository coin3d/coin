#!/bin/sh
# Compile a client against the historical header, then run against the new Coin.
# Usage: run-old-header.sh /absolute/path/to/build/lib [base-ref]
CDPATH= cd "$(dirname "$0")" || exit 2
LIBDIR=$1
[ -n "$LIBDIR" ] || exit 2
ROOT=$(CDPATH= cd ../../.. && pwd) || exit 2
BASE=${2:-origin/master}
TEMP=$(mktemp -d) || exit 2
trap 'rm -rf "$TEMP"' EXIT HUP INT TERM
mkdir -p "$TEMP/include/Inventor/nodes" || exit 2
git -C "$ROOT" show "$BASE:include/Inventor/nodes/SoSelection.h" > "$TEMP/include/Inventor/nodes/SoSelection.h" || exit 2
"${CXX:-c++}" -g old-header-client.cpp -I"$TEMP/include" -I"$ROOT/include" -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin -o "$TEMP/client" || exit 2
LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" "$TEMP/client"
