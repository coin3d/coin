#!/bin/sh
# Requires a debug Coin build and the executable produced by run.sh.
set -eu
cd "$(dirname "$0")"
LIBDIR="${1:?usage: verify-arrays.sh /path/to/build/lib}"
export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export COIN_GLX_PIXMAP_DIRECT_RENDERING=1 COIN_VERTEX_ARRAYS=1
binary="${COIN_TEXTURE_ARRAY_TEST_BINARY:-/tmp/coin-issue-722-shapes}"
logfile="$(mktemp)"
trap 'rm -f "$logfile"' EXIT HUP INT TERM
modes="${2:-texcoord explicit function multi texgen generated-normal}"
for vbo in 0 1; do
  export COIN_VBO="$vbo"
  for shape in indexed-face face point indexed-point line vrml; do
    for mode in $modes; do
      gdb -q -batch -x arrays.gdb --args "$binary" "$shape" "$mode" > "$logfile" 2>&1
      if [ "$mode" = function ] || [ "$mode" = multi ] ||
         { [ "$mode" = generated-normal ] &&
           { [ "$shape" = face ] || [ "$shape" = indexed-face ] || [ "$shape" = vrml ]; }; }; then
        expected="ARRAY_CALLS=0 VBO_CALLS=0"
      else
        expected="ARRAY_CALLS=2 VBO_CALLS=$((2 * vbo))"
      fi
      if ! grep -q "$expected" "$logfile"; then
        cat "$logfile"
        exit 1
      fi
      printf '%s %s vbo=%s %s\n' "$shape" "$mode" "$vbo" "$expected"
    done
  done
done
