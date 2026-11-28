#!/bin/sh
# Run under Xvfb with a Debug CoinSharedCoordinateBenchmark.
set -eu
binary="${1:?usage: verify-cache-budget.sh /path/to/CoinSharedCoordinateBenchmark}"
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT HUP INT TERM
cat > "$work/check.gdb" <<'GDB'
set pagination off
set print thread-events off
set breakpoint pending on
set $builds = 0
set $allocations = 0
set $updates = 0
break SoVertexArrayTexCoordCache::SoVertexArrayTexCoordCache
commands
silent
set $allocations = $allocations + 1
continue
end
break SoVertexArrayTexCoordCache::generate
commands
silent
set $builds = $builds + 1
continue
end
break cc_glglue_glBufferSubData
commands
silent
if target == 0x8892 && size == 32768
set $updates = $updates + 1
end
continue
end
run
printf "COORDINATE_CACHE_BUILDS=%d\n", $builds
printf "COORDINATE_CACHE_ALLOCATIONS=%d TEXTURE_UPDATES=%d\n", $allocations, $updates
GDB
export COIN_GLX_PIXMAP_DIRECT_RENDERING=1 COIN_VERTEX_ARRAYS=1 COIN_VBO=1
# Own the fixture sizes rather than inherit unrelated benchmark overrides.
export COIN_BENCHMARK_SHARED_POINTS=410881 COIN_BENCHMARK_VERTICES_PER_SHAPE=4096
for layout in shared own; do
  gdb -q -batch -x "$work/check.gdb" --args "$binary" \
    "$layout" face explicit uv 2d 8 1 coord > "$work/$layout.log" 2>&1
  if ! grep -q 'exited normally' "$work/$layout.log"; then
    cat "$work/$layout.log"
    exit 1
  fi
  builds=$(sed -n 's/^COORDINATE_CACHE_BUILDS=//p' "$work/$layout.log")
  case "$builds" in ''|*[!0-9]*) cat "$work/$layout.log"; exit 1;; esac
  if { [ "$layout" = shared ] && [ "$builds" -ne 0 ]; } ||
     { [ "$layout" = own ] && [ "$builds" -ne 16 ]; }; then
    cat "$work/$layout.log"
    exit 1
  fi
  if [ "$layout" = own ] && ! grep -q '^COORDINATE_CACHE_ALLOCATIONS=8 TEXTURE_UPDATES=8$' "$work/$layout.log"; then
    cat "$work/$layout.log"; exit 1
  fi
  printf '%s: COORDINATE_CACHE_BUILDS=%s\n' "$layout" "$builds"
  sed -n '/^COORDINATE_CACHE_ALLOCATIONS=/p' "$work/$layout.log"
done
