#!/bin/sh
set -eu
binary="${1:?usage: cache.sh /path/to/cache-test}"
if [ -z "${DISPLAY:-}" ]; then exit 77; fi
export COIN_GLX_PIXMAP_DIRECT_RENDERING=1
work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT HUP INT TERM
for source in property inherited homogeneous shared state empty-bounds; do
  for cachemode in uncached cached; do
    COIN_VERTEX_ARRAYS=0 COIN_VBO=0 "$binary" "$source" reference "$cachemode" > "$work/reference"
    for vbo in 0 1; do
      COIN_VERTEX_ARRAYS=1 COIN_VBO="$vbo" "$binary" "$source" "$cachemode" > "$work/actual"
      if ! diff -u "$work/reference" "$work/actual"; then exit 1; fi
      printf '%s %s vbo=%s: 16 frames match independently computed explicit UVs\n' "$source" "$cachemode" "$vbo"
    done
  done
done
