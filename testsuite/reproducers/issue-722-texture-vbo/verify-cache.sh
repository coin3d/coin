#!/bin/sh
# Requires a debug Coin build and CoinTextureCacheTest; run under Xvfb.
set -eu
cd "$(dirname "$0")"
binary="${1:?usage: verify-cache.sh /path/to/CoinTextureCacheTest}"
export COIN_GLX_PIXMAP_DIRECT_RENDERING=1 COIN_VERTEX_ARRAYS=1
logfile="$(mktemp)"
trap 'rm -f "$logfile"' EXIT HUP INT TERM
for vbo in 0 1; do
  export COIN_VBO="$vbo"
  for source in property inherited homogeneous; do
    gdb -q -batch -x cache.gdb --args "$binary" "$source" > "$logfile" 2>&1
    uploads=$((14 * vbo))
    expected="CACHE_BUILDS=7 BBOX_BUILDS=7 TEXTURE_UPLOADS=$uploads TWO_CONTEXTS=$vbo"
    if ! grep -q "$expected" "$logfile"; then cat "$logfile"; exit 1; fi
    if [ "$vbo" -eq 1 ] && ! grep -Eq '^TEXTURE_UPDATES=[1-9][0-9]*$' "$logfile"; then
      cat "$logfile"; exit 1
    fi
    printf '%s vbo=%s %s\n' "$source" "$vbo" "$expected"
    sed -n '/^TEXTURE_UPDATES=/p' "$logfile"
  done
done
