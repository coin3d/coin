#!/bin/sh
# Reproducer for the unchecked ftell() bug in cc_xml_load_file()
# (src/xml/utils.cpp). POSIX-only (uses fork()/mkfifo()), so this is
# not part of the default `ctest` suite. Run manually against a build
# tree's shared library:
#
#   testsuite/reproducers/xml-load-file-ftell-error/run.sh /path/to/build/lib
#
# Before the fix: glibc's _FORTIFY_SOURCE aborts with
# "*** buffer overflow detected ***" (fread() asked to read SIZE_MAX
# bytes into a zero-byte buffer). After the fix: cc_xml_load_file()
# returns NULL cleanly.

cd "$(dirname "$0")"

LIBDIR="$1"
if [ -z "$LIBDIR" ]; then
  echo "usage: $0 /path/to/build/lib   (directory containing libCoin.so)" >&2
  exit 2
fi

CXX=${CXX:-c++}

"$CXX" -O1 -g repro.cpp -o repro -L"$LIBDIR" -lCoin || exit 2

export LD_LIBRARY_PATH="$LIBDIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
./repro
status=$?
if [ "$status" -ne 0 ]; then
  echo "=== FAIL: repro exited with status $status ===" >&2
  exit "$status"
fi
