#!/bin/sh
# Regression test for the glxglue_context_create_pbuffer() fbc_cnt
# uninitialized-variable fix -- see repro.cpp for the full
# explanation.
#
#   testsuite/reproducers/glx-pbuffer-fbccnt-uninitialized/run.sh /path/to/build/lib
#
# Prints PASS and exits 0 if the deterministic
# glxglue_choose_fbconfig() check passes (the SoOffscreenRenderer
# check is purely informational and never affects this).
#
# For the strongest, tool-guaranteed confirmation that the
# uninitialized-read bug itself is gone (rather than just that the
# fixed code behaves correctly, which this plain run already checks),
# build libCoin.so with Clang's MemorySanitizer and run this
# reproducer against it directly (bypassing this script, since MSan
# needs matching -fsanitize=memory flags on both the library and this
# translation unit):
#
#   cmake -S . -B /tmp/coin-msan -DCMAKE_BUILD_TYPE=Debug \
#     -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
#     -DCMAKE_C_FLAGS="-fsanitize=memory -fno-omit-frame-pointer" \
#     -DCMAKE_CXX_FLAGS="-fsanitize=memory -fno-omit-frame-pointer" \
#     -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=memory" \
#     -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=memory" \
#     -DCOIN_BUILD_TESTS=OFF
#   cmake --build /tmp/coin-msan --target Coin -j
#   clang++ -O1 -g -fsanitize=memory -fno-omit-frame-pointer repro.cpp \
#     -o repro-msan -I../../../include -I/tmp/coin-msan/include \
#     -L/tmp/coin-msan/lib -lCoin -lX11
#   LD_LIBRARY_PATH=/tmp/coin-msan/lib ./repro-msan
#
# MSan flags "use-of-uninitialized-value" at gl_glx.cpp's
# assert(fbc_cnt >= 0) line against the unfixed code, and reports
# nothing at all for the deterministic check against the fix -- see
# the top-of-file comment in repro.cpp for why this, and not the
# stack-poisoning trick an earlier version of this fix attempted, is
# what's actually reliable across compilers. (SoDB::init(), further
# down in main(), triggers an unrelated, pre-existing MSan finding
# elsewhere in Coin's scxml subsystem when run under this build --
# unrelated to and out of scope for this fix; the deterministic check
# under test runs and completes, cleanly, before that.)

CDPATH= cd "$(dirname "$0")" || exit 2

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}
SRCINCLUDE="$(CDPATH= cd ../../.. && pwd)/include" || exit 2

"$CXX" -O1 -g repro.cpp -o repro -I"$SRCINCLUDE" -I"$LIBDIR/../include" -L"$LIBDIR" -lCoin -lX11 || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
