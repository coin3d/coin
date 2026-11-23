#!/bin/sh
set -eu
cd "$(dirname "$0")"
LIBDIR="${1:?usage: run.sh /path/to/build/lib}"
"${CXX:-c++}" -O0 -g repro.cpp -o /tmp/coin-issue-722-repro -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 /tmp/coin-issue-722-repro no-texture
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 /tmp/coin-issue-722-repro texture
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 /tmp/coin-issue-722-repro texcoord

"${CXX:-c++}" -O0 -g shapes.cpp -o /tmp/coin-issue-722-shapes -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin
./matrix.sh /tmp/coin-issue-722-shapes
