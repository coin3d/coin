#!/bin/sh
# Reproducer for SoTextureCoordinate2/SoVertexProperty hardcoding texture
# unit 0 when registering their VBO with SoGLVBOElement, instead of the
# real active unit (src/nodes/SoTextureCoordinate2.cpp,
# src/nodes/SoVertexProperty.cpp). Run manually against a build tree's
# shared library:
#
#   testsuite/reproducers/multitexture-coord-vbo-unit/run.sh /path/to/build/lib
#
# Before the fix: unit 0 ends up sampled with unit 1's texture coordinates
# once a shape's rendering goes through SoShape::startVertexArray()'s
# VBO/vertex-array path (>= 20 vertices, unlit or no per-vertex normals
# needed) -- unit 0's own left/right color split collapses to a single
# color. After the fix: the split renders correctly.
#
# Needs a working GLX context. If offscreen rendering fails to get direct
# rendering (common in headless/sandboxed environments), try:
#   COIN_GLX_PIXMAP_DIRECT_RENDERING=1 testsuite/reproducers/multitexture-coord-vbo-unit/run.sh ...

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}

"$CXX" -O1 -g repro.cpp -o repro -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
