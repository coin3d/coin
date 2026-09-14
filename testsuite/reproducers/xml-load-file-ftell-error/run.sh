#!/bin/sh
# Build and run the POSIX reproducer without linking libCoin. Compiling
# src/xml/utils.cpp directly keeps cc_xml_load_file() private to Coin.
set -eu

source_root=$(CDPATH= cd -- "$(dirname -- "$0")/../../.." && pwd)
if [ "$#" -ne 1 ]; then
  echo "usage: $0 /path/to/configured-coin-build" >&2
  exit 2
fi
build_root=$(CDPATH= cd -- "$1" && pwd)
scratch=$(mktemp -d)
trap 'rm -rf "$scratch"' EXIT HUP INT TERM

"${CXX:-c++}" -O1 -g \
  -Wall -Wextra -Wpedantic -Wsign-compare \
  -fsanitize=address,undefined -fno-omit-frame-pointer \
  -DCOIN_INTERNAL \
  -I"$build_root/include" \
  -I"$source_root/include" \
  -I"$source_root/src" \
  "$source_root/src/xml/utils.cpp" \
  "$source_root/testsuite/reproducers/xml-load-file-ftell-error/repro.cpp" \
  -o "$scratch/repro"

cd "$scratch"
timeout 10 ./repro
