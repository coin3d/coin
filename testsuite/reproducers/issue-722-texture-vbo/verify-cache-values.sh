#!/bin/sh
# Requires Debug Coin libraries with parameter symbols. Run under Xvfb.
set -eu
binary="${1:?usage: verify-cache-values.sh /path/to/cache-test before-lib after-lib}"
before="${2:?missing before library directory}"
after="${3:?missing after library directory}"
cd "$(dirname "$0")"
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT HUP INT TERM
export LC_ALL=C COIN_VERTEX_ARRAYS=1 COIN_VBO=0 COIN_GLX_PIXMAP_DIRECT_RENDERING=1
for source in ${COIN_CACHE_SOURCES:-property inherited homogeneous shared state empty-bounds axes axes-homogeneous}; do
  case "$source" in
    shared) expected=32;;
    property|inherited|homogeneous|state|empty-bounds|axes|axes-homogeneous|wide-bounds) expected=16;;
    *) printf 'Unknown cache source: %s\n' "$source"; exit 2;;
  esac
  for slot in before after; do
    if [ "$slot" = before ]; then library="$before"; else library="$after"; fi
    if ! env LD_LIBRARY_PATH="$library" COIN_UV_DUMP="$work/$slot.bin" \
      COIN_UV_EXPECTED="$expected" gdb -q -batch -x values.gdb \
      --args "$binary" "$source" > "$work/$slot.log" 2>&1; then
      cat "$work/$slot.log"; exit 1
    fi
    if ! grep -q 'exited normally' "$work/$slot.log" ||
       ! grep -q "^VALUES_SAMPLES=$expected VALUES_ERRORS=0$" "$work/$slot.log"; then
      cat "$work/$slot.log"; exit 1
    fi
  done
  if ! cmp "$work/before.bin" "$work/after.bin"; then
    printf '%s: different coordinate bits\n' "$source"; exit 1
  fi
  printf '%s: %s draws have bit-identical S/T arrays\n' "$source" "$expected"
done
