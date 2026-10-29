#!/bin/sh
set -eu
binary="${1:?usage: matrix.sh /path/to/texture-array-test}"
if [ -z "${DISPLAY:-}" ]; then
  echo 'Texture array test requires an X display.' >&2
  exit 77
fi
export COIN_GLX_PIXMAP_DIRECT_RENDERING=1
for arrays in 0 1; do
  for vbo in 0 1; do
    if [ "$arrays" = 0 ] && [ "$vbo" = 1 ]; then continue; fi
    export COIN_VERTEX_ARRAYS="$arrays" COIN_VBO="$vbo"
    for shape in indexed-face face point indexed-point line vrml; do
      for mode in no-texture texture texcoord explicit function multi texgen generated-normal; do
        "$binary" "$shape" "$mode"
      done
    done
  done
done
