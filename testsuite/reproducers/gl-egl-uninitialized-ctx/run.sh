#!/bin/sh
# Reproducer for the -Wuninitialized bug in
# eglglue_context_create_offscreen() (src/glue/gl_egl.cpp).
#
# Requires a Linux machine with EGL headers/lib (COIN_BUILD_EGL=ON at
# configure time) and, ideally, a real EGL-capable GPU -- this is not
# run as part of the default `ctest` suite because CI runners may lack
# both. Run manually against a build tree's shared library:
#
#   testsuite/reproducers/gl-egl-uninitialized-ctx/run.sh /path/to/build/lib
#
# Before the fix: crashes with SIGSEGV (the uninitialized `ctx` pointer
# is dereferenced as ctx->width/ctx->height while building the
# surface_attrib[] array, before ctx is ever assigned).
# After the fix: does not crash, and -- when LD_PRELOAD-ing egl_shim.so
# -- the EGL_WIDTH/EGL_HEIGHT values actually reaching the real EGL
# surface-creation call are verified to equal the requested size.

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CC=${CC:-cc}
CXX=${CXX:-c++}

"$CC" -shared -fPIC -O0 -o egl_shim.so egl_shim.c -ldl -lEGL || exit 2
"$CXX" -O0 -g repro.cpp -o repro -L"$LIBDIR" -lCoin -lEGL || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Run both phases even if one is inconclusive: run 2 (via the shim) can
# still reach a real verdict on environments where run 1's raw,
# non-intercepted EGL surface creation fails for unrelated reasons (e.g.
# a real driver rejecting the NULL native window this reproducer passes,
# independent of anything eglglue_context_create_offscreen() does wrong)
# -- so a run 1 inconclusive must not hide a real pass or fail from run 2.
any_inconclusive=0

echo "=== run 1/2: direct call, no interception ==="
# EGL_SHIM_OUTFILE deliberately unset here: no shim is loaded in this
# run, so repro would have nothing valid to read from it and (correctly,
# since context creation not going through the shim isn't a bug in this
# run) report FAIL for the wrong reason if it were set.
unset EGL_SHIM_OUTFILE
./repro
status=$?
if [ "$status" -eq 2 ]; then
  echo "=== INCONCLUSIVE (run 1/2): no context created in this environment ==="
  any_inconclusive=1
elif [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status (SIGSEGV is status 139) ==="
  exit "$status"
fi

echo
echo "=== run 2/2: LD_PRELOAD egl_shim.so, verifying EGL_WIDTH/EGL_HEIGHT ==="
export EGL_SHIM_OUTFILE="$PWD/captured.txt"
rm -f "$EGL_SHIM_OUTFILE"
LD_PRELOAD="$PWD/egl_shim.so" ./repro
status=$?
if [ "$status" -eq 2 ]; then
  echo "=== INCONCLUSIVE (run 2/2): shim never intercepted a call in this environment ==="
  any_inconclusive=1
elif [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status under interception ==="
  exit "$status"
fi

echo
if [ "$any_inconclusive" -ne 0 ]; then
  echo "=== INCONCLUSIVE: neither run crashed or failed, but at least one couldn't verify anything in this environment ==="
  exit 2
fi
echo "=== PASS ==="
