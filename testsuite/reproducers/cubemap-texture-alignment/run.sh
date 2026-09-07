#!/bin/sh
# Reproducer for the missing GL_UNPACK_ALIGNMENT in
# SoGLCubeMapImage::getGLDisplayList() (src/rendering/SoGLCubeMapImage.cpp).
# Run manually against a build tree's shared library:
#
#   testsuite/reproducers/cubemap-texture-alignment/run.sh /path/to/build/lib
#
# Before the fix: a large fraction of the rendered cube's pixels don't
# exactly match any of the six solid colors uploaded to the cubemap faces
# (misaligned texture row reads corrupt the uploaded texel data). After the
# fix: every pixel is an exact match.
#
# Needs a working GLX context. If offscreen rendering fails to get direct
# rendering (common in headless/sandboxed environments), try:
#   COIN_GLX_PIXMAP_DIRECT_RENDERING=1 testsuite/reproducers/cubemap-texture-alignment/run.sh ...

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}

"$CXX" -O1 -g repro.cpp -o repro -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin -lGL || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
